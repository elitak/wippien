// SDKMessageLink.cpp: implementation of the CSDKMessageLink class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifdef _WODXMPPLIB
namespace WODXMPPCOMLib
{
#include "\WeOnlyDo\wodXMPP\Code\Win32LIB\Win32LIB.h"
}
void XMPPStateChange(void *wodXMPP, WODXMPPCOMLib::StatesEnum OldState);
#endif

#include "SDKMessageLink.h"
#include "MainDlg.h"
#include "Ethernet.h"
#include "Jabber.h"
#include "Buffer.h"

extern CMainDlg _MainDlg;
extern CEthernet _Ethernet;
extern CJabber *_Jabber;
CSDKMessageLink *_SDK = NULL;

BOOL fWndClassRegistered = FALSE;


#ifndef _WODXMPPLIB

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Define the ftp events to be handled:
_ATL_FUNC_INFO SDKJabberConnectedInfo = {CC_STDCALL, VT_EMPTY, 0};
_ATL_FUNC_INFO SDKJabberDisconnectedInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_BSTR}};

class CSDKMessageLink::CJabberEvents : public IDispEventSimpleImpl<1, CSDKMessageLink::CJabberEvents, &__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents)>
{
public:
    CJabberEvents (CSDKMessageLink::CJabberWiz * ppJ)
    {
        m_pJ = ppJ;
        DispEventAdvise ( (IUnknown*)m_pJ->m_Jabb);
    }

    virtual ~CJabberEvents ()
    {
        DispEventUnadvise ( (IUnknown*)m_pJ->m_Jabb);
//        m_pJ->m_Jabb.Release();
    }
#endif

#ifdef _WODXMPPLIB
	void SDKXMPPConnected(void *wodXMPP)
#else
    void __stdcall DispConnected ()
#endif
    {
		if (_SDK)
		{
			Buffer b;
			b.PutCString("");

			if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_ACCOUNTCREATED, b.Ptr(), b.Len()))
				return;
		}
//		delete m_pJ;
    }
#ifdef _WODXMPPLIB
	void SDKXMPPDisconnected(void *wodXMPP, long ErrorCode, char *ErrorText)
#else
    void __stdcall DispDisconnected (long ErrorCode, BSTR ErrorText)
#endif
    {
		if (ErrorCode && _SDK)
		{
			Buffer b;
			b.PutCString(ErrorText);

			if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_ACCOUNTCREATED, b.Ptr(), b.Len()))
				return;
		}
//		delete m_pJ;
    }    
#ifndef _WODXMPPLIB
    BEGIN_SINK_MAP (CJabberEvents)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),0,DispConnected,&SDKJabberConnectedInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),1,DispDisconnected,&SDKJabberDisconnectedInfo)
        END_SINK_MAP ()
private:
	CJabberWiz * m_pJ;
};
#endif


CSDKMessageLink::CJabberWiz::CJabberWiz(CSDKMessageLink *Owner)
{
#ifndef _WODXMPPLIB
	m_Jabb.CoCreateInstance(__uuidof(WODXMPPCOMLib::wodXMPPCom));
	m_Events = new CJabberEvents(this);
#else
	m_Events.Connected = SDKXMPPConnected;
	m_Events.Disconnected = SDKXMPPDisconnected;
	memset(&m_Events, 0, sizeof(m_Events));
	m_Jabb = WODXMPPCOMLib::__XMPP_Create(&m_Events);
#endif

#ifdef WODXMPP_LICENSE_KEY
	CComBSTR blic(WODXMPP_LICENSE_KEY);
	m_Jabb->put_LicenseKey(blic);
#endif
	m_Owner = Owner;
}

CSDKMessageLink::CJabberWiz::~CJabberWiz()
{
#ifndef _WODXMPPLIB
	m_Jabb->Disconnect();
	delete m_Events;
#else
	WODXMPPCOMLib::XMPP_Disconnect(m_Jabb);
#endif
}

