// Settings.cpp: implementation of the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Settings.h"
#include "MainDlg.h"
#include "Buffer.h"
#include "Markup.h"
#include "ComBSTR2.h"
#include "Notify.h"
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>

#ifndef _WODVPNLIB
const char *hex="0123456789abcdef";
#else
extern const char *hex;
#endif

const char *GROUP_GENERAL = "General\0";
const char *GROUP_OFFLINE = "Offline\0";
const char *AWAY_MESSAGE = "Away due to inactivity.";
const char *EXTAWAY_MESSAGE = "Away for a loooong time.";

extern CMainDlg _MainDlg;
extern CNotify	_Notify;

char *trim(char *text);

char *REGISTRYKEY = "Software\\Kresimir Petric\\Wippien";


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSettings::CSettings()
{
	m_LoadSuccess = FALSE;

	m_TimestampMessages = TRUE;
	m_ShowMessageHistory = TRUE;
	m_SoundOn = TRUE;
	m_ShowInTaskbar = FALSE;
//	m_LicenseCount = 0;
//	m_LicenseExpireTime = 0;
	m_Skin = "Cheetah.smf";
	m_LastOperatorMessageID = LASTOPERATORMSGID;

	m_ServerPort = 5222;
	m_UDPPort = 0;

	m_EnableVoiceChat = TRUE;
	m_VoiceChatRecordingDevice = -1;
	m_VoiceChatPlaybackDevice = -1;


	memset(m_DoNotShow, '0', sizeof(m_DoNotShow));
	m_DoNotShow[MAXDONOTSHOWANYMORESETTINGS] = 0;
	m_DoNotShow[DONOTSHOW_NOETHERNET] = '0';
	m_DoNotShow[DONOTSHOW_NOALLOWEXIT] = '1';

	memset(&m_RosterRect, 0, sizeof(RECT));

	m_MyLastNetwork = m_MyLastNetmask = 0;
	m_AllowAnyMediator = TRUE;
	m_AllowLinkMediatorToBeProvidedByIPMediator = TRUE;
	m_UseIPMediator = TRUE;
	m_ShowMediatorOnContacts = TRUE;
	m_IPMediator = "mediator@wippien.com";
	m_LinkMediator = "mediator.wippien.com";
	m_LinkMediatorPort = 8000;
	m_ObtainIPAddress = 1;

	m_AutoAwayMinutes = 10;
	m_ExtendedAwayMinutes = 30;
	m_AutoDisconnectMinutes = 0;
	m_AutoSetBack = TRUE;

	m_AutoAwayMessage = AWAY_MESSAGE;
	m_ExtendedAwayMessage = EXTAWAY_MESSAGE;
	m_DeleteFunctionLogMb = 10*1024*1024;
	m_UseSSLWrapper = FALSE;

	HKEY hkey = NULL;
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, REGISTRYKEY , 0, KEY_ALL_ACCESS, &hkey);

	m_AES.SessionKey((unsigned char *)REGISTRYKEY);


	char buff[1024];
	Buffer st;
	AESRead(&st);
	if (st.Len())
	{
		char *a = st.GetString(NULL);
		if (a)
		{
			m_SettingsFolder = a;
			free(a);
		}
	}
	else
	{
		m_SettingsFolder = "%APPDATA%\\Wippien";
	}
	CComBSTR2 ms = m_SettingsFolder;
	strcpy(buff, ms.ToString());
	char *ms1 = NULL;
	while (ms1 = strchr(buff, '%'))
	{
		char *ms2 = strchr(ms1+1, '%');
		if (ms2)
		{
			Buffer b1;
			*ms1++ = 0;
			*ms2++ = 0;
			b1.Append(buff);

			HKEY hkey;
			if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders", 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
			{
				BOOL found = FALSE;
				char valname[1024];
				unsigned char valdata[1024];
				DWORD index = 0;
				int err;
				do
				{
					DWORD valdatasize = sizeof(valdata);
					DWORD valsize = sizeof(valname);
					DWORD dwtype = 0;
					err = RegEnumValue(hkey, index++, valname, &valsize, NULL, &dwtype, valdata, &valdatasize);
					if (err == ERROR_SUCCESS)
					{
						if (!stricmp(ms1, valname))
						{
							err = ERROR_NO_MORE_ITEMS;
							b1.Append((char *)valdata);
							found = TRUE;
						}
					}
				} while (err == ERROR_SUCCESS);
				RegCloseKey(hkey);

				if (!found)
				{
					char *env = getenv(ms1);
					if (env)
						b1.Append(env);
				}
			}
			b1.Append(ms2);
			b1.Append("\0",1);
			strcpy(buff, b1.Ptr());
		}

	}
	

	memset(m_CfgFilename, 0, sizeof(m_CfgFilename));
	memset(m_UserImagePath, 0, sizeof(m_UserImagePath));
	memset(m_HistoryPath, 0, sizeof(m_HistoryPath));
	memset(m_UsrFilename, 0, sizeof(m_UsrFilename));

	strcpy(m_CfgFilename, trim(buff));

	int i = strlen(m_CfgFilename);
	if (i>1)
	{
		if (m_CfgFilename[i-1] == '\\')
			m_CfgFilename[i-1]=0;
	}

	// just in case, create folder
	CreateDirectory(m_CfgFilename, NULL);

	GetModuleFileName(_Module.GetModuleInstance(), m_MyPath, MAX_PATH);

	i = strlen(m_MyPath);
	while (i>0 && m_MyPath[i]!='\\')
		i--;
	if (i)
		m_MyPath[i] = 0;
	
	strcpy(m_UsrFilename, m_CfgFilename);
	strcpy(m_UserImagePath, m_CfgFilename);
	strcpy(m_HistoryPath, m_CfgFilename);
//	strcpy(m_MyPath, m_CfgFilename);
	strcat(m_CfgFilename, "\\Wippien.config");
	strcat(m_UsrFilename, "\\Wippien.users");
	strcat(m_MyPath, "\\");
	strcat(m_UserImagePath, "\\Images\\");
	strcat(m_HistoryPath, "\\History\\");
//	m_RSA = NULL;
	m_RSA = RSA_generate_key(1024,35,NULL,NULL);


	CreateDirectory(m_UserImagePath, NULL);
	CreateDirectory(m_HistoryPath, NULL);


	// window stuff
	m_IsAligned = FALSE;
	m_IsTopMost = TRUE;
	m_DoAlign = FALSE;

//	m_DHCPAddress.push_back(NULL); // push back nothing at place 0
//	m_DHCPAddress.push_back(NULL); // push back nothing at place 1
//	for (i=0;i<256;i++)
//		m_DHCPAddress[i] = NULL;

	m_CheckUpdate = TRUE;
	m_CheckUpdateSilently = FALSE;
	m_CheckUpdateConnect = FALSE;
	m_CheckUpdateTimed = TRUE;
	m_CheckUpdateTimedNum = 60;
	m_NowProtected = FALSE;
	m_SnapToBorder = FALSE;

	m_DeleteContactsOnStartup = FALSE;
	m_DeleteContactsOnConnect = FALSE;
	m_AutoConnectVPNOnNetwork = TRUE;
	m_AutoConnectVPNOnStartup = TRUE;

	m_FixedMTU = FALSE;
	m_FixedMTUNum = 1200;


	m_ShowContactPicture = m_ShowContactName = m_ShowContactIP = m_ShowContactStatus = m_ShowContactLastOnline = TRUE;
	m_ShowMyPicture = m_ShowMyName = m_ShowMyIP = m_ShowMyStatus = TRUE;

	m_SortContacts = 1;
	m_AuthContacts = 1;
}	

