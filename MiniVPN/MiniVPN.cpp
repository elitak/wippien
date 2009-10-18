// MiniVPN.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include "Ethernet.h"
#include "JabberLib.h"

HINSTANCE hMainInstance = NULL;
HWND hMainWnd = NULL;
CEthernet *_Ethernet = NULL;
CJabberLib *_Jabber = NULL;
char gResource[1024] = {0};

#ifndef _WIPPIENSERVICE
void SetStatus(char *Text)
{
	if (IsWindow(hMainWnd))
		SetDlgItemText(hMainWnd, IDC_STATUS, Text);
}
#endif

BOOL CALLBACK DialogFunc(HWND hdwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  switch(Msg)
  {
    case WM_INITDIALOG:
    {
		char JIDbuff[1024], passbuff[1024], mediatorbuff[1024], tempbuff[1024];

		strcpy(JIDbuff, "test@wippien.com");
		strcpy(passbuff, "test");
		strcpy(mediatorbuff, "mediator.wippien.com");

		
		strcpy(tempbuff, __argv[0]);
		strcat(tempbuff, ".config");
		FILE *stream = fopen(tempbuff, "rt");
		
		if (stream)
		{
			tempbuff[0] = 0;
			char *a = fgets(tempbuff, sizeof(tempbuff), stream);
			if (a && *a)
			{
				a = strchr(tempbuff, '\r');if (a) *a = 0;
				a = strchr(tempbuff, '\n');if (a) *a = 0;
				a = strchr(tempbuff, '/');
				if (a)
				{
					*a = 0;
					a++;
					strcpy(gResource, a);
				}
				strcpy(JIDbuff, tempbuff);
			}

			tempbuff[0] = 0;
			a = fgets(tempbuff, sizeof(tempbuff), stream);
			if (a && *a)
			{
				a = strchr(tempbuff, '\r');if (a) *a = 0;
				a = strchr(tempbuff, '\n');if (a) *a = 0;
				strcpy(passbuff, tempbuff);
			}

			tempbuff[0] = 0;
			a = fgets(tempbuff, sizeof(tempbuff), stream);
			if (a && *a)
			{
				a = strchr(tempbuff, '\r');if (a) *a = 0;
				a = strchr(tempbuff, '\n');if (a) *a = 0;
				strcpy(mediatorbuff, tempbuff);
			}
			
			fclose(stream);
			HWND h = ::GetDlgItem(hdwnd, IDC_CONNECT);
			::PostMessage(hdwnd, WM_COMMAND,IDC_CONNECT,(LPARAM)h);
		}

		

      //modify system menu
      //(remove Restore, Maximize and Resize entries)
      HMENU hSysMenu=GetSystemMenu(hdwnd, FALSE);
      DeleteMenu(hSysMenu, 0, MF_BYPOSITION);
      DeleteMenu(hSysMenu, 1, MF_BYPOSITION);
      DeleteMenu(hSysMenu, 2, MF_BYPOSITION);
      hMainWnd=hdwnd;

      //set dialog icon
	  HICON icon = (HICON)::LoadImage(hMainInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	  SendMessage(hdwnd, WM_SETICON, TRUE, (LPARAM)icon);
	icon = (HICON)::LoadImage(hMainInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	  SendMessage(hdwnd, WM_SETICON, FALSE, (LPARAM)icon);
//      SetClassLong(hdwnd, GCL_HICON, (LONG)LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPICON)));

#ifndef _WIPPIENSERVICE
		SetDlgItemText(hdwnd, IDC_JID, JIDbuff);
		SetDlgItemText(hdwnd, IDC_PASSWORD, passbuff);
		SetDlgItemText(hdwnd, IDC_MEDIATOR, mediatorbuff);
#else
		strcpy(gJID, JIDbuff);
		strcpy(gPassword, passbuff);
		strcpy(gMediator, mediatorbuff);
#endif		

	  _Jabber = new CJabberLib();
		_Ethernet = new CEthernet();


	if (!_Ethernet->InitAdapter() && !_Ethernet->InitOpenVPNAdapter())
	{
		MessageBox(NULL, "Failed to open Network adapter", "Network error", MB_OK);
	}
	else
		_Ethernet->GetMyIP();

#ifndef _WIPPIENSERVICE
	char buff[1024];
	struct  in_addr sa1, sa2;
	sa1.S_un.S_addr = _Ethernet->m_MyIP;
	sa2.S_un.S_addr = _Ethernet->m_MyNetmask;
	
	sprintf(buff, "Network IP address %s", inet_ntoa(sa1));
	SetStatus(buff);
#endif

      return TRUE;
    }

    case WM_COMMAND:
      switch(LOWORD(wParam))
      {
        case IDCANCEL:
        case IDOK:
          DestroyWindow(hdwnd);
          return FALSE;

		case IDC_CONNECT:
			{
			_Jabber->Disconnect();
			
			char lpStrJID[1024], lpStrPassword[1024];
			if (GetDlgItemText(hdwnd, IDC_JID, lpStrJID, sizeof(lpStrJID)))
			{
				if (GetDlgItemText(hdwnd, IDC_PASSWORD, lpStrPassword, sizeof(lpStrPassword)))
				{
					_Jabber->Connect(lpStrJID, lpStrPassword);
				}
				else
					MessageBox(hdwnd, "Error", "You must enter password", MB_OK);
			}
			else
				MessageBox(hdwnd, "Error", "You must enter JID", MB_OK);			
			}
			break;

		case IDC_DISCONNECT:
			_Jabber->Disconnect();
			break;


        default:
          return FALSE;
      }
	  break;

    case WM_DESTROY:
      EndDialog(hdwnd, 0);
      return TRUE;
  }
  return FALSE;
}

extern "C"
{
	void _XMPP_LibInit(HINSTANCE hInst);
	void _XMPP_LibDeInit(void);

	void _VPN_LibInit(HINSTANCE hInst);
	void _VPN_LibDeinit(void);

};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
  hMainInstance=hInstance;

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES | ICC_LISTVIEW_CLASSES | ICC_INTERNET_CLASSES | ICC_PROGRESS_CLASS | ICC_DATE_CLASSES | ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	
	// socket stuff
	WSADATA wsaData;

	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return FALSE;

	_XMPP_LibInit(hMainInstance);
	_VPN_LibInit(hMainInstance);
  
  int nRes=DialogBox(hMainInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), 0, DialogFunc);

	if (_Jabber)
		delete _Jabber;
	if (_Ethernet)
		delete _Ethernet;
  
			WSACleanup();

	_VPN_LibDeinit();
	_XMPP_LibDeInit();

  return 0;
}
