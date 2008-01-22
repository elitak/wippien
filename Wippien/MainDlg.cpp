// MainDlg.cpp: implementation of the CMainDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlcrack.h>
#include "MainDlg.h"
#include "BaloonTip.h"
#include "Settings.h"
#include "ComBSTR2.h"
#include "Notify.h"
#include "colors.h"
#include "AboutDLG.h"
#include "Jabber.h"
#include "Ethernet.h"
#include "SettingsDlg.h"
#include "ContactAuthDlg.h"
#include "BaloonHelp.h"
#include "SDKMessageLink.h"
#include "UpdateHandler.h"
#include "MsgWin.h"
#include "SimpleHttpRequest.h"
#include "SimpleXmlParser.h"


#ifdef _SKINMAGICKEY
#include "SkinMagicLib.h"
#endif
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WODVPNLIB
namespace WODVPNCOMLib
{
#include "\WeOnlyDo\wodVPN\Code\Win32LIB\Win32LIB.h"
}
#endif

void XMPPStateChange(void *wodXMPP, WODXMPPCOMLib::StatesEnum OldState);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define COMPILE_MULTIMON_STUBS
#include "MultiMon.h"

#define MINWIDTH	100
#define MINHEIGHT	200
//#define SNAPWIDTH	30

#define INACTIVITYTIMERCHECK		5000
extern const char * CONFIGURING_ADAPTER_TEXT;
extern CRITICAL_SECTION m_IcmpCS;
extern HINSTANCE m_IcmpInst;

#define REGISTERAPPBAR(x, y); RegisterAppBar(TRUE, (x), (y));
#define UNREGISTERAPPBAR(); RegisterAppBar(FALSE, NULL, NULL);

extern CAppModule _Module;
extern CSettings _Settings;
extern CNotify _Notify;
extern CJabber *_Jabber;
extern CEthernet _Ethernet;
extern CContactAuthDlg *_ContactAuthDlg;
extern CMainDlg _MainDlg;
extern CSDKMessageLink *_SDK;
UINT WM_TASKBARCREATEDMESSAGE = 0;


void ResampleImageIfNeeded(CxImage *img, int size);
BOOL ChangeWindowMessageFilter(          UINT message,
    DWORD dwFlag
);

CMainDlg::CMainDlg()
{
	DumpDebug("*MainDlg::MainDlg\r\n");
	m_PendingVCardRequests = 0;
	m_CanTooltip = TRUE;
	m_WhiteBrush = CreateSolidBrush(WIP_WHITE);
	m_LightBlueBrush = CreateSolidBrush(WIP_LIGHTBLUE);
	
	m_Identity_NameFont = NULL;
	m_Identity_IPFont = NULL;
	m_Identity_StatusFont = NULL;
	m_AppbarRegistered = FALSE;
	m_IMove = FALSE;
	m_IMinimized = FALSE;
	m_AuthDlgBlinker = 0;
	memset(&m_TooltipPoint, 0, sizeof(m_TooltipPoint));
	InitMultipleMonitorStubs();

	m_GoAppbar = FALSE;

	m_ReconnectWait = 1;
	m_WearSkin = FALSE;
	m_DidGoAutoAway = FALSE;

	m_EmoticonsInstance.GetCount = NULL;
	m_EmoticonsInstance.GetItem = NULL;
	m_EmoticonsInstance.hInst = NULL;
	m_SettingsDlg = NULL;
	m_pBalloon = NULL;
//	memset(&m_ToolTipPosition, 0, sizeof(m_ToolTipPosition));
	memset(&m_DefChatWindowRect, 0, sizeof(m_DefChatWindowRect));

	m_LastUpdateMin = 0;
	InitializeCriticalSection(&m_SocketCS);
	InitializeCriticalSection(&m_IcmpCS);

	m_User32Module =::LoadLibrary(_T("User32.dll"));
	m_SimpleHttpRequest = NULL;

}

CMainDlg::~CMainDlg()
{
	if (m_SimpleHttpRequest)
		delete m_SimpleHttpRequest;
	DumpDebug("*MainDlg::~MainDlg\r\n");
	if (_Settings.m_LoadSuccess)
		_Settings.Save(FALSE);
	DeleteObject(m_IconSmall);
	DeleteObject(m_Icon);
	DeleteObject(m_WhiteBrush);
	DeleteObject(m_LightBlueBrush);
	DeleteObject(m_Identity_NameFont);
	DeleteObject(m_Identity_IPFont);
	DeleteObject(m_Identity_StatusFont);

	for (int i=0;i<7;i++)
		DeleteObject(m_OnlineStatus[i]);

//	while (m_EmoticonsInstance.size())
	{
//		EmoticonsStruct *st = (EmoticonsStruct *)m_EmoticonsInstance[0];
		if (m_EmoticonsInstance.hInst)
			FreeLibrary(m_EmoticonsInstance.hInst);
		while (m_EmoticonsInstance.m_Image.size())
		{
			CxImage *i = m_EmoticonsInstance.m_Image[0];
			m_EmoticonsInstance.m_Image.erase(m_EmoticonsInstance.m_Image.begin());
			delete i;
		}


//		delete st;
//		m_EmoticonsInstance.erase(m_EmoticonsInstance.begin());
	}

	if (m_pBalloon)
	{
		if (m_pBalloon->IsWindow())
			m_pBalloon->DestroyWindow();
		delete m_pBalloon;
	}

#ifdef _SKINMAGICKEY
	ExitSkinMagicLib();
#endif
	DeleteCriticalSection(&m_SocketCS);
	DeleteCriticalSection(&m_IcmpCS);

	if (m_IcmpInst)
		FreeLibrary(m_IcmpInst);
	m_IcmpInst = NULL;
	FreeLibrary(m_User32Module);
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
//	DumpDebug("*MainDlg::PreTranslatMessage\r\n");
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
//	DumpDebug("*MainDlg::OnIdle\r\n");
	return FALSE;
}

LRESULT CMainDlg::OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//	return NULL;
	DumpDebug("*MainDlg::OnCltColorDlg\r\n");
	return (LRESULT)m_LightBlueBrush;
}

void CMainDlg::RegisterAppBar(BOOL isregister, RECT *rc, int style)
{
	DumpDebug("*MainDlg:: RegisterAppBar\r\n");
//	ATLTRACE("RegisterAppBar\r\n");
	APPBARDATA	abd;
	memset(&abd, 0, sizeof(APPBARDATA));
	abd.cbSize = sizeof(APPBARDATA);
	abd.hWnd	= m_hWnd;
	abd.uCallbackMessage = WM_APPBRMESSAGE;
	if (isregister)
	{
		if (!m_AppbarRegistered)
		{
			::SHAppBarMessage(ABM_NEW, &abd);
			m_AppbarRegistered = TRUE;
		}
		
		abd.uEdge	= style;
		memcpy(&abd.rc, rc, sizeof(abd.rc));
		//		::SHAppBarMessage(ABM_QUERYPOS, &abd);
		
		// the proposed rect might be changed, we need to ajust the width or height
		// AdjustDockingRect(abd.rc, uFlag);
		
		// set docking pos
		::SHAppBarMessage(ABM_SETPOS, &abd);
	}
	else
	{
		if (m_AppbarRegistered)
		{
			m_AppbarRegistered = FALSE;
			::SHAppBarMessage(ABM_REMOVE, &abd);
		}
	}
}

LRESULT CMainDlg::OnWindowPosChanging(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnWindowPosChanging \r\n");
//	ATLTRACE("OnWindowPosChanging\r\n");
	bHandled = FALSE;
/*
	if (_Settings.m_IsAligned && m_AppbarRegistered)
	{
		WINDOWPOS *wp = (WINDOWPOS *)lParam;
		//		wp->x = m_AppbarRect.left;
		//		bHandled = TRUE;
		wp->flags |= SWP_NOMOVE;
		return FALSE;
	}
*/
	return FALSE;
}
LRESULT CMainDlg::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnWindowPosChanged\r\n");
//	ATLTRACE("OnWidnowPosChanged\r\n");
	bHandled = FALSE;
/*
	if (_Settings.m_IsAligned && m_AppbarRegistered)
	{
		REGISTERAPPBAR(&m_AppbarRect, _Settings.m_RosterSnap);
		MoveWindow(&m_AppbarRect, FALSE);
	}

	 WINDOWPLACEMENT wp;
	 GetWindowPlacement(&wp);
	
	 return OnSize(uMsg, (wp.showCmd & SW_SHOWMINIMIZED)?SIZE_MINIMIZED:0, lParam, bHandled);
*/ 
	return FALSE;
}

LRESULT CMainDlg::OnAppBarNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnAppBarNotify \r\n");
//	ATLTRACE("OnAppBarNotify\r\n");
	APPBARDATA abd; 
	UINT uState; 
	
	abd.cbSize = sizeof(abd); 
	abd.hWnd = m_hWnd; 
	
	switch (wParam)
	{
		case ABN_FULLSCREENAPP:
			// to be implemented
			//...
			break;
		case ABN_POSCHANGED:
			// adjust window position and docking area
//			ATLTRACE("ABN_POSCHANGED\r\n");
			RECT	rc;
			::GetWindowRect(::GetDesktopWindow(), &rc);
			if (_Settings.m_RosterSnap == ABE_LEFT)
			{
				m_AppbarRect.right = rc.left + m_SizeX;
			}
			if (_Settings.m_RosterSnap == ABE_RIGHT)
			{
				m_AppbarRect.left  = rc.right - m_SizeX;		
			}
//			memcpy(&m_AppbarRect, &rc, sizeof(RECT));
			MoveWindow(&m_AppbarRect);
			break;
		case ABN_STATECHANGE:
			// Check to see if the taskbar's always-on-top state has 
			// changed and, if it has, change the appbar's state 
			// accordingly. 
			uState = SHAppBarMessage(ABM_GETSTATE, &abd); 
//			ATLTRACE("ABN_STATECHANGE\r\n");
			SetWindowPos((ABS_ALWAYSONTOP & uState) ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); 
			break;
		case ABN_WINDOWARRANGE:
			// exclude the appbar from window arrangement
			if (lParam)// begin
			{
				ShowWindow(SW_HIDE);
			}
			else // completed
			{
				if (!_Settings.m_NowProtected)
					ShowWindow(SW_SHOW);
			}
			break;
	}
	return 0;
}

LRESULT CMainDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnMoving \r\n");
//	m_GoAppbar = FALSE;
	//	ATLTRACE("OnMoving\r\n");
	POINT cur_pos;
	GetCursorPos(&cur_pos);
	return _OnMoving(TRUE, cur_pos, uMsg, wParam, lParam, bHandled);
}


BOOL isClose(int x, int y)
{
	BOOL isctrl = (GetAsyncKeyState(VK_LCONTROL)&0x8000) || (GetAsyncKeyState(VK_RCONTROL)&0x8000);
	if (isctrl && (GetAsyncKeyState(VK_RMENU)&0x8000))
		isctrl = FALSE; // right-alt was pressed

	if (isctrl)
		return FALSE;

	int ab = abs(x-y);
	BOOL dosnap = TRUE;

	if (_Settings.m_SnapToBorder)
		dosnap = (ab < SNAPWIDTH);
	else
		dosnap = (ab < 1);
	return dosnap;
}