CSettings::~CSettings()
{
	if (m_RSA)
		RSA_free(m_RSA);
	while (m_Groups.size())
	{
		TreeGroup *tg = m_Groups[0];
		m_Groups.erase(m_Groups.begin());
		free(tg->Name);
		delete tg;
	}

	while (m_MenuTools.size())
	{
		MenuTool *mt = m_MenuTools[0];
		m_MenuTools.erase(m_MenuTools.begin());
		if (mt->Menu)
			free(mt->Menu);
		if (mt->Exec)
			free(mt->Exec);
		if (mt->IconPath)
			free(mt->IconPath);
		if (mt->FilterGroup)
			free(mt->FilterGroup);
		if (mt->FilterJID)
			free(mt->FilterJID);
		if (mt->FilterVisibleName)
			free(mt->FilterVisibleName);

/*		while (mt->FilterGroup.size())
		{
			char *mtf = mt->FilterGroup[0];
			mt->FilterGroup.erase(mt->FilterGroup.begin());
			free(mtf);
		}
		while (mt->FilterJID.size())
		{
			char *mtf = mt->FilterJID[0];
			mt->FilterJID.erase(mt->FilterJID.begin());
			free(mtf);
		}
		while (mt->FilterVisibleName.size())
		{
			char *mtf = mt->FilterVisibleName[0];
			mt->FilterVisibleName.erase(mt->FilterVisibleName.begin());
			free(mtf);
		}
*/
		
		
		delete mt;
	}


	while (m_AuthRequests.size())
	{
		BSTR b = m_AuthRequests[0];
		m_AuthRequests.erase(m_AuthRequests.begin());
		::SysFreeString(b);
	}

	while (m_MenuImages.size())
	{
		CxImage *img = m_MenuImages[0];
		m_MenuImages.erase(m_MenuImages.begin());
		delete img;
	}
}
/* tinyxmlparser
#define READCFG(x,y,def) \
if (!strcmp(node->Value(), y))\
{\
x = !(def);\
TiXmlNode* child = node->FirstChild();\
if (child)\
{\
data = child->Value();\
x = atol(data);\
}else x = def;\
}

#define READCFGSTRING(x,y) \
if (!strcmp(node->Value(), y))\
{\
x.Empty();\
TiXmlNode* child = node->FirstChild();\
if (child)\
{\
data = child->Value();\
x = data;\
}else x.Empty();\
}

#define READCFGSTRING2(x,y,def) \
if (!strcmp(node->Value(), y))\
{\
x.Empty();\
TiXmlNode* child = node->FirstChild();\
if (child)\
{\
data = child->Value();\
x = data;\
}else x = def;\
}
*/

