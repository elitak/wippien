// Wippien.cpp : Implementation of WinMain


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f Wippienps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "Wippien.h"
#include "Ethernet.h"
#include "Settings.h"
#include "MainDlg.h"
#include "Jabber.h"
#include "SettingsDlg.h"
#include "ContactAuthDlg.h"
#include "UpdateHandler.h"
#include "SDKMessageLink.h"
#include <snmp.h>
#pragma	comment(lib, "snmpapi.lib")



CAppModule _Module;
CSettings _Settings;
CEthernet _Ethernet;
CMainDlg _MainDlg;
extern CJabber *_Jabber;
extern CSDKMessageLink *_SDK;
extern CContactAuthDlg *_ContactAuthDlg;
PFNSNMPEXTENSIONQUERY	pfnSnmpExtensionQuery = NULL;	// Pointer to function: SnmpExtensionQuery
int pfnSnmpAdapterIndex = -1;

void ResampleImageIfNeeded(CxImage *img, int size);
void ResampleImageIfNeeded(CxImage *img, int sizeX, int sizeY);


int CheckSettingsWizard(void)
{
	// now check if settings are sufficient to proceed
	BOOL needwizard;
//	CWizardDlg *dlg;
	int nRet = 0;

	do
	{
		CSettingsDlg dlg(TRUE);
		needwizard = FALSE;

		if (!_Settings.LoadConfig())
		{
			// add some defaults
			char *a = (char *)malloc(strlen(GROUP_GENERAL)+1);
			memcpy(a, GROUP_GENERAL, strlen(GROUP_GENERAL)+1);
			CSettings::TreeGroup *tg = new CSettings::TreeGroup;
			tg->Item = NULL;
			tg->Open = FALSE;
			tg->Name = a;
			tg->VisibleName = a;
			tg->Block = FALSE;
			tg->CountBuff[0] = 0;
			_Settings.PushGroupSorted(tg);
			
			a = (char *)malloc(strlen(GROUP_OFFLINE)+1);
			memcpy(a, GROUP_OFFLINE, strlen(GROUP_OFFLINE)+1);
			tg = new CSettings::TreeGroup;
			tg->Item = NULL;
			tg->Open = FALSE;
			tg->Block = FALSE;
			tg->CountBuff[0] = 0;
			tg->Name = a;
			tg->VisibleName = a;
			_Settings.PushGroupSorted(tg);

			needwizard = TRUE;
		}

		// if JID defined?
		CSettingsDlg::_CSettingsTemplate *pgjid = NULL;
		if (!_Settings.m_JID.Length() || !_Settings.m_Password.Length())
		{
			pgjid = new CSettingsDlg::CSettingsJID();
			dlg.m_Dialogs.push_back(pgjid);

			needwizard = TRUE;
		}

		CSettingsDlg::_CSettingsTemplate *pgicon = NULL;
		if (!_Settings.m_Icon.Len())
		{
			char szPath[MAX_PATH*2];
			char *a = getenv("ALLUSERSPROFILE");
			if (!a)
				a = "C:\\Documents and Settings\\All Users";
			strcpy(szPath, a);

			char szName[1024];
			memset(szName, 0, sizeof(szName));
			unsigned long l = sizeof(szName);
			if (GetUserName(szName, &l))
			{
				int i = strlen(szPath);
				if (i>1)
				{
					if (szPath[i-1]!='\\')
						strcat(szPath, "\\");
					strcat(szPath, "Application Data\\Microsoft\\User Account Pictures\\");
					strcat(szPath, szName);
					strcat(szPath, ".bmp");
					CxImage *image = new CxImage();
					if (image->Load(szPath, CXIMAGE_FORMAT_BMP))
					{
						// cool! let's resize it
						CxFile *fTmp;
						CxMemFile *fMem = new CxMemFile(NULL, 0);
						fMem->Open();
						fTmp = fMem;
						ResampleImageIfNeeded(image, 64);
						image->Encode(fTmp, CXIMAGE_FORMAT_PNG);
						_Settings.m_Icon.Clear();
						_Settings.m_Icon.Append((char *)fMem->GetBuffer(), fMem->Size());
					}
					delete image;
				}
			}

			pgicon = new CSettingsDlg::CSettingsIcon();
			dlg.m_Dialogs.push_back(pgicon);
		}

//		CSettingsDlg::_CSettingsTemplate *pgrsa = NULL;
//		if (!_Settings.m_RSA)
//		{
//			pgrsa = new CSettingsDlg::CSettingsRSA();
//			needwizard = TRUE;
//			dlg.m_Dialogs.push_back(pgrsa);
//		}

		CSettingsDlg::_CSettingsTemplate *pgeth = NULL;
/*		if (!_Settings.m_MyLastNetwork || !_Settings.m_MyLastNetmask || !_Settings.m_Mediator.Length())
		{
			pgeth = new CSettingsDlg::CSettingsEthernet();
			needwizard = TRUE;
			dlg.m_Dialogs.push_back(pgeth);
		}
*/
		if (needwizard)
		{
			if (!_Settings.CheckPasswordProtect())
				return FALSE;

			nRet = dlg.DoModal();
//			nRet = dlg->Show();
			if (!nRet)
				return 0;
		}

		if (needwizard)
		{
			_Settings.SaveConfig();
		}


		if (pgeth)
			delete pgeth;
		if (pgicon)
			delete pgicon;
//		if (pgrsa)
//			delete pgrsa;
		if (pgjid)
			delete pgjid;
//		if (dlg)
//			delete dlg;

	} while(needwizard);

	_Settings.LoadUsers();
	_Settings.LoadRooms();

	return 1;
}