LRESULT CMainDlg::_OnMoving(BOOL adjustmousepos, POINT cur_pos, UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::_OnMoving \r\n");
//	ATLTRACE("_OnMoving\r\n");
    RECT wa;
	RECT *rect = (RECT *)lParam;
	MONITORINFO mi;
	
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	

	if (_Settings.m_IsAligned)
	{
		UNREGISTERAPPBAR();
		_Settings.m_IsAligned = FALSE;
		if (m_SizeX && m_SizeY)
			SetWindowPos(NULL, 0, 0, m_SizeX, m_SizeY, SWP_NOMOVE | SWP_NOZORDER);

//		return 0;
	}


	HMONITOR hmon = MonitorFromPoint(cur_pos, MONITOR_DEFAULTTONEAREST);
	if (hmon && GetMonitorInfo(hmon, &mi))
	{
		memcpy(&wa, &mi.rcWork, sizeof(RECT));
	}
	else
		SystemParametersInfo(SPI_GETWORKAREA, 0, &wa, 0);

	if (!m_SizeX || !m_SizeY)
	{
		BOOL b;
		OnEnterSizeMove(0, 0, 0, b);
	}
	
	if (adjustmousepos)
	    OffsetRect(rect, cur_pos.x - (rect->left + m_MoveX), cur_pos.y - (rect->top + m_MoveY));
	
//	ATLTRACE("_OnMoving check: rect.right=%d, wa.right=%d\r\n", rect->right, wa.right);

    if (isClose(rect->left, wa.left))
	{
//		ATLTRACE("_OnMoving isclose left = TRUE\r\n");
		memcpy(rect, &wa, sizeof(RECT));
		rect->right = rect->left + m_SizeX;
		memcpy(&m_AppbarRect, rect, sizeof(RECT));
		_Settings.m_RosterSnap = ABE_LEFT;
		_Settings.m_IsAligned = TRUE;
	}
    else
		if (isClose(wa.right, rect->right))
		{
//			ATLTRACE("_OnMoving isclose right = TRUE\r\n");
			memcpy(rect, &wa, sizeof(RECT));
			rect->left= rect->right - m_SizeX;
			memcpy(&m_AppbarRect, rect, sizeof(RECT));
			_Settings.m_RosterSnap = ABE_RIGHT;
			_Settings.m_IsAligned = TRUE;
		}
		else
		{
//			ATLTRACE("_OnMoving isclose = FALSE\r\n");
			rect->right = rect->left + m_SizeX;
			rect->bottom = rect->top + m_SizeY;
			if (_Settings.m_IsAligned)
			{
				UNREGISTERAPPBAR();
			}
			_Settings.m_IsAligned = FALSE;
		}
		
		
		if (isClose(rect->top, wa.top))
		{
			OffsetRect(rect, 0, wa.top - rect->top);
		}
		else if (isClose(wa.bottom, rect->bottom))
		{
			OffsetRect(rect, 0, wa.bottom - rect->bottom);
		}

		return TRUE;
}

LRESULT CMainDlg::OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnExitSizeMove \r\n");
//	ATLTRACE("OnExitSizeMove\r\n");
//	m_IMove=FALSE;
	CheckAppbar();


	
	bHandled = FALSE;
	return FALSE;
}

void CMainDlg::CheckAppbar(void)
{
	DumpDebug("*MainDlg::CheckAppbar \r\n");
	if (m_GoAppbar)
	{
		m_GoAppbar = FALSE;
		if (_Settings.m_IsAligned && _Settings.m_DoAlign)
		{
//				m_GoAppbar = TRUE;
			REGISTERAPPBAR(&m_AppbarRect, _Settings.m_RosterSnap);
			_Settings.Save(FALSE);
		}
		SetWindowPos(NULL, 0, 0, m_AppbarRect.right-m_AppbarRect.left, m_AppbarRect.bottom-m_AppbarRect.top, SWP_NOMOVE  | SWP_NOZORDER);
	}
	else
	{
		m_GoAppbar = FALSE;
		UNREGISTERAPPBAR();
		_Settings.Save(FALSE);
	}
}

LRESULT CMainDlg::CalcAlignment(RECT *rcin)
{
	DumpDebug("*MainDlg::CalcAlignment \r\n");
//	ATLTRACE("CalcAlignment\r\n");
	RECT rc;
//	memcpy(&rc, &_Settings.m_RosterRect, sizeof(RECT));
	memcpy(&rc, rcin, sizeof(RECT));
	BOOL bh;
	POINT cur_pos;
	cur_pos.x = rc.left;
	cur_pos.y = rc.top;
	_OnMoving(FALSE, cur_pos, 0, 0, (LPARAM)&rc, bh);
	m_IMove = TRUE;
	OnMove(0,0, (LPARAM)&rc, bh);
	m_IMove = FALSE;

	return FALSE;
}

LRESULT CMainDlg::CalcSizeXSizeY(void)
{
	DumpDebug("*MainDlg::CalcSizeXSizeY \r\n");
//	ATLTRACE("CalcSizeXSizeY\r\n");
    if (m_IMinimized)
		return FALSE;
	CRect rcWindow;
    GetWindowRect(rcWindow);
	m_SizeX = rcWindow.right - rcWindow.left;
	if (!_Settings.m_IsAligned || !m_SizeY)
	{
		m_SizeY = rcWindow.bottom - rcWindow.top;
	}

	return NULL;
}

LRESULT CMainDlg::OnEnterSizeMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnEnterSizeMove \r\n");
//	ATLTRACE("OnEnterSizeMove\r\n");
	m_IMove = TRUE;
	m_GoAppbar = FALSE;
//	UNREGISTERAPPBAR();
	bHandled = FALSE;

	POINT cur_pos;
    CRect rcWindow;
    GetWindowRect(rcWindow);
    GetCursorPos(&cur_pos);
    m_MoveX = cur_pos.x - rcWindow.left;
    m_MoveY = cur_pos.y - rcWindow.top;
/*
	if (!_Settings.m_IsAligned)
	{
//		m_SizeX = rcWindow.right - rcWindow.left;
//		m_SizeY = rcWindow.bottom - rcWindow.top;
//		ATLTRACE("EnterSizeMove x=%d, y=%d\r\n", m_SizeX,m_SizeY);
	}
	else
	{
//		if (!m_SizeX)
//			m_SizeX = rcWindow.right - rcWindow.left;
//		if (!m_SizeY)
//			m_SizeY = rcWindow.bottom - rcWindow.top;

//		UNREGISTERAPPBAR();
	}
*/
	return 0;
}

DWORD WINAPI UpdateThreadProc(LPVOID lpParameter)
{
	// should we check for updates?
//	CUpdateHandler *h = new CUpdateHandler();
	// already have something new?
/* changed with wodAppUpdate
	h->m_Silently = FALSE;
	if (h->ReplaceUpdates())
	{
		return FALSE;
	}
*/
//		if (_Settings.m_CheckUpdate)
	if (_UpdateHandler)
	{
		_UpdateHandler->m_Silently = TRUE;
		_UpdateHandler->DownloadUpdates(TRUE);
	}

	return 0;
}


LRESULT CMainDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnTimer \r\n");
	switch (wParam)
	{
		case 101:
			// this is blinking authdlg
			if (m_AuthDlgBlinker == 0)
				::ShowWindow(GetDlgItem(IDC_MYAUTHDLG), SW_HIDE);
			if (m_AuthDlgBlinker == 1 && _Settings.m_AuthRequests.size())
				::ShowWindow(GetDlgItem(IDC_MYAUTHDLG), SW_SHOW);
				


			m_AuthDlgBlinker++;
			if (m_AuthDlgBlinker >= 10)
				m_AuthDlgBlinker = 0;
			break;

		case 102:
			{
				// if no auto then go out
				if (!_Settings.m_AutoDisconnectMinutes && !_Settings.m_AutoAwayMinutes && !_Settings.m_ExtendedAwayMinutes)
					return TRUE;

				LASTINPUTINFO li = {0};
				li.cbSize = sizeof(LASTINPUTINFO);
				if (GetLastInputInfo(&li))
				{
					unsigned long l1 = GetTickCount() - li.dwTime;
					unsigned long l = l1/60000L; // minutes
					
					// get our status
					WODXMPPCOMLib::StatusEnum stat;
#ifndef _WODXMPPLIB					
					if (_Jabber && SUCCEEDED(_Jabber->m_Jabb->get_Status(&stat)))
#else
					if (_Jabber && SUCCEEDED(WODXMPPCOMLib::XMPP_GetStatus(_Jabber->m_Jabb, &stat)))
#endif
					{
						if (m_DidGoAutoAway && _Settings.m_AutoSetBack && l1<=INACTIVITYTIMERCHECK)

						/*							(_Settings.m_AutoAwayMinutes?_Settings.m_AutoAwayMinutes:
							(_Settings.m_ExtendedAwayMinutes?_Settings.m_ExtendedAwayMinutes:
							(_Settings.m_AutoDisconnectMinutes?_Settings.m_AutoDisconnectMinutes:0)
																							)
																					 )
																				)*/
						{
							m_DidGoAutoAway = FALSE;
							VARIANT var;
							var.vt = VT_ERROR;
							if (stat == 2/*Away*/ || stat == 5/*ExtendedAway*/) // but now 1
#ifndef _WODXMPPLIB
								_Jabber->m_Jabb->SetStatus((WODXMPPCOMLib::StatusEnum)1/*Online*/, var);
#else
							WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)1,NULL);
#endif
						}
						if (_Settings.m_AutoDisconnectMinutes && l > _Settings.m_AutoDisconnectMinutes)
						{
#ifndef _WODXMPPLIB
							_Jabber->m_Jabb->Disconnect();
#else
							WODXMPPCOMLib::XMPP_Disconnect(_Jabber->m_Jabb);
#endif
							ShowStatusText("Disconnected due to inactivity rule");
						}
						else
						if (_Settings.m_ExtendedAwayMinutes && l > _Settings.m_ExtendedAwayMinutes)
						{
							m_DidGoAutoAway = TRUE;
							VARIANT var;
							var.vt = VT_ERROR;
							if (_Settings.m_ExtendedAwayMessage.Length())
							{
								var.vt = VT_BSTR;
								var.bstrVal = _Settings.m_ExtendedAwayMessage;
							}
							if (stat == 1/*Online*/ || stat == 2/*Away*/) 
#ifndef _WODXMPPLIB
								_Jabber->m_Jabb->SetStatus((WODXMPPCOMLib::StatusEnum)5/*ExtAway*/, var);
#else
							{
								CComBSTR2 et1 = _Settings.m_ExtendedAwayMessage;
								WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)5, et1.ToString());
							}
#endif
						}
						else
						if (_Settings.m_AutoAwayMinutes && l > _Settings.m_AutoAwayMinutes)
						{
							m_DidGoAutoAway = TRUE;
							VARIANT var;
							var.vt = VT_ERROR;
							if (_Settings.m_AutoAwayMessage.Length())
							{
								var.vt = VT_BSTR;
								var.bstrVal = _Settings.m_AutoAwayMessage;
							}
							if (stat == 1/*Online*/) 
#ifndef _WODXMPPLIB
								_Jabber->m_Jabb->SetStatus((WODXMPPCOMLib::StatusEnum)2/*Away*/, var);
#else
							{
								CComBSTR2 et1 = _Settings.m_AutoAwayMessage;
								WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)5, et1.ToString());
							}