void CSDKMessageLink::CJabberWiz::Connect(char *JID, char *pass, char *hostname, int port, BOOL registernew, BOOL usessl)
{
	char *a = strchr(JID, '/');
	if (a)
		*a = 0;

	CComBSTR l = JID,p = pass, h = hostname;
	l += "/WippienTest";


	VARIANT var;
	if (h.Length())
	{
		var.vt = VT_BSTR;
		var.bstrVal = h;
	}
	else
		var.vt = VT_ERROR;

#ifndef _WODXMPPLIB
	m_Jabb->put_Login(l);
	m_Jabb->put_Password(p);
	if (port)
		m_Jabb->put_Port(port);

	if (registernew)
		m_Jabb->put_Register(VARIANT_TRUE);

	m_Jabb->put_AutoVisible(VARIANT_FALSE);

	if (usessl)
		m_Jabb->put_Security((WODXMPPCOMLib::SecurityEnum)3);
	else
		m_Jabb->put_Security((WODXMPPCOMLib::SecurityEnum)1);


	try
	{
		m_Jabb->Connect(var);
	}
	catch (_com_error e)
	{		
		CComBSTR2 b;
		m_Jabb->get_LastErrorText(&b);
		::MessageBox(NULL, b.ToString(), "Jabber error", MB_OK);
	}

#else
	CComBSTR2 l1 = l;
	WODXMPPCOMLib::XMPP_SetLogin(m_Jabb, l1.ToString());
	WODXMPPCOMLib::XMPP_SetPassword(m_Jabb, pass);
	if (port)
		WODXMPPCOMLib::XMPP_SetPort(m_Jabb, port);
	if (registernew)
		WODXMPPCOMLib::XMPP_SetRegister(m_Jabb, TRUE);
	WODXMPPCOMLib::XMPP_SetAutoVisible(m_Jabb, FALSE);

	if (usessl)
		WODXMPPCOMLib::XMPP_SetSecurity(m_Jabb, (WODXMPPCOMLib::SecurityEnum)3); // security implicit
	else
		WODXMPPCOMLib::XMPP_SetSecurity(m_Jabb, (WODXMPPCOMLib::SecurityEnum)1); // security allowed


	long hr = WODXMPPCOMLib::XMPP_Connect(m_Jabb, hostname);
	if (!hr)
	{
		char buff[1024];
		int bflen = sizeof(buff);
		WODXMPPCOMLib::XMPP_GetLastErrorText(m_Jabb, buff, &bflen);
		::MessageBox(NULL, buff, "Jabber error", MB_OK);
	}
#endif
}

void CSDKMessageLink::CJabberWiz::Disconnect(void)
{
#ifndef _WODXMPPLIB
	m_Jabb->Disconnect();
#else
	WODXMPPCOMLib::XMPP_Disconnect(m_Jabb);
#endif
}



CSDKMessageLink::CSDKMessageLink()
{
	m_hWnd = NULL;
	m_RemoteWnd = NULL;
	Wiz = NULL;
}

CSDKMessageLink::~CSDKMessageLink()
{
	if (Wiz)
		delete Wiz;
}


void CSDKMessageLink::CreateLinkWindow(void)
{
    WNDCLASS wndclass;

	char clsname[128], capname[128];
	sprintf(capname, "Wippien SDK Server Link Window");
	sprintf(clsname, "CWippienSDKMessageLinkClassServer");

    if(!fWndClassRegistered) 
	{
        memset(&wndclass, 0, sizeof(wndclass));
        wndclass.lpfnWndProc = WndProc;
        wndclass.hInstance   = _Module.GetModuleInstance();
	    wndclass.lpszClassName = clsname;
		wndclass.cbWndExtra  = 4;
        RegisterClass(&wndclass);
        fWndClassRegistered = TRUE;
    }

	m_hWnd = CreateWindow(clsname,
      capname,
      WS_POPUP,
      0, 0, 0, 0,
      NULL,
      NULL,
      _Module.GetModuleInstance(),
      NULL);

	if( m_hWnd != NULL )
		SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
}