HANDLE WippienMutex = INVALID_HANDLE_VALUE;
int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	// double-check if this is only running instance
	int mutexctr = 0;
	BOOL mutexsucc = TRUE;
	do 
	{
		mutexsucc = TRUE;
		WippienMutex = CreateMutex(NULL, TRUE, "WippienMutex");
		if (!WippienMutex || GetLastError() == ERROR_ALREADY_EXISTS)
		{
			if (WippienMutex)
				CloseHandle(WippienMutex);
			WippienMutex = INVALID_HANDLE_VALUE;
			mutexsucc = FALSE;
			if (mutexctr == 5)
			{
				return FALSE;
			}
			mutexctr++;
			Sleep(1000);
		}
	} while (!mutexsucc);
	
	if (!CheckSettingsWizard())
		return FALSE;

/*	if (_Settings.m_Language.Length())
	{
		CComBSTR2 l = _Settings.m_Language;
		_Settings.LoadLanguage(l.ToString());
	}
*/
	_UpdateHandler = new CUpdateHandler();
	_UpdateHandler->InitUpdater();
//	delete h;

	// now we start up Ethernet
	if (!_Ethernet.Init())
	{
		if (_Settings.m_DoNotShow[DONOTSHOW_NOETHERNET] != '1')
		{
			int yesno = MessageBox(NULL, _Settings.Translate("Wippien network adapter not available. Proceed with IM only?"), _Settings.Translate("Adapter error"), MB_YESNO | MB_ICONQUESTION);
			if (yesno == IDNO)
				return FALSE;
		}
	}
	unsigned long myip = _Settings.m_MyLastNetwork;
	unsigned long mynetmask = _Settings.m_MyLastNetmask;

	_Settings.m_MyLastNetwork = 0;
	_Settings.m_MyLastNetmask = 0;

	if (myip)
		_Ethernet.Start(myip, mynetmask);

	memcpy(_Settings.m_MAC, _Ethernet.m_MAC, sizeof(MACADDR));
	_Settings.SaveConfig();

	// loop through all users and fix MACs
	for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
	{
		CUser *user = (CUser *)_MainDlg.m_UserList.m_Users[i];
		if (user->m_HisVirtualIP && !memcmp(user->m_MAC, "\0\0\0\0\0\0", 6))
		{
			// calculate new mac
			__int64 m = 0;
			memcpy(&m, _Settings.m_MAC, 6);
			m += user->m_HisVirtualIP;
			memcpy(user->m_MAC, &m, 6);
		}
	}


	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	// load additional tools
	_Settings.LoadTools();

	if(_MainDlg.Create(NULL) == NULL)
	{
#ifdef _DEBUGWNDMSG
//		ATLTRACE(_T("Main dialog creation failed!\n"));
#endif
		return 0;
	}

	if (!_Settings.m_NowProtected)
		_MainDlg.ShowWindow(nCmdShow);

	if (_Settings.m_CheckUpdate)
	{
		::SetTimer(_MainDlg.m_hWnd, 111, 2000, NULL);
	}


	// now we initialize COM object support
	_SDK = new CSDKMessageLink();
	_SDK->CreateLinkWindow();

	int nRet = theLoop.Run();
	_Ethernet.Die();
	if (WippienMutex != INVALID_HANDLE_VALUE)
	{
		ReleaseMutex(WippienMutex);
		CloseHandle(WippienMutex);
	}
	WippienMutex = INVALID_HANDLE_VALUE;

	delete _SDK;
	_SDK = NULL;

	if (_ContactAuthDlg && _ContactAuthDlg->IsWindow())
		_ContactAuthDlg->DestroyWindow();
	
	delete _ContactAuthDlg;

	if (_Jabber)
	{
		_Jabber->Disconnect();
		delete _Jabber;
	}
	_Jabber = NULL;
	if (_UpdateHandler)
		delete _UpdateHandler;
	_UpdateHandler = NULL;


	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);