#endif
						}
					}
				}
			}
			break;

		case 103:
			{
				KillTimer(103);
				if (!m_CanTooltip)
					return FALSE;

				POINT p;
				GetCursorPos(&p);
				RECT rc;
				m_UserList.GetWindowRect(&rc);
				if (p.x == (m_TooltipPoint.x+rc.left) && p.y == (m_TooltipPoint.y+rc.top))
				{
					TVHITTESTINFO tvht = {0};
					HTREEITEM hitTarget;

					tvht.pt.x  = p.x - rc.left;
					tvht.pt.y  = p.y - rc.top;

					if (hitTarget = (HTREEITEM)::SendMessage(m_UserList.m_hWndParent, TVM_HITTEST, NULL, (LPARAM)&tvht)) // if there is a hit
					{
						CUser *user = (CUser *)m_UserList.GetItemData(hitTarget);
						if (user)
						{
							Buffer textbuff;
							char buff[1024];
							textbuff.Append(user->m_SubText);
							textbuff.Append("\r\n\r\n");


							if (user->m_WippienState == WipConnected)
							{
								CComBSTR2 ra = user->m_RemoteAddr;
								sprintf(buff, "%s (%d)\r\nMTU: %d", ra.ToString(), user->m_RemotePort, user->m_MTU);
								textbuff.Append(buff);
							}
							else
							{
								sprintf(buff, "<< VPN not established yet >>");
								textbuff.Append(buff);
							}

							if (m_pBalloon)
							{
								if (m_pBalloon->IsWindow())
									m_pBalloon->DestroyWindow();
								delete m_pBalloon;
							}
							m_pBalloon =new CBalloonHelp;
							CxImage img;
//							if (user->m_Icon.Len())
							if (user->LoadUserImage(img))
							{
//								img->Decode((unsigned char *)user->m_Icon.Ptr(), user->m_Icon.Len(), CXIMAGE_FORMAT_PNG);
								ResampleImageIfNeeded(&img, 100);
							}

							Buffer b1;
							b1.Append(user->m_JID);
							if (*user->m_Resource)
							{
								b1.Append("\r\n(");
								b1.Append(user->m_Resource);
								b1.Append(")");
							}
							b1.Append("\0",1);
							m_pBalloon->Create(m_hWnd, b1.Ptr(), textbuff.Ptr(), &img, &p,	
							CBalloonHelp::BallonOptions::BOCloseOnButtonDown | 
							CBalloonHelp::BallonOptions::BOCloseOnButtonUp | 
							CBalloonHelp::BallonOptions::BOCloseOnMouseMove | 
							CBalloonHelp::BallonOptions::BOCloseOnKeyDown | 
							CBalloonHelp::BallonOptions::BOCloseOnAppDeactivate | 
							CBalloonHelp::BallonOptions::BODisableFadeOut | 
							CBalloonHelp::BallonOptions::BOShowTopMost | 
							/*CBalloonHelp::BallonOptions::BODeleteThisOnClose, // delete pBalloon on close */
							NULL,
							0);
//							delete img;
							SetTimer(108, 7000);
						}
					}
				}
			}
			break;
	
	
			case 108:
				if (m_pBalloon)
				{
					if (m_pBalloon->IsWindow())
						m_pBalloon->DestroyWindow();
					delete m_pBalloon;
					m_pBalloon = NULL;
				}
				KillTimer(108);
				break;

			case 999:
				{

					if (_SDK)
					{
						Buffer b;
						b.PutCString("");
						b.PutCString(m_SimpleHttpRequest->m_Out.Ptr());
						
						if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_MEDIATORMESSAGE, b.Ptr(), b.Len()))
							return FALSE;
					}

					// not real timer, but fired when below simplehttprequest finishes
					CXmlParser xml;
					xml.SkipHeaders(&m_SimpleHttpRequest->m_Out);
					CXmlEntity *xmlent = xml.Parse(&m_SimpleHttpRequest->m_Out);
					if (xmlent)
					{
						if (_Settings.m_ObtainIPAddress == 1)
						{
							// what's our IP?
							CXmlEntity *virtip = CXmlEntity::FindByName(xmlent, "VirtualIP", 1);
							if (virtip)
							{
								CXmlEntity *virtmask = CXmlEntity::FindByName(xmlent, "VirtualMask", 1);
								if (virtmask)
								{
									_Settings.m_MyLastNetwork = 0;
									_Settings.m_MyLastNetmask = 0;
									_Ethernet.Start(inet_addr(virtip->Value), inet_addr(virtmask->Value));
									_Settings.Save(FALSE);
								}
							}
						}

						if (_Settings.m_UseLinkMediatorFromDatabase)
						{
							CXmlEntity *start = NULL;
							do 
							{
								start = CXmlEntity::FindByName(xmlent, "Mediator", 1);
								if (start)
								{
									CXmlEntity *medip = CXmlEntity::FindByName(start, "IP", 1);
									CXmlEntity *medport = CXmlEntity::FindByName(start, "UDPPort", 1);
									if (medip && medport)
									{
										_Settings.m_LinkMediator = medip->Value;
										_Settings.m_LinkMediatorPort = atol(medport->Value);
										_Settings.Save(FALSE);
										break; //TODO REMOVE THIS!
									}
									start->Name[0] = 0; // to disable this mediator from future search
								}
							} while(start);
						}

						// special fix for Roland
						{
							CXmlEntity *ent = CXmlEntity::FindByName(xmlent, "Restart", 1);
							if (ent)
							{
								_Jabber->m_DoReconnect = TRUE;
#ifndef _WODXMPPLIB
								_Jabber->m_Jabb->Disconnect();
#else
								WODXMPPCOMLib::XMPP_Disconnect(_Jabber->m_Jabb);
#endif
							}
						}
					}

					if (m_SimpleHttpRequest)
						delete m_SimpleHttpRequest;
					m_SimpleHttpRequest = NULL;
				}
				break;

			case 106:
				{
					// this is connect timer
					if (m_ReconnectWait)
					{
						char buff[1024];
						sprintf(buff, "Reconnect in %d seconds.", m_ReconnectWait);
						ShowStatusText(buff);
					}
	
					if (m_ReconnectWait < 1)
					{
						KillTimer(106);
						CComBSTR2 j = _Settings.m_JID, p = _Settings.m_Password, s = _Settings.m_ServerHost;

						if (_Settings.m_UseLinkMediatorFromDatabase || _Settings.m_ObtainIPAddress == 1)
						{
							// send request to obtain our IP address
							if (m_SimpleHttpRequest)
								delete m_SimpleHttpRequest;
							m_SimpleHttpRequest = new CSimpleHttpRequest(m_hWnd, 999); // which timer event occurs?
							CComBSTR j1 = _Settings.m_IPProviderURL;
							j1 += _Settings.m_JID;
							CComBSTR2 j2 = j1;
							m_SimpleHttpRequest->Get(j2.ToString());
						}

						// now connect!
						_Jabber->Connect(j.ToString(), p.ToString(), s.ToString(), _Settings.m_ServerPort, _Settings.m_UseSSLWrapper);
					}
					
					if (m_ReconnectWait>0)
						m_ReconnectWait--;
					
				}
				break;

			case 109:
				// check for updates
				{
					if (_Settings.m_CheckUpdateTimed)
					{
						// get our status
						WODXMPPCOMLib::StatesEnum stat;
#ifndef _WODXMPPLIB
						if (_Jabber && SUCCEEDED(_Jabber->m_Jabb->get_State(&stat)))
#else
						if (_Jabber && SUCCEEDED(WODXMPPCOMLib::XMPP_GetState(_Jabber->m_Jabb, &stat)))
#endif
						{
							int i = (int)stat;
							if (i>0)
							{
								m_LastUpdateMin++;
								if (m_LastUpdateMin > _Settings.m_CheckUpdateTimedNum)
								{
									m_LastUpdateMin = 0;
									// request update
//									DWORD thrid = 0;
//									HANDLE thr = CreateThread(NULL, 0, UpdateThreadProc, this, 0, &thrid);
//									CloseHandle(thr);
									UpdateThreadProc(this);
								}
							}
						}
						else
							m_LastUpdateMin = 0;
					}
				}
				break;

			case 111:
				{
					::KillTimer(m_hWnd, 111);
					if (_Settings.m_CheckUpdate)
					{
						_UpdateHandler->m_Silently = _Settings.m_CheckUpdateSilently;
						_UpdateHandler->DownloadUpdates(TRUE);
					}
				}
				break;

			case 110:
				{
					// MTU setting
					if (_Settings.m_FixedMTUNum)
					{
						// read from registry
						int regmtu = 0;

						char bf4[1024];
						sprintf(bf4, "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\%s", _Ethernet.m_Guid);
						HKEY key1;
						if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, bf4, 0, KEY_ALL_ACCESS, &key1) == ERROR_SUCCESS)
						{
							DWORD dwtype = REG_DWORD;
							DWORD cb = sizeof(bf4);
							if (RegQueryValueEx(key1, "MTU", NULL, &dwtype, (unsigned char *)bf4, &cb) == ERROR_SUCCESS)
							{
								DWORD *t = (DWORD *)bf4;
								regmtu = *t;
							}

							if (regmtu && regmtu == _Settings.m_FixedMTUNum)
							{
								// all ok, change timer to one minute
								SetTimer(110, 60000L);
							}
							else
							{
								RegSetValueEx(key1, "MTU", NULL, REG_DWORD, (unsigned char *)&_Settings.m_FixedMTUNum, sizeof(_Settings.m_FixedMTUNum));
								ATLTRACE("setting MTU to %d\r\n", _Settings.m_FixedMTUNum);
//								sprintf(bf4, "netsh interface ip set interface \"Wippien\" MTU=%d store=persistent", _Settings.m_FixedMTUNum);
//								if (_Ethernet.ExecNETSH(bf4))
								{
									_Settings.Save(FALSE);
									// specify new one and change to 10 seconds
									SetTimer(110, 10000L);
								}
							}
							RegCloseKey(key1);
						}
					}

				}
				break;

			
			case 105:
				// look for images and details and delete jabber debug log
				{

#ifndef _WODXMPPLIB
					WODXMPPCOMLib::IXMPPContacts *cts = NULL;
					WODXMPPCOMLib::IXMPPContact *ct = NULL;
#endif

					// let's see if jabber log is also above specified size
					if (_Settings.m_JabberDebugFile.Length())
					{
						CComBSTR2 jabfile = _Settings.m_JabberDebugFile;
						int handle = open(jabfile.ToString(), O_BINARY | O_RDONLY, S_IREAD | S_IWRITE);
						if (handle != (-1))
						{
							long len = filelength(handle);
							close(handle);
							if (len> _Settings.m_DeleteFunctionLogMb)
								unlink(jabfile.ToString());	

							close(handle);
						}
					}


/*
					// let's remove if there are any nonsubscribed user
					USES_CONVERSION;
					
					BOOL loopmore = TRUE;
					if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
					{
						short cnt = 0;
						if (SUCCEEDED(cts->get_Count(&cnt)))
						{
							for (int i=0;loopmore && i<cnt;i++)
							{
								VARIANT var;
								var.vt = VT_I2;
								var.iVal = i;

								if (SUCCEEDED(cts->get_Item(var, &ct)))
								{
									WODXMPPCOMLib::SubscriptionsEnum sub = (WODXMPPCOMLib::SubscriptionsEnum)0;
									ct->get_Subscription(&sub);
									if (!sub)
									{
										CComBSTR2 bs;
										ct->get_JID(&bs);
										ct->raw_Unsubscribe();
										CUser * user = _MainDlg.m_UserList.GetUserByJID(bs.ToString());
			
										var.vt = VT_DISPATCH;
										var.pdispVal = ct;
										cts->raw_Remove(var);
										loopmore = FALSE;

										// and remove contact from list of users
										for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
										{
											CUser *us = _MainDlg.m_UserList.m_Users[i];
											if (us == user)
											{
												_MainDlg.m_UserList.m_Users.erase(_MainDlg.m_UserList.m_Users.begin() + i);
												delete user;
												//Refresh(NULL);
												_Settings.Save(TRUE);
												PostMessage(WM_REFRESH, NULL, FALSE);
												break;
											}
										}
									}
									ct->Release();
								}
							}
						}
						cts->Release();
					}
*/


					time_t now;
					time( &now );
					
					for (int i=0;i<m_UserList.m_Users.size();i++)
					{
						CUser *us = (CUser *)m_UserList.m_Users[i];
						if (us->m_Online)
						{
							us->m_LastOnline = now;
							us->SetSubtext();
						}
					}


					if (_Settings.m_ShowContactPicture)
					{
						typedef std::vector<int> ints;
						ints intcol;
								
						for (int i=0;i<m_UserList.m_Users.size();i++)
						{
							CUser *us = (CUser *)m_UserList.m_Users[i];
							if (us->m_Online)
							{
								if ((now - REFRESHUSERDETAILS) > us->m_GotVCard)
									intcol.push_back(i);
							}
						}
						if (!intcol.size())
						{
							for (int i=0;i<m_UserList.m_Users.size();i++)
							{
								CUser *us = (CUser *)m_UserList.m_Users[i];
								if (now - REFRESHUSERDETAILS > us->m_GotVCard)
									intcol.push_back(i);
							}
						}

						if (intcol.size())
						{
							srand( (unsigned)time( NULL ) );
							int r = rand()%(intcol.size());

							CUser *us = (CUser *)m_UserList.m_Users[intcol[r]];

#ifndef _WODXMPPLIB
							if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
							{
								VARIANT var;
								var.vt = VT_BSTR;
								var.bstrVal = T2BSTR(us->m_JID);
								if (SUCCEEDED(cts->get_Item(var, &ct)))
								{
									// and request vcard
									WODXMPPCOMLib::IXMPPVCard *vc;
									if (SUCCEEDED(ct->get_VCard(&vc)))
									{
										vc->Receive();
										vc->Release();
									}
									ct->Release();
								}
								::SysFreeString(var.bstrVal);
								cts->Release();
							}
#else
							void *ct = NULL;
							if (SUCCEEDED(WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, us->m_JID, &ct)) && ct)
							{
								WODXMPPCOMLib::XMPP_Contact_VCard_Receive(ct);
							}
#endif
						}
					}					
				}
				break;

	}

	return TRUE;
}