BOOL CSDKMessageLink::FireEvent(int Cmd, void *Data, int Len)
{
	for (int i=0;i<m_Windows.size();i++)
	{
		LPARAM l = m_Windows[i];
		if (SendBuffer((HWND)l, Cmd, Data, Len) == 2)
			return FALSE;
	}

	return TRUE;
}

BOOL  CSDKMessageLink::FireEvent(int Cmd, char *Data)
{
	return FireEvent(Cmd, (void *)Data, strlen(Data));
}

BOOL  CSDKMessageLink::FireEvent(int Cmd, BSTR Data)
{
	CComBSTR2 d = Data;
	return FireEvent(Cmd, d.ToString());
}


long CSDKMessageLink::SendBuffer(HWND hwnd, int Event, void *Data, int Len)
{
	COPYDATASTRUCT cd;

	cd.dwData = Event;
	cd.cbData = Len;
	cd.lpData = Data;

	return SendMessage(hwnd, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)&cd);
}

long CSDKMessageLink::SendBuffer(HWND hwnd, void *Data, int Len)
{
	return SendBuffer(hwnd, 0, Data, Len);
}

long CSDKMessageLink::SendBuffer(HWND hwnd, char *Data)
{
	return SendBuffer(hwnd, (void *)Data, strlen(Data));
}

long CSDKMessageLink::SendBuffer(HWND hwnd, BSTR Data)
{
	CComBSTR2 d = Data;
	return SendBuffer(hwnd, d.ToString(), d.Length());
}

