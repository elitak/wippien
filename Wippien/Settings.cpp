// Settings.cpp: implementation of the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Settings.h"
#include "MainDlg.h"
#include "Ethernet.h"
#include "Buffer.h"
#include "ChatRoom.h"
#include "SimpleXmlParser.h"
#include "ComBSTR2.h"
#include "Notify.h"
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include "VoiceChat.h"

#ifndef _WODVPNLIB
const char *hex="0123456789abcdef";
#else
extern const char *hex;
#endif

char *GROUP_GENERAL = "General";
char *GROUP_OFFLINE = "Offline";
char *AWAY_MESSAGE = "Away due to inactivity.";
char *EXTAWAY_MESSAGE = "Away for a loooong time.";

extern CMainDlg _MainDlg;
extern CNotify	_Notify;
extern CEthernet _Ethernet;
extern CVoiceChat _VoiceChat;

char *trim(char *text);

char *REGISTRYKEY = "Software\\Kresimir Petric\\Wippien";
extern const char * CONFIGURING_ADAPTER_TEXT;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSettings::CSettings()
{
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

	memset(m_DoNotShow, '0', sizeof(m_DoNotShow));
	m_DoNotShow[MAXDONOTSHOWANYMORESETTINGS] = 0;
	m_DoNotShow[DONOTSHOW_NOETHERNET] = '0';
	m_DoNotShow[DONOTSHOW_NOALLOWEXIT] = '1';

	memset(&m_RosterRect, 0, sizeof(RECT));

	m_MyLastNetwork = m_MyLastNetmask = 0;
	m_AllowAnyMediator = TRUE;
	m_IPProviderURL = "http://wippien.com/ip/?jid=";
	LinkMediatorStruct *st1 = AddLinkMediator("mediator.wippien.com", 8000);
	st1->Permanent = TRUE;
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
	memset(m_ChatRoomFilename, 0, sizeof(m_ChatRoomFilename));

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
	strcpy(m_ChatRoomFilename, m_CfgFilename);
	strcpy(m_UserImagePath, m_CfgFilename);
	strcpy(m_HistoryPath, m_CfgFilename);
	strcpy(m_LanguagePath, m_MyPath);

	
	
	strcat(m_CfgFilename, "\\Wippien.config");
	strcat(m_UsrFilename, "\\Wippien.users");
	strcat(m_ChatRoomFilename, "\\Wippien.rooms");
	strcat(m_MyPath, "\\");
	strcat(m_UserImagePath, "\\Images\\");
	strcat(m_HistoryPath, "\\History\\");
	strcat(m_LanguagePath, "\\Language\\");

	m_RSA = RSA_generate_key(1024,35,NULL,NULL);

	CreateDirectory(m_LanguagePath, NULL); // language folder
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
	m_ShowUpdaterMessages = TRUE;
	m_CheckUpdateConnect = FALSE;
	m_CheckUpdateTimed = TRUE;
	m_CheckUpdateTimedNum = 60;
	m_NowProtected = FALSE;
	m_SnapToBorder = FALSE;
	m_AutoHide = TRUE;
	m_AutoHideSeconds = 20;

	m_DeleteContactsOnStartup = FALSE;
	m_DeleteContactsOnConnect = FALSE;
	m_AutoConnectVPNOnNetwork = TRUE;
	m_AutoConnectVPNOnStartup = TRUE;
	m_ShowNotificationPopup = TRUE;

	m_FixedMTU = FALSE;
	m_FixedMTUNum = 1200;


	m_ShowContactPicture = m_ShowContactName = m_ShowContactIP = m_ShowContactStatus = m_ShowContactLastOnline = m_ShowContactActivity = TRUE;
	m_ShowMyPicture = m_ShowMyName = m_ShowMyIP = m_ShowMyStatus = TRUE;

	m_SortContacts = 1;
	m_AuthContacts = 1;
	m_UseSSLWrapper = FALSE;
	m_UsePowerOptions = TRUE;

	m_UpdateURL = "http://wippien.com/Download/update.php";

	m_FirewallDefaultAllowRule = TRUE;
	m_DisconnectEthernetOnExit = FALSE;
	m_LanguageEnglishTotal = 0;
	m_LanguageFileVersion = 0;
	m_LanguageEngFileVersion = 0;
	m_LanguageOther = new Buffer();
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
	while (m_LinkMediators.size())
	{
		LinkMediatorStruct *lms = (LinkMediatorStruct *)m_LinkMediators[0];
		free(lms->Host);
		delete lms;
		m_LinkMediators.erase(m_LinkMediators.begin());
	}

	delete m_LanguageOther;
}

CXmlEntity *CSettings::ReadSettingsCfg(CXmlEntity *own, char *Name, BOOL *Value, BOOL default_value)
{
	CXmlEntity *ent = CXmlEntity::FindByName(own, Name, 1);
	if (ent)
	{
		*Value = atoi(ent->Value);
		return ent;
	}
	else
		*Value = default_value;
	return NULL;
}
CXmlEntity *CSettings::ReadSettingsCfg(CXmlEntity *own, char *Name, long *Value, long default_value)
{
	CXmlEntity *ent = CXmlEntity::FindByName(own, Name, 1);
	if (ent)
	{
		*Value = atol(ent->Value);
		return ent;
	}
	else
		*Value = default_value;
	return NULL;
}
CXmlEntity *CSettings::ReadSettingsCfg(CXmlEntity *own, char *Name, unsigned long *Value, unsigned long default_value)
{
	CXmlEntity *ent = CXmlEntity::FindByName(own, Name, 1);
	if (ent)
	{
		*Value = atol(ent->Value);
		return ent;
	}
	else
		*Value = default_value;
	return NULL;
}
CXmlEntity *CSettings::ReadSettingsCfg(CXmlEntity *own, char *Name, CComBSTR &Value, char *default_value)
{
	CXmlEntity *ent = CXmlEntity::FindByName(own, Name, 1);
	if (ent)
	{
		CComBSTR2 v;
		v.FromUTF8String(ent->Value, strlen(ent->Value));
		Value.Empty();
		Value.Attach(v.Detach());
		return ent;
	}
	else
	{
		CComBSTR2 v;
		v.FromUTF8String(default_value, strlen(default_value));
		Value.Empty();
		Value.Attach(v.Detach());
	}
	return NULL;
}
CXmlEntity *CSettings::ReadSettingsCfg(CXmlEntity *own, char *Name, char *Value, char *default_value)
{
	CXmlEntity *ent = CXmlEntity::FindByName(own, Name, 1);
	if (ent)
	{
		strcpy(Value, ent->Value);
		return ent;
	}
	else
		strcpy(Value, default_value);
	return NULL;
}
CXmlEntity *CSettings::ReadSettingsCfg(CXmlEntity *own, char *Name, Buffer *Value)
{
	CXmlEntity *ent = CXmlEntity::FindByName(own, Name, 1);
	if (ent)
	{
		Buffer in;
		in.Append(ent->Value);
		FromHex(&in, Value);
		return ent;
	}
	else
		Value->Clear();
	return NULL;
}