LRESULT CMainDlg::OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnSizing \r\n");
//	ATLTRACE("OnSizing\r\n");

	LPRECT rc = (LPRECT)lParam;
	if (_Settings.m_IsAligned)
	{
		// no resize while topmost
		memcpy(rc, &m_AppbarRect, sizeof(RECT));
		return FALSE;
	}
	if (rc->right - rc->left < MINWIDTH)
	{
		if (wParam == WMSZ_RIGHT || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_TOPRIGHT)
			rc->right = rc->left + MINWIDTH;
		else
			rc->left = rc->right - MINWIDTH;
	}
	if (rc->bottom - rc->top < MINHEIGHT)
	{
		if (wParam == WMSZ_BOTTOM || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_BOTTOMLEFT)
			rc->bottom = rc->top + MINHEIGHT;
		else
			rc->top = rc->bottom - MINHEIGHT;
	}
	return OnSizeImpl(uMsg, /*wParam*/0, lParam, bHandled);
}

LRESULT CMainDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnSize \r\n");
	LRESULT r = OnSizeImpl(uMsg, wParam, lParam, bHandled);
	Invalidate();
	return r;
}

LRESULT CMainDlg::OnSizeImpl(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnSizeImpl \r\n");
//	ATLTRACE("OnSize\r\n");

	if (wParam == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
		m_IMove = TRUE;
		m_IMinimized = TRUE;
		if (_Settings.m_PasswordProtectPassword)
			_Settings.m_NowProtected = TRUE;

		if (_Settings.m_IsAligned)
		{
			UNREGISTERAPPBAR();
		}
		SetWindowPos(NULL, 0,0,m_SizeX, m_SizeY, SWP_NOZORDER | SWP_NOMOVE);
		ShowWindow(SW_HIDE);
		if (m_EmptyWin)
			::ShowWindow(m_EmptyWin, SW_HIDE);
		_Settings.m_IsAligned = FALSE;
	}
	else
	{
		m_IMinimized = FALSE;
		RecalcInternalItems();

		if (!_Settings.m_IsAligned)
			CalcSizeXSizeY();

		 if (!m_IMinimized)
		 {
			RECT rc;
			GetWindowRect(&rc);
			if (rc.right - rc.left > 0)
			{
				m_DefChatWindowRect.left = rc.left - 600;
				m_DefChatWindowRect.top = rc.top;
				m_DefChatWindowRect.right = rc.left;
				m_DefChatWindowRect.bottom = 600;

				if (m_DefChatWindowRect.left<0)
					m_DefChatWindowRect.left = 0;
				if (m_DefChatWindowRect.top<0)
					m_DefChatWindowRect.top = 0;

				if (m_DefChatWindowRect.right<m_DefChatWindowRect.left+200)
					m_DefChatWindowRect.right = m_DefChatWindowRect.left + 200;
				if (m_DefChatWindowRect.bottom<m_DefChatWindowRect.top+200)
					m_DefChatWindowRect.bottom = m_DefChatWindowRect.top+200;
			}
		 }

	}
	return FALSE;
}

void CMainDlg::RecalcInternalItems(void)
{
	RECT rc;
	GetClientRect(&rc);
	HWND hBtn1 = GetDlgItem(IDB_SETUP);
	HWND hBtn2 = GetDlgItem(ID_APP_ABOUT);
	HWND hBtn3 = GetDlgItem(ID_EXIT);

	
	HWND hList = GetDlgItem(IDC_TREELIST);
	HWND hStat = GetDlgItem(IDC_STATUS);

	HWND hBtn4 = GetDlgItem(IDC_MYSTATUS);
	HWND hBtn5 = GetDlgItem(IDC_MYCONTACTS);
	HWND hBtn6 = GetDlgItem(IDC_CHATROOMS);
	HWND hBtn7 = GetDlgItem(IDC_SMALLMUTE);
	HWND hBtn8 = GetDlgItem(IDC_MYAUTHDLG);


	int size = rc.right/3;
	
	::SetWindowPos(hBtn1, NULL, rc.left, rc.bottom - 25, size, 25, SWP_NOZORDER);
	::SetWindowPos(hBtn2, NULL, (rc.right - size)/2, rc.bottom - 25, size, 25, SWP_NOZORDER);
	::SetWindowPos(hBtn3, NULL, rc.right - size, rc.bottom - 25, size, 25, SWP_NOZORDER);


	int ftrsize = 0;
	if (_Settings.m_ShowMyStatus)
	{
		ftrsize = 25;
		::SetWindowPos(hStat, NULL, rc.left, rc.bottom - 51, rc.right, 25, SWP_NOZORDER);
		::ShowWindow(hStat, SW_SHOW);
	}
	else
		::ShowWindow(hStat, SW_HIDE);

	int hdrsize = 0;
	if (_Settings.m_ShowMyIP || _Settings.m_ShowMyName || _Settings.m_ShowMyPicture)
		hdrsize = 58;

	::SetWindowPos(hList, NULL, rc.left, rc.top + hdrsize + 22, rc.right, rc.bottom - 51 - hdrsize - ftrsize+3, SWP_NOZORDER);

	::SetWindowPos(hBtn4, NULL, rc.left, rc.top+hdrsize, 20, 20, SWP_NOZORDER);
	::SetWindowPos(hBtn5, NULL, rc.left + 20, rc.top + hdrsize, 20, 20, SWP_NOZORDER);
	::SetWindowPos(hBtn6, NULL, rc.left + 40, rc.top + hdrsize, 20, 20, SWP_NOZORDER);
	::SetWindowPos(hBtn7, NULL, rc.left + 60, rc.top + hdrsize, 20, 20, SWP_NOZORDER);
	::SetWindowPos(hBtn8, NULL, rc.left + 80, rc.top + hdrsize, 20, 20, SWP_NOZORDER);
}


LRESULT CMainDlg::OnMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnMove \r\n");
//	WINDOWPLACEMENT pc;
//	GetWindowPlacement(&pc);

	if (m_IMinimized)
	{
		RECT rc;
		GetWindowRect(&rc);
		m_IMinimized = FALSE;
		CalcAlignment(&rc);
		if (_Settings.m_IsAligned && _Settings.m_DoAlign)
		{
			REGISTERAPPBAR(&m_AppbarRect, _Settings.m_RosterSnap);
		}
		if (m_EmptyWin)
		{
			if (_Settings.m_ShowInTaskbar)
				::ShowWindow(m_EmptyWin, SW_SHOW);
			else
				::ShowWindow(m_EmptyWin, SW_HIDE);
		}
		return FALSE;
	}
	if (m_IMove)
	{
//		int xPos = (int)(short) LOWORD(lParam);   // horizontal position 
//		int yPos = (int)(short) HIWORD(lParam);   // vertical position 
		if (_Settings.m_IsAligned)
		{
//			ATLTRACE("OnMove setting appbar\r\n");
//			SetWindowPos(NULL, xPos, yPos, m_AppbarRect.right-m_AppbarRect.left, m_AppbarRect.bottom-m_AppbarRect.top, ((xPos|yPos)?0:SWP_NOMOVE) | SWP_NOZORDER);
			m_GoAppbar = TRUE;
//			REGISTERAPPBAR(&m_AppbarRect, _Settings.m_RosterSnap);
		}
		else
		{
			m_GoAppbar = FALSE;
//			ATLTRACE("OnMove removing appbar\r\n");
			UNREGISTERAPPBAR();
//			SetWindowPos(NULL, xPos, yPos, m_SizeX, m_SizeY, ((xPos|yPos)?0:SWP_NOMOVE) | SWP_NOZORDER);
		}
	}

	return FALSE;
}


LRESULT CMainDlg::OnEraseBkGnd(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnEraseBackground \r\n");
	bHandled = TRUE;
	
	HBRUSH brush = (HBRUSH)SendMessage(WM_CTLCOLORDLG, wParam, (LPARAM)m_hWnd);
	if (!brush)
		brush = (HBRUSH)::DefWindowProc(m_hWnd, WM_CTLCOLORDLG, wParam, (LPARAM)m_hWnd);
	if (brush)
	{
		RECT rect;
		HDC hdc = (HDC)wParam;
		::GetClientRect(m_hWnd, &rect);
		rect.top = rect.bottom - 51;
		DPtoLP(hdc, (LPPOINT)&rect, 2);
		FillRect(hdc, &rect, brush);
	}
	
	return TRUE;
}

LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnPaint \r\n");
	CPaintDC dc(m_hWnd);
	CDC dc_ff;			// Memory base device context for flicker free painting
	CBitmap bm_ff;		// The bitmap we paint into
	HBITMAP bm_old;
	
	
	CRect rt, rtorg;
	GetClientRect(&rt);
	GetClientRect(&rtorg);
	
	// Create an offscreen dc to paint with (prevents flicker issues)
	dc_ff.CreateCompatibleDC(dc);
	bm_ff.CreateCompatibleBitmap(dc, rt.Width(), rt.Height());
	
	// Select the bitmap into the off-screen DC.
	bm_old = dc_ff.SelectBitmap(bm_ff);
	
	FillRect(dc_ff, &rt, m_LightBlueBrush);

	if (_Settings.m_ShowMyPicture)
	{
		// draw user icon
		m_UserImage.Draw(dc_ff, 2, 2);
	}
	
	// draw user nickname
	HGDIOBJ oldfont = SelectObject(dc_ff, m_Identity_NameFont);
	::SetTextColor(dc_ff, WIP_IDENTITYNAMETEXT);
	::SetBkColor(dc_ff, WIP_LIGHTBLUE);
	if (_Settings.m_ShowMyPicture)
		rt.left += 60;
	else
		rt.left += 10;

	rt.top += 2;
	
	if (_Settings.m_ShowMyName)
	{
		CComBSTR2 j = _Settings.m_JID;
		char *j1 = j.ToString();
		char *j2 = strchr(j1, '@');
		if (j2)
			*j2 = 0;
		::DrawText(dc_ff, j1, strlen(j1), &rt, DT_LEFT | DT_NOPREFIX); 
		//::TextOut(dc, ps.rcPaint.left, ps.rcPaint.top, _Settings.Nickname, strlen(_Settings.Nickname));
		rt.top += 20;
	}	
	if (_Settings.m_ShowMyIP)
	{
		char *a = "No network";
		if (_Ethernet.m_Available)
		{
			struct  in_addr sin_addr;
		
			sin_addr.S_un.S_addr = _Settings.m_MyLastNetwork;
			a = inet_ntoa(sin_addr);
		}
		SelectObject(dc_ff, m_Identity_IPFont);
		::SetTextColor(dc_ff, WIP_IDENTITYIPTEXT);
		::DrawText(dc_ff, a, strlen(a), &rt, DT_LEFT | DT_NOPREFIX); 
		SelectObject(dc_ff, oldfont);
		rt.top += 14;
	}	



	char *st = NULL;
#ifndef _WODXMPPLIB
	CComBSTR2 k;
	VARIANT var;
	var.vt = VT_ERROR;
	if (SUCCEEDED(_Jabber->m_Jabb->get_StatusText(var, &k)))
	{
		st = k.ToString();
	}
#else
	char stat[1024] = {0};	
	int slen = 1024;
	WODXMPPCOMLib::XMPP_GetStatusText(_Jabber->m_Jabb, stat, &slen);
	st = stat;
#endif
	if (_Settings.m_ShowMyIP)
	{
		SelectObject(dc_ff, m_Identity_StatusFont);
		::SetTextColor(dc_ff, WIP_IDENTITYSTATUSTEXT);
		if (_Settings.m_ShowMyIP)
			::DrawText(dc_ff, st, strlen(st), &rt, DT_LEFT | DT_NOPREFIX); 
		SelectObject(dc_ff, oldfont);
	}	





	// Now Blt the changes to the real device context - this prevents flicker.
	dc.BitBlt(rtorg.left, rtorg.top, rtorg.Width(), 80, dc_ff, 0, 0, SRCCOPY);
	dc.BitBlt(0, rtorg.bottom - 51, rtorg.Width(), 51, dc_ff, 0, rtorg.bottom - 51, SRCCOPY);
	
	// dc_ff.SelectFont(old_font);
	// dc_ff.SetBkMode(old_mode);
	dc_ff.SelectBitmap(bm_old);
	
	return TRUE;
}

LRESULT CALLBACK EmptyWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DumpDebug("*MainDlg::EmptyWndProc \r\n");
	switch (uMsg)
	{
		case WM_SETFOCUS:
			if (!_MainDlg.m_IMinimized)
			{
				if (!_Settings.m_NowProtected)
				{
					_MainDlg.ShowWindow(SW_SHOW);
					SetForegroundWindow(_MainDlg.m_hWnd);
				}
			}
			break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return TRUE;
}


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnInitDialog \r\n");

//	m_AppbarRegistered = TRUE;
//	UNREGISTERAPPBAR();

    HDC hdc;
    long lfHeightName, lfHeightIP;
    
    hdc = ::GetDC(NULL);
    lfHeightName = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    lfHeightIP = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ::ReleaseDC(NULL, hdc);
    m_Identity_NameFont = CreateFont(lfHeightName, 0, 0, 0, FW_BOLD, TRUE, 0, 0, 0, 0, 0, 0, 0, "Arial");
    m_Identity_IPFont = CreateFont(lfHeightIP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	m_Identity_StatusFont = CreateFont(lfHeightIP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");


	// center the dialog on the screen
//	CenterWindow();
	_Notify.Init(this);
	
	SetWindowText("Wippien");
	m_UserList.Init(this, GetDlgItem(IDC_TREELIST));

	// wear skin
	m_WearSkin = FALSE;
	long exstyle = GetWindowLong(GWL_EXSTYLE);
	exstyle |= WS_EX_TOOLWINDOW;
	exstyle -= WS_EX_TOOLWINDOW;
	long style = GetWindowLong(GWL_STYLE);
	style |= WS_SYSMENU;
#ifdef _SKINMAGICKEY
	InitSkinMagicLib(_Module.GetModuleInstance(), "WippienExe" , _SKINMAGICKEY, NULL);
	if (_Settings.m_Skin.Length())
	{
		char buff[MAX_PATH];
		strcpy(buff, _Settings.m_MyPath);
		strcat(buff, "Skin\\");
		CComBSTR2 mss = _Settings.m_Skin;
		strcat(buff, mss.ToString());

		if (LoadSkinFile(buff))
			m_WearSkin = TRUE;

		// remove toolwindow
		if (m_WearSkin)
		{
			exstyle |= WS_EX_TOOLWINDOW;
			style -= WS_SYSMENU;
			SetWindowLong(GWL_STYLE, style);
			SetWindowLong(GWL_EXSTYLE, exstyle);
			// remove toolwindow attribute
			SetWindowSkin(m_hWnd, "MainFrame");
		}
	}
	if (!m_WearSkin)
	{
		SetWindowLong(GWL_STYLE, style);
		SetWindowLong(GWL_EXSTYLE, exstyle);
	}

     HMENU hSysMenu=GetSystemMenu(FALSE);
     DeleteMenu(hSysMenu, 6, MF_BYPOSITION);
     DeleteMenu(hSysMenu, 5, MF_BYPOSITION);


#endif

	// set icons
	m_Icon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_PROGRAMICON), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(m_Icon, TRUE);
	m_IconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_PROGRAMICON), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(m_IconSmall, FALSE);
	
	m_OnlineStatus[0] = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_STATUS_OFFLINE),  IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	CxImage *img = new CxImage();
	img->CreateFromHICON(m_OnlineStatus[0]);
	img->pUserData = (void *)ID_POPUP3_OFFLINE;
	_Settings.m_MenuImages.push_back(img);
	
	m_OnlineStatus[1] = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_STATUS_ONLINE),  IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	img = new CxImage();
	img->CreateFromHICON(m_OnlineStatus[1]);
	img->pUserData = (void *)ID_POPUP3_ONLINE;
	_Settings.m_MenuImages.push_back(img);

	m_OnlineStatus[2] = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_STATUS_AWAY),  IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	img = new CxImage();
	img->CreateFromHICON(m_OnlineStatus[2]);
	img->pUserData = (void *)ID_POPUP3_AWAY;
	_Settings.m_MenuImages.push_back(img);
	
	m_OnlineStatus[3] = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_STATUS_FFC),  IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	img = new CxImage();
	img->CreateFromHICON(m_OnlineStatus[3]);
	img->pUserData = (void *)ID_POPUP3_CHAT;
	_Settings.m_MenuImages.push_back(img);
	
	m_OnlineStatus[4] = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_STATUS_DND),  IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	img = new CxImage();
	img->CreateFromHICON(m_OnlineStatus[4]);
	img->pUserData = (void *)ID_POPUP3_DONOTDISTURB;
	_Settings.m_MenuImages.push_back(img);
	
	m_OnlineStatus[5] = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_STATUS_XA),  IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	img = new CxImage();
	img->CreateFromHICON(m_OnlineStatus[5]);
	img->pUserData = (void *)ID_POPUP3_EXTENDEDAWAY;
	_Settings.m_MenuImages.push_back(img);
	
	m_OnlineStatus[6] = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_STATUS_INVISIBLE),  IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	img = new CxImage();
	img->CreateFromHICON(m_OnlineStatus[6]);
	img->pUserData = (void *)ID_POPUP3_INVISIBLE;
	_Settings.m_MenuImages.push_back(img);
	


	// init hidden window
	WNDCLASSEX wcx; 
 
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW; 
    wcx.lpfnWndProc = EmptyWndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = _Module.GetModuleInstance();
    wcx.hIcon = m_Icon;
    wcx.hCursor = NULL;
    wcx.hbrBackground = NULL;
    wcx.lpszMenuName =  NULL;
    wcx.lpszClassName = "EmptyWippienClass";  // name of window class 
    wcx.hIconSm = m_IconSmall;
 
    // Register the window class. 
 
    if (RegisterClassEx(&wcx))
		m_EmptyWin = CreateWindow(wcx.lpszClassName, "Wippien", WS_POPUP | (_Settings.m_ShowInTaskbar?WS_VISIBLE:0), 0, 0, 0, 0, NULL, NULL, _Module.GetModuleInstance(), NULL);

	//m_Status.Attach(GetDlgItem(IDC_STATUS));
	m_StatusMessage[0] = 0;
	
	// create our own image
	if (_Settings.m_Icon.Len())
	{
		m_UserImage.Decode((unsigned char *)_Settings.m_Icon.Ptr(), _Settings.m_Icon.Len(), CXIMAGE_FORMAT_PNG);
	}
	
	_Jabber = new CJabber();

	if (_Settings.m_DeleteContactsOnStartup)
	{
		m_UserList.DeleteAllItems();
		// we should now delete all our contacts
		while (m_UserList.m_Users.size())
		{
			CUser *user = m_UserList.m_Users[0];
			m_UserList.m_Users.erase(m_UserList.m_Users.begin());
#ifdef _WODVPNLIB
			delete user;
#else
			user->Release();
#endif						
		}
	}
	m_ReconnectWait = 0;
	SetTimer(105, 600000L); // to request images and details each 10 minutes
	SetTimer(109, 60000L); // update timer
	SetTimer(106, 10L);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	
//	UIAddChildWindowContainer(m_hWnd);
	
//	m_btnOK.m_ToolTip = "Exits Wippien";
	m_btnOK.SetCaption("E&xit");
	m_btnOK.SetToolTipText("Exit Wippien");
	m_btnOK.SetStyle(PBS_UNDERLINE);
	m_btnOK.SubclassWindow(GetDlgItem(ID_EXIT));
	
	m_btnSettings.SetToolTipText("Open settings");
	m_btnSettings.SetCaption("&Settings");
	m_btnSettings.SubclassWindow(GetDlgItem(IDB_SETUP));
	
	m_btnAbout.SetToolTipText("Show AboutBox");
	m_btnAbout.SetCaption("&About");
	m_btnAbout.SubclassWindow(GetDlgItem(ID_APP_ABOUT));

	m_btnMyStatus.SetToolTipText("Set your status\r\nAway, Invisible, Free for chat...");
	m_btnMyStatus.SubclassWindow(GetDlgItem(IDC_MYSTATUS));
	m_btnMyStatus.LoadPNG(IDC_MYSTATUS);

	m_btnMyContacts.SetToolTipText("Add contacts and alien accounts");
	m_btnMyContacts.SubclassWindow(GetDlgItem(IDC_MYCONTACTS));
	m_btnMyContacts.LoadPNG(IDC_MYCONTACTS);

	m_btnSmallMute.SetToolTipText("Mute sound");
	m_btnSmallMute.SubclassWindow(GetDlgItem(IDC_SMALLMUTE));
	int mutimg = _Settings.m_SoundOn?ID_PNG1_MUTEON:ID_PNG1_MUTEOFF;
	m_btnSmallMute.LoadPNG(mutimg);

	m_btnChatRooms.SetToolTipText("Open chat room");
	m_btnChatRooms.SubclassWindow(GetDlgItem(IDC_CHATROOMS));
	m_btnChatRooms.LoadPNG(ID_PNG1_CHATROOMS);
	
	m_btnAuthDlg.SetToolTipText("Authorize your friends");
	m_btnAuthDlg.SubclassWindow(GetDlgItem(IDC_MYAUTHDLG));
	m_btnAuthDlg.LoadPNG(IDC_MYAUTHDLG);
	
		
	ShowWindow(SW_HIDE);
	