LRESULT CALLBACK CSDKMessageLink::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CSDKMessageLink * pLink = (CSDKMessageLink *)GetWindowLong(hwnd, GWL_USERDATA);
	switch (msg)
	{
		case WM_WIPPIEN_INIT:
			{
				BOOL found = FALSE;
				for (int i=0;!found && i<pLink->m_Windows.size();i++)
				{
					LPARAM l = pLink->m_Windows[i];
					if (wParam == l)
					{
						if (lParam)
							found = TRUE;
						else
						{
							pLink->m_Windows.erase(pLink->m_Windows.begin() + i);
							return TRUE;
						}
					}

				}
				if (!found && lParam)
					pLink->m_Windows.push_back(wParam);
				return TRUE;
			}
			break;

		case WM_WIPPIEN_VISIBLE:
			{
/*				BOOL found = FALSE;
				for (int i=0;!found && i<pLink->m_Windows.size();i++)
				{
					if (lParam == pLink->m_Windows[i])
					{
						pLink->m_Windows.erase(pLink->m_Windows.begin() + i);
						return TRUE;
					}

				}
*/
				// just obey
				if (lParam)
					_MainDlg.ShowWindow(SW_SHOW);
				else
					_MainDlg.ShowWindow(SW_HIDE);

				return TRUE;
			}
			break;

		case WM_WIPPIEN_DIE:
			{
				if (lParam)
				{
					::PostMessage(_MainDlg.m_hWnd, WM_COMMAND, MAKELONG(ID_EXIT, 0), NULL);
				}
				return TRUE;
			}
			break;

		case WM_WIPPIEN_SAVESETTINGS:
			{
				if (lParam)
				{
					_Settings.Save(FALSE);
				}
				return TRUE;
			}
			break;

		case WM_WIPPIEN_GETSTATUS:
			{
				if (_Jabber && _Jabber->m_Jabb)
				{

					WODXMPPCOMLib::StatusEnum st;
#ifndef _WODXMPPLIB
					if (SUCCEEDED(_Jabber->m_Jabb->get_Status(&st)))
#else
					WODXMPPCOMLib::XMPP_GetStatus(_Jabber->m_Jabb, &st);
#endif
					{
						return st+1;
					}
				}
				return 0;
			}
			break;

		case WM_WIPPIEN_GETSOUND:
				return _Settings.m_SoundOn + 1;
			break;

		case WM_WIPPIEN_SETSOUND:
			{
				_Settings.m_SoundOn = !lParam;
				_MainDlg.ToggleMute();
				return TRUE;
			}
			break;

		case WM_WIPPIEN_SETSTATUS:
			{
				if (_Jabber && _Jabber->m_Jabb)
				{
					if (lParam == 0)
					{
						// we must disconnect
						if (_Jabber)
						{
							_Jabber->Disconnect();
							return TRUE;
						}
					}
					else
					{
						WODXMPPCOMLib::StatusEnum st;
#ifndef _WODXMPPLIB
						if (SUCCEEDED(_Jabber->m_Jabb->get_Status(&st)))
#else
						WODXMPPCOMLib::XMPP_GetStatus(_Jabber->m_Jabb, &st);
#endif
						{
							HRESULT hr = E_FAIL;
							if (st == 0/*Offline*/)
							{
								// connect
								CComBSTR2 j = _Settings.m_JID, p = _Settings.m_Password, s = _Settings.m_ServerHost;
								_Jabber->Connect(j.ToString(), p.ToString(), s.ToString(), _Settings.m_ServerPort, _Settings.m_UseSSLWrapper);
								hr = S_OK;
							}
							else							
							{
								if (_Jabber && _Jabber->m_Jabb)
								{
#ifndef _WODXMPPLIB
									hr = _Jabber->m_Jabb->raw_SetStatus((WODXMPPCOMLib::StatusEnum)/*Online*/lParam);
									if (SUCCEEDED(hr))
										_Jabber->m_Events->DispStateChange((WODXMPPCOMLib::StatesEnum)0);
#else
									WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)lParam, NULL);
									XMPPStateChange(_Jabber->m_Jabb, (WODXMPPCOMLib::StatesEnum)0);
#endif

								}
							}
							if (SUCCEEDED(hr))
								return TRUE;
						}
					}
				}
				return 0;
			}
			break;

		case WM_WIPPIEN_SETPORT:
			{
				_Settings.m_ServerPort = lParam;
				return TRUE;
			}
			break;

		case WM_WIPPIEN_GETPORT:
			{
				return _Settings.m_ServerPort+1;
			}
			break;

		case WM_WIPPIEN_GETUSERCOUNT:
			{
				return _MainDlg.m_UserList.m_Users.size()+1;
			}
			break;

		case WM_WIPPIEN_GETHOSTNAME:
			{
				pLink->SendBuffer((HWND) wParam, _Settings.m_ServerHost);
				return TRUE;
			}
			break;

		case WM_WIPPIEN_GETMYIP:
			{
				char *a = "";
				if (_Ethernet.m_Available)
				{
					struct  in_addr sin_addr;					
					sin_addr.S_un.S_addr = _Settings.m_MyLastNetwork;
					a = inet_ntoa(sin_addr);
				}
				pLink->SendBuffer((HWND) wParam, a);
				return TRUE;
			}
			break;

		case WM_WIPPIEN_GETMYNETMASK:
			{
				char *a = "";
				if (_Ethernet.m_Available)
				{
					struct  in_addr sin_addr;					
					sin_addr.S_un.S_addr = _Settings.m_MyLastNetmask;
					a = inet_ntoa(sin_addr);
				}
				pLink->SendBuffer((HWND) wParam, a);
				return TRUE;
			}
			break;

		case WM_WIPPIEN_GETJID:
			{
				pLink->SendBuffer((HWND) wParam, _Settings.m_JID);
				return TRUE;
			}
			break;

		case WM_WIPPIEN_GETUSERJID:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					pLink->SendBuffer((HWND) wParam, user->m_JID);
					return TRUE;
				}
			}
			break;

		case WM_WIPPIEN_GETUSERRESOURCE:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					pLink->SendBuffer((HWND) wParam, user->m_Resource);
					return TRUE;
				}
			}
			break;

		case WM_WIPPIEN_GETUSEREMAIL:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					pLink->SendBuffer((HWND) wParam, user->m_Email);
					return TRUE;
				}
			}
			break;

		case WM_WIPPIEN_GETUSERVISIBLENAME:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					pLink->SendBuffer((HWND) wParam, user->m_VisibleName);
					return TRUE;
				}
			}
			break;

		case WM_WIPPIEN_GETUSERGROUP:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					pLink->SendBuffer((HWND) wParam, user->m_Group);
					return TRUE;
				}
			}
			break;

		case WM_WIPPIEN_GETUSERBLOCK:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					return user->m_Block+1;
				}
			}
			break;

		case WM_WIPPIEN_GETUSERPRESENCESTATUS:
			{
				int jid1 = lParam;
				HRESULT hr = E_FAIL;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					if (_Jabber && _Jabber->m_Jabb)
					{
						WODXMPPCOMLib::StatusEnum st;
#ifndef _WODXMPPLIB
						
						WODXMPPCOMLib::IXMPPContacts *cts = NULL;
						_Jabber->m_Jabb->get_Contacts(&cts);
						if (cts)
						{
							VARIANT var;
							var.vt = VT_BSTR;
							var.bstrVal = T2BSTR(user->m_JID);

							WODXMPPCOMLib::IXMPPContact *ct = NULL;
							cts->get_Item(var, &ct);
							if (ct)
							{
								hr = ct->get_Status(&st);
								ct->Release();
							}
							else
							{
								st = (WODXMPPCOMLib::StatusEnum)0;
								hr = S_OK;
							}
							cts->Release();
						}
#else
						void *ct = NULL;
						hr = WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, user->m_JID, &ct);
						if (SUCCEEDED(hr))
							WODXMPPCOMLib::XMPP_Contact_GetStatus(ct, &st);
						if (ct)
							WODXMPPCOMLib::XMPP_Contacts_Free(ct);
#endif
						if (SUCCEEDED(hr))
						{
							return st+1;
						}
					}
				}
			}
			break;

		case WM_WIPPIEN_GETUSERVPNSTATUS:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					return user->m_WippienState+1;
				}
			}
			break;

		case WM_WIPPIEN_GETUSERIP:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					struct in_addr in;
					in.S_un.S_addr  = user->m_HisVirtualIP;
					pLink->SendBuffer((HWND) wParam, inet_ntoa(in));
					return TRUE;
				}
			}
			break;

		case WM_WIPPIEN_GETUSERCONNECTEDIP:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					CComBSTR2 ra = user->m_RemoteAddr;
					pLink->SendBuffer((HWND) wParam, ra.ToString());
					return TRUE;
				}
			}
			break;

		case WM_WIPPIEN_GETUSERCONNECTEDPORT:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					return ntohs(user->m_RemotePort)+1;
				}
			}
			break;