int CSettings::LoadConfig(void)
{
	char buff[32768];

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

	// go to root
	while (m_Groups.size())
	{
		TreeGroup *tg = m_Groups[0];
		//		free(tg->Name);
		m_Groups.erase(m_Groups.begin());
		delete tg;
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

	b.Append("\0",1);

	CXmlParser xmlparser;
	CXmlEntity *start = xmlparser.Parse(&b);
	if (start)
	{
		CXmlEntity *wip = CXmlEntity::FindByName(start, "Wippien", 1);
		if (wip)
		{
			CXmlEntity *ent = NULL;
			// check language
			CXmlEntity *langent = CXmlEntity::FindByName(wip, "Language", 1);
			if (langent)
			{
				m_Language = langent->Value;
				m_LanguageFileVersion = 0;
			}
			if (!m_Language.Length())
			{
				// if local is available, use it.. 
				int lcid = GetUserDefaultLCID();
				char buff2[8192];
				if (GetLocaleInfo(lcid, LOCALE_SENGLANGUAGE, buff2, sizeof(buff2)))
				{
					m_Language = buff2;
					m_LanguageFileVersion = 0;
				}
			}
			if (m_Language.Length() && !m_LanguageOther->Len())
			{
				CComBSTR2 ml = m_Language;
				LoadLanguage(ml.ToString());
			}
			ReadSettingsCfg(wip, "ShowInTaskbar", &m_ShowInTaskbar, FALSE);
			ReadSettingsCfg(wip, "SoundOn", &m_SoundOn, TRUE);
			ReadSettingsCfg(wip, "DeleteContactsOnStartup", &m_DeleteContactsOnStartup, FALSE);
			ReadSettingsCfg(wip, "DeleteContactsOnConnect", &m_DeleteContactsOnConnect, FALSE);
			ReadSettingsCfg(wip, "AuthContacts", &m_AuthContacts, 1);
			ReadSettingsCfg(wip, "AutoConnectVPNOnNetwork", &m_AutoConnectVPNOnNetwork, TRUE);
			ReadSettingsCfg(wip, "AutoConnectVPNOnStartup", &m_AutoConnectVPNOnStartup, TRUE);
			ReadSettingsCfg(wip, "ShowNotificationPopup", &m_ShowNotificationPopup, TRUE);
			ReadSettingsCfg(wip, "CheckUpdate", &m_CheckUpdate, TRUE);
			ReadSettingsCfg(wip, "CheckUpdateConnect", &m_CheckUpdateConnect, TRUE);
			ReadSettingsCfg(wip, "CheckUpdateTimed", &m_CheckUpdateTimed, TRUE);
			ReadSettingsCfg(wip, "CheckUpdateTimedNum", &m_CheckUpdateTimedNum, 60);
			ReadSettingsCfg(wip, "CheckUpdateSilently", &m_CheckUpdateSilently, FALSE);
			ReadSettingsCfg(wip, "ShowUpdaterMessages", &m_ShowUpdaterMessages, TRUE);
			ReadSettingsCfg(wip, "UsePowerOptions", &m_UsePowerOptions, TRUE);
			ReadSettingsCfg(wip, "DisconnectEthernetOnExit", &m_DisconnectEthernetOnExit, FALSE);
			ReadSettingsCfg(wip, "FixedMTU", &m_FixedMTU, FALSE);
			ReadSettingsCfg(wip, "FixedMTUNum", &m_FixedMTUNum, 1200);
			ReadSettingsCfg(wip, "SortContacts", &m_SortContacts, 1);
			ReadSettingsCfg(wip, "LastOperatorMessageID", &m_LastOperatorMessageID, LASTOPERATORMSGID);
			ReadSettingsCfg(wip, "JID", m_JID, "");
			CComBSTR2 mj = m_JID;
			char *mj2 = mj.ToUTF8String();
			char *mj3 = strchr(mj2, '@');
			if (mj3)
				*mj3 = NULL;
			ReadSettingsCfg(wip, "Nick", m_Nick, mj2);
			if (!m_Nick.Length()) m_Nick = mj2;
			if (!m_Password.Length())
			{
				ReadSettingsCfg(wip, "Password", m_Password, "");
			}
			ReadSettingsCfg(wip, "UseSSLWrapper", &m_UseSSLWrapper, FALSE);
			ReadSettingsCfg(wip, "Resource", m_Resource, WIPPIENIM);
			if (!m_Resource.Length())
			{
				m_Resource = WIPPIENIM;
			}
			m_StaticResource = m_Resource;

			ReadSettingsCfg(wip, "ServerHost", m_ServerHost, "");
			ReadSettingsCfg(wip, "ServerPort", &m_ServerPort, 5222);
			ReadSettingsCfg(wip, "UDPPort", &m_UDPPort, 0);
			ReadSettingsCfg(wip, "IPProviderURL", m_IPProviderURL, "http://wippien.com/ip/?jid=");
			ReadSettingsCfg(wip, "UpdateURL", m_UpdateURL, "http://wippien.com/Download/update.php");
			CComBSTR2 mlm;
			int mlmport = 0;
			ReadSettingsCfg(wip, "LinkMediator", mlm, "mediator.wippien.com");
			ReadSettingsCfg(wip, "LinkMediatorPort", &mlmport, 8000);
			LinkMediatorStruct *st = AddLinkMediator(mlm.ToString(), mlmport);
			st->Permanent = TRUE;

			ReadSettingsCfg(wip, "ObtainIPAddress", &m_ObtainIPAddress, 1);
			ReadSettingsCfg(wip, "AllowMediator", &m_AllowAnyMediator, TRUE);
			ReadSettingsCfg(wip, "ShowContactPicture", &m_ShowContactPicture, TRUE);
			ReadSettingsCfg(wip, "ShowContactLastOnline", &m_ShowContactLastOnline, TRUE);
			ReadSettingsCfg(wip, "ShowContactActivity", &m_ShowContactActivity, TRUE);
			ReadSettingsCfg(wip, "ShowContactIP", &m_ShowContactIP, TRUE);
			ReadSettingsCfg(wip, "ShowContactStatus", &m_ShowContactStatus, TRUE);
			ReadSettingsCfg(wip, "ShowMyPicture", &m_ShowMyPicture, TRUE);
			ReadSettingsCfg(wip, "ShowMyName", &m_ShowMyName, TRUE);
			ReadSettingsCfg(wip, "ShowMyIP", &m_ShowMyIP, TRUE);
			ReadSettingsCfg(wip, "ShowMyStatus", &m_ShowMyStatus, TRUE);
			ReadSettingsCfg(wip, "TimestampMessages", &m_TimestampMessages, TRUE);
			ReadSettingsCfg(wip, "SnapToBorder", &m_SnapToBorder, FALSE);
			ReadSettingsCfg(wip, "AutoHideOnInactivity", &m_AutoHide, TRUE);
			ReadSettingsCfg(wip, "AutoHideOnInactivitySeconds", &m_AutoHideSeconds, 20);
			ReadSettingsCfg(wip, "ShowMessageHistory", &m_ShowMessageHistory, TRUE);
			ReadSettingsCfg(wip, "FirewallDefaultAllowRule", &m_FirewallDefaultAllowRule, TRUE);		
			ReadSettingsCfg(wip, "Skin", m_Skin, "");
			ReadSettingsCfg(wip, "VoiceChatEnabled", &_VoiceChat.m_Enabled, FALSE);
			ReadSettingsCfg(wip, "VoiceChatLocalEcho", &_VoiceChat.m_LocalEcho, FALSE);
			ReadSettingsCfg(wip, "VoiceChatVadThreshold", &_VoiceChat.m_VadThreshold, 0);
			ReadSettingsCfg(wip, "VoiceChatPlaybackDevice", &_VoiceChat.m_WaveOutDevice, 0);
			ReadSettingsCfg(wip, "VoiceChatRecordingDevice", &_VoiceChat.m_WaveInDevice, 0);
			CComBSTR2 donotshow;
			ReadSettingsCfg(wip, "DoNotShow", donotshow, "00000000");
			int dnsi = donotshow.Length();
			if (dnsi>MAXDONOTSHOWANYMORESETTINGS)
				dnsi=MAXDONOTSHOWANYMORESETTINGS;
			memcpy(m_DoNotShow, donotshow.ToString(), dnsi);
			ReadSettingsCfg(wip, "LastNetwork", &m_MyLastNetwork, 0);
			m_MyLastNetmask &= 0x00FFFFFF;
			ReadSettingsCfg(wip, "LastNetmask", &m_MyLastNetmask, FALSE);
			ReadSettingsCfg(wip, "Icon", &m_Icon);

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

			CXmlEntity *rst = CXmlEntity::FindByName(wip, "Roster", 1);
			if (rst)
			{
				CXmlEntity *ent = NULL;
				BOOL foundOff = FALSE;
				BOOL OffOpen = FALSE;
				BOOL foundGen = FALSE;
				do 
				{
					ent = CXmlEntity::FindByName(rst, "Group", 1);
					if (ent)
					{
						CXmlEntity *entf = NULL;
						char *a = (char *)malloc(strlen(ent->Value)+1);
						memset(a, 0, strlen(ent->Value)+1);
						memcpy(a, ent->Value, strlen(ent->Value));
						if (!strcmp(a, GROUP_OFFLINE))
						{

							entf = CXmlEntity::FindAttrByName(ent, "Open");
							if (entf && !strcmp(entf->Value, "true"))
								OffOpen = TRUE;
							foundOff = TRUE;
							free(a);
						}
						else
						{				
							TreeGroup *tg = new TreeGroup;
							tg->Item = NULL;
							tg->Open = FALSE;
							tg->Block = FALSE;
							tg->Name = a;
							tg->VisibleName = a;
							tg->Temporary = FALSE;
							tg->CountBuff[0] = 0;
							entf = CXmlEntity::FindAttrByName(ent, "Open");
							if (entf && !strcmp(entf->Value, "true"))
								tg->Open = TRUE;
							entf = CXmlEntity::FindAttrByName(ent, "Block");
							if (entf && !strcmp(entf->Value, "true"))
								tg->Block = TRUE;
							if (!strcmp(a, GROUP_GENERAL))
								foundGen = TRUE;
							PushGroupSorted(tg);						
						}
						ent->Name[0] = 0;
					}
				} while (ent);

				if (!foundGen && m_Groups.size()<2)
				{
					char *a = (char *)malloc(strlen(GROUP_GENERAL)+1);
					memcpy(a, GROUP_GENERAL, strlen(GROUP_GENERAL)+1);
					TreeGroup *tg = new TreeGroup;
					tg->Item = NULL;
					tg->Open = FALSE;
					tg->Block = FALSE;
					tg->Name = a;
					tg->VisibleName = a;
					tg->CountBuff[0] = 0;
					tg->Temporary = FALSE;
					PushGroupSorted(tg);
				}
				char *a = (char *)malloc(strlen(GROUP_OFFLINE)+1);
				memcpy(a, GROUP_OFFLINE, strlen(GROUP_OFFLINE)+1);
				TreeGroup *tg = new TreeGroup;
				tg->Item = NULL;
				tg->Temporary = FALSE;
				tg->Block = FALSE;
				tg->Open = OffOpen;
				tg->Name = a;
				tg->VisibleName = a;
				tg->CountBuff[0] = 0;
				PushGroupSorted(tg);

			}

			CXmlEntity *lnm = NULL;
			do 
			{
				lnm = CXmlEntity::FindByName(wip, "Mediator", 1);
				if (lnm)
				{
					CComBSTR2 h;
					ReadSettingsCfg(lnm, "Host", h, "");
					if (h.ToString())
					{
						int port = 0, valid = 1;
						ReadSettingsCfg(lnm, "Port", &port, 0);
						ReadSettingsCfg(lnm, "Valid", &valid, 1);
						if (h.Length() && port)
						{
							LinkMediatorStruct *st = AddLinkMediator(h.ToString(), port);
							st->Permanent = TRUE;
						}
					}
					lnm->Name[0] = 0;
				}
			} while (lnm);

			m_HiddenContactsBuffer.Clear();
			CXmlEntity *hid = CXmlEntity::FindByName(wip, "HiddenContacts", 1);
			if (hid)
			{
				CXmlEntity *ent = NULL;
				do 
				{
					ent = CXmlEntity::FindByName(hid, "JID", 1);
					if (ent)
					{
						char *to = NULL;
						int l1 = strlen(ent->Value);
						m_HiddenContactsBuffer.AppendSpace(&to, l1+1);
						memset(to, 0, l1+1);
						memcpy(to, ent->Value, l1);
						m_HiddenContacts.push_back(to - m_HiddenContactsBuffer.Ptr());
						ent->Name[0] = 0;
					}
				} while (ent);
			}

			CXmlEntity *fwr = CXmlEntity::FindByName(wip, "FirewallRules", 1);
			if (fwr)
			{
				while (m_FirewallRules.size())
				{
					FirewallStruct *fs = (FirewallStruct *)m_FirewallRules[0];
					delete fs;
					m_FirewallRules.erase(m_FirewallRules.begin());
				}
				CXmlEntity *ent = NULL;
				do 
				{
					ent = CXmlEntity::FindByName(fwr, "Rule", 1);
					if (ent)
					{
						int proto = 0, port = 0;
						CXmlEntity *attr = CXmlEntity::FindAttrByName(ent, "Proto");
						if (attr)
							proto = atol(attr->Value);
						attr = CXmlEntity::FindAttrByName(ent, "Port");
						if (attr)
							port = atol(attr->Value);
						ent->Name[0] = 0;
						if (port && proto || proto == IPPROTO_ICMP)
						{
							FirewallStruct *fs = new FirewallStruct();
							fs->Port = port;
							fs->Proto = proto;
							m_FirewallRules.push_back(fs);
						}
					}
				} while (ent);
				_Ethernet.m_FirewallRulesChanged = TRUE;
			}

			
			CXmlEntity *snd = CXmlEntity::FindByName(wip, "Sounds", 1);
			if (snd)
			{
				ReadSettingsCfg(snd, "ContactOnline", _Notify.m_Online, "");
				ReadSettingsCfg(snd, "ContactOffline", _Notify.m_Offline, "");
				ReadSettingsCfg(snd, "MessageIn", _Notify.m_MsgIn, "");
				ReadSettingsCfg(snd, "MessageOut", _Notify.m_MsgOut, "");
				ReadSettingsCfg(snd, "Error", _Notify.m_Error, "");
			}

			CXmlEntity *auth = CXmlEntity::FindByName(wip, "AuthRequests", 1);
			if (auth)
			{
				// delete old ones
				while (m_AuthRequests.size())
				{
					BSTR b = m_AuthRequests[0];
					m_AuthRequests.erase(m_AuthRequests.begin());
					::SysFreeString(b);
				}

				CXmlEntity *ent = NULL;
				do 
				{
					ent = CXmlEntity::FindByName(auth, "JID", 1);
					if (ent)
					{
						CComBSTR2 d;
						d.FromUTF8String(ent->Value, strlen(ent->Value));
						m_AuthRequests.push_back(d.Copy());
						ent->Name[0] = 0;
					}
				} while (ent);
			}

			ReadSettingsCfg(wip, "JabberDebugFile", m_JabberDebugFile, "");
			ReadSettingsCfg(wip, "SocketDebugFile", m_SocketDebugFile, "");
			ReadSettingsCfg(wip, "VPNSocketDebugFolder", m_VPNSocketDebugFolder, "");
			ReadSettingsCfg(wip, "FunctionDebugFile", m_FunctionDebugFile, "");
			ReadSettingsCfg(wip, "DeleteFunctionLogMb", &m_DeleteFunctionLogMb, 10*1024*1024);
			ReadSettingsCfg(wip, "AutoAwayMinutes", &m_AutoAwayMinutes, 10);
			ReadSettingsCfg(wip, "ExtendedAwayMinutes", &m_ExtendedAwayMinutes, 20);
			ReadSettingsCfg(wip, "AutoDisconnectMinutes", &m_AutoDisconnectMinutes, 0);
			ReadSettingsCfg(wip, "AutoAwayMessage", m_AutoAwayMessage, "");
			if (!m_AutoAwayMessage.Length()) m_AutoAwayMessage =AWAY_MESSAGE;
			ReadSettingsCfg(wip, "ExtendedAwayMessage", m_ExtendedAwayMessage, "");
			if (!m_ExtendedAwayMessage.Length()) m_ExtendedAwayMessage= EXTAWAY_MESSAGE;
			ReadSettingsCfg(wip, "AutoSetBack", &m_AutoSetBack, TRUE);
		}
		CXmlEntity *wnd = CXmlEntity::FindByName(start, "Message_Dialog_Window", 1);
		if (wnd)
		{
			ReadSettingsCfg(wnd, "Left", &m_RosterRect.left, m_RosterRect.left);
			ReadSettingsCfg(wnd, "Top", &m_RosterRect.top, m_RosterRect.top);
			ReadSettingsCfg(wnd, "Right", &m_RosterRect.right, m_RosterRect.right);
			ReadSettingsCfg(wnd, "Bottom", &m_RosterRect.bottom, m_RosterRect.bottom);
			ReadSettingsCfg(wnd, "Snap", &m_RosterSnap, m_RosterSnap);
			ReadSettingsCfg(wnd, "Aligned", &m_IsAligned, m_IsAligned);
			ReadSettingsCfg(wnd, "TopMost", &m_IsTopMost, m_IsTopMost);
			ReadSettingsCfg(wnd, "DoAlign ", &m_DoAlign, m_DoAlign);
		}


	}
	delete start; // delete cxmlentity
	return TRUE;
}

int CSettings::LoadRooms(void)
{
	// read rooms
	int handle = open(m_ChatRoomFilename, O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
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

		CXmlParser xmlparser;
		CXmlEntity *start = xmlparser.Parse(&b);
		if (start)
		{
			CXmlEntity *ent = NULL;
			do 
			{
				ent = CXmlEntity::FindByName(start, "ChatRoom", 1);
				if (ent)
				{
					CChatRoom *room = new CChatRoom();

					ReadSettingsCfg(ent, "Name", room->m_JID, "");
					ReadSettingsCfg(ent, "Nick", room->m_Nick, "");
					ReadSettingsCfg(ent, "Block", &room->m_Block, TRUE);
					char buff2[32768];
					ReadSettingsCfg(ent, "Password", buff2, "");
					if (*buff2)
					{
						Buffer in, out;
						in.Append(buff2);
						_Settings.AESDecrypt(&in, &out);
						strcpy(room->m_Password, out.Ptr());
					}


					room->m_DoOpen = TRUE;
					ent->Name[0] = 0;

					if (*room->m_JID)
						_MainDlg.m_ChatRooms.push_back(room);
					else
						delete room;
				}
			} while (ent);
		}
		delete start; // delete cxmlentity
	}
	return TRUE;
}

int CSettings::LoadUsers(void)
{
	// also read users
	if (!m_DeleteContactsOnStartup)
	{
		int handle = open(m_UsrFilename, O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
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

			CXmlParser xmlparser;
			CXmlEntity *start = xmlparser.Parse(&b);
			if (start)
			{
				CXmlEntity *ent = NULL;
				do 
				{
					ent = CXmlEntity::FindByName(start, "User", 1);
					if (ent)
					{
#ifdef _WODVPNLIB
						CUser *user = new CUser();
#else
						CComObject<CUser> *user;
						CComObject<CUser>::CreateInstance(&user);
						user->AddRef();
#endif

						ReadSettingsCfg(ent, "Name", user->m_JID, "");
						ReadSettingsCfg(ent, "VisibleName", user->m_VisibleName, user->m_JID);
						user->m_bstrVisibleName.FromUTF8String(user->m_VisibleName);
						ReadSettingsCfg(ent, "Block", &user->m_Block, FALSE);
						ReadSettingsCfg(ent, "VCard", &user->m_GotVCard, 0);
						ReadSettingsCfg(ent, "Group", user->m_Group, "");
						if (! *user->m_Group) strcpy(user->m_Group, GROUP_GENERAL);
						ReadSettingsCfg(ent, "Email", user->m_Email, "");
						ReadSettingsCfg(ent, "LastOnline", &user->m_LastOnline, 0);
						user->SetSubtext();
						ReadSettingsCfg(ent, "AllowMediatorIP", &user->m_AllowedRemoteMediator, TRUE);
						ReadSettingsCfg(ent, "AllowAnyIP", &user->m_AllowedRemoteAny, TRUE);

						CXmlEntity *wnd = CXmlEntity::FindByName(ent, "Chat_Dialog_Window", 1);
						if (wnd)
						{
							ReadSettingsCfg(wnd, "Left", &user->m_ChatWindowRect.left, user->m_ChatWindowRect.left);
							ReadSettingsCfg(wnd, "Top", &user->m_ChatWindowRect.top, user->m_ChatWindowRect.top);
							ReadSettingsCfg(wnd, "Right", &user->m_ChatWindowRect.right, user->m_ChatWindowRect.right);
							ReadSettingsCfg(wnd, "Bottom", &user->m_ChatWindowRect.bottom, user->m_ChatWindowRect.bottom);
						}

						CXmlEntity *aip = CXmlEntity::FindByName(ent, "Allowed_IPs", 1);
						if (aip)
						{
							CXmlEntity *ip = NULL;
							do 
							{
								ip = CXmlEntity::FindByName(aip, "IP", 1);
								if (ip)
								{
									unsigned long x = inet_addr(ip->Value);
									if (x != INADDR_NONE)
									{
										IPAddressConnectionStruct *ips  = new IPAddressConnectionStruct();
										ips->Address = x;
										CXmlEntity *attr = CXmlEntity::FindAttrByName(ip, "Allowed");
										if (attr)
											ips->Allowed = TRUE;
										else
											ips->Allowed = FALSE;
										
										ips->Ignored = FALSE;
										
										user->m_AllowedRemoteIPs.push_back(ips);
									}
									ip->Name[0] = 0;
								}
							} while (ip);
						}

						CComBSTR j = user->m_JID;
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
							
							user->m_Changed = FALSE;
							user->m_ChangeNotify = FALSE;
							user->m_Online = FALSE;
							if (!strlen(user->m_StatusText))
								strcpy(user->m_StatusText, _Settings.Translate("Offline"));
						 }

						ent->Name[0] = 0;
					}
				} while (ent);
			}
			delete start; // delete cxmlentity
		}
	}
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

BOOL CSettings::Save(void)
{
	if (SaveConfig())
		if (SaveUsers())
			return SaveRooms();

	return FALSE;
}

BOOL CSettings::SaveConfig(void)
{
	int handle;
	Buffer x;
	x.Append("<Wippien>\r\n");	

	CComBSTR2 ml = m_Language;
	x.AddChildElem("Language", ml.ToString());
	x.AddChildElem("ShowInTaskbar", m_ShowInTaskbar?"1":"0");
	x.AddChildElem("SoundOn", m_SoundOn?"1":"0");
	x.AddChildElem("DeleteContactsOnStartup", m_DeleteContactsOnStartup?"1":"0");
	x.AddChildElem("DeleteContactsOnConnect", m_DeleteContactsOnConnect?"1":"0");
	x.AddChildElem("AuthContacts", m_AuthContacts);
	x.AddChildElem("AutoConnectVPNOnNetwork", m_AutoConnectVPNOnNetwork?"1":"0");
	x.AddChildElem("AutoConnectVPNOnStartup", m_AutoConnectVPNOnStartup?"1":"0");
	x.AddChildElem("ShowNotificationPopup", m_ShowNotificationPopup?"1":"0");
	x.AddChildElem("CheckUpdate", m_CheckUpdate?"1":"0");
	x.AddChildElem("CheckUpdateConnect", m_CheckUpdateConnect?"1":"0");
	x.AddChildElem("CheckUpdateTimed", m_CheckUpdateTimed?"1":"0");
	x.AddChildElem("CheckUpdateTimedNum", m_CheckUpdateTimedNum);
	x.AddChildElem("CheckUpdateSilently", m_CheckUpdateSilently?"1":"0");
	x.AddChildElem("ShowUpdaterMessages", m_ShowUpdaterMessages?"1":"0");
	x.AddChildElem("UsePowerOptions", m_UsePowerOptions?"1":"0");
	x.AddChildElem("DisconnectEthernetOnExit", m_DisconnectEthernetOnExit?"1":"0");
	x.AddChildElem("FixedMTU", m_FixedMTU?"1":"0");
	x.AddChildElem("FixedMTUNum", m_FixedMTUNum);
	x.AddChildElem("SortContacts", m_SortContacts);
	x.AddChildElem("LastOperatorMessageID", m_LastOperatorMessageID);
	
	CComBSTR2 j = m_JID;
	x.AddChildElem("JID", j.ToUTF8String());
	j.Empty();
	j = m_Nick;
	x.AddChildElem("Nick", j.ToUTF8String());
	x.AddChildElem("UseSSLWrapper", m_UseSSLWrapper?"1":"0");



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
	
	CComBSTR2 res = m_StaticResource;
	x.AddChildElem("Resource", res.ToString());

	CComBSTR2 msh = m_ServerHost;
	x.AddChildElem("ServerHost", msh.ToString());
	x.AddChildElem("ServerPort", m_ServerPort);
	x.AddChildElem("UDPPort", m_UDPPort);
	
	CComBSTR2 m = m_IPProviderURL;
	x.AddChildElem("IPProviderURL", m.ToString());
	CComBSTR2 uurl = m_UpdateURL;
	x.AddChildElem("UpdateURL", uurl.ToString());
	x.AddChildElem("ObtainIPAddress", m_ObtainIPAddress);
	x.AddChildElem("AllowAnyMediator", m_AllowAnyMediator?"1":"0");
	x.AddChildElem("ShowContactPicture", m_ShowContactPicture?"1":"0");
	x.AddChildElem("ShowContactLastOnline", m_ShowContactLastOnline?"1":"0");
	x.AddChildElem("ShowContactActivity", m_ShowContactActivity?"1":"0");
	x.AddChildElem("ShowContactIP", m_ShowContactIP?"1":"0");
	x.AddChildElem("ShowContactStatus", m_ShowContactStatus?"1":"0");
	x.AddChildElem("ShowMyPicture", m_ShowMyPicture?"1":"0");
	x.AddChildElem("ShowMyName", m_ShowMyName?"1":"0");
	x.AddChildElem("ShowMyIP", m_ShowMyIP?"1":"0");
	x.AddChildElem("ShowMyStatus", m_ShowMyStatus?"1":"0");		
	x.AddChildElem("TimestampMessages", m_TimestampMessages?"1":"0");
	x.AddChildElem("SnapToBorder", m_SnapToBorder?"1":"0");
	x.AddChildElem("AutoHideOnInactivity", m_AutoHide?"1":"0");
	x.AddChildElem("AutoHideOnInactivitySeconds", m_AutoHideSeconds);
	x.AddChildElem("ShowMessageHistory", m_ShowMessageHistory?"1":"0");		
	x.AddChildElem("FirewallDefaultAllowRule", m_FirewallDefaultAllowRule?"1":"0");		

	char icobuf[1024];
	strcpy(icobuf, m_CfgFilename);
	strcat(icobuf, ".png");
	handle = open(icobuf, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
	if (handle != (-1))
	{
		write(handle, m_Icon.Ptr(), m_Icon.Len());
		close(handle);
	}

	x.AddChildElem("DoNotShow", m_DoNotShow);

	long mip;
	memcpy(&mip, &m_MyLastNetwork, sizeof(long));
	x.AddChildElem("LastNetwork", mip);
	memcpy(&mip, &m_MyLastNetmask, sizeof(long));
	x.AddChildElem("LastNetmask", mip);
	
	CComBSTR2 mssk = m_Skin;
	x.AddChildElem("Skin", mssk.ToString());
	x.AddChildElem("VoiceChatEnabled", _VoiceChat.m_Enabled?"1":"0");		
	x.AddChildElem("VoiceChatLocalEcho", _VoiceChat.m_LocalEcho?"1":"0");		
	x.AddChildElem("VoiceChatVadThreshold", _VoiceChat.m_VadThreshold);
	x.AddChildElem("VoiceChatPlaybackDevice", _VoiceChat.m_WaveOutDevice);
	x.AddChildElem("VoiceChatRecordingDevice", _VoiceChat.m_WaveInDevice);

	x.Append("<Roster>\r\n");

	for (int i=0;i<m_Groups.size();i++)
	{
		TreeGroup *tg = m_Groups[i];
		if (!tg->Temporary)
		{
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

			x.AddChildAttrib("Group", tg->Name, "Open", exp?"true":"false", "Block", tg->Block?"true":"false");
		}	
	}
	x.Append("</Roster>\r\n");

	for (i=0;i<m_LinkMediators.size();i++)
	{
		LinkMediatorStruct *st =(LinkMediatorStruct *)m_LinkMediators[i];
		if (st->Permanent)
		{
			x.Append("<Mediator>\r\n");
			x.AddChildElem("Host", st->Host);
			x.AddChildElem("Port", st->Port);
			x.AddChildElem("Valid", st->Permanent);
			x.Append("</Mediator>\r\n");
		}
	}

	x.Append("<HiddenContacts>\r\n");
	char *cb = m_HiddenContactsBuffer.Ptr();
	for (i=0;i<m_HiddenContacts.size();i++)
	{
		x.AddChildElem("JID", cb + m_HiddenContacts[i]);
	}
	x.Append("</HiddenContacts>\r\n");

	x.Append("<FirewallRules>\r\n");
	FirewallStruct *fw = NULL;
	for (i=0;i<m_FirewallRules.size();i++)
	{
		fw = (FirewallStruct *)m_FirewallRules[i];
		x.AddChildAttrib("Rule", "", "Proto", fw->Proto, "Port", fw->Port);
	}
	x.Append("</FirewallRules>\r\n");
	

	// sounds
	x.Append("<Sounds>\r\n");

	x.AddChildElem("ContactOnline", ((CComBSTR2)_Notify.m_Online).ToString());
	x.AddChildElem("ContactOffline", ((CComBSTR2)_Notify.m_Offline).ToString());
	x.AddChildElem("MessageIn", ((CComBSTR2)_Notify.m_MsgIn).ToString());
	x.AddChildElem("MessageOut", ((CComBSTR2)_Notify.m_MsgOut).ToString());
	x.AddChildElem("Error", ((CComBSTR2)_Notify.m_Error).ToString());
	x.Append("</Sounds>\r\n");


	x.Append("<AuthRequests>\r\n");
	for (i=0;i<m_AuthRequests.size();i++)
	{
		CComBSTR2 b = m_AuthRequests[i];
		x.AddChildElem("JID", b.ToUTF8String());
	}
	x.Append("</AuthRequests>\r\n");

	CComBSTR2 dbf1 = m_JabberDebugFile;
	x.AddChildElem("JabberDebugFile", dbf1.ToString());

	CComBSTR2 dbf2 = m_SocketDebugFile;
	x.AddChildElem("SocketDebugFile", dbf2.ToString());

	CComBSTR2 dbf22 = m_VPNSocketDebugFolder;
	x.AddChildElem("VPNSocketDebugFolder", dbf22.ToString());
	
	CComBSTR2 dbf21 = m_FunctionDebugFile;
	x.AddChildElem("FunctionDebugFile", dbf21.ToString());
	x.AddChildElem("DeleteFunctionLogMb", m_DeleteFunctionLogMb);

	x.AddChildElem("AutoAwayMinutes", m_AutoAwayMinutes);
	x.AddChildElem("ExtendedAwayMinutes", m_ExtendedAwayMinutes);
	x.AddChildElem("AutoDisconnectMinutes", m_AutoDisconnectMinutes);
	
	CComBSTR2 dbf3 = m_AutoAwayMessage;
	x.AddChildElem("AutoAwayMessage", dbf3.ToString());
	CComBSTR2 dbf4 = m_ExtendedAwayMessage;
	x.AddChildElem("ExtendedAwayMessage", dbf4.ToString());

	x.AddChildElem("AutoSetBack", m_AutoSetBack?"1":"0");

//		CComBSTR2 pp2 = m_PasswordProtectPassword;
	x.Append("</Wippien>\r\n");
	x.Append("<Message_Dialog_Window>\r\n");
	x.AddChildElem("Left", m_RosterRect.left);
	x.AddChildElem("Top", m_RosterRect.top);
	x.AddChildElem("Right", m_RosterRect.right);
	x.AddChildElem("Bottom", m_RosterRect.bottom);
	x.AddChildElem("Snap", m_RosterSnap);
	x.AddChildElem("Aligned", m_IsAligned);
	x.AddChildElem("TopMost", m_IsTopMost);
	x.AddChildElem("DoAlign", m_DoAlign);
	x.Append("</Message_Dialog_Window>\r\n");

	handle = open(m_CfgFilename, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
	if (handle != (-1))
	{
		write(handle, x.Ptr(), x.Len());
		close(handle);
	}

	return TRUE;
}

BOOL CSettings::SaveRooms(void)
{
	// save to file
	Buffer x;

	// dump chat rooms
	x.Clear();
	for (int i=0;i<_MainDlg.m_ChatRooms.size();i++)
	{
		CChatRoom *room = (CChatRoom *)_MainDlg.m_ChatRooms[i];
		if (room->m_DoSave)
		{
			x.Append("<ChatRoom>\r\n");
			x.AddChildElem("Name", room->m_JID);
			x.AddChildElem("Nick", room->m_Nick);
			x.AddChildElem("Block", room->m_Block);
			Buffer in, out;
			in.Append(room->m_Password);
			AESEncrypt(&in, &out);
			x.AddChildElem("Password", out.Ptr());
			x.Append("</ChatRoom>\r\n");
		}
	}		
	int handle = open(m_ChatRoomFilename, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
	if (handle != (-1))
	{
		if (x.Len()>10) // if there's anything inside at all
			write(handle, x.Ptr(), x.Len());
		close(handle);
	}

	return TRUE;
}

BOOL CSettings::SaveUsers(void)
{	
	Buffer x;

	int i;
	for (i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
	{
		CUser *user = _MainDlg.m_UserList.m_Users[i];
		user->m_Saved = FALSE;
	}
	
	
	for (i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
	{
		CUser *user = _MainDlg.m_UserList.m_Users[i];
		if (!user->m_ChatRoomPtr && !user->m_Saved) // we do not dump temporary users
		{
			for (int o=i+1;o<_MainDlg.m_UserList.m_Users.size();o++)
			{
				CUser *u = _MainDlg.m_UserList.m_Users[o];
				if (!stricmp(user->m_JID, u->m_JID))
					u->m_Saved = TRUE;
			}
			user->m_Saved = FALSE;
			CComBSTR j = user->m_JID;
			x.Append("<User>\r\n");
			x.AddChildElem("Name", user->m_JID);
			x.AddChildElem("VisibleName", user->m_VisibleName);
			x.AddChildElem("Block", user->m_Block?"1":"0");
			
			long mip;
			memcpy(&mip, &user->m_GotVCard, sizeof(long));
			x.AddChildElem("VCard", mip);

			x.AddChildElem("Group", user->m_Group);
			x.AddChildElem("Email", user->m_Email);
			x.AddChildElem("LastOnline", user->m_LastOnline);

			x.Append("<Chat_Dialog_Window>\r\n");
			x.AddChildElem("Left", user->m_ChatWindowRect.left);
			x.AddChildElem("Top", user->m_ChatWindowRect.top);
			x.AddChildElem("Right", user->m_ChatWindowRect.right);
			x.AddChildElem("Bottom", user->m_ChatWindowRect.bottom);
			x.Append("</Chat_Dialog_Window>\r\n");

			x.AddChildElem("AllowMediatorIP", user->m_AllowedRemoteMediator?"1":"0");
			x.AddChildElem("AllowAnyIP", user->m_AllowedRemoteAny?"1":"0");
			x.Append("<Allowed_IPs>\r\n");
			struct in_addr sa;
			for (int x1=0;x1<user->m_AllowedRemoteIPs.size();x1++)
			{
				IPAddressConnectionStruct *ips = (IPAddressConnectionStruct *)user->m_AllowedRemoteIPs[x1];
				if (!ips->Ignored)
				{
					sa.S_un.S_addr = ips->Address;
					if (ips->Allowed)
						x.AddChildAttrib("IP", inet_ntoa(sa), "Allowed", "1");
					else
						x.AddChildElem("IP", inet_ntoa(sa));

				}
			}
			x.Append("</Allowed_IPs>\r\n");
			x.Append("</User>\r\n");
		}
	}

	int handle = open(m_UsrFilename, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
	if (handle != (-1))
	{
		if (x.Len()>10) // if there's anything inside at all
			write(handle, x.Ptr(), x.Len());
		close(handle);
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

		b.Append("\r\n\0",3);

		Buffer c;
		char *a = NULL;
		do 
		{
			a = b.GetNextLine();
			if (a)
			{
				if (*a != '#')
				{
					c.Append(a);
					c.Append("\r\n");

				}
			}
		} while (a);

		CXmlParser xmlparser;
		CXmlEntity *start = xmlparser.Parse(&c);
		if (start)
		{
			CXmlEntity *tool = NULL;
			do 
			{
				tool = CXmlEntity::FindByName(start, "Tool", 1);
				if (tool)
				{
					char *buff = (char *)malloc(MAX_PATH+1);
					memset(buff, 0, MAX_PATH+1);
					ReadSettingsCfg(tool, "Exec", buff, "");	
					
					// does this exist?
					Buffer in;
					in.Append(buff);
					Buffer *out = CUser::ExpandSystemArgs(&in);

					// let's find first space inside
					char *os = out->Ptr();
					char *o1 = out->Ptr();
					BOOL inquote = FALSE;
					while (o1 && *o1)
					{
						if (*o1 == ' ' && inquote == FALSE)
						{
							*o1 = 0;
							break;
						}
						if (*o1 == '\"')
						{
							if (!inquote)
							{
								inquote = TRUE;
								os++;
							}
							else
							{
								*o1 = 0;
								break;
							}
						}
						o1++;
					}
					int h2 = open(os,O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
					if (h2 ==(-1))
						free(buff);
					else
					{
						close(h2);

						MenuTool *mt = new MenuTool;
						memset(mt, 0, sizeof(MenuTool));
						mt->Exec = buff;

						buff = (char *)malloc(MAX_PATH+1);
						memset(buff, 0, MAX_PATH+1);
						ReadSettingsCfg(tool, "Menu", buff, "");	
						mt->Menu = buff;


						buff = (char *)malloc(MAX_PATH+1);
						memset(buff, 0, MAX_PATH+1);
						ReadSettingsCfg(tool, "Icon", buff, "");	
						char *j = strchr(buff, ',');
						if (!j)
							j = strchr(buff, ';');
						if (j)
						{
							*j=0;
							j++;
							mt->IconID = atoi(j);
						}	
						mt->IconPath = buff;

						CXmlEntity *t = NULL;
						do 
						{
							buff = (char *)malloc(MAX_PATH+1);
							memset(buff, 0, MAX_PATH+1);
							t = ReadSettingsCfg(tool, "Filter", buff, "");
							if (t)
							{
								CXmlEntity *t1 = CXmlEntity::FindAttrByName(t, "type");
								if (t1)
								{
									if (!strcmp(t1->Value, "Group"))
									{
										if (mt->FilterGroup)
											free(mt->FilterGroup);
										mt->FilterGroup = buff;
									}
									else
										if (!strcmp(t1->Value, "JID"))
										{
											if (mt->FilterJID)
												free(mt->FilterJID);
											mt->FilterJID = buff;
										}
										else
										{
											if (mt->FilterVisibleName)
												free(mt->FilterVisibleName);
											mt->FilterVisibleName = buff;
										}
								}
								else
									free(buff);

								t->Name[0] = 0;
							}
						} while (t);

						ReadSettingsCfg(tool, "State", &mt->State, 0);	
						m_MenuTools.push_back(mt);
					}

					tool->Name[0] = 0;
				}
			} while (tool);
		}
		delete start;
	}

	return TRUE;
}

void CSettings::PushGroupSorted(TreeGroup *grp)
{
	if (!stricmp(grp->Name, GROUP_GENERAL))
		grp->VisibleName = Translate(GROUP_GENERAL);
	if (!stricmp(grp->Name, GROUP_OFFLINE))
		grp->VisibleName = Translate(GROUP_OFFLINE);

	for (int i = 0; i < _Settings.m_Groups.size(); i++)
	{
		CSettings::TreeGroup *tg = (CSettings::TreeGroup *)_Settings.m_Groups[i];
		if (!stricmp(tg->Name, grp->Name))
		{
			delete grp;
			return;
		}
	}
	if (!stricmp(grp->Name, GROUP_GENERAL))
	{
		_Settings.m_Groups.insert(_Settings.m_Groups.begin(), grp);
		return;
	}

	if (stricmp(grp->Name, GROUP_OFFLINE))
	{
		for (i = 0; i < _Settings.m_Groups.size(); i++)
		{
			CSettings::TreeGroup *tg = (CSettings::TreeGroup *)_Settings.m_Groups[i];
			if (stricmp(tg->Name, grp->Name)>0 && stricmp(tg->Name, GROUP_GENERAL))
			{
				_Settings.m_Groups.insert(_Settings.m_Groups.begin()+i, grp);
				return;
			}
		}
	}
	_Settings.m_Groups.push_back(grp);
}


BOOL m_PasswordDialogShown = FALSE;
BOOL CSettings::CheckPasswordProtect(void)
{
	if (m_NowProtected)
	{
		if (m_PasswordProtectPassword.Length())
		{
			if (!m_PasswordDialogShown)
			{
				m_PasswordDialogShown = TRUE;
				CSettingsDlg::CEnterPassDialog ndlg;
				ndlg.DoModal();
				m_PasswordDialogShown = FALSE;
				if (!ndlg.m_Success)
					return FALSE;
				CComBSTR2 pp = m_PasswordProtectPassword;
				if (!strcmp(pp.ToString(), ndlg.m_Password))
					m_NowProtected = FALSE;
			}
			else
				return FALSE;
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

CSettings::LinkMediatorStruct *CSettings::AddLinkMediator(char *Host, int Port)
{
	LinkMediatorStruct *st = NULL;
	for (int i=0;!st && i<m_LinkMediators.size();i++)
	{
		LinkMediatorStruct *st1 = (LinkMediatorStruct *)m_LinkMediators[i];
		if (!stricmp(Host, st1->Host) && Port == st1->Port)
			st = st1;
	}

	if (!st)
	{
		st = new LinkMediatorStruct();
		memset(st, 0, sizeof(LinkMediatorStruct));
		st->Host = (char *)malloc(strlen(Host)+1);
		strcpy(st->Host, Host);
		st->Port = Port;
		st->Permanent = FALSE;
		m_LinkMediators.push_back(st);
	}

	return st;
}

CSettings::TreeGroup *CSettings::GetGroupByName(char *Name)
{
	for (int j=0;j<_Settings.m_Groups.size();j++)
	{
		TreeGroup *tg = m_Groups[j];
		if (!strcmp(tg->Name, Name))
			return tg;
	}

	return NULL;
}

BOOL CSettings::LoadLanguageFile(char *Language, Buffer *temp1)
{
	char buff[32768];
	int i;
	Buffer temp;
	temp.Clear();
	int handle = open(Language, O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
	if (handle >= 0)
	{
		BOOL initial = TRUE;
		unsigned char *a1;
		BOOL isunicode = FALSE;
		do
		{
			a1 = (unsigned char *)buff;
			i = read(handle, buff, sizeof(buff));
			if (i>0)
			{
				if (initial && i>5)
				{
					initial = FALSE;
					if (a1[0] == 239)
					{
						//							isutf8 = TRUE;
						a1 = (unsigned char *)(buff+3);
						i-=3;
					}
					if (a1[0] == 254)
					{
						//							isunicode = TRUE; // but ignore
						a1 = (unsigned char *)(buff+2);
						i-=2;
					}
					if (a1[0] == 255)
					{
						isunicode = TRUE;
						a1 = (unsigned char *)(buff+2);
						i-=2;
					}
				}
				temp.Append((char *)a1, i);
			}
		} while (i>0);
		close(handle);
		
		if (isunicode)
		{
			char buff2[65536];
			_Buffer t2;
			do 
			{
				i = temp.FindNextLine(TRUE);
				if (i>0)
				{
					memset(buff2, 0, sizeof(buff2));
					memcpy(buff2, temp.Ptr(), i);
					temp.Consume(i);
					int ret = WideCharToMultiByte(CP_ACP, 0, (BSTR)buff2, -1, NULL, NULL, NULL, NULL);
					if (ret>0)
					{
						WideCharToMultiByte(CP_ACP, 0, (BSTR)buff2, -1, buff, sizeof(buff), NULL, NULL);
						ret--; // remove null terminator
						t2.Append(buff, ret);
//						t2.Append("\r\n");
					}
				}
			} while (i>0);

			temp.Clear();	
			temp.Append(t2.Ptr(), t2.Len());
		}
		else
		{
			_Buffer t2;
			wchar_t tt[32768];
			do 
			{

				i = temp.FindNextLine(FALSE);
				if (i>0)
				{
					memset(tt, 0, sizeof(tt));
					int ret = MultiByteToWideChar(CP_UTF8, 0, temp.Ptr(), i, NULL, NULL);
					if (ret>0)
					{
						MultiByteToWideChar(CP_UTF8, 0, temp.Ptr(), i, tt, sizeof(tt));

						temp.Consume(i);
						ret = WideCharToMultiByte(CP_ACP, 0, (BSTR)tt, -1, NULL, NULL, NULL, NULL);
						if (ret>0)
						{
							WideCharToMultiByte(CP_ACP, 0, (BSTR)tt, -1, buff, sizeof(buff), NULL, NULL);
							t2.Append(buff);
						}
					}
				}
			} while (i>0);
			
			temp.Clear();	
			temp.Append(t2.Ptr(), t2.Len());
		}

		temp1->Append(temp.Ptr(), temp.Len());

		return TRUE;
	}
	return FALSE;
}


BOOL CSettings::LoadLanguage(char *Language)
{
	char buff[32768];
	strcpy(buff, m_LanguagePath);
	strcat(buff, "English.txt");


	m_LanguageEnglish.Clear();
	m_LanguageEnglishIndex.Clear();
	m_LanguageOther = new Buffer(); // I intentionally don't delete previous copy.. small intentional memory leak
	m_LanguageOtherIndex.Clear();
	m_LanguageEnglishTotal = 0;

	Buffer temp;
	unsigned int offset;
	int i;
	int handle = open(buff, O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
	if (handle >= 0)
	{
		do
		{
			i = read(handle, buff, sizeof(buff));
			if (i>0)
				temp.Append(buff, i);
		} while (i>0);
		close(handle);

		temp.Append("\r\n", 2);
		m_LanguageEnglishTotal = 0;
		char *a;
		do 
		{
			offset = m_LanguageEnglish.m_end;
			a = temp.GetNextLine();
			if (a && *a)
			{
				if (*a == '#')
				{
					char *b = strstr(a, "Version:");
					if (b)
					{
						m_LanguageEngFileVersion = atoi(trim(b+8));
					}
				}
				else
				{
					FixCFormatting(a, buff);
					char *h = strstr(buff, "  ##");
					if (h)
						*h = 0;
					m_LanguageEnglish.Append(trim(buff));
					m_LanguageEnglish.PutShort(0);
					m_LanguageEnglishTotal++;
					m_LanguageEnglishIndex.Append((char *)&offset, sizeof(offset));
				}
			}
		} while (a);




		// foreign file
		strcpy(buff, _Settings.m_LanguagePath);
		strcat(buff, Language);
		strcat(buff, ".txt");

		m_LanguageOther->Clear();
		m_LanguageOtherIndex.Clear();
		temp.Clear();
		unsigned int m_LanguageOtherTotal = 0;

		if (LoadLanguageFile(buff, &temp))
		{
			temp.Append("\r\n", 2);
			do 
			{
				offset = m_LanguageOther->m_end;
				a = temp.GetNextLine();
				if (a && *a)
				{
					if (*a == '#')
					{
						char *b = strstr(a, "Version:");
						if (b)
						{
							m_LanguageFileVersion = atoi(trim(b+8));
						}
					}
					else
					{
						FixCFormatting(a, buff);
						char *h = strstr(buff, "  ##");
						if (h)
							*h = 0;
						m_LanguageOther->Append(trim(buff));
						m_LanguageOther->PutShort(0);
						m_LanguageOtherIndex.Append((char *)&offset, sizeof(offset));
						m_LanguageOtherTotal++;
					}
				}
			} while (a);
			
			if (m_LanguageEnglishTotal>m_LanguageOtherTotal)
				m_LanguageEnglishTotal = m_LanguageOtherTotal;
			
			m_LanguageEnglish.m_offset = 0;
			m_LanguageOther->m_offset = 0;
			
			AWAY_MESSAGE = Translate("Away due to inactivity.");
			EXTAWAY_MESSAGE = Translate("Away for a loooong time.");
			CONFIGURING_ADAPTER_TEXT = Translate("Configuring Wippien network adapter...");

			return TRUE;
		}
		if (m_LanguageEnglishTotal>m_LanguageOtherTotal)
			m_LanguageEnglishTotal = m_LanguageOtherTotal;

	}
	return FALSE;
}

char *CSettings::Translate(char *text)
{
	char *orga = m_LanguageEnglish.Ptr();
	unsigned int *orgp = (unsigned int *)m_LanguageEnglishIndex.Ptr();

	char *dsta = m_LanguageOther->Ptr();
	unsigned int *dstp = (unsigned int *)m_LanguageOtherIndex.Ptr();
	
	for (int i=0;i<m_LanguageEnglishTotal;i++)
	{
		if (!strcmp(text, orga+orgp[i]))
		{
			orga = dsta + dstp[i];
			if (*orga)
				return orga;
			else
				return text;
		}
	}

	return text;
}

void CSettings::FixCFormatting(char *in, char *out)
{
	while (*in)
	{
		if (*in=='\\')
		{
			in++;
			if (*in)
			{
				switch (*in)
				{
					case '0':
						*out = 0;
						break;

					case 'n':
						*out = '\n';
						break;

					case 'r':
						*out = '\r';
						break;

					case '\\':
						*out = '\\';
						break;

					case '\"':
						*out = '\"';
						break;

					case '\'':
						*out = '\'';
						break;

				default:
//					MessageBeep(-1);
					break;
				}
			}
		}
		else
			*out = *in;

		in++;
		out++;
	}
	*out = 0;
}