#ifdef _WODVPNLIB
	WODVPNCOMLib::_VPN_LibInit(hInstance);
#endif
	
#ifdef _APPUPDLIB
	WODAPPUPDCOMLib::_AppUpd_LibInit(hInstance);
#endif

#ifdef _WODXMPPLIB
	WODXMPPCOMLib::_XMPP_LibInit(hInstance);
#endif
	
	
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_INTERNET_CLASSES | ICC_PROGRESS_CLASS | ICC_DATE_CLASSES | ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	AtlAxWinInit();

	// initialize ARP library
	HMODULE					hMIBLibrary;			// Handle for library: inetmib1.dll
	
	PFNSNMPEXTENSIONINIT	pfnSnmpExtensionInit = NULL;	// Pointer to function: SnmpExtensionInit

	hMIBLibrary	= LoadLibrary(TEXT("inetmib1.dll"));
	
	// If library loaded, get addresses of (SnmpExtensionInit, pfnSnmpExtensionQuery) functions
	if (hMIBLibrary)
	{
		pfnSnmpExtensionInit	= (PFNSNMPEXTENSIONINIT)	GetProcAddress(hMIBLibrary, "SnmpExtensionInit");
		pfnSnmpExtensionQuery	= (PFNSNMPEXTENSIONQUERY)	GetProcAddress(hMIBLibrary, "SnmpExtensionQuery");
		
		// If success get addresses and initialize SNMP, bInitialized = true
		if (pfnSnmpExtensionInit && pfnSnmpExtensionQuery)
		{
			HANDLE				hPollForTrapEvent;
			AsnObjectIdentifier	aoiSupportedView;
			
			if (!pfnSnmpExtensionInit(0, &hPollForTrapEvent, &aoiSupportedView))
				pfnSnmpExtensionQuery = NULL;
		}
	}

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	// free up ARP library
	if (hMIBLibrary)
		FreeLibrary(hMIBLibrary);


#ifdef _WODVPNLIB
	WODVPNCOMLib::_VPN_LibDeinit();
#endif
	
#ifdef _APPUPDLIB
	WODAPPUPDCOMLib::_AppUpd_LibDeInit();
#endif

#ifdef _WODXMPPLIB
	WODXMPPCOMLib::_XMPP_LibDeInit();
#endif

	return nRet;
}