int CSettings::Load(void)
{
	char buff[32768];

	m_LoadSuccess = FALSE;

	Buffer pout;
	AESRead(&pout);
	if (pout.Len())
	{
		char *a = pout.GetString(NULL);
		if (a)
			free(a);
		a = pout.GetString(NULL);
		if (a)
		{
			m_Password.Empty();
			m_Password = a;
			free(a);
		}
		a = pout.GetString(NULL);
		if (a)
		{
			m_PasswordProtectPassword.Empty();
			m_PasswordProtectPassword = a;
			free(a);
			if (m_PasswordProtectPassword.Length())
				m_NowProtected = TRUE;
		}
		if (pout.GetChar() != 1)
			m_PasswordProtectAll = FALSE;
		else
			m_PasswordProtectAll = TRUE;
	}

	// read from file
	int handle = open(m_CfgFilename, O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
	if (handle == (-1))
		return FALSE;

	Buffer b;
	int i;
	do
	{
		i = read(handle, buff, 32768);
		if (i>0)
			b.Append(buff, i);
	} while (i>0);
	close(handle);

	if (b.Len())
		m_LoadSuccess = TRUE;

	b.Append("\0",1);

	// now parse this
	CMarkup xml;
	xml.SetDoc(b.Ptr());
	CString data;


	/* Tinyxmlparser attempt
	TiXmlDocument doc( "packet.xml" );
	doc.Parse(b.Ptr());


	// go to root
	while (m_Groups.size())
	{
		TreeGroup *tg = m_Groups[0];
//		free(tg->Name);
		m_Groups.erase(m_Groups.begin());
		delete tg;
	}

	TiXmlNode* node = NULL;
	TiXmlElement* todoElement = NULL;

	node = doc.FirstChildElement();
	
	do
	{
		if (node)
		{
			char *name = (char *)node->Value();
			if (!strcmp(name, "Wippien"))
			{
				node = node->FirstChild();
				break;
			}
				
			TiXmlNode *n = node->FirstChild();
			if (n)
				node = n;
			else
				node = node->NextSibling();
		}
	} while (node);

	do
	{
		if (node)
		{
			CComBSTR2 cc = node->Value();
			ATLTRACE("Reading %s\r\n", cc.ToString());
			READCFG(m_ShowInTaskbar, "ShowInTaskbar", FALSE);
			READCFG(m_SoundOn, "SoundOn", TRUE);
			READCFG(m_DeleteContactsOnStartup, "DeleteContactsOnStartup", FALSE);
			READCFG(m_DeleteContactsOnConnect, "DeleteContactsOnConnect", FALSE);
			READCFG(m_AutoConnectVPNOnNetwork, "AutoConnectVPNOnNetwork", TRUE);
			READCFG(m_EnableVoiceChat, "VoiceChat", TRUE);
			READCFG(m_CheckUpdate, "CheckUpdate", TRUE);
			READCFG(m_CheckUpdateConnect, "CheckUpdateConnect", TRUE);
			READCFG(m_CheckUpdateTimed, "CheckUpdateTimed", TRUE);
			READCFG(m_CheckUpdateTimedNum, "CheckUpdateTimedNum", 60);
			READCFG(m_CheckUpdateSilently, "CheckUpdateSilently", FALSE);
			READCFG(m_FixedMTU, "FixedMTU", FALSE);
			READCFG(m_FixedMTUNum, "FixedMTUNum", 1200);
			READCFG(m_VoiceChatRecordingDevice, "VoiceChatRecordingDevice", -1);
			READCFG(m_VoiceChatPlaybackDevice, "VoiceChatPlaybackDevice", -1);
			READCFG(m_LastOperatorMessageID, "LastOperatorMessageID", -1);
			READCFGSTRING(m_JID, "JID");
			READCFGSTRING(m_Password, "Password");
			READCFGSTRING(m_ServerHost, "ServerHost");
			READCFG(m_ServerPort, "ServerPort", 5222);
			READCFG(m_UDPPort, "UDPPort", 0);
			READCFGSTRING(m_Mediator, "Mediator");
			READCFG(m_AllowAnyMediator, "AllowMediator", FALSE);
			READCFG(m_ShowMediatorOnContacts, "ShowMediator", TRUE);
			READCFG(m_TimestampMessages, "TimestampMessages", TRUE);
			READCFG(m_SnapToBorder, "SnapToBorder", FALSE);
			READCFG(m_ShowMessageHistory, "ShowMessageHistory", TRUE);
			READCFGSTRING(m_Skin, "Skin");
			READCFGSTRING(m_JabberDebugFile, "JabberDebugFile");
			READCFGSTRING(m_SocketDebugFile, "SocketDebugFile");
			READCFGSTRING(m_FunctionDebugFile, "FunctionDebugFile");
			READCFG(m_DeleteFunctionLogMb, "DeleteFunctionLogMb", 10485760L);
			READCFG(m_AutoAwayMinutes, "AutoAwayMinutes", 10);
			READCFG(m_ExtendedAwayMinutes, "ExtendedAwayMinutes", 20);
			READCFG(m_AutoDisconnectMinutes, "AutoDisconnectMinutes", 0);
			READCFGSTRING2(m_AutoAwayMessage, "AutoAwayMessage", AWAY_MESSAGE);
			READCFGSTRING2(m_ExtendedAwayMessage, "ExtendedAwayMessage", EXTAWAY_MESSAGE);
			READCFG(m_AutoSetBack, "AutoSetBack", TRUE);
			
			if (!strcmp(node->Value(), "Icon"))
			{
				data = node->Value();
				Buffer in;
				in.Append(data, strlen(data));
				FromHex(&in, &m_Icon);
			}
			if (!strcmp(node->Value(), "DoNotShow"))
			{
				data = node->Value();
				int i = strlen(data);
				if (i>MAXDONOTSHOWANYMORESETTINGS)
					i = MAXDONOTSHOWANYMORESETTINGS;
				memcpy(m_DoNotShow, data, i);
			}
			if (!strcmp(node->Value(), "LastNetwork"))
			{
				data = node->Value();
				long mip = atol(data);
				memcpy(&m_MyLastNetwork, &mip, sizeof(long));
				m_MyLastNetmask &= 0x00FFFFFF; 
			}
			if (!strcmp(node->Value(), "LastNetmask"))
			{
				data = node->Value();
				long mnm = atol(data);
				memcpy(&m_MyLastNetmask, &mnm, sizeof(long));
			}
			if (!strcmp(node->Value(), "AuthRequests"))
			{
				TiXmlElement* child = node->FirstChildElement();

				do
				{
					if (child && !strcmp(child->Value(), "Group"))
					{
						TiXmlNode *ch = child->FirstChild();
						if (ch)
						{
							data = ch->Value();
							CComBSTR d = data;
							m_AuthRequests.push_back(d.Copy());
						}
						child = child->NextSiblingElement();
					}
				} while (child);
			}
			if (!strcmp(node->Value(), "Roster"))
			{
				TiXmlElement* child = node->FirstChildElement();

				BOOL foundOff = FALSE;
				BOOL OffOpen = FALSE;
				BOOL foundGen = FALSE;

				do 
				{
					if (child && !strcmp(child->Value(), "Group"))
					{
						TiXmlNode *ch = child->FirstChild();
						if (ch)
						{
							data = ch->Value();
							char *a = (char *)malloc(strlen(data)+1);
							memset(a, 0, strlen(data)+1);
							memcpy(a, data, strlen(data));
							if (!strcmp(a, GROUP_OFFLINE))
							{
								TiXmlAttribute *attrib = 0;
								attrib = child->FirstAttribute();
								while (attrib)
								{
									if (attrib->Name() == "Open" && attrib->Value() == "true")
									{
										OffOpen = TRUE;
										break;
									}
									attrib = attrib->Next();
								}
								foundOff = TRUE;
								free(a);
							}
							else
							{				
								TreeGroup *tg = new TreeGroup;
								tg->Item = NULL;
								tg->Open = FALSE;
								tg->Name = a;
								TiXmlAttribute *attrib = 0;
								attrib = child->FirstAttribute();
								while (attrib)
								{
									if (attrib->Name() == "Open" && attrib->Value() == "true")
									{
										tg->Open = TRUE;
										break;
									}
									attrib = attrib->Next();
								}
								if (!strcmp(a, GROUP_GENERAL))
									foundGen = TRUE;
								PushGroupSorted(tg);
							}
						}

						child = child->NextSiblingElement();
					}
				} while (child);

				if (!foundGen && m_Groups.size()<2)
				{
					char *a = (char *)malloc(strlen(GROUP_GENERAL)+1);
					memcpy(a, GROUP_GENERAL, strlen(GROUP_GENERAL)+1);
					TreeGroup *tg = new TreeGroup;
					tg->Item = NULL;
					tg->Open = FALSE;
					tg->Name = a;
					PushGroupSorted(tg);
				}
				char *a = (char *)malloc(strlen(GROUP_OFFLINE)+1);
				memcpy(a, GROUP_OFFLINE, strlen(GROUP_OFFLINE)+1);
				TreeGroup *tg = new TreeGroup;
				tg->Item = NULL;
				tg->Open = OffOpen;
				tg->Name = a;
				m_Groups.push_back(tg);
			}


			node = node->NextSibling();
		}
	} while  (node);
	
	// re-read password
	if (!m_Password.Length())
	{
		memset(buff, 0, sizeof(buff));
		if (EXECryptor_SecureRead("Password", buff))
			m_Password = buff;
	}
	
	
	node = doc.FirstChildElement();
	if (node) node = node->NextSibling();
	do
	{
		if (node)
		{
			char *name = (char *)node->Value();
			if (!strcmp(name, "Message_Dialog_Window"))
			{
				node = node->FirstChild();
				break;
			}
				
			TiXmlNode *n = node->FirstChild();
			if (n)
				node = n;
			else
				node = node->NextSibling();
		}
	} while (node);

	do
	{
		if (node)
		{
			READCFG(m_RosterRect.left, "Left", 500);
			READCFG(m_RosterRect.top, "Top", 0);
			READCFG(m_RosterRect.right, "Right", 800);
			READCFG(m_RosterRect.bottom, "Bottom", 300);
			READCFG(m_RosterSnap, "Snap", FALSE);
			READCFG(m_IsAligned, "Aligned", FALSE);
			READCFG(m_IsTopMost, "TopMost", FALSE);
			READCFG(m_DoAlign, "DoAlign", FALSE);

			node = node->NextSibling();
		}
	} while  (node);
	*/
	// go to root
	while (m_Groups.size())
	{
		TreeGroup *tg = m_Groups[0];
//		free(tg->Name);
		m_Groups.erase(m_Groups.begin());
		delete tg;
	}
	if (xml.FindElem("Wippien"))
	{
		xml.IntoElem();
		
		m_ShowInTaskbar = FALSE;
		if (xml.FindElem("ShowInTaskbar"))
		{
			data = xml.GetData();
			if (data == "1")
				m_ShowInTaskbar = TRUE;
		}

		if (xml.FindElem("SoundOn"))
		{
			m_SoundOn = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_SoundOn = TRUE;
		}
		else
			m_SoundOn = TRUE;

		if (xml.FindElem("DeleteContactsOnStartup"))
		{
			m_DeleteContactsOnStartup = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_DeleteContactsOnStartup = TRUE;
		}
		else
			m_DeleteContactsOnStartup = FALSE;

		if (xml.FindElem("DeleteContactsOnConnect"))
		{
			m_DeleteContactsOnConnect = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_DeleteContactsOnConnect = TRUE;
		}
		else
			m_DeleteContactsOnConnect = FALSE;

		m_AuthContacts = 1;
		if (xml.FindElem("AuthContacts"))
		{
			data = xml.GetData();
			m_AuthContacts = atoi(data);
		}

		if (xml.FindElem("AutoConnectVPNOnNetwork"))
		{
			m_AutoConnectVPNOnNetwork = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_AutoConnectVPNOnNetwork = TRUE;
		}
		else
			m_AutoConnectVPNOnNetwork = TRUE;

		if (xml.FindElem("AutoConnectVPNOnStartup"))
		{
			m_AutoConnectVPNOnStartup = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_AutoConnectVPNOnStartup = TRUE;
		}
		else
			m_AutoConnectVPNOnNetwork = TRUE;
		
		if (xml.FindElem("VoiceChat"))
		{
			m_EnableVoiceChat = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_EnableVoiceChat = TRUE;
		}
		else
			m_EnableVoiceChat = TRUE;

		if (xml.FindElem("CheckUpdate"))
		{
			m_CheckUpdate = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_CheckUpdate = TRUE;
		}
		else
			m_CheckUpdate = TRUE;

		m_CheckUpdateConnect = TRUE;
		if (xml.FindElem("CheckUpdateConnect"))
		{
			m_CheckUpdateConnect = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_CheckUpdateConnect = TRUE;
		}

		if (xml.FindElem("CheckUpdateTimed"))
		{
			m_CheckUpdateTimed = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_CheckUpdateTimed = TRUE;
		}
		else
			m_CheckUpdateTimed = TRUE;

		m_CheckUpdateTimedNum = 60;
		if (xml.FindElem("CheckUpdateTimedNum"))
		{
			data = xml.GetData();
			m_CheckUpdateTimedNum = atoi(data);
		}

		if (xml.FindElem("CheckUpdateSilently"))
		{
			m_CheckUpdateSilently = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_CheckUpdateSilently = TRUE;
		}
		else
			m_CheckUpdateSilently = FALSE;

		if (xml.FindElem("FixedMTU"))
		{
			m_FixedMTU = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_FixedMTU = TRUE;
		}
		else
			m_FixedMTU = FALSE;

		if (xml.FindElem("FixedMTUNum"))
		{
			data = xml.GetData();
			m_FixedMTUNum = atoi(data);
		}
		else
			m_FixedMTUNum = 1200;

		if (xml.FindElem("SortContacts"))
		{
			data = xml.GetData();
			m_SortContacts = atoi(data);
		}
		else
			m_SortContacts = 1;


		m_VoiceChatRecordingDevice = -1;
		if (xml.FindElem("VoiceChatRecordingDevice"))
		{
			data = xml.GetData();
			m_VoiceChatRecordingDevice = atoi(data);
		}

		m_VoiceChatPlaybackDevice = -1;
		if (xml.FindElem("VoiceChatPlaybackDevice"))
		{
			data = xml.GetData();
			m_VoiceChatPlaybackDevice = atoi(data);
		}

		m_LastOperatorMessageID = LASTOPERATORMSGID;
		if (xml.FindElem("LastOperatorMessageID"))
		{
			data = xml.GetData();
			m_LastOperatorMessageID = atoi(data);
		}

		m_JID.Empty();
		if (xml.FindElem("JID"))
		{
			data = xml.GetData();
			m_JID = data;
		}	

		m_UseSSLWrapper = TRUE;
		if (xml.FindElem("UseSSLWrapper"))
		{
			data = xml.GetData();
			m_UseSSLWrapper = atoi(data);
		}
		

		if (!m_Password.Length())
		{
			if (xml.FindElem("Password"))
			{
//			m_Password.Empty();
				data = xml.GetData();
				m_Password = data;
			}	
			else
			{
				memset(buff, 0, sizeof(buff));
//				if (EXECryptor_SecureRead("Password", buff))
//					m_Password = buff;
			}
		}

		m_ServerHost.Empty();
		if (xml.FindElem("ServerHost"))
		{
			data = xml.GetData();
			m_ServerHost = data;
		}

		m_ServerPort = 5222;
		if (xml.FindElem("ServerPort"))
		{
			data = xml.GetData();
			m_ServerPort = atoi(data);
		}

		m_UDPPort = 0;
		if (xml.FindElem("UDPPort"))
		{
			data = xml.GetData();
			m_UDPPort = atoi(data);
		}

		if (xml.FindElem("Mediator"))
		{
			data = xml.GetData();
			m_IPMediator = data;
		}
		else
			m_IPMediator = "mediator@wippien.com";

		if (xml.FindElem("LinkMediator"))
		{
			data = xml.GetData();
			m_LinkMediator = data;
		}
		else
			m_LinkMediator = "mediator.wippien.com";

		if (xml.FindElem("LinkMediatorPort"))
		{
			data = xml.GetData();
			m_LinkMediatorPort = atoi(data);
		}
		else
			m_LinkMediatorPort = 8000;
		
		if (xml.FindElem("ObtainIPAddress"))
		{
			data = xml.GetData();
			m_ObtainIPAddress = atoi(data);
		}
		else
			m_ObtainIPAddress = 1;
		
		
		m_AllowAnyMediator = TRUE;
		if (xml.FindElem("AllowMediator"))
		{
			data = xml.GetData();
			if (data != "1")
				m_AllowAnyMediator = FALSE;
		}
		m_AllowLinkMediatorToBeProvidedByIPMediator = TRUE;
		if (xml.FindElem("AllowLinkMediatorToBeProvidedByIP"))
		{
			data = xml.GetData();
			if (data != "1")
				m_AllowLinkMediatorToBeProvidedByIPMediator = FALSE;
		}
		
		m_UseIPMediator = TRUE;
		if (xml.FindElem("UseMediator"))
		{
			data = xml.GetData();
			if (data != "1")
				m_UseIPMediator = FALSE;
		}
		
		m_ShowMediatorOnContacts = FALSE;
		if (xml.FindElem("ShowMediator"))
		{
			data = xml.GetData();
			if (data == "1")
				m_ShowMediatorOnContacts = TRUE;
		}

		m_ShowContactPicture = TRUE;
		if (xml.FindElem("ShowContactPicture"))
		{
			m_ShowContactPicture = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_ShowContactPicture = TRUE;
		}

		m_ShowContactLastOnline = TRUE;
		if (xml.FindElem("ShowContactLastOnline"))
		{
			m_ShowContactLastOnline = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_ShowContactLastOnline = TRUE;
		}

/*		m_ShowContactName = TRUE;
		if (xml.FindElem("ShowContactName"))
		{
			m_ShowContactName = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_ShowContactName = TRUE;
		}
*/

		m_ShowContactIP = TRUE;
		if (xml.FindElem("ShowContactIP"))
		{
			m_ShowContactIP = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_ShowContactIP = TRUE;
		}

		m_ShowContactStatus = TRUE;
		if (xml.FindElem("ShowContactStatus"))
		{
			m_ShowContactStatus = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_ShowContactStatus = TRUE;
		}

		m_ShowMyPicture = TRUE;
		if (xml.FindElem("ShowMyPicture"))
		{
			m_ShowMyPicture = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_ShowMyPicture = TRUE;
		}

		m_ShowMyName = TRUE;
		if (xml.FindElem("ShowMyName"))
		{
			m_ShowMyName = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_ShowMyName = TRUE;
		}

		m_ShowMyIP = TRUE;
		if (xml.FindElem("ShowMyIP"))
		{
			m_ShowMyIP = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_ShowMyIP = TRUE;
		}

		m_ShowMyStatus = TRUE;
		if (xml.FindElem("ShowMyStatus"))
		{
			m_ShowMyStatus = FALSE;
			data = xml.GetData();
			if (data == "1")
				m_ShowMyStatus = TRUE;
		}


		m_TimestampMessages = FALSE;
		if (xml.FindElem("TimestampMessages"))
		{
			data = xml.GetData();
			if (data == "1")
				m_TimestampMessages = TRUE;
		}

		m_SnapToBorder = FALSE;
		if (xml.FindElem("SnapToBorder"))
		{
			data = xml.GetData();
			if (data == "1")
				m_SnapToBorder = TRUE;
		}


		m_ShowMessageHistory = FALSE;
		if (xml.FindElem("ShowMessageHistory"))
		{
			data = xml.GetData();
			if (data == "1")
				m_ShowMessageHistory = TRUE;
		}

		if (xml.FindElem("Icon"))
		{
			data = xml.GetData();
			Buffer in;
			in.Append(data, strlen(data));
			FromHex(&in, &m_Icon);
		}

		char icobuf[1024];
		strcpy(icobuf, m_CfgFilename);
		strcat(icobuf, ".png");
		handle = open(icobuf, O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
		if (handle != (-1))
		{
			m_Icon.Clear();
			do
			{
				i = read(handle, icobuf, 1024);
				if (i>0)
					m_Icon.Append(icobuf, i);
			} while (i>0);
			close(handle);
		}

		if (xml.FindElem("DoNotShow"))
		{
			data = xml.GetData();
			int i = strlen(data);
			if (i>MAXDONOTSHOWANYMORESETTINGS)
				i = MAXDONOTSHOWANYMORESETTINGS;
			memcpy(m_DoNotShow, data, i);
		}

		if (xml.FindElem("LastNetwork"))
		{
			data = xml.GetData();
			long mip = atol(data);
			memcpy(&m_MyLastNetwork, &mip, sizeof(long));
			m_MyLastNetmask &= 0x00FFFFFF; 
		}
		if (xml.FindElem("LastNetmask"))
		{
			data = xml.GetData();
			long mnm = atol(data);
			memcpy(&m_MyLastNetmask, &mnm, sizeof(long));
		}

/*		if (xml.FindElem("Key"))
		{
			data = xml.GetData();
			Buffer in, out;
			in.Append(data, strlen(data));
			FromHex(&in, &out);
			KeyFromBlob(&out);
		}
*/		
		if (xml.FindElem("Skin"))
		{
			m_Skin.Empty();
			data = xml.GetData();
			m_Skin = data;
		}

		if (xml.FindElem("Roster"))
		{
			xml.IntoElem();

			BOOL foundOff = FALSE;
			BOOL OffOpen = FALSE;
			BOOL foundGen = FALSE;
			while (xml.FindElem("Group"))
			{
				data = xml.GetData();
				char *a = (char *)malloc(strlen(data)+1);
				memset(a, 0, strlen(data)+1);
				memcpy(a, data, strlen(data));
				if (!strcmp(a, GROUP_OFFLINE))
				{
					if (xml.GetAttrib("Open") != _T(""))
						OffOpen = TRUE;
					foundOff = TRUE;
					free(a);
				}
				else
				{				
					TreeGroup *tg = new TreeGroup;
					tg->Item = NULL;
					tg->Open = FALSE;
					tg->Name = a;
					if (xml.GetAttrib("Open") != _T(""))
						tg->Open = TRUE;
					if (!strcmp(a, GROUP_GENERAL))
						foundGen = TRUE;
					PushGroupSorted(tg);
//					m_Groups.push_back(tg);
				}

			}

			if (!foundGen && m_Groups.size()<2)
			{
				char *a = (char *)malloc(strlen(GROUP_GENERAL)+1);
				memcpy(a, GROUP_GENERAL, strlen(GROUP_GENERAL)+1);
				TreeGroup *tg = new TreeGroup;
				tg->Item = NULL;
				tg->Open = FALSE;
				tg->Name = a;
				PushGroupSorted(tg);
//				m_Groups.push_back(tg);
			}
			char *a = (char *)malloc(strlen(GROUP_OFFLINE)+1);
			memcpy(a, GROUP_OFFLINE, strlen(GROUP_OFFLINE)+1);
			TreeGroup *tg = new TreeGroup;
			tg->Item = NULL;
			tg->Open = OffOpen;
			tg->Name = a;
			m_Groups.push_back(tg);

			xml.OutOfElem();
		}

		m_HiddenContactsBuffer.Clear();
		if (xml.FindElem("HiddenContacts"))
		{
			xml.IntoElem();

			while (xml.FindElem("JID"))
			{
				data = xml.GetData();
				char *to = NULL;
				int l1 = strlen(data);
				m_HiddenContactsBuffer.AppendSpace(&to, l1+1);
				memset(to, 0, l1+1);
				memcpy(to, data, l1);
				m_HiddenContacts.push_back(to - m_HiddenContactsBuffer.Ptr());
			}
			xml.OutOfElem();
		}

		if (xml.FindElem("Sounds"))
		{
			xml.IntoElem();

			_Notify.m_Online.Empty();
			if (xml.FindElem("ContactOnline"))
			{
				data = xml.GetData();
				_Notify.m_Online = data;
			}
			
			_Notify.m_Offline.Empty();
			if (xml.FindElem("ContactOffline"))
			{
				data = xml.GetData();
				_Notify.m_Offline = data;
			}
			
			_Notify.m_MsgIn.Empty();
			if (xml.FindElem("MessageIn"))
			{
				data = xml.GetData();
				_Notify.m_MsgIn = data;
			}
			
			_Notify.m_MsgOut.Empty();
			if (xml.FindElem("MessageOut"))
			{
				data = xml.GetData();
				_Notify.m_MsgOut = data;
			}
			
			_Notify.m_Error.Empty();
			if (xml.FindElem("Error"))
			{
				data = xml.GetData();
				_Notify.m_Error = data;
			}
			
			xml.OutOfElem();
		}


		if (xml.FindElem("AuthRequests"))
		{
			xml.IntoElem();

			while (xml.FindElem("JID"))
			{
				data = xml.GetData();
				CComBSTR d = data;
				m_AuthRequests.push_back(d.Copy());
			}
			xml.OutOfElem();
		}

		m_JabberDebugFile.Empty();
		if (xml.FindElem("JabberDebugFile"))
		{
			data = xml.GetData();
			m_JabberDebugFile = data;
		}

		m_SocketDebugFile.Empty();
		if (xml.FindElem("SocketDebugFile"))
		{
			data = xml.GetData();
			m_SocketDebugFile = data;
		}

		m_FunctionDebugFile.Empty();
		if (xml.FindElem("FunctionDebugFile"))
		{
			data = xml.GetData();
			m_FunctionDebugFile = data;
		}
		m_DeleteFunctionLogMb  = 10*1024*1024;
		if (xml.FindElem("DeleteFunctionLogMb"))
		{
			data = xml.GetData();
			m_DeleteFunctionLogMb = atol(data);
		}

		m_AutoAwayMinutes = 10;
		if (xml.FindElem("AutoAwayMinutes"))
		{
			data = xml.GetData();
			m_AutoAwayMinutes = atoi(data);
		}
		m_ExtendedAwayMinutes = 20;
		if (xml.FindElem("ExtendedAwayMinutes"))
		{
			data = xml.GetData();
			m_ExtendedAwayMinutes = atoi(data);
		}
		m_AutoDisconnectMinutes = 0;
		if (xml.FindElem("AutoDisconnectMinutes"))
		{
			data = xml.GetData();
			m_AutoDisconnectMinutes = atoi(data);
		}
		m_AutoAwayMessage.Empty();
		if (xml.FindElem("AutoAwayMessage"))
		{
			data = xml.GetData();
			m_AutoAwayMessage = data;
		}
		else
			m_AutoAwayMessage = AWAY_MESSAGE;

		m_ExtendedAwayMessage.Empty();
		if (xml.FindElem("ExtendedAwayMessage"))
		{
			data = xml.GetData();
			m_ExtendedAwayMessage = data;
		}
		else
			m_ExtendedAwayMessage = EXTAWAY_MESSAGE;

		m_AutoSetBack = FALSE;
		if (xml.FindElem("AutoSetBack"))
		{
			data = xml.GetData();
			if (data == "1")
				m_AutoSetBack = TRUE;
		}
		else
			m_AutoSetBack = TRUE;

		xml.OutOfElem();


		if (xml.FindElem("Message_Dialog_Window"))
		{
			xml.IntoElem();
			if (xml.FindElem("Left"))
			{
				data = xml.GetData();
				m_RosterRect.left = atoi(data);
			}			
			if (xml.FindElem("Top"))
			{
				data = xml.GetData();
				m_RosterRect.top = atoi(data);
			}			
			if (xml.FindElem("Right"))
			{
				data = xml.GetData();
				m_RosterRect.right = atoi(data);
			}			
			if (xml.FindElem("Bottom"))
			{
				data = xml.GetData();
				m_RosterRect.bottom = atoi(data);
			}			
			if (xml.FindElem("Snap"))
			{
				data = xml.GetData();
				m_RosterSnap = atoi(data);
			}			
			if (xml.FindElem("Aligned"))
			{
				data = xml.GetData();
				m_IsAligned = atoi(data);
			}			
			if (xml.FindElem("TopMost"))
			{
				data = xml.GetData();
				m_IsTopMost = atoi(data);
			}			
			if (xml.FindElem("DoAlign"))
			{
				data = xml.GetData();
				m_DoAlign = atoi(data);
			}			
			xml.OutOfElem();
		}
	}

	// also, read users
	// also read users
	if (!m_DeleteContactsOnStartup)
	{
		handle = open(m_UsrFilename, O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
		if (handle != (-1))
		{
			Buffer b;
			char buff[32768];
			int i;
			do
			{
				i = read(handle, buff, 32768);
				if (i>0)
					b.Append(buff, i);
			} while (i>0);
			close(handle);

			b.Append("\0",1);

			// now parse this
			CMarkup xml;
			xml.SetDoc(b.Ptr());
			CString data;

			// go to root
			while (xml.FindElem("User"))
			{
				xml.IntoElem();
				
				//CUser *user = new CUser();
#ifdef _WODVPNLIB
				CUser *user = new CUser();
#else
				CComObject<CUser> *user;
				CComObject<CUser>::CreateInstance(&user);
				user->AddRef();
#endif
				if (xml.FindElem("Name"))
				{
					data = xml.GetData();
					if (data.GetLength()<sizeof(user->m_JID))
					{
						strcpy(user->m_JID, data);
						strcpy(user->m_VisibleName , data);
					}
				}

				CComBSTR j = user->m_JID;
				if (j == m_IPMediator && !m_ShowMediatorOnContacts)
#ifdef _WODVPNLIB
					delete user;
#else
					user->Release();
#endif						
				else
				{
					// does this one already exist?
					if (_MainDlg.m_UserList.GetUserByJID(j))
					{
#ifdef _WODVPNLIB
						delete user;
#else
						user->Release();
#endif						
					}
					else
					{
						_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
						_MainDlg.m_UserList.m_Users.push_back(user);
					}


					if (xml.FindElem("VisibleName"))
					{
						data = xml.GetData();
						if (data.GetLength()<sizeof(user->m_VisibleName))
							strcpy(user->m_VisibleName, data);
					}

					if (xml.FindElem("Block"))
					{
						data = xml.GetData();
						if (data == "1")
							user->m_Block = TRUE;
						else
							user->m_Block = FALSE;
					}

					if (xml.FindElem("VCard"))
					{
						data = xml.GetData();
						long mnm = atol(data);
						memcpy(&user->m_GotVCard, &mnm, sizeof(long));
					}

					if (xml.FindElem("Group"))
					{
						data = xml.GetData();
						if (data.GetLength()<sizeof(user->m_Group))
							strcpy(user->m_Group, data);
//						if (!data.GetLength())
//							strcpy(user->m_Group, GROUP_GENERAL);
					}

					if (xml.FindElem("Email"))
					{
						data = xml.GetData();
						if (data.GetLength()<sizeof(user->m_Email))
							strcpy(user->m_Email, data);
					}

					if (xml.FindElem("LastOnline"))
					{
						data = xml.GetData();
						long lo = atoi(data);
						if (lo)
						{
							user->m_LastOnline = lo;
							user->SetSubtext();
						}
					}

					if (xml.FindElem("Chat_Dialog_Window"))
					{
						xml.IntoElem();
						if (xml.FindElem("Left"))
						{
							data = xml.GetData();
							user->m_ChatWindowRect.left = atoi(data);
						}			
						if (xml.FindElem("Top"))
						{
							data = xml.GetData();
							user->m_ChatWindowRect.top = atoi(data);
						}			
						if (xml.FindElem("Right"))
						{
							data = xml.GetData();
							user->m_ChatWindowRect.right = atoi(data);
						}			
						if (xml.FindElem("Bottom"))
						{
							data = xml.GetData();
							user->m_ChatWindowRect.bottom = atoi(data);
						}			
						xml.OutOfElem();
					}

					if (xml.FindElem("AllowMediatorIP"))
					{
						user->m_AllowedRemoteMediator = FALSE;
						data = xml.GetData();
						if (data == "1")
							user->m_AllowedRemoteMediator = TRUE;
					}
					if (xml.FindElem("AllowAnyIP"))
					{
						user->m_AllowedRemoteAny = FALSE;
						data = xml.GetData();
						if (data == "1")
							user->m_AllowedRemoteAny = TRUE;
					}
					xml.AddElem("Allowed_IPs");
					if (xml.FindElem("Allowed_IPs"))
					{
						xml.IntoElem();
						while (xml.FindElem("IP"))
						{
							data = xml.GetData();
							unsigned long x = inet_addr(data);
							if (x != INADDR_NONE)
							{
								IPAddressConnectionStruct *ips  = new IPAddressConnectionStruct();
								ips->Address = x;
								if (xml.GetAttrib("Allowed=") != _T(""))
									ips->Allowed = TRUE;
								else
									ips->Allowed = FALSE;

								ips->Ignored = FALSE;

								user->m_AllowedRemoteIPs.push_back(ips);
							}
						}
						xml.OutOfElem();
					}

					user->m_Changed = FALSE;
					user->m_ChangeNotify = FALSE;
					user->m_Online = FALSE;
					if (!strlen(user->m_StatusText))
						strcpy(user->m_StatusText, "Offline");

					xml.OutOfElem();
				}
			}
		}
	}

//	if (!m_RSA)
//	{
		// regenerate RSA
//		m_RSA = RSA_generate_key(1024,35,NULL,NULL);
//	}

	return TRUE;
}

void CSettings::ToHex(Buffer *in, Buffer *out)
{
	while (in->Len())
	{
		char *a = in->Ptr();
		unsigned char b = (unsigned char)*a;
		out->Append(&hex[b/16],1);
		out->Append(&hex[b%16],1);
		in->Consume(1);
	}
	in->Clear();
}

void CSettings::FromHex(Buffer *in, Buffer *out)
{
	char buff[3];
	buff[2] = 0;

	while (in->Len())
	{
		if (in->Len()<2)
			return;

		memcpy(buff, in->Ptr(), 2);
		strlwr(buff);

		char *s1, *s2;
		s1 = strchr(hex, buff[0]);
		s2 = strchr(hex, buff[1]);

		if (!s1 || !s2)
			return;

		buff[0] = (s1 - hex)*16 + (s2-hex);
		out->Append(buff,1);
		in->Consume(2);
	}
}

void CSettings::KeyToBlob(Buffer *out, BOOL withprivate)
{
	out->PutBignum2(m_RSA->e);
	out->PutBignum2(m_RSA->n);
	if (withprivate)
	{
		out->PutBignum2(m_RSA->p);
		out->PutBignum2(m_RSA->q);
		out->PutBignum2(m_RSA->dmp1);
		out->PutBignum2(m_RSA->dmq1);
		out->PutBignum2(m_RSA->iqmp);
		out->PutBignum2(m_RSA->d);
	}
}

/*
void CSettings::KeyFromBlob(Buffer *in)
{
	if (m_RSA)
		RSA_free(m_RSA);
	m_RSA = RSA_new();

	m_RSA->e = BN_new();
	in->GetBignum2(m_RSA->e);
	m_RSA->n = BN_new();
	in->GetBignum2(m_RSA->n);
	m_RSA->p = BN_new();
	in->GetBignum2(m_RSA->p);
	m_RSA->q = BN_new();
	in->GetBignum2(m_RSA->q);
	m_RSA->dmp1 = BN_new();
	in->GetBignum2(m_RSA->dmp1);
	m_RSA->dmq1 = BN_new();
	in->GetBignum2(m_RSA->dmq1);
	m_RSA->iqmp = BN_new();
	in->GetBignum2(m_RSA->iqmp);
	m_RSA->d = BN_new();
	in->GetBignum2(m_RSA->d);
}

*/

BOOL CSettings::Save(BOOL UserOnly)
{
	int handle;
	if (!UserOnly)
	{
		CMarkup xml;
		xml.AddElem("Wippien");

		xml.AddChildElem("ShowInTaskbar", m_ShowInTaskbar?"1":"0");
		xml.AddChildElem("SoundOn", m_SoundOn?"1":"0");
		xml.AddChildElem("DeleteContactsOnStartup", m_DeleteContactsOnStartup?"1":"0");
		xml.AddChildElem("DeleteContactsOnConnect", m_DeleteContactsOnConnect?"1":"0");
		xml.AddChildElem("AuthContacts", m_AuthContacts);
		xml.AddChildElem("AutoConnectVPNOnNetwork", m_AutoConnectVPNOnNetwork?"1":"0");
		xml.AddChildElem("AutoConnectVPNOnStartup", m_AutoConnectVPNOnStartup?"1":"0");
		xml.AddChildElem("VoiceChat", m_EnableVoiceChat?"1":"0");
		xml.AddChildElem("CheckUpdate", m_CheckUpdate?"1":"0");
		xml.AddChildElem("CheckUpdateConnect", m_CheckUpdateConnect?"1":"0");
		xml.AddChildElem("CheckUpdateTimed", m_CheckUpdateTimed?"1":"0");
		xml.AddChildElem("CheckUpdateTimedNum", m_CheckUpdateTimedNum);
		xml.AddChildElem("CheckUpdateSilently", m_CheckUpdateSilently?"1":"0");
		xml.AddChildElem("FixedMTU", m_FixedMTU?"1":"0");
		xml.AddChildElem("FixedMTUNum", m_FixedMTUNum);
		xml.AddChildElem("SortContacts", m_SortContacts);
		xml.AddChildElem("VoiceChatRecordingDevice", m_VoiceChatRecordingDevice);
		xml.AddChildElem("VoiceChatPlaybackDevice", m_VoiceChatPlaybackDevice);
		xml.AddChildElem("LastOperatorMessageID", m_LastOperatorMessageID);

		CComBSTR2 j = m_JID;
		xml.AddChildElem("JID", j.ToString());
		xml.AddChildElem("UseSSLWrapper", m_UseSSLWrapper?"1":"0");



		CComBSTR2 p = m_Password;
		
		// we write password to a config file

		Buffer aes;

		CComBSTR2 sf = m_SettingsFolder;
		aes.PutCString(sf.ToString());
		aes.PutCString(p.ToString());
		CComBSTR2 pp = m_PasswordProtectPassword;
		aes.PutCString(pp.ToString());
		if (m_PasswordProtectAll)
			aes.PutChar(1);
		else
			aes.PutChar(0);
		AESWrite(&aes);

		Buffer in, out;
/*			in.Append((char *)&_Settings.m_MAC, sizeof(MACADDR));
		ToHex(&in, &out);
		out.Append("\0",1);
		xml.AddChildElem("MAC", out.Ptr());
*/
		CComBSTR2 msh = m_ServerHost;
		xml.AddChildElem("ServerHost", msh.ToString());
		xml.AddChildElem("ServerPort", m_ServerPort);
		xml.AddChildElem("UDPPort", m_UDPPort);

		CComBSTR2 m = m_IPMediator;
		xml.AddChildElem("Mediator", m.ToString());
		m.Empty();
		m = m_LinkMediator;
		xml.AddChildElem("LinkMediator", m.ToString());
		xml.AddChildElem("LinkMediatorPort", m_LinkMediatorPort);
		xml.AddChildElem("ObtainIPAddress", m_ObtainIPAddress);
		xml.AddChildElem("AllowAnyMediator", m_AllowAnyMediator?"1":"0");
		xml.AddChildElem("AllowLinkMediatorToBeProvidedByIP", m_AllowLinkMediatorToBeProvidedByIPMediator?"1":"0");
		xml.AddChildElem("UseMediator", m_UseIPMediator?"1":"0");
		xml.AddChildElem("ShowMediator", m_ShowMediatorOnContacts?"1":"0");
		xml.AddChildElem("ShowContactPicture", m_ShowContactPicture?"1":"0");
		xml.AddChildElem("ShowContactLastOnline", m_ShowContactLastOnline?"1":"0");
//		xml.AddChildElem("ShowContactName", m_ShowContactName?"1":"0");
		xml.AddChildElem("ShowContactIP", m_ShowContactIP?"1":"0");
		xml.AddChildElem("ShowContactStatus", m_ShowContactStatus?"1":"0");
		xml.AddChildElem("ShowMyPicture", m_ShowMyPicture?"1":"0");
		xml.AddChildElem("ShowMyName", m_ShowMyName?"1":"0");
		xml.AddChildElem("ShowMyIP", m_ShowMyIP?"1":"0");
		xml.AddChildElem("ShowMyStatus", m_ShowMyStatus?"1":"0");		
		xml.AddChildElem("TimestampMessages", m_TimestampMessages?"1":"0");
		xml.AddChildElem("SnapToBorder", m_SnapToBorder?"1":"0");
		xml.AddChildElem("ShowMessageHistory", m_ShowMessageHistory?"1":"0");



/*		out.Clear();
		in.Append(m_Icon.Ptr(), m_Icon.Len());
		ToHex(&in, &out);
		out.Append("\0", 1);
		xml.AddChildElem("Icon", out.Ptr());
*/
		char icobuf[1024];
		strcpy(icobuf, m_CfgFilename);
		strcat(icobuf, ".png");
		handle = open(icobuf, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
		if (handle != (-1))
		{
			write(handle, m_Icon.Ptr(), m_Icon.Len());
			close(handle);
		}


		xml.AddChildElem("DoNotShow", m_DoNotShow);

		long mip;
		memcpy(&mip, &m_MyLastNetwork, sizeof(long));
		xml.AddChildElem("LastNetwork", mip);
		memcpy(&mip, &m_MyLastNetmask, sizeof(long));
		xml.AddChildElem("LastNetmask", mip);

/*		if (m_RSA)
		{
			in.Clear();
			KeyToBlob(&in, TRUE);
			out.Clear();
			ToHex(&in, &out);
			out.Append("\0", 1);
			xml.AddChildElem("Key", out.Ptr());
		}
*/		
		
		CComBSTR2 mssk = m_Skin;
		xml.AddChildElem("Skin", mssk.ToString());

		xml.AddChildElem("Roster");
		xml.IntoElem();

		for (int i=0;i<m_Groups.size();i++)
		{
			TreeGroup *tg = m_Groups[i];
			xml.AddChildElem("Group", tg->Name);
			BOOL exp = FALSE;
			if (_MainDlg.IsWindow())
			{
				TVITEM tv = {0};
				tv.hItem = tg->Item;
				tv.mask = TVIF_STATE;
				_MainDlg.m_UserList.GetItem(&tv);
				if (tv.state & TVIS_EXPANDED)
					exp = TRUE;
			}
			else
			if (tg->Open)
				exp = TRUE;

			if (exp)
				xml.AddChildAttrib("Open", "true");
		}
		xml.OutOfElem();

		xml.AddChildElem("HiddenContacts");
		xml.IntoElem();
		char *cb = m_HiddenContactsBuffer.Ptr();
		for (i=0;i<m_HiddenContacts.size();i++)
		{
			xml.AddChildElem("JID", cb + m_HiddenContacts[i]);
		}
		xml.OutOfElem();

		// sounds
		xml.AddChildElem("Sounds");
		xml.IntoElem();

		xml.AddChildElem("ContactOnline", ((CComBSTR2)_Notify.m_Online).ToString());
		xml.AddChildElem("ContactOffline", ((CComBSTR2)_Notify.m_Offline).ToString());
		xml.AddChildElem("MessageIn", ((CComBSTR2)_Notify.m_MsgIn).ToString());
		xml.AddChildElem("MessageOut", ((CComBSTR2)_Notify.m_MsgOut).ToString());
		xml.AddChildElem("Error", ((CComBSTR2)_Notify.m_Error).ToString());
		xml.OutOfElem();


		xml.AddChildElem("AuthRequests");
		xml.IntoElem();
		for (i=0;i<m_AuthRequests.size();i++)
		{
			CComBSTR2 b = m_AuthRequests[i];
			xml.AddChildElem("JID", b.ToString());
		}
		xml.OutOfElem();

		CComBSTR2 dbf1 = m_JabberDebugFile;
		xml.AddChildElem("JabberDebugFile", dbf1.ToString());

		CComBSTR2 dbf2 = m_SocketDebugFile;
		xml.AddChildElem("SocketDebugFile", dbf2.ToString());

		CComBSTR2 dbf21 = m_FunctionDebugFile;
		xml.AddChildElem("FunctionDebugFile", dbf21.ToString());
		xml.AddChildElem("DeleteFunctionLogMb", m_DeleteFunctionLogMb);

		xml.AddChildElem("AutoAwayMinutes", m_AutoAwayMinutes);
		xml.AddChildElem("ExtendedAwayMinutes", m_ExtendedAwayMinutes);
		xml.AddChildElem("AutoDisconnectMinutes", m_AutoDisconnectMinutes);
		
		CComBSTR2 dbf3 = m_AutoAwayMessage;
		xml.AddChildElem("AutoAwayMessage", dbf3.ToString());
		CComBSTR2 dbf4 = m_ExtendedAwayMessage;
		xml.AddChildElem("ExtendedAwayMessage", dbf4.ToString());

		xml.AddChildElem("AutoSetBack", m_AutoSetBack?"1":"0");

//		CComBSTR2 pp2 = m_PasswordProtectPassword;
//		xml.AddChildElem("PasswordProtect", pp2.ToString());
//		xml.AddChildElem("PasswordProtectAll", m_PasswordProtectAll?"1":"0");

		xml.AddElem("Message_Dialog_Window");
		xml.AddChildElem("Left", m_RosterRect.left);
		xml.AddChildElem("Top", m_RosterRect.top);
		xml.AddChildElem("Right", m_RosterRect.right);
		xml.AddChildElem("Bottom", m_RosterRect.bottom);
		xml.AddChildElem("Snap", m_RosterSnap);
		xml.AddChildElem("Aligned", m_IsAligned);
		xml.AddChildElem("TopMost", m_IsTopMost);
		xml.AddChildElem("DoAlign", m_DoAlign);

		CString csXML = xml.GetDoc();
		if (csXML)
		{
			char *xmldata = csXML.GetBuffer(0);
			int xmllen = csXML.GetLength();
			// save to file
			handle = open(m_CfgFilename, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
			if (handle != (-1))
			{
				write(handle, xmldata, xmllen);
				close(handle);
			}
		}		
	}

	Buffer out;

	CMarkup xml;
	for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
	{
		CUser *user = _MainDlg.m_UserList.m_Users[i];

		CComBSTR j = user->m_JID;
		if (!(j == m_IPMediator))
		{
			xml.AddElem("User");
			xml.IntoElem();
			xml.AddElem("Name", user->m_JID);
			xml.AddElem("VisibleName", user->m_VisibleName);
//			out.Clear();
//			Buffer in;
//			in.Append(user->m_Icon.Ptr(), user->m_Icon.Len());
//			ToHex(&in, &out);
//			out.Append("\0", 1);
//			xml.AddElem("Icon", out.Ptr());
			xml.AddElem("Block", user->m_Block?"1":"0");
//			if (user->m_StaticIP)
//				xml.AddElem("StaticIP", user->m_HisDHCPAddressOffset);
			
			long mip;
			memcpy(&mip, &user->m_GotVCard, sizeof(long));
			xml.AddElem("VCard", mip);

			xml.AddElem("Group", user->m_Group);
			xml.AddElem("Email", user->m_Email);
			xml.AddElem("LastOnline", user->m_LastOnline);

			xml.AddElem("Chat_Dialog_Window");
			xml.AddChildElem("Left", user->m_ChatWindowRect.left);
			xml.AddChildElem("Top", user->m_ChatWindowRect.top);
			xml.AddChildElem("Right", user->m_ChatWindowRect.right);
			xml.AddChildElem("Bottom", user->m_ChatWindowRect.bottom);

			xml.AddElem("AllowMediatorIP", user->m_AllowedRemoteMediator?"1":"0");
			xml.AddElem("AllowAnyIP", user->m_AllowedRemoteAny?"1":"0");
			xml.AddElem("Allowed_IPs");
			struct in_addr sa;
			for (int x=0;x<user->m_AllowedRemoteIPs.size();x++)
			{
				IPAddressConnectionStruct *ips = (IPAddressConnectionStruct *)user->m_AllowedRemoteIPs[x];
				if (!ips->Ignored)
				{
					sa.S_un.S_addr = ips->Address;
					xml.AddChildElem("IP", inet_ntoa(sa));
					if (ips->Allowed)
						xml.AddChildAttrib("Allowed", "1");
				}
			}

/*
			xml.AddChildElem("Time", user->m_Time);
*/
			xml.OutOfElem();
		}
	}

	CString csXML = xml.GetDoc();
	Buffer csb;
	csb.Append(csXML.GetBuffer(0), csXML.GetLength());
	if (csb.Len()>10) // if there's anything inside at all
	{
		handle = open(m_UsrFilename, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
		if (handle != (-1))
		{
			write(handle, csb.Ptr(), csb.Len());
			close(handle);
		}
	}
	return TRUE;
}

int CSettings::LoadTools(void)
{

	// read from file
	char buff[32768];
	strcpy(buff, m_CfgFilename);
	int i = strlen(buff);
	while (i>0 && m_CfgFilename[i]!='.')
			i--;
	if (i)
	{
		buff[i] = 0;
		strcat(buff, ".tools");

		int handle = open(buff, O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
		if (handle == (-1))
			return FALSE;

		Buffer b;
		int i;
		do
		{
			i = read(handle, buff, 32768);
			if (i>0)
				b.Append(buff, i);
		} while (i>0);
		close(handle);

		b.Append("\0",1);

		// now parse this
		CMarkup xml;
		xml.SetDoc(b.Ptr());
		CString data;

		while (xml.FindElem("Tool"))
		{
			xml.IntoElem();
			MenuTool *mt = new MenuTool;
			memset(mt, 0, sizeof(MenuTool));
			
			if (xml.FindElem("Menu"))
			{
				data = xml.GetData();
				char *a = (char *)malloc(strlen(data)+1);
				memset(a, 0, strlen(data)+1);
				memcpy(a, data, strlen(data));
				mt->Menu = a;
			}	

			if (xml.FindElem("Exec"))
			{
				data = xml.GetData();
				char *a = (char *)malloc(strlen(data)+1);
				memset(a, 0, strlen(data)+1);
				memcpy(a, data, strlen(data));
				mt->Exec = a;
			}	
			if (xml.FindElem("Icon"))
			{
				data = xml.GetData();
				char *a = (char *)malloc(strlen(data)+1);
				memset(a, 0, strlen(data)+1);
				memcpy(a, data, strlen(data));
				char *j = strchr(a, ',');
				if (!j)
					j = strchr(a, ';');
				if (j)
				{
					*j=0;
					j++;
					mt->IconID = atoi(j);
				}	
				mt->IconPath = a;
			}	
			while (xml.FindElem("Filter"))
			{
				data = xml.GetData();
				char *a = (char *)malloc(strlen(data)+1);
				memset(a, 0, strlen(data)+1);
				memcpy(a, data, strlen(data));

				if (xml.GetAttrib("type") == _T("Group"))
				{
					if (mt->FilterGroup)
						free(mt->FilterGroup);
					mt->FilterGroup = a;
				}
				else
					if (xml.GetAttrib("type") == _T("JID"))
					{
						if (mt->FilterJID)
							free(mt->FilterJID);
						mt->FilterJID=a;
					}
					else
					{
						if (mt->FilterVisibleName)
							free(mt->FilterVisibleName);
						mt->FilterVisibleName = a;
					}
			}

			if (xml.FindElem("State"))
			{
				data = xml.GetData();
				mt->State = atol(data);
			}

			m_MenuTools.push_back(mt);
			xml.OutOfElem();
		}
	}

	return TRUE;
}

void CSettings::PushGroupSorted(TreeGroup *grp)
{
	for (int i = 0; i < _Settings.m_Groups.size(); i++)
	{
		CSettings::TreeGroup *tg = (CSettings::TreeGroup *)_Settings.m_Groups[i];
		if (stricmp(tg->Name, grp->Name)>0 || !stricmp(tg->Name, GROUP_OFFLINE))
		{
			_Settings.m_Groups.insert(_Settings.m_Groups.begin()+i, grp);
			return;
		}
	}
	_Settings.m_Groups.push_back(grp);
}


BOOL CSettings::CheckPasswordProtect(void)
{
	if (m_NowProtected)
	{
		if (m_PasswordProtectPassword.Length())
		{
			CSettingsDlg::CEnterPassDialog ndlg;
			ndlg.DoModal();
			CComBSTR2 pp = m_PasswordProtectPassword;
			if (!strcmp(pp.ToString(), ndlg.m_Password))
				m_NowProtected = FALSE;

		}
		else
			m_NowProtected = FALSE;
	}
	if (m_NowProtected)
	{
		MessageBeep(-1000);
		return FALSE;
	}

	return TRUE;
}


void CSettings::AESRead(Buffer *out)
{
	HKEY hkey = NULL;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, REGISTRYKEY , 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		char buff[16384];

		DWORD regsz = REG_SZ;
		DWORD cbbuff = sizeof(buff);
		if (RegQueryValueEx(hkey, "", NULL, &regsz, (unsigned char *)buff, &cbbuff) == ERROR_SUCCESS)
		{
			Buffer t;
			t.Append(buff, cbbuff);
			AESDecrypt(&t, out);
		}
		RegCloseKey(hkey);
	}
}

void CSettings::AESWrite(Buffer *in)
{
	HKEY hkey = NULL;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, REGISTRYKEY , 0, KEY_ALL_ACCESS, &hkey) != ERROR_SUCCESS)
		RegCreateKey(HKEY_LOCAL_MACHINE, REGISTRYKEY, &hkey);

	if (hkey)
	{
		Buffer out;
		AESEncrypt(in, &out);
		RegSetValueEx(hkey, "", NULL, REG_SZ, (unsigned char *)out.Ptr(), out.Len());

		RegCloseKey(hkey);

	}
}

void CSettings::AESEncrypt(Buffer *in, Buffer *out)
{
	Buffer t;
	char buff[16];

	char *packet = in->Ptr();
	int len = in->Len();

	in->Append("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16);

	// let's determine size
	int j = len%16;
	int o = 0;
	while (o<len)
	{
		m_AES.rijndael_encrypt(&m_AES.myaes.enc, (unsigned char *)packet, (unsigned char *)buff);
		packet += 16;
		o += 16;
		t.Append(buff, 16);
	}

	in->ConsumeEnd(16);

	// now convert it to HEX
	ToHex(&t, out);
	out->Append("\0",1);
	out->ConsumeEnd(1);
}
void CSettings::AESDecrypt(Buffer *in, Buffer *out)
{
	Buffer t;
	char buff[16];

	FromHex(in, &t);
	char *packet = t.Ptr();
	int len = t.Len();

	if (len>0)
	{
		int o = 0;
		while (o<len)
		{
			m_AES.rijndael_decrypt(&m_AES.myaes.dec, (unsigned char *)packet, (unsigned char *)buff);
			packet += 16;
			o += 16;
			out->Append(buff, 16);
		}
	}
}

BOOL CSettings::IsHiddenContact(char *contact)
{
	char buff[16384];
	strcpy(buff, contact);
	char *a = strchr(buff, '/');
	if (a)
		*a = 0;

	char *cb = m_HiddenContactsBuffer.Ptr();
	for (int i=0;i<m_HiddenContacts.size();i++)
	{
		if (!stricmp(buff, cb+m_HiddenContacts[i]))
			return TRUE;
	}

	return FALSE;
}

char *trim(char *text)
{
	while (*text == ' ')
		text++;
	
	int i = strlen(text);
	if (i>0)
	{
		while (i>0 && text[i-1]==' ')
		{
			text[i-1]=0;
			i--;
		}
	}
	
	return text;
}