//	m_UserList.Refresh(NULL);
	m_UserList.PostMessage(WM_REFRESH, NULL, 0);


	typedef BOOL (WINAPI* fnChangeWindowMessageFilter )(UINT message, DWORD dwFlag);
	fnChangeWindowMessageFilter cf=(fnChangeWindowMessageFilter)::GetProcAddress(m_User32Module,"ChangeWindowMessageFilter");

	WM_TASKBARCREATEDMESSAGE = ::RegisterWindowMessage(_T("TaskbarCreated"));
	if (cf)
		cf(WM_TASKBARCREATEDMESSAGE, 1);
	::PostMessage(m_hWnd, WM_TASKBARCREATEDMESSAGE, 0, 0);

//	ShellIcon(NIM_ADD);

	// set position
	if (_Settings.m_RosterRect.right>_Settings.m_RosterRect.left)
	{
		SetWindowPos(NULL, &_Settings.m_RosterRect, SWP_NOZORDER);
		CalcSizeXSizeY();
		GetWindowRect(&m_AppbarRect);
		CalcAlignment(&_Settings.m_RosterRect);
		if (_Settings.m_IsAligned)
			SetWindowPos(NULL, 0, 0, m_AppbarRect.right-m_AppbarRect.left, m_AppbarRect.bottom-m_AppbarRect.top, SWP_NOMOVE | SWP_NOZORDER);
		else
			SetWindowPos(NULL, 0, 0, m_SizeX, m_SizeY, SWP_NOMOVE | SWP_NOZORDER);


		CheckAppbar();

		if (_Settings.m_IsTopMost)
			::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}



	ReloadEmoticons(FALSE);

	// setup timers
	::SetTimer(m_hWnd, 101, 200, NULL); //AUTHDLG timer
	::SetTimer(m_hWnd, 102, INACTIVITYTIMERCHECK, NULL); // INVACTIVITY DETECT timer
	
	SetTimer(110, 10000); // each 10 seconds check MTU if it's changed


	return TRUE;
}

BOOL CMainDlg::ReloadEmoticons(BOOL ReleaseOnly)
{
	if (m_EmoticonsInstance.hInst)
		FreeLibrary(m_EmoticonsInstance.hInst);
	m_EmoticonsInstance.hInst = 0;

	// init emoticons
	char buff[32768];
	sprintf(buff, _Settings.m_MyPath);
	strcat(buff, "Emotico2.dll");
	BOOL isbig = TRUE;
	HINSTANCE h = LoadLibrary(buff);
	if (!h)
	{
		isbig = FALSE;
		sprintf(buff, _Settings.m_MyPath);
		strcat(buff, "Emoticon.dll");
		h = LoadLibrary(buff);
	}
	if (h)
	{
		FdTransform tr = (FdTransform)::GetProcAddress(h,"Transform");
		FdRollback rl = (FdRollback)::GetProcAddress(h,"Rollback");
		FdGetItem gi = (FdGetItem)::GetProcAddress(h,"GetItem");
		FdGetText gt = (FdGetText)::GetProcAddress(h,"GetText");
		FdGetHtml gh = (FdGetHtml)::GetProcAddress(h,"GetHtml");
		FdGetCount gc = (FdGetCount)::GetProcAddress(h,"GetCount");
		if (tr && rl)
		{
//			EmoticonsStruct *st = new EmoticonsStruct();
			m_EmoticonsInstance.hInst = h;
			m_EmoticonsInstance.Rollback = rl;
			m_EmoticonsInstance.Transform = tr;
			m_EmoticonsInstance.GetItem = gi;
			m_EmoticonsInstance.GetHtml = gh;
			m_EmoticonsInstance.GetCount = gc;
			m_EmoticonsInstance.GetText = gt;
			m_EmoticonsInstance.ClassRegistered = FALSE;	
			m_EmoticonsInstance.IsBig = isbig;
//			m_EmoticonsInstance.push_back(st);
			return TRUE;
		}
		else
		{
			FreeLibrary(h);
			return FALSE;
		}
	}
	else
	{
		m_EmoticonsInstance.Rollback = NULL;
		m_EmoticonsInstance.Transform = NULL;
		m_EmoticonsInstance.GetItem = NULL;
		m_EmoticonsInstance.GetCount = NULL;
		m_EmoticonsInstance.GetText = NULL;
		return FALSE;
	}	
}


void CMainDlg::ShellIcon(int Message, HICON Icon, char *Tip)
{
	DumpDebug("*MainDlg::ShellIcon \r\n");
	// add icon to tray
	NOTIFYICONDATA nid;
	memset(&nid, 0, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.hIcon = Icon;
	nid.hWnd = m_hWnd;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	char buff[1024];
	sprintf(buff, "Wippien - %s", Tip);
	memcpy(nid.szTip, buff, 128);
//	nid.szTip = buff;
	nid.uCallbackMessage = WM_TRAYICON;
	Shell_NotifyIcon(Message, &nid);	
}

LRESULT CMainDlg::OnCreateTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ShellIcon(NIM_ADD, m_IconSmall, "Idle");
	return FALSE;
}
LRESULT CMainDlg::OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnTrayIcon \r\n");
	switch (lParam)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			if (!_Settings.CheckPasswordProtect())
				return FALSE;
			ShowWindow(SW_SHOWNORMAL);
//			SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
//			SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
//			SetFocus();
			SetForegroundWindow(m_hWnd);
			break;
	}
	return FALSE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnDestroy \r\n");
	GetWindowRect(&_Settings.m_RosterRect);
	_Settings.m_RosterRect.bottom = _Settings.m_RosterRect.top + m_SizeY;

	ShellIcon(NIM_DELETE, m_IconSmall, "");
	UNREGISTERAPPBAR();
	if (m_EmptyWin)
		::DestroyWindow(m_EmptyWin);

	EnterCriticalSection(&_MainDlg.m_SocketCS);
	if (IsWindow())
	{
		while (m_UserList.m_Users.size())
		{
			CUser *user = m_UserList.m_Users[0];
			m_UserList.m_Users.erase(m_UserList.m_Users.begin());
#ifdef _WODVPNLIB
			delete user;
#else
			user->Release();
#endif						
		}

//		PostQuitMessage(0);
	}
	LeaveCriticalSection(&_MainDlg.m_SocketCS);
	_Module.GetMessageLoop()->RemoveMessageFilter(this);

	bHandled = FALSE;
	return FALSE;
}

LRESULT CMainDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnClose \r\n");
//	UNREGISTERAPPBAR();
//	DestroyWindow();
	ShowWindow(SW_MINIMIZE);
	return FALSE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnAppAbout \r\n");
	CAboutDlg dlg;
//	CBalloonTipDlg dlg(NULL);
//	dlg.SetText("Wippien will automatically update itself each time you start it.");
//	dlg.SetCaption("Tip of the day");
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (_Settings.m_DoNotShow[DONOTSHOW_NOALLOWEXIT] != '1')
	{
		int yesno = ::MessageBox(NULL, "Really exit Wippien?", "Are you sure?", MB_YESNO | MB_ICONQUESTION);
			if (yesno == IDNO)
				return TRUE;
	}

	DumpDebug("*MainDlg::OnExit \r\n");
	GetWindowRect(&_Settings.m_RosterRect);
	_Settings.m_RosterRect.bottom = _Settings.m_RosterRect.top + m_SizeY;

	ShellIcon(NIM_DELETE, m_IconSmall, "");
	UNREGISTERAPPBAR();
	_Settings.Save(FALSE);

	CloseDialog(wID);
	return TRUE;
}

LRESULT CMainDlg::OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnOk\r\n");
	HTREEITEM hitem = m_UserList.GetSelectedItem();
	if (hitem)
	{
		CUser *user = (CUser *)m_UserList.GetItemData(hitem);
		if (user)
		{
			user->OpenMsgWindow(TRUE);
		}
	}
	return 0;
}


void CMainDlg::CloseDialog(int nVal)
{
	DumpDebug("*MainDlg::CloseDialog\r\n");
	DestroyWindow();

	//	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnSetup(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!_Settings.CheckPasswordProtect())
		return FALSE;


	DumpDebug("*MainDlg::OnSetup\r\n");
	if (m_SettingsDlg && ::IsWindow(m_SettingsDlg->m_hWnd))
	{
		::ShowWindow(m_SettingsDlg->m_hWnd, SW_SHOW);
		::SetActiveWindow(m_SettingsDlg->m_hWnd);
	}
	else
	{
		m_SettingsDlg = new CSettingsDlg(FALSE);

		CSettingsDlg::_CSettingsTemplate *pg = NULL;
		
		pg = new CSettingsDlg::CSettingsJID();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsSystem();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsAppearance();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsSystemUpdate();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsIcon();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsContacts();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsContactsAddRemove();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsContactsHide();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsContactsSort();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsEthernet();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsMediator();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsMTU();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsAccounts();
		m_SettingsDlg->m_Dialogs.push_back(pg);
		
		pg = new CSettingsDlg::CSettingsLogging();
		m_SettingsDlg->m_Dialogs.push_back(pg);

		pg = new CSettingsDlg::CSettingsMsgWin();
		m_SettingsDlg->m_Dialogs.push_back(pg);

		pg = new CSettingsDlg::CSettingsSound();
		m_SettingsDlg->m_Dialogs.push_back(pg);

#ifdef _SKINMAGICKEY
		pg = new CSettingsDlg::CSettingsSkins();
		m_SettingsDlg->m_Dialogs.push_back(pg);
#endif

		pg = new CSettingsDlg::CSettingsAutoAway();
		m_SettingsDlg->m_Dialogs.push_back(pg);


#ifndef _WODXMPPLIB
		WODXMPPCOMLib::IXMPPVCard *vc = NULL;
		SUCCEEDED(_Jabber->m_Jabb->get_VCard(&vc));
#else
		void *vc = NULL;
		WODXMPPCOMLib::XMPP_GetVCard(_Jabber->m_Jabb, &vc);
#endif

		CSettingsDlg::CSettingsUser1 *us1 = new CSettingsDlg::CSettingsUser1();
		CComBSTR2 b2 = _Settings.m_JID;
		strcpy(us1->m_Text2, b2.ToString());		
		sprintf(us1->m_Path, "Identity\\ID Card");
		us1->PATH = us1->m_Path;
		us1->m_IsContact = FALSE;
		pg = us1;
		m_SettingsDlg->m_Dialogs.push_back(pg);

		CSettingsDlg::CSettingsUser2 *us2 = new CSettingsDlg::CSettingsUser2();
		b2 = _Settings.m_JID;
		strcpy(us2->m_Text2, b2.ToString());		
		sprintf(us2->m_Path, "Identity\\ID Card\\Home Address");
		us2->PATH = us2->m_Path;
		us2->m_IsContact = FALSE;
		pg = us2;
		m_SettingsDlg->m_Dialogs.push_back(pg);

		CSettingsDlg::CSettingsUser3 *us3 = new CSettingsDlg::CSettingsUser3();
		b2 = _Settings.m_JID;
		strcpy(us3->m_Text2, b2.ToString());		
		sprintf(us3->m_Path, "Identity\\ID Card\\Work Address");
		us3->PATH = us3->m_Path;
		us3->m_IsContact = FALSE;
		pg = us3;
		m_SettingsDlg->m_Dialogs.push_back(pg);


		m_SettingsDlg->Create(NULL, NULL);
		m_SettingsDlg->ShowWindow(SW_SHOW);

		us1->InitData(vc);
		us2->InitData(vc);
		us3->InitData(vc);

		if (vc)
#ifndef _WODXMPPLIB
			vc->Release();
#else
		WODXMPPCOMLib::XMPP_VCard_Free(vc);
#endif
	}

//	dlg.DoModal();

/*	for (int i=0;i<dlg.m_Dialogs.size();i++)
	{
		_CSettingsTemplate * cs = (_CSettingsTemplate *)dlg.m_Dialogs[i];
		delete cs;
	}
*/
	return TRUE;
}