/*		case WM_WIPPIEN_GETUSERGROUP:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					pLink->SendBuffer((HWND) wParam, user->m_Group);
					return TRUE;
				}
			}
			break;
*/

		case WM_WIPPIEN_USERCONNECT:
			{
				int jid1 = lParam;
				if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
				{
					CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
					if (user->m_WippienState != WipConnected)
					{
						_MainDlg.m_UserList.ConnectIfPossible(user, TRUE);
						return TRUE;
					}
					else
						if (user->m_WippienState == WipConnected)
							return TRUE;

				}
			}
			break;

		case WM_COPYDATA:
			{				
				PCOPYDATASTRUCT cd = (PCOPYDATASTRUCT) lParam;
				Buffer b;
				b.Append((char *)cd->lpData, cd->cbData);
				b.Append("\0",1);

				switch (cd->dwData)
				{
					case WM_WIPPIEN_SETHOSTNAME:
						_Settings.m_ServerHost = b.Ptr();
						break;

					case WM_WIPPIEN_SETJID:
						_Settings.m_JID = b.Ptr();
						break;

					case WM_WIPPIEN_GETUSERID:
						{
							// locate user
							for (int i = 0; i < _MainDlg.m_UserList.m_Users.size(); i++)
							{
								CUser *user = _MainDlg.m_UserList.m_Users[i];
								if (!strnicmp(user->m_JID, b.Ptr(), b.Len()))
									return i+1;
							}

							b.Append("\0",1);
							// let's add this user
							_MainDlg.m_UserList.AddNewUser(b.Ptr(), NULL);
							return _MainDlg.m_UserList.m_Users.size();
						}
						break;

					case WM_WIPPIEN_SENDMESSAGE:
						{
							int jid1 = b.GetInt();

							char *plaintext1 = b.GetString(NULL);
							char *htmltext1 = b.GetString(NULL);

							if (plaintext1 && htmltext1)
							{
								if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
								{
									CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
									if (_Jabber)
										_Jabber->Message(NULL, user->m_JID, plaintext1, htmltext1);
								
								}
							}

							free(plaintext1);
							free(htmltext1);
						}
						break;

					case WM_WIPPIEN_CHATWINDOW:
						{
							int jid1 = b.GetInt();
							char v = b.GetChar();

							if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
							{
								CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
								if (v)
									user->OpenMsgWindow(TRUE);
								else
									user->CloseMsgWindow();
							}
						}
						break;

					case WM_WIPPIEN_PUTUSERBLOCK:
						{
							int jid1 = b.GetInt();
							char v = b.GetChar();

							if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
							{
								CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
								user->m_Block = v;
								user->NotifyBlock();
							}
						}
						break;

					case WM_WIPPIEN_PUTUSERJID:
						{
							int jid1 = b.GetInt();

							if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
							{
								CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
								char *a = b.GetString(NULL);
								if (a)
								{
									strcpy(user->m_JID, a);
									free(a);
								}
							}
						}
						break;

					case WM_WIPPIEN_PUTUSERRESOURCE:
						{
							int jid1 = b.GetInt();

							if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
							{
								CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
								char *a = b.GetString(NULL);
								if (a)
								{
									strcpy(user->m_Resource, a);
									free(a);
								}
							}
						}
						break;

					case WM_WIPPIEN_PUTUSEREMAIL:
						{
							int jid1 = b.GetInt();

							if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
							{
								CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
								char *a = b.GetString(NULL);
								if (a)
								{
									strcpy(user->m_Email, a);
									free(a);
								}
							}
						}
						break;

					case WM_WIPPIEN_PUTUSERVISIBLENAME:
						{
							int jid1 = b.GetInt();

							if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
							{
								CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
								char *a = b.GetString(NULL);
								if (a)
								{
									strcpy(user->m_VisibleName, a);
									free(a);
								}
							}
						}
						break;

					case WM_WIPPIEN_PUTUSERGROUP:
						{
							int jid1 = b.GetInt();

							if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
							{
								CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
								char *a = b.GetString(NULL);
								if (a)
								{
									strcpy(user->m_Group, a);
									free(a);
								}
							}
						}
						break;

					case WM_WIPPIEN_CHANGEPASSWORD:
						{
							char *aold = b.GetString(NULL);
							if (aold)
							{
								char *anew = b.GetString(NULL);
								if (anew)
								{
									CComBSTR2 np = _Settings.m_PasswordProtectPassword;
									if (!strcmp(np.ToString(), aold))
									{
										_Settings.m_PasswordProtectPassword = anew;
										if (!_Settings.m_PasswordProtectPassword.Length())
											_Settings.m_NowProtected = FALSE;
										_Settings.m_PasswordProtectAll = b.GetChar()?TRUE:FALSE;
									}
									free(anew);
								}
								free(aold);
							}
						}
						break;		
						
					case WM_WIPPIEN_ADDCONTACT:
						{
							char *aold = b.GetString(NULL);
							if (aold)
							{
								BOOL subscribe = b.GetChar();

#ifndef _WODXMPPLIB
								// create new contact
								WODXMPPCOMLib::IXMPPContact *ct;
								WODXMPPCOMLib::IXMPPContacts *cts;

								if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
								{
									CComBSTR b = aold;
									if (SUCCEEDED(cts->raw_Add(b, &ct)))
									{
										if (subscribe)
										{
											try
											{
												ct->Subscribe();
											}
											catch(_com_error e)
											{
												MessageBeep(-1);
											}
										}
									}
									cts->Release();
								}
#else
								void *ct = NULL;
								if (SUCCEEDED(WODXMPPCOMLib::XMPP_ContactsAdd(_Jabber->m_Jabb,aold, &ct)))
								{
									if (ct)
									{
										if (subscribe)
										{
											WODXMPPCOMLib::XMPP_Contact_Subscribe(ct);
										}
									}
								}
								if (ct)
									WODXMPPCOMLib::XMPP_Contacts_Free(ct);
#endif

							}
						}
						break;		
						
					case WM_WIPPIEN_CREATECONTACT:
						{
							char *ajid = b.GetString(NULL);
							if (ajid)
							{
								char *apass = b.GetString(NULL);
								if (apass)
								{
									char *ahost = b.GetString(NULL);
									if (ahost)
									{
										int aport = b.GetInt();


										if (pLink->Wiz)
											delete pLink->Wiz;

										pLink->Wiz = new CJabberWiz(pLink);
										pLink->Wiz->Connect(ajid, apass, ahost, aport, TRUE, _Settings.m_UseSSLWrapper);

										free(ahost);
									}
									free(apass);
								}
								free(ajid);
							}
						}
						break;		
						
					case WM_WIPPIEN_REMOVECONTACT:
						{
							char *aold = b.GetString(NULL);
							if (aold)
							{
								BOOL unsubscribe = b.GetChar();


								if (unsubscribe)
								{
#ifndef _WODXMPPLIB
									// locate this user 
									WODXMPPCOMLib::IXMPPContact *ct;
									WODXMPPCOMLib::IXMPPContacts *cts;
									if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
									{
										VARIANT var;
										var.vt = VT_BSTR;
										CComBSTR b1 = aold;
										var.bstrVal = b1;
										if (SUCCEEDED(cts->get_Item(var, &ct)))
										{
											ct->raw_Unsubscribe();
				//							ct->Remove();

											var.vt = VT_DISPATCH;
											var.pdispVal = ct;
											cts->raw_Remove(var);
											ct->Release();
										}
										cts->Release();
									}
#else
									void *ct = NULL;
									WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, aold, &ct);
									if (ct)
									{
										WODXMPPCOMLib::XMPP_Contact_Unsubscribe(ct);
										WODXMPPCOMLib::XMPP_ContactsRemove(_Jabber->m_Jabb, ct);
										WODXMPPCOMLib::XMPP_Contacts_Free(ct);
									}
#endif

									// and remove contact from list of users
									for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
									{
										CUser *us = _MainDlg.m_UserList.m_Users[i];
										if (!strcmp(us->m_JID, aold))
										{
											_MainDlg.m_UserList.m_Users.erase(_MainDlg.m_UserList.m_Users.begin() + i);
				//							delete user;
											//Refresh(NULL);
											_Settings.Save(TRUE);
											_MainDlg.m_UserList.PostMessage(WM_REFRESH, NULL, FALSE);
											return TRUE;
										}
									}
									_MainDlg.m_UserList.PostMessage(WM_REFRESH, NULL, FALSE);
								}
							}
						}
						break;		
						

					case WM_WIPPIEN_USERCONNECTIP:
						{
							int jid1 = b.GetInt();

							if (jid1>=0 && jid1 < _MainDlg.m_UserList.m_Users.size())
							{
								CUser *user = _MainDlg.m_UserList.m_Users[jid1];	
								if (user->m_WippienState != WipConnected)
								{
/*virtchange									if (user->m_HisVirtualIP)
									{
										_Settings.m_DHCPAddress[user->m_HisDHCPAddressOffset] = 0;
										user->m_HisDHCPAddressOffset = 0;
										user->m_HisDHCPAddress = 0;
									}

									user->m_HisDHCPAddressOffset = b.GetInt();
									user->m_Block = FALSE; //unblocks user automatically
									user->m_StaticIP = TRUE;
//									user->AllocateDHCP();

									_MainDlg.m_UserList.ConnectIfPossible(user, TRUE);
*/
									return TRUE;
								}
								else
									if (user->m_WippienState == WipConnected)
										return TRUE;
							}
						}
						break;
				}
				return TRUE;
			}



		default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return FALSE;
}

