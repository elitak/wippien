// SDKMessageLink.cpp: implementation of the CSDKMessageLink class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../SDK/SDKMessageLink.h"
#include "../../Public/Wippien/Buffer.h"
#include "SDK.h"
#include "WippienSDK.h"


BOOL fWndClassRegistered = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSDKMessageLink::CSDKMessageLink(CWippienSDK *Owner)
{
	m_hWnd = NULL;
	m_RemoteWnd = NULL;
	m_WaitBuffer = new _Buffer();
	m_WaitHandle = FALSE;
	m_Owner = Owner;
}

CSDKMessageLink::~CSDKMessageLink()
{

	if (m_RemoteWnd && m_hWnd)
		PostMessage(m_RemoteWnd, WM_WIPPIEN_INIT, (WPARAM)m_hWnd, 0);

	if (m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	delete m_WaitBuffer;
}


HWND CSDKMessageLink::FindServerWindow(void)
{
	char clsname[128], capname[128];
	sprintf(capname, "Wippien SDK Server Link Window");
	sprintf(clsname, "CWippienSDKMessageLinkClassServer");

	return FindWindow(clsname, capname);
}

void CSDKMessageLink::CreateLinkWindow(void)
{
    WNDCLASS wndclass;

	char clsname[128], capname[128];
	sprintf(capname, "Wippien SDK Client Link Window");
	sprintf(clsname, "CWippienSDKMessageLinkClassClient");

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

LRESULT CALLBACK CSDKMessageLink::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
//	ATLTRACE("stiglo\r\n");
	CSDKMessageLink * pLink = (CSDKMessageLink *)GetWindowLong(hwnd, GWL_USERDATA);
	switch (msg)
	{
		case WM_COPYDATA:
			{
				PCOPYDATASTRUCT cd = (PCOPYDATASTRUCT)lParam;
				_Buffer b;
				b.Append((char *)cd->lpData, cd->cbData);
				
				int result = TRUE;
				switch (cd->dwData) // this is an event
				{
					case WM_WIPPIEN_EVENT_STATECHANGE:
						{
							int oldst = b.GetInt();
							int newst = b.GetInt();
							if (pLink->m_Owner)
								pLink->m_Owner->FireStatusChange(oldst, newst);
						}
						break;

					case WM_WIPPIEN_EVENT_INCOMINGMESSAGE:
						{
							char *contact = b.GetString(NULL);
							char *text = b.GetString(NULL);
							char *html = b.GetString(NULL);
							if (!text)
								text = "";
							if (!html)
								html = "";

							if (contact && text && html)
							{
								CComBSTR c = contact;
								CComBSTR t = text;
								CComBSTR h = html;

								if (pLink->m_Owner)
									pLink->m_Owner->FireIncomingMessage(c,&t,&h);

								if (!t.Length() && !h.Length())
									result = 2; // stop processing
							}
						}
						break;

					case WM_WIPPIEN_EVENT_MEDIATORMESSAGE:
						{
							char *contact = b.GetString(NULL);
							char *text = b.GetString(NULL);
							if (!text)
								text = "";

							if (contact && text)
							{
								CComBSTR c = contact;
								CComBSTR t = text;

								if (pLink->m_Owner)
									pLink->m_Owner->FireMediatorMessage(c,&t);

								if (!t.Length())
									result = 2; // stop processing
							}
						}
						break;

					case WM_WIPPIEN_EVENT_ACCOUNTCREATED:
						{
							char *text = b.GetString(NULL);
							if (!text)
								text = "";

							if (text)
							{
								CComBSTR t = text;

								if (pLink->m_Owner)
									pLink->m_Owner->FireAccountCreated(t);
							}
						}
						break;

					case WM_WIPPIEN_EVENT_AUTHREQUEST:
						{
							char *text = b.GetString(NULL);
							if (!text)
								text = "";

							if (text)
							{
								CComBSTR t = text;

								if (pLink->m_Owner)
									pLink->m_Owner->FireAuthRequest(t);
							}
						}
						break;

					case WM_WIPPIEN_EVENT_DISCONNECTED:
						{
							char *err = b.GetString(NULL);
							if (!err)
								err = "";
							CComBSTR e = err;

							if (pLink->m_Owner)
								pLink->m_Owner->FireDisconnected(e);
						}
						break;

					case WM_WIPPIEN_EVENT_CONNECTED:
						{
							if (pLink->m_Owner)
								pLink->m_Owner->FireConnected();
						}
						break;

					case WM_WIPPIEN_EVENT_USERPRESENCECHANGE:
						{
							char *contact = b.GetString(NULL);
							int st = b.GetInt();
							int vt = b.GetInt();
							if (contact )
							{
								CComBSTR c = contact;

								if (pLink->m_Owner)
									pLink->m_Owner->FireUserPresenceChange(c, st, vt);
							}
						}
						break;

					case WM_WIPPIEN_EVENT_USERVPNCHANGE:
						{
							char *contact = b.GetString(NULL);
							int st = b.GetInt();
							int vt = b.GetInt();
							if (contact )
							{
								CComBSTR c = contact;

								if (pLink->m_Owner)
									pLink->m_Owner->FireUserVPNChange(c, st, vt);
							}
						}
						break;

					case WM_WIPPIEN_EVENT_DIE:
						{
							if (pLink->m_Owner)
								pLink->m_Owner->FireDie();
						}
						break;

					case 0: // no event
						pLink->m_WaitBuffer->Append(b.Ptr(), b.Len());
						pLink->m_WaitBuffer->Append("\0",1);
						pLink->m_WaitBuffer->ConsumeEnd(1);
						pLink->m_WaitHandle = TRUE;
						break;
				}
				return result;
			}
			break;

/*		case WM_WIPPIEN_EVENT_STATECHANGE:
			{
				pLink->m_Owner->FireStatusChange(wParam, lParam);
				return TRUE;
			}
*/
		case WM_TIMER:
				pLink->m_WaitHandle = TRUE;
				return TRUE;

		default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return FALSE;
}

BOOL CSDKMessageLink::SendCommand(int Command, LPARAM lParam)
{
	if (!m_RemoteWnd)
	{
		m_RemoteWnd = FindServerWindow();
		if (m_RemoteWnd)
			SendCommand(WM_WIPPIEN_INIT, 1);
	}
	if (!m_RemoteWnd)
		return FALSE;

	return SendMessage(m_RemoteWnd, Command, (WPARAM)m_hWnd, lParam);
}

BOOL CSDKMessageLink::SendCommandWaitResult(int Command, LPARAM lParam)
{
	if (!m_RemoteWnd)
	{
		m_RemoteWnd = FindServerWindow();
		if (m_RemoteWnd)
			SendCommand(WM_WIPPIEN_INIT, 1);
	}
	if (!m_RemoteWnd)
		return FALSE;

	m_WaitHandle = FALSE;
	m_WaitBuffer->Clear();
	::SetTimer(m_hWnd, 1, 1000, NULL);
	int res = PostMessage(m_RemoteWnd, Command, (WPARAM)m_hWnd, lParam);
	if (res)
	{
		while (!m_WaitHandle && IsWindow(m_hWnd))
		{
			if (WaitMessage())
			{
				MSG msg;
				while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

		} 
	}
	::KillTimer(m_hWnd, 1);
	if (m_WaitBuffer->Len())
		return TRUE;

	return FALSE;
}

BOOL CSDKMessageLink::SendCommand(int Command, char *Data, int Len)
{
	COPYDATASTRUCT cd;

	cd.dwData = Command;
	cd.cbData = Len;
	cd.lpData = Data;

	if (!m_RemoteWnd)
	{
		m_RemoteWnd = FindServerWindow();
		if (m_RemoteWnd)
			SendCommand(WM_WIPPIEN_INIT, 1);
	}
	if (!m_RemoteWnd)
		return FALSE;

	return SendMessage(m_RemoteWnd, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)&cd);
}