BOOL EditARPEntry(unsigned char IPAddress[4], unsigned char MACAddress[6])
{
	if (!pfnSnmpExtensionQuery || pfnSnmpAdapterIndex<0)
		return 0;
	
	SnmpVarBindList		SVBList;
	SnmpVarBind			SVBVars[4];
	UINT				OID[4][10];
	AsnInteger32		aiErrorStatus, aiErrorIndex;
	BOOL				bReturn	= FALSE;
	
	//-----------------------------------------------------------------------
	//	Fill array of 4 OIDs
	//	
	//	OID[0]	:	"1.3.6.1.2.1.4.22.1.1", ipNetToMediaIfIndex
	//				The interface on which this entry's equivalence is effective
	//
	//	OID[1]	:	"1.3.6.1.2.1.4.22.1.2", ipNetToMediaPhysAddress
	//				The media-dependent 'physical' address
	//
	//	OID[2]	:	"1.3.6.1.2.1.4.22.1.3", ipNetToMediaNetAddress
	//				The IpAddress corresponding to the media-dependent 'physical' address
	//
	//	OID[3]	:	"1.3.6.1.2.1.4.22.1.4", ipNetToMediaType
	//				Entry type: 1:Other, 2:Invalid(Remove), 3:Dynamic, 4:Static
	//-----------------------------------------------------------------------
	for (int count=0; count<4; count++)
	{
		OID[count][0]		= 1;
		OID[count][1]		= 3;
		OID[count][2]		= 6;
		OID[count][3]		= 1;
		OID[count][4]		= 2;
		OID[count][5]		= 1;
		OID[count][6]		= 4;
		OID[count][7]		= 22;
		OID[count][8]		= 1;
		OID[count][9]		= 1 + count;
		
		switch(count)
		{
		case 0:
			//	OID[0]	:	"1.3.6.1.2.1.4.22.1.1", ipNetToMediaIfIndex
			//				The interface on which this entry's equivalence is effective
			SVBVars[count].value.asnType				= ASN_INTEGER;
			SVBVars[count].value.asnValue.number		= pfnSnmpAdapterIndex;
			break;
			
		case 1:
			//	OID[1]	:	"1.3.6.1.2.1.4.22.1.2", ipNetToMediaPhysAddress
			//				The media-dependent 'physical' address
			SVBVars[count].value.asnType				= ASN_OCTETSTRING;
			SVBVars[count].value.asnValue.string.stream	= MACAddress;
			SVBVars[count].value.asnValue.string.length	= 6;	// MAC Address length
			SVBVars[count].value.asnValue.string.dynamic= FALSE;
			break;
			
		case 2:
			//	OID[2]	:	"1.3.6.1.2.1.4.22.1.3", ipNetToMediaNetAddress
			//				The IpAddress corresponding to the media-dependent 'physical' address
			SVBVars[count].value.asnType				= ASN_IPADDRESS;
			SVBVars[count].value.asnValue.string.stream	= IPAddress;
			SVBVars[count].value.asnValue.string.length	= 4;	// IP Address length
			SVBVars[count].value.asnValue.string.dynamic= FALSE;
			break;
			
		case 3:
			//	OID[3]	:	"1.3.6.1.2.1.4.22.1.4", ipNetToMediaType
			//				Entry type: 2:Remove, 3:Dynamic, 4:Static
			SVBVars[count].value.asnType				= ASN_INTEGER;
			SVBVars[count].value.asnValue.number		= 3; //	(2:Remove, 3:Dynamic, 4:Static)
			break;
		}
		AsnObjectIdentifier	AsnOID = {sizeof(OID[count])/sizeof(UINT), OID[count]};
		SnmpUtilOidCpy(&SVBVars[count].name, &AsnOID);
	}
	
	SVBList.len		= 4;
	SVBList.list	= SVBVars;
	
	aiErrorStatus	= 0;
	aiErrorIndex	= 0;
	
	// Set information of entry (4 OIDs)
	if (pfnSnmpExtensionQuery(SNMP_PDU_SET, &SVBList, &aiErrorStatus, &aiErrorIndex))
		if (aiErrorStatus == SNMP_ERRORSTATUS_NOERROR)
			bReturn = TRUE; // If success set bReturn = true
		
		// Frees the memory allocated for the specified object identifiers
		SnmpUtilOidFree(&SVBVars[3].name);
		SnmpUtilOidFree(&SVBVars[2].name);
		SnmpUtilOidFree(&SVBVars[1].name);
		SnmpUtilOidFree(&SVBVars[0].name);
		
		return bReturn;		// TRUE if set successfully, FALSE otherwise.
}