LRESULT CMainDlg::OnBtnAuthDlg(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnBtnAuthDlg\r\n");
		// and now, if needed, reopen authdlg...
		if (!_ContactAuthDlg)
		{
			_ContactAuthDlg = new CContactAuthDlg();
		}
		if (_ContactAuthDlg && !_ContactAuthDlg->IsWindow())
		{
			RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
			_ContactAuthDlg->Create(NULL, rcDefault, NULL);
		}
		if (_ContactAuthDlg && _ContactAuthDlg->IsWindow())
			_ContactAuthDlg->ShowWindow(SW_SHOW);


	return TRUE;
}

LRESULT CMainDlg::OnBtnStatus(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnBtnStatus\r\n");
	RECT rt;
	::GetWindowRect(GetDlgItem(IDC_MYSTATUS), &rt);


	HMENU hm = LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_AWAYPOPUP));
	HMENU h = GetSubMenu(hm, 0);


	BOOL online = FALSE;
	BOOL connecting = FALSE;
	if (_Jabber)
	{
		WODXMPPCOMLib::StatusEnum st = (WODXMPPCOMLib::StatusEnum)0;
#ifndef _WODXMPPLIB
		_Jabber->m_Jabb->get_Status(&st);
#else
		WODXMPPCOMLib::XMPP_GetStatus(_Jabber->m_Jabb, &st);
#endif
		int i = (int)st;
		if (i)
			online = TRUE;

		WODXMPPCOMLib::StatesEnum st1 = (WODXMPPCOMLib::StatesEnum)0;
#ifndef _WODXMPPLIB
		_Jabber->m_Jabb->get_State(&st1);
#else
		WODXMPPCOMLib::XMPP_GetState(_Jabber->m_Jabb, &st1);
#endif
		i = (int)st1;
		if (i<5 /*connected*/ && i>0 /*disconnected*/)
			connecting = TRUE;
	
	}
	MENUITEMINFO lpmii = {0};
	lpmii.cbSize = sizeof(lpmii);
	lpmii.fMask = MIIM_STATE;

	GetMenuItemInfo(h, ID_POPUP3_ONLINE, FALSE, &lpmii);
	lpmii.fState = MFS_ENABLED;
	SetMenuItemInfo(h, ID_POPUP3_ONLINE, FALSE, &lpmii);

	GetMenuItemInfo(h, ID_POPUP3_AWAY, FALSE, &lpmii);
	if (online)
		lpmii.fState = MFS_ENABLED;
	else
		lpmii.fState = MFS_DISABLED;
	SetMenuItemInfo(h, ID_POPUP3_AWAY, FALSE, &lpmii);

	GetMenuItemInfo(h, ID_POPUP3_CHAT, FALSE, &lpmii);
	if (online)
		lpmii.fState = MFS_ENABLED;
	else
		lpmii.fState = MFS_DISABLED;
	SetMenuItemInfo(h, ID_POPUP3_CHAT, FALSE, &lpmii);

	GetMenuItemInfo(h, ID_POPUP3_DONOTDISTURB, FALSE, &lpmii);
	if (online)
		lpmii.fState = MFS_ENABLED;
	else
		lpmii.fState = MFS_DISABLED;
	SetMenuItemInfo(h, ID_POPUP3_DONOTDISTURB, FALSE, &lpmii);

	GetMenuItemInfo(h, ID_POPUP3_EXTENDEDAWAY, FALSE, &lpmii);
	if (online)
		lpmii.fState = MFS_ENABLED;
	else
		lpmii.fState = MFS_DISABLED;
	SetMenuItemInfo(h, ID_POPUP3_EXTENDEDAWAY, FALSE, &lpmii);

	GetMenuItemInfo(h, ID_POPUP3_INVISIBLE, FALSE, &lpmii);
	if (online)
		lpmii.fState = MFS_ENABLED;
	else
		lpmii.fState = MFS_DISABLED;
	SetMenuItemInfo(h, ID_POPUP3_INVISIBLE, FALSE, &lpmii);

	GetMenuItemInfo(h, ID_POPUP3_OFFLINE, FALSE, &lpmii);
	if (online || connecting)
		lpmii.fState = MFS_ENABLED;
	else
		lpmii.fState = MFS_DISABLED;
	SetMenuItemInfo(h, ID_POPUP3_OFFLINE, FALSE, &lpmii);
/*
	_MainDlg.m_UserList.AddMenuIcon(IDI_STATUS_ONLINE, ID_POPUP3_ONLINE);
	_MainDlg.m_UserList.AddMenuIcon(IDI_STATUS_AWAY, ID_POPUP3_AWAY);
	_MainDlg.m_UserList.AddMenuIcon(IDI_STATUS_FFC, ID_POPUP3_CHAT);
	_MainDlg.m_UserList.AddMenuIcon(IDI_STATUS_DND, ID_POPUP3_DONOTDISTURB);
	_MainDlg.m_UserList.AddMenuIcon(IDI_STATUS_XA, ID_POPUP3_EXTENDEDAWAY);
	_MainDlg.m_UserList.AddMenuIcon(IDI_STATUS_OFFLINE, ID_POPUP3_OFFLINE);
	_MainDlg.m_UserList.AddMenuIcon(IDI_STATUS_INVISIBLE, ID_POPUP3_INVISIBLE);
*/


	m_CanTooltip = FALSE;
	int i = m_UserList.m_AwayPopupMenu->TrackPopupMenu(h, TPM_LEFTALIGN | TPM_RETURNCMD, rt.left, rt.bottom, 0);
	m_CanTooltip = TRUE;
	DestroyMenu(hm);

	HRESULT hr = S_OK;
	switch (i)
	{
		case ID_POPUP3_ONLINE:
				if (_Jabber)
				{
					if (!online)
					{
						_MainDlg.m_ReconnectWait = 1;
						if (_MainDlg.IsWindow())
							_MainDlg.SetTimer(106,200);
					}
					else
#ifndef _WODXMPPLIB
					hr = _Jabber->m_Jabb->raw_SetStatus((WODXMPPCOMLib::StatusEnum)/*Online*/1);
#else
					hr = WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)1, NULL);
#endif
				}
			break;

		case ID_POPUP3_AWAY:
				if (_Jabber)
#ifndef _WODXMPPLIB
					hr = _Jabber->m_Jabb->raw_SetStatus((WODXMPPCOMLib::StatusEnum)2);
#else
				hr = WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)2, NULL);
#endif
			break;

		case ID_POPUP3_CHAT:
				if (_Jabber)
#ifndef _WODXMPPLIB
					hr = _Jabber->m_Jabb->raw_SetStatus((WODXMPPCOMLib::StatusEnum)3);
#else
				hr = WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)3, NULL);
#endif
			break;

		case ID_POPUP3_DONOTDISTURB:
				if (_Jabber)
#ifndef _WODXMPPLIB
					hr = _Jabber->m_Jabb->raw_SetStatus((WODXMPPCOMLib::StatusEnum)2);
#else
				hr = WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)4, NULL);
#endif
			break;

		case ID_POPUP3_EXTENDEDAWAY:
				if (_Jabber)
#ifndef _WODXMPPLIB
					hr = _Jabber->m_Jabb->raw_SetStatus((WODXMPPCOMLib::StatusEnum)2);
#else
				hr = WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)5, NULL);
#endif
			break;

		case ID_POPUP3_INVISIBLE:
				if (_Jabber)
#ifndef _WODXMPPLIB
					hr = _Jabber->m_Jabb->raw_SetStatus((WODXMPPCOMLib::StatusEnum)2);
#else
				hr = WODXMPPCOMLib::XMPP_SetStatus(_Jabber->m_Jabb, (WODXMPPCOMLib::StatusEnum)6, NULL);
#endif
			break;

		case ID_POPUP3_OFFLINE:
				if (_Jabber)
				{
					_Jabber->m_DoReconnect = FALSE;
#ifndef _WODXMPPLIB
					_Jabber->m_Jabb->Disconnect();
#else	
					WODXMPPCOMLib::XMPP_Disconnect(_Jabber->m_Jabb);
#endif
					_Jabber->m_DoReconnect = TRUE;
				}

			break;

	
	}
	if (SUCCEEDED(hr))
#ifndef _WODXMPPLIB
		_Jabber->m_Events->DispStateChange((WODXMPPCOMLib::StatesEnum)0);
#else
	XMPPStateChange(_Jabber->m_Jabb, (WODXMPPCOMLib::StatesEnum)0);
#endif

	if (FAILED(hr))
	{
		_Jabber->ShowError();
	}
	return TRUE;
}

LRESULT CMainDlg::OnBtnSmallMute(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CMainDlg::OnBtnChatRooms(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSettingsDlg dlg(TRUE);
	CSettingsDlg::_CSettingsTemplate *pgchat = NULL;
	pgchat = new CSettingsDlg::CSettingsChatRooms();
	dlg.m_Dialogs.push_back(pgchat);
	
	dlg.DoModal();
	delete pgchat;

	return 0;
}

LRESULT CMainDlg::OnBtnContacts(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnBtnContacts\r\n");
	RECT rt;
	::GetWindowRect(GetDlgItem(IDC_MYCONTACTS), &rt);

	HMENU hm = LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_SETUPPOPUP));
	HMENU h = GetSubMenu(hm, 0);


	m_CanTooltip = FALSE;
	m_UserList.m_SetupPopupMenu->AttachMenu(hm);
	int i = m_UserList.m_SetupPopupMenu->TrackPopupMenu(h, TPM_LEFTALIGN | TPM_RETURNCMD, rt.left, rt.bottom, 0);
	m_CanTooltip = TRUE;
	DestroyMenu(hm);


	switch (i)
	{
		case ID_POPUP2_IDENTITIES:
			{
				CSettingsDlg dlg(TRUE);
				CSettingsDlg::_CSettingsTemplate *pgacc = NULL;
				pgacc = new CSettingsDlg::CSettingsAccounts();
				dlg.m_Dialogs.push_back(pgacc);

				dlg.DoModal();
				delete pgacc;
			}
			break;

		case ID_POPUP2_NEWCONTACT:
			{
				CSettingsDlg dlg(TRUE);
				CSettingsDlg::_CSettingsTemplate *pgcon = NULL;
				pgcon = new CSettingsDlg::CSettingsContactsAddRemove();
				dlg.m_Dialogs.push_back(pgcon);

				dlg.DoModal();
				delete pgcon;
			}
			break;
	
	}
	return 0;
}

LRESULT CMainDlg::OnStatusMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnStatusMessage\r\n");
	::SetWindowText(GetDlgItem(IDC_STATUS), m_StatusMessage);
	return TRUE;
}

void CMainDlg::ShowStatusText(char *text)
{
	DumpDebug("*MainDlg::ShowStatusText\r\n");
	int i = strlen(text);
	if (i > 255)
		i = 255;
	memset(m_StatusMessage, 0, sizeof(m_StatusMessage));
	memcpy(m_StatusMessage, text, i);
	
	if (IsWindow())
		PostMessage(WM_STATUSMESSAGE, 0);
}

void CMainDlg::OnIncomingMessage(char *Contact, char *Message, char *HtmlMessage)
{
	DumpDebug("*MainDlg::OnIncomingMessage\r\n");
	int j = strlen(Contact);
	char *j1 = strchr(Contact, '/');
	if (j1)
		*j1 = 0;
	
	if (_SDK)
	{
		Buffer b;
		b.PutCString(Contact);
		b.PutCString(Message);
		b.PutCString(HtmlMessage);

		if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_INCOMINGMESSAGE, b.Ptr(), b.Len()))
			return;
	}

	for (int i = 0; i < m_UserList.m_Users.size(); i++)
	{
		CUser *user = m_UserList.m_Users[i];
		if (!strnicmp(user->m_JID, Contact, j))
		{
			user->PrintMsgWindow(FALSE, Message, HtmlMessage);
			return;
		}
	}
}

void CMainDlg::OnIncomingNotification(char *Contact, int NotID, VARIANT Data)
{
	DumpDebug("*MainDlg::OnIncomingNotification\r\n");
	int j = strlen(Contact);
	for (int i = 0; i < m_UserList.m_Users.size(); i++)
	{
		CUser *user = m_UserList.m_Users[i];
		if (!strnicmp(user->m_JID, Contact, j))
		{
			user->NotifyUserIsTyping(Data.bVal);
			return;
		}
	}
}

LRESULT CMainDlg::OnRButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TVHITTESTINFO ht;
	ht.pt.x = GET_X_LPARAM(lParam); 
	ht.pt.y = GET_Y_LPARAM(lParam); 

	if (ht.pt.x<58 && ht.pt.y<58)
	{
		HMENU hm = LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_PICTUREPOPUP));
		HMENU h = GetSubMenu(hm, 0);

		POINT p;
		GetCursorPos(&p);

		int i = m_UserList.m_AwayPopupMenu->TrackPopupMenu(h, TPM_LEFTALIGN | TPM_RETURNCMD, p.x, p.y, 0);
		switch (i)
		{
			case ID_POPUP4_CHANGEPICTURE:
				{
					m_SettingsDlg = new CSettingsDlg(FALSE);

					CSettingsDlg::_CSettingsTemplate *pg = NULL;
					
					pg = new CSettingsDlg::CSettingsIcon();
					m_SettingsDlg->m_Dialogs.push_back(pg);
					

					m_SettingsDlg->Create(NULL, NULL);
					m_SettingsDlg->ShowWindow(SW_SHOW);
				}
				break;

			case ID_POPUP4_YOUR_DETAILS:
				{
					m_SettingsDlg = new CSettingsDlg(FALSE);

					CSettingsDlg::_CSettingsTemplate *pg = NULL;
					
#ifndef _WODXMPPLIB
					WODXMPPCOMLib::IXMPPVCard *vc = NULL;
					SUCCEEDED(_Jabber->m_Jabb->get_VCard(&vc));
#else
					void *vc;
					WODXMPPCOMLib::XMPP_GetVCard(_Jabber->m_Jabb, &vc);
#endif

					CSettingsDlg::CSettingsUser1 *us1 = new CSettingsDlg::CSettingsUser1();
					CComBSTR2 b2 = _Settings.m_JID;
					strcpy(us1->m_Text2, b2.ToString());		
					sprintf(us1->m_Path, "Identity\\ID Card");
					us1->PATH = us1->m_Path;
					us1->m_IsContact = FALSE;
					pg = us1;
					m_SettingsDlg->m_Dialogs.push_back(pg);

					CSettingsDlg::CSettingsUser2 *us2 = new CSettingsDlg::CSettingsUser2();
					b2 = _Settings.m_JID;
					strcpy(us2->m_Text2, b2.ToString());		
					sprintf(us2->m_Path, "Identity\\ID Card\\Home Address");
					us2->PATH = us2->m_Path;
					us2->m_IsContact = FALSE;
					pg = us2;
					m_SettingsDlg->m_Dialogs.push_back(pg);

					CSettingsDlg::CSettingsUser3 *us3 = new CSettingsDlg::CSettingsUser3();
					b2 = _Settings.m_JID;
					strcpy(us3->m_Text2, b2.ToString());		
					sprintf(us3->m_Path, "Identity\\ID Card\\Work Address");
					us3->PATH = us3->m_Path;
					us3->m_IsContact = FALSE;
					pg = us3;
					m_SettingsDlg->m_Dialogs.push_back(pg);


					m_SettingsDlg->Create(NULL, NULL);
					m_SettingsDlg->ShowWindow(SW_SHOW);

					us1->InitData(vc);
					us2->InitData(vc);
					us3->InitData(vc);

					if (vc)
#ifndef _WODXMPPLIB
						vc->Release();
#else
					WODXMPPCOMLib::XMPP_VCard_Free(vc);
#endif
				}
		}
	}

	return FALSE;
}


LRESULT CMainDlg::OnLButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnLButtonup\r\n");
	if (m_UserList.m_Dragging)
	{	
		ImageList_DragLeave(m_UserList.m_hWndParent);
		ImageList_EndDrag();
		HTREEITEM Selected = (HTREEITEM)::SendMessage(m_UserList.m_hWndParent, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0);
		CUser *group = (CUser *)m_UserList.GetItemData(Selected);
		if (!group)
		{
			::SendMessage(m_UserList.m_hWndParent, TVM_SELECTITEM, TVGN_DROPHILITE, 0);
			for (int i = 0; i < _Settings.m_Groups.size(); i++)
			{
				CSettings::TreeGroup *tg = (CSettings::TreeGroup *)_Settings.m_Groups[i];
				if (tg->Item == Selected)
				{
					if (strcmp(tg->Name, GROUP_OFFLINE))
					{
						// found it!
						CUser *user = (CUser *)m_UserList.GetItemData(m_UserList.m_Dragging);
						if (user)
						{
							strcpy(user->m_Group, tg->Name);
							user->m_Changed = TRUE;
							m_UserList.m_SortedUsersBuffer.Clear();
//							m_UserList.Refresh(NULL);
							m_UserList.PostMessage(WM_REFRESH, NULL, TRUE);
							break;
						}
					}
				}
			}
		}
		ReleaseCapture();
		ShowCursor(TRUE); 
		m_UserList.m_Dragging = FALSE;		
		return TRUE;
	}
	else
	{
		CheckAppbar();
	}
	
	bHandled = TRUE;
	return TRUE;
}

LRESULT CMainDlg::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DumpDebug("*MainDlg::OnMouseMove \r\n");
	
	POINT p;
	p.x = GET_X_LPARAM(lParam); 
	p.y = GET_Y_LPARAM(lParam); 	
	if (m_CanTooltip && !m_pBalloon)
	{
		if (p.x != m_TooltipPoint.x || p.y != m_TooltipPoint.y)
		{
			m_TooltipPoint.x = p.x;
			m_TooltipPoint.y = p.y;
			::SetTimer(m_hWnd, 103, 500, NULL); // TOOLTIP timer
		}
	}
	
	if (m_UserList.m_Dragging)
	{
		TVHITTESTINFO tvht; 
		HTREEITEM hitTarget;
		POINTS Pos;
		
		Pos = MAKEPOINTS(lParam);
		ImageList_DragMove(Pos.x, Pos.y - 70); // where to draw the drag from
		ImageList_DragShowNolock(FALSE);
		
		// the highlight items should be as
		// the same points as the drag
		tvht.pt.x = Pos.x; 
		tvht.pt.y = Pos.y - 58; //
		
		if (hitTarget = (HTREEITEM)::SendMessage(m_UserList.m_hWndParent, TVM_HITTEST, NULL, (LPARAM)&tvht)) // if there is a hit
		{
			CUser *user = (CUser *)m_UserList.GetItemData(hitTarget);
			if (!user)
			{
				::SendMessage(m_UserList.m_hWndParent, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)hitTarget); // highlight it
				m_UserList.Expand(hitTarget);
			}
		}
		
		ImageList_DragShowNolock(TRUE); 	
		return TRUE;
	}
	
	bHandled = FALSE;
	return FALSE;
}

LRESULT CMainDlg::OnPowerBroadcast(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	DumpDebug("*MainDlg::OnPowerBroadcast \r\n");
	if (wParam == PBT_APMRESUMEAUTOMATIC || wParam == PBT_APMRESUMECRITICAL || wParam == PBT_APMRESUMESUSPEND)
	{
		// if dhcp isn't running, restart it
		if (_Ethernet.hSetupThread == INVALID_HANDLE_VALUE)
		{
			_Ethernet.hSetupThread = CreateThread(NULL, 0, _Ethernet.SetupThreadFunc, &_Ethernet, 0, &_Ethernet.SetupThreadId);
		}


		if (_Settings.m_UsePowerOptions)
		{
			m_ReconnectWait = 1;
			if (IsWindow())
				_MainDlg.SetTimer(106,200);
		}
	}
	if (wParam == PBT_APMSUSPEND)
	{
		if (_Settings.m_UsePowerOptions)
		{
#ifndef _WODXMPPLIB
			_Jabber->m_Jabb->Disconnect();
#else
		WODXMPPCOMLib::XMPP_Disconnect(_Jabber->m_Jabb);
#endif
		}

	}

	return FALSE;
}

void CMainDlg::ToggleMute(void)
{
	DumpDebug("*MainDlg::ToggleMute \r\n");
	_Settings.m_SoundOn = !_Settings.m_SoundOn;

	if (!_Settings.m_SoundOn)
		PlaySound(NULL, _Module.GetModuleInstance(), 0);

	// redraw main screen
	int mutimg = _Settings.m_SoundOn?ID_PNG1_MUTEON:ID_PNG1_MUTEOFF;
	m_btnSmallMute.LoadPNG(mutimg);
	m_btnSmallMute.Invalidate();

	// redraw anywhere
	for (int i=0;i<m_UserList.m_Users.size();i++)
	{
		CUser *us = (CUser *)m_UserList.m_Users[i];
		if (us->IsMsgWindowOpen())
		{
			// toggle their button too
			mutimg = _Settings.m_SoundOn?ID_PNG2_MUTEON:ID_PNG2_MUTEOFF;
			us->m_MessageWin->m_btnMuteOnOff.LoadPNG(mutimg);
			//m_btnMuteOnOff.m_cxImage.Resample(24,24);
//			ResampleImageIfNeeded(&us->m_MessageWin->m_btnMuteOnOff.m_cxImage, 32);
			us->m_MessageWin->m_btnMuteOnOff.Invalidate();
		}
	}
	return;
}

void DumpDebug(char *text,...)
{
	va_list marker;
	char buff[16384];
	int handle;
	
	CComBSTR2 file = _Settings.m_FunctionDebugFile;
	long fl = 0;
	if (text && text[0] && file.Length())
	{
		va_start(marker, text);
		wvsprintf(buff, text, marker);
		va_end(marker);
		

		handle = open(file.ToString(), O_BINARY | O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
		if (handle != (-1))
		{
			fl = lseek(handle, 0, SEEK_END);
			if (fl > _Settings.m_DeleteFunctionLogMb)
			{
				close(handle);
				handle = open(file.ToString(), O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
				if (handle == (-1))
					return;
			}

			write(handle, buff, strlen(buff));
			close(handle);
		}
	}



}
