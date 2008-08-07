// MsgWin.cpp: implementation of the CMsgWin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlcrack.h>
#include "MsgWin.h"
#include "User.h"
#include "ChatRoom.h"
#include "Notify.h"
#include "colors.h"
#include "Jabber.h"
#include "SkinMagicLib.h"
#include "ComBSTR2.h"
#include "Settings.h"
#include "oleauto.h"
#include <mshtml.h>
#include <MsHtmcid.h>
#include <Mshtmdid.h>
#include "BaloonHelp.h"
#include "MainDlg.h"
#include "ProgressDlg.h"
#include "../CxImage/zlib/zlib.h"
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include "BaloonTip.h"
extern CNotify _Notify;
extern CSettings _Settings;
extern CMainDlg _MainDlg;
extern CJabber *_Jabber;

void ResampleImageIfNeeded(CxImage *img, int size);
BOOL __LoadIconFromResource(CxImage *img, HINSTANCE hInst, char *restype, int imgformat, int resid);
int FindCxImageFormat(const CString& ext);
void _CalcRect(int imgwidth, int imgheight, int rcwidth, int rcheight, long *xpos, long *ypos, long *xwidth, long *xheight, int size);
void BufferUncompress(z_stream z_str, char *input_buffer, int len, _Buffer * output_buffer);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// Define the sWeb events to be handled:
_ATL_FUNC_INFO StatusTextChangeInfo = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};
_ATL_FUNC_INFO DownloadBeginInfo = {CC_STDCALL, VT_EMPTY, 0, {NULL}};
_ATL_FUNC_INFO NewWindow2Info = {CC_STDCALL, VT_EMPTY, 2, {VT_BYREF|VT_BOOL,VT_BYREF|VT_DISPATCH}}; 
_ATL_FUNC_INFO CommandStateChangeInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_BOOL}};
_ATL_FUNC_INFO BeforeNavigate2Info = {CC_STDCALL, VT_EMPTY, 7, {VT_DISPATCH,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_BOOL}};
_ATL_FUNC_INFO ProgressChangeInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_I4}};
_ATL_FUNC_INFO NavigateComlete2Info = {CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_BYREF|VT_VARIANT}};
_ATL_FUNC_INFO OnVisibleInfo = {CC_STDCALL, VT_EMPTY, 1, {VT_BOOL}};
class wWebEvents : public IDispEventSimpleImpl<1, wWebEvents, &DIID_DWebBrowserEvents2>
{
public:
	CMsgWin::CChatBox *m_ChatBox;
	BOOL m_IgnoreNavigate, m_NavComplete;
    wWebEvents (CMsgWin::CChatBox *chatbox/*IWebBrowser2 *pWebBrowser*/)
    {
		m_ChatBox = chatbox;		
        m_pWebBrowser = m_ChatBox->m_wb2ChatBox;
        DispEventAdvise ( (IUnknown*)m_pWebBrowser);
		m_IgnoreNavigate = FALSE;
		m_NavComplete = FALSE;
    }

    virtual ~wWebEvents ()
    {
        DispEventUnadvise ( (IUnknown*)m_pWebBrowser);
//        m_pWebBrowser->Release();
    }

	void __stdcall __StatusTextChange(/*[in]*/ BSTR bstrText)
	{
	}

	void __stdcall __ProgressChange(/*[in]*/ long progress, /*[in]*/ long progressMax)
	{
	}

	void __stdcall __CommandStateChange(/*[in]*/ long Command, /*[in]*/ VARIANT_BOOL Enable)
	{
	}

	void __stdcall __DownloadBegin()
	{
	}

	void __stdcall __DownloadComplete()
	{
	}
	
	void __stdcall __TitleChange(/*[in]*/ BSTR bstrText)
	{
	}
	
	void __stdcall __NavigateComplete2(/*[in]*/ IDispatch* pDisp, /*[in]*/ VARIANT* URL)
	{
		IDispatch *lpDisp;
		HRESULT hr = m_ChatBox->m_wb2ChatBox->get_Document(&lpDisp);
		m_ChatBox->m_htmlChatBox = (IHTMLDocument2 *)lpDisp;
		

		m_NavComplete = TRUE;

		// let's get the hWnd
		if (_Settings.m_ShowMessageHistory)
		{
			Buffer c;
			c.Append("<html><head><style type=\"text/css\">body{padding: 0px;}td{line-height: 11px;}</style></head><body>");
			m_ChatBox->m_ParentDlg->LoadHistory(&c);
//			c.Append("</body>");
			c.Append("\0", 1);
			CComBSTR d(c.Ptr());
			m_ChatBox->AddHtml(d);
		}
		else
			m_ChatBox->AddHtml(m_ChatBox->m_ParentDlg->m_EmptyBody);

		m_ChatBox->SetDefMargin();
		m_IgnoreNavigate = FALSE;
		if (m_ChatBox->m_AddHtml.Len())
		{
			m_ChatBox->m_AddHtml.Append("\0",1);
			CComBSTR b = m_ChatBox->m_AddHtml.Ptr();
			m_ChatBox->m_AddHtml.Clear();
			m_ChatBox->AddHtml(b);
		}
	}
	
	void __stdcall __BeforeNavigate2(/*[in]*/ IDispatch* pDisp, /*[in]*/ VARIANT* URL, /*[in]*/ VARIANT* Flags, 
		/*[in]*/ VARIANT* TargetFrameName, /*[in]*/ VARIANT* PostData, /*[in]*/ VARIANT* Headers, 
		/*[out]*/ VARIANT_BOOL* Cancel)
	{
		if (!m_IgnoreNavigate)
		{
			if (Cancel)
				*Cancel = VARIANT_TRUE;

			if (URL)
			{
				CComBSTR2 url;

				if (URL->vt == VT_BSTR)
					url = URL->bstrVal;
				else
				if (URL->vt == (VT_BSTR | VT_BYREF))
					url = *URL->pbstrVal;

				if (url.Length())
				{
					ShellExecute(NULL, "open", url.ToString(), "", "", 0);
				}
			}
		}
	}

	void __stdcall __PropertyChange(/*[in]*/ BSTR bstrProperty)
	{
	}
	
	void __stdcall __NewWindow2(/*[out]*/ IDispatch** ppDisp, /*[out]*/ VARIANT_BOOL* Cancel)
	{
	}
	
	void __stdcall __DocumentComplete(/*[in]*/ IDispatch* pDisp, /*[in]*/ VARIANT* URL)
	{
	}
	
	void __stdcall __OnQuit()
	{
	}
	
	void __stdcall __OnVisible(/*[in]*/ VARIANT_BOOL Visible)
	{
	}

	void __stdcall __OnToolBar(/*[in]*/ VARIANT_BOOL ToolBar)
	{
	}

	void __stdcall __OnMenuBar(/*[in]*/ VARIANT_BOOL MenuBar)
	{
	}
	
	void __stdcall __OnStatusBar(/*[in]*/ VARIANT_BOOL StatusBar)
	{
	}

	void __stdcall __OnFullScreen(/*[in]*/ VARIANT_BOOL FullScreen)
	{
	}

	void __stdcall __OnTheaterMode(/*[in]*/ VARIANT_BOOL TheaterMode)
	{
	}
    BEGIN_SINK_MAP (wWebEvents)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 102, __StatusTextChange, &StatusTextChangeInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 108, __ProgressChange, &ProgressChangeInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 105, __CommandStateChange, &CommandStateChangeInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 106, __DownloadBegin, &DownloadBeginInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 104, __DownloadComplete, &DownloadBeginInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 113, __TitleChange, &StatusTextChangeInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 252, __NavigateComplete2, &NavigateComlete2Info)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 250, __BeforeNavigate2, &BeforeNavigate2Info)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 112, __PropertyChange, &StatusTextChangeInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 251, __NewWindow2, &NewWindow2Info)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 259, __DocumentComplete, &NavigateComlete2Info)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 253, __OnQuit, &DownloadBeginInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 254, __OnVisible, &OnVisibleInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 255, __OnToolBar, &OnVisibleInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 256, __OnMenuBar, &OnVisibleInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 257, __OnStatusBar, &OnVisibleInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 258, __OnFullScreen, &OnVisibleInfo)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 260, __OnTheaterMode, &OnVisibleInfo)
	END_SINK_MAP ()
private:
    IWebBrowser2 *m_pWebBrowser;
};
  // delete pwEvents;



void TmToSystime(SYSTEMTIME *st, struct tm *tm)
{
	st->wDay = tm->tm_mday;
	st->wMonth = tm->tm_mon + 1;
	st->wYear = tm->tm_year + 1900;
	st->wHour = tm->tm_hour;
	st->wMinute = tm->tm_min;
	st->wSecond = tm->tm_sec;
}

void SystimeToTm(struct tm *tm, SYSTEMTIME *st)
{
	tm->tm_mday = st->wDay;
	tm->tm_mon = st->wMonth - 1;
	tm->tm_year = st->wYear - 1900;
	tm->tm_hour = st->wHour;
	tm->tm_min = st->wMinute;
	tm->tm_sec = st->wSecond;
}

CMsgWin::CMsgWin(CUser *Owner) : /*m_List(this, 1), m_Edit(this, 2), */m_Button(this, 1)
{
	Init();
	m_User = Owner;
}

CMsgWin::CMsgWin(CChatRoom *Owner) : /*m_List(this, 1), m_Edit(this, 2), */m_Button(this, 1)
{
	Init();
	m_Room = Owner;
}


void CMsgWin::Init(void)
{
	m_Room = NULL;
	m_User = NULL;

	LOGBRUSH brush;
	
	// drawing button separators 
	brush.lbStyle = BS_SOLID;
	brush.lbColor = RGB(156, 160, 156);
	
	m_hSeparatorPen =	NULL;
    
	CComBSTR b("<html><head><style type=\"text/css\">body {padding: 0px;}td{line-height: 10px;}</style></head><body>");
	m_EmptyBody = b.Copy();
	
	m_BackBrush = CreateSolidBrush(RGB(209,226,251));
	
	m_hSeparatorPen = ExtCreatePen(PS_COSMETIC | PS_ALTERNATE, 1, &brush, 0, 0 );
	m_ImagesLoaded = FALSE;
	m_ListLoaded = FALSE;
	m_EmoticonList = NULL;
}

CMsgWin::~CMsgWin()
{
//	_Module.GetMessageLoop()->RemoveMessageFilter(this);
	if (m_EmoticonList)
		delete m_EmoticonList;

	DeleteObject(m_hSeparatorPen);
	::SysFreeString(m_EmptyBody);
//	if (m_ChatBox.m_hWnd)
//		m_ChatBox.UnsubclassWindow(FALSE);
	//m_InputBox.UnsubclassWindow(FALSE);
	DeleteObject(m_BackBrush);
	if (IsWindow() && m_User)
	{
		WINDOWPLACEMENT wp = {0};
		GetWindowPlacement(&wp);
		if (wp.flags != SW_SHOWMINIMIZED && wp.flags != SW_HIDE)
			GetWindowRect(&m_User->m_ChatWindowRect);
	}
	if (IsWindow() && m_Room)
	{
		WINDOWPLACEMENT wp = {0};
		GetWindowPlacement(&wp);
		if (wp.flags != SW_SHOWMINIMIZED && wp.flags != SW_HIDE)
			GetWindowRect(&m_Room->m_ChatWindowRect);
	}
//	delete m_Text;
}

BOOL CMsgWin::OnIdle()
{
	return FALSE;
}

LRESULT CMsgWin::OnGotFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	if (m_hwndIEBox != NULL) 
	{
		SendMessage(m_hwndIEBox, WM_MOUSEMOVE, 0, 0);
		SendMessage(m_hwndIEBox, WM_LBUTTONDOWN, 0, 0);
		
	//	::SetFocus(m_hwndIEBox);
		::InvalidateRect(m_hwndIEBox, NULL,TRUE);
	}
	return TRUE;
}

BOOL CMsgWin::PreTranslateMessage(MSG* pMsg)
{
	_MainDlg.CheckIfAntiInactivityMessage(pMsg->message);

	HWND hWndCtl = ::GetFocus();
	
	if (pMsg->message == WM_KEYDOWN)
	{
		if (::IsChild(*this, hWndCtl))
		{
			BOOL isctrl = (GetAsyncKeyState(VK_LCONTROL)&0x8000) || (GetAsyncKeyState(VK_RCONTROL)&0x8000);
			if (isctrl && (GetAsyncKeyState(VK_RMENU)&0x8000))
				isctrl = FALSE; // right-alt was pressed

/*			short ah1 = GetAsyncKeyState(VK_LCONTROL);
			ah1 = ah1;
			short ah2 = GetAsyncKeyState(VK_RCONTROL);
			ah2 = ah2;
			short ah3 = GetAsyncKeyState(VK_LMENU);
			ah3 = ah3;
			short ah4 = GetAsyncKeyState(VK_RMENU);
			ah4 = ah4;

			ATLTRACE("wparam %x, ah1=%x, ah2=%x, ah3=%x, ah4=%x\r\n", pMsg->wParam, ah1, ah2, ah3, ah4);
*/
			if ( pMsg->wParam == 0x56 && isctrl)
			{
				HRESULT hr;

				IHTMLSelectionObject *htmlSelection;
				CComVariant var;

				hr = m_InputBox.m_htmlInputBox->get_selection(&htmlSelection);

				if (htmlSelection) 
				{
					CComBSTR bstrType;
					hr = htmlSelection->get_type(&bstrType);

					if (SUCCEEDED(hr))
					{
						CComQIPtr<IHTMLTxtRange, &IID_IHTMLTxtRange> htmlTxtRange;

						hr = htmlSelection->createRange((IDispatch **)&htmlTxtRange);

						if (SUCCEEDED(hr))
						{
							if (OpenClipboard()) 
							{
							  HANDLE hClipboardData = GetClipboardData(CF_TEXT);

							  char *pchData = (char*)GlobalLock(hClipboardData);

							  CString strFromClipboard = pchData;

							  htmlTxtRange->put_text(CComBSTR(strFromClipboard));

							  GlobalUnlock(hClipboardData);

							  CloseClipboard();
							}
						}
					}

				return TRUE;
				}

			}

			if ( pMsg->wParam == 0x43 && isctrl || (pMsg->wParam == 0x58 && isctrl)) // c key
			{
				HRESULT hr;

				IHTMLSelectionObject *htmlSelection;
				CComVariant var;

				hr = m_InputBox.m_htmlInputBox->get_selection(&htmlSelection);

				if (htmlSelection) 
				{
					CComBSTR bstrType;
					hr = htmlSelection->get_type(&bstrType);

					if (SUCCEEDED(hr))
					{
						CString str = bstrType;

						if (!str.CompareNoCase("text"))
						{
							CComQIPtr<IHTMLTxtRange, &IID_IHTMLTxtRange> htmlTxtRange;

							hr = htmlSelection->createRange((IDispatch **)&htmlTxtRange);

							if (SUCCEEDED(hr))
							{
								CComBSTR bstrText; 
								htmlTxtRange->get_text(&bstrText);

								CString txt = bstrText;
								
								if (txt) 
								{
									if (OpenClipboard())
									{

										   EmptyClipboard();

										   HGLOBAL hClipboardData;
										   hClipboardData = GlobalAlloc(GMEM_DDESHARE, txt.GetLength()+1);

   
										   char * pchData;
										   pchData = (char*)GlobalLock(hClipboardData);
		  
										   strcpy(pchData, LPCSTR(txt));
										
										   GlobalUnlock(hClipboardData);
												  
										   SetClipboardData(CF_TEXT,hClipboardData);						  
										  
										   CloseClipboard();

										   if (pMsg->wParam == 0x58 && isctrl)
											   htmlTxtRange->put_text(CComBSTR(""));
									}
																		
								}
							}
						}

					}
				}
				return TRUE;
			}
		}
	}

	if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
	(pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
	{
		if (IsWindow())
			return IsDialogMessage(pMsg);
	}
	
	if(::IsChild(*this, hWndCtl))
	{
		// find a direct child of the dialog from the window that has focus
		while(::GetParent(hWndCtl) != m_hWnd)
		{
			hWndCtl = ::GetParent(hWndCtl);
		}

		// give control a chance to translate this message
		if(::SendMessageW(hWndCtl, WM_FORWARDMSG, 0, (LPARAM)pMsg) != 0)
		{
			return TRUE;
		}
	}
	if (IsWindow())
		return IsDialogMessage(pMsg);

	return FALSE;
}

LRESULT CMsgWin::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (m_ChatBox)
		m_ChatBox.Free();
	DestroyWindow();
	
	bHandled = TRUE;
	m_LastSay.Empty();

	return TRUE;
}

LRESULT CMsgWin::OnMoving(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	POINT cur_pos;
    RECT wa;
    GetCursorPos(&cur_pos);
	 RECT *rect = (RECT *)lParam;
 
    OffsetRect(rect, cur_pos.x - (rect->left + m_MoveX),cur_pos.y - (rect->top + m_MoveY));
    SystemParametersInfo(SPI_GETWORKAREA, 0, &wa, 0);
 
    if(isClose(rect->left, wa.left))
        OffsetRect( rect, wa.left - rect->left, 0 );
    else
  if(isClose(wa.right, rect->right))
   OffsetRect( rect, wa.right - rect->right, 0 );
 
    if(isClose(rect->top, wa.top))
        OffsetRect(rect, 0, wa.top - rect->top);
    else if(isClose(wa.bottom, rect->bottom))
        OffsetRect(rect, 0, wa.bottom - rect->bottom);

    return TRUE;
}

LRESULT CMsgWin::OnEnterSizeMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	POINT cur_pos;
    CRect rcWindow;
    GetWindowRect(rcWindow);
    GetCursorPos(&cur_pos);
    m_MoveX = cur_pos.x - rcWindow.left;
    m_MoveY = cur_pos.y - rcWindow.top;
	m_SizeX = rcWindow.right - rcWindow.left;
	m_SizeY = rcWindow.bottom - rcWindow.top;

	return TRUE;
}

LRESULT CMsgWin::OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return (LRESULT)m_BackBrush;
}

LRESULT CMsgWin::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	PAINTSTRUCT ps;
	RECT rt;
	HPEN hPenOld;
	
	
	GetClientRect(&rt);
	HDC hdc = BeginPaint(&ps);
	HDC dc = GetDC();
		

	
	if (m_hSeparatorPen)
	{	
		
		hPenOld = (HPEN)SelectObject(dc, (HGDIOBJ) m_hSeparatorPen);
		
		// main toolbar

		::MoveToEx(dc, rt.left + 2, rt.top+3, NULL); // fake gripper       
		::LineTo(dc, rt.left+2, rt.top+47);
		
		// middle toolbar separators
		::MoveToEx(dc, rt.left+74+44, rt.bottom-76, NULL);
		::LineTo(dc, rt.left+74+44, rt.bottom-56);
		::MoveToEx(dc, rt.left+145+44, rt.bottom-76, NULL);
		::LineTo(dc, rt.left+145+44, rt.bottom-56);
		::MoveToEx(dc, rt.left+172+44, rt.bottom-76, NULL);
		::LineTo(dc, rt.left+172+44, rt.bottom-56);

		
	
		// let's try to draw a nifty blue border ;)
		HWND hBox= GetDlgItem(IDC_LIST);
		RECT rc;
		::GetWindowRect(hBox, &rc);
		ScreenToClient(&rc);
		Rectangle(dc, rc.left, rc.top-1, rc.right, rc.bottom+1);
		
		hBox= GetDlgItem(IDC_EDITOR);
		::GetWindowRect(hBox, &rc);
		ScreenToClient(&rc);
		Rectangle(dc, rc.left, rc.top-1, rc.right, rc.bottom+1);
		SelectObject(dc, (HGDIOBJ) hPenOld);
	}
	ReleaseDC(dc);
	EndPaint(&ps);
	return TRUE;
}


LRESULT CMsgWin::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_Module.GetMessageLoop()->RemoveMessageFilter(this);

	m_ChatBox.Free();
	m_InputBox.Free();

	if (m_User)
	{
		WINDOWPLACEMENT wp = {0};
		GetWindowPlacement(&wp);
		if (wp.flags != SW_SHOWMINIMIZED && wp.flags != SW_HIDE)
			GetWindowRect(&m_User->m_ChatWindowRect);
	}
	if (m_Room)
	{
		WINDOWPLACEMENT wp = {0};
		GetWindowPlacement(&wp);
		if (wp.flags != SW_SHOWMINIMIZED && wp.flags != SW_HIDE)
			GetWindowRect(&m_Room->m_ChatWindowRect);
	}

	if (m_ChatBox.m_Events)
		delete m_ChatBox.m_Events;
	m_ChatBox.m_Events = NULL;

	m_ListLoaded = FALSE;

	if (m_EmoticonList)
	{
		if (m_EmoticonList->m_BalloonHwnd)
			::DestroyWindow(m_EmoticonList->m_BalloonHwnd);

		m_EmoticonList->m_BalloonHwnd = NULL;
	}
	

	if (m_Room)
	{
		m_Room->Leave();
	}


	bHandled = TRUE;
	m_hWnd = NULL;
	return TRUE;
}

LRESULT CMsgWin::OnSendButtonClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
	m_InputBox.Send();
	m_InputBox.SetFocus();
	PostMessage(WM_PURGEINPUTBOX, 0,0);
	return TRUE;
}

LRESULT CMsgWin::OnHumanHead(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	::SetFocus(m_hwndIEBox);

	return TRUE;
}

LRESULT CMsgWin::OnBoldBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_InputBox.ToggleBold();

	return TRUE;
}

LRESULT CMsgWin::OnItalicBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_InputBox.ToggleItalic();
	return TRUE;
}

LRESULT CMsgWin::OnUnderlineBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_InputBox.ToggleUnderline();
	return TRUE;
}

LRESULT CMsgWin::OnHyperlinkBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CInsertHyperlinkDlg dlg(this);
	dlg.DoModal();
	return TRUE;
}

LRESULT CMsgWin::OnEmoticonBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (_MainDlg.m_EmoticonsInstance.hInst)
	{
		RECT rc;
		::GetWindowRect(GetDlgItem(IDC_EMOTICONS), &rc);

		rc.top+=22;
//		::SetWindowPos(m_hwndEmoticonList, HWND_TOP, rc.left, rc.top, 300, 160,SWP_NOREDRAW);
//		::ShowWindow(m_hwndEmoticonList, SW_SHOW);
		::SetWindowPos(m_EmoticonList->m_hWnd, HWND_TOP, rc.left, rc.top, _MainDlg.m_EmoticonsInstance.IsBig?300:130, _MainDlg.m_EmoticonsInstance.IsBig?160:130,SWP_NOREDRAW);
		::ShowWindow(m_EmoticonList->m_hWnd, SW_SHOW);
//		m_EmoticonList->SetCapture();
//		m_EmoticonListCapture = TRUE;

		
		if (!m_ImagesLoaded)
		{
			m_ImagesLoaded = TRUE;
			int cnt = _MainDlg.m_EmoticonsInstance.GetCount();
			// load pictures
			for (int i=0;i<cnt;i++)
			{
				char *restype, *imagetype;
				int id = _MainDlg.m_EmoticonsInstance.GetItem(i, &restype, &imagetype);
				if (id)
				{
					CxImage *img = new CxImage();
					CString t = imagetype;
					t.MakeLower();
					int t1 = FindCxImageFormat(t);
					if (t1)
					{
						int res = __LoadIconFromResource(img, _MainDlg.m_EmoticonsInstance.hInst, restype, t1, id);
						if (res)
						{
							_MainDlg.m_EmoticonsInstance.m_Image.push_back(img);
							::SendMessage(m_EmoticonList->m_hWnd, LB_ADDSTRING, NULL, (LPARAM)"");
							m_ListLoaded = TRUE;
//							m_IconList.InsertString(_MainDlg.m_EmoticonsInstance.m_Image.size()-1, "");
						}
						else
							delete img;
					}
					else
						delete img;
				}
			}			
		}
		else
		{
			if (!m_ListLoaded)
			{
				m_ListLoaded = TRUE;
				for (int i=0;i<_MainDlg.m_EmoticonsInstance.m_Image.size();i++)
				{
					::SendMessage(m_EmoticonList->m_hWnd, LB_ADDSTRING, NULL, (LPARAM)"");
//					m_IconList.InsertString(i, "");
				}
			}
		}


//	HMENU hm = LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_USERLISTPOPUP));
//	HMENU h = GetSubMenu(hm, 0);
/*
		HWND h = GetDlgItem(IDC_CHOOSEEMOTICON);
		::ShowWindow(h, SW_SHOW);
		::SetWindowPos(h, HWND_TOPMOST, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
*/	

//	int i = ::TrackPopupMenu(h, TPM_LEFTALIGN | TPM_RETURNCMD, rc.left, rc.top, 0, this->m_hWnd, NULL);
//	i = i;

	}
	return TRUE;
}

LRESULT CMsgWin::OnFontSizeDownBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_InputBox.ChangeFontSize(-1);
	return TRUE;
}

LRESULT CMsgWin::OnFontSizeNormalBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_InputBox.ChangeFontSize(0);
	return TRUE;
}

LRESULT CMsgWin::OnFontSizeUpBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_InputBox.ChangeFontSize(1);
	return TRUE;
}

LRESULT CMsgWin::OnFontForecolorBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CColorDialog dlg;

	if (dlg.DoModal() == IDOK)
		m_InputBox.ChangeFontForecolor(dlg.GetColor());

	return TRUE;
}

LRESULT CMsgWin::OnFontBackcolorBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CColorDialog dlg;
	
	COLORREF *cCustom;
	cCustom = dlg.GetCustomColors();
	cCustom[0] = RGB(64,128,255);

	if (dlg.DoModal() == IDOK)
		m_InputBox.ChangeFontBackcolor(dlg.GetColor());


	return TRUE;
}

LRESULT CMsgWin::OnBtnDetails(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_User)
		_MainDlg.m_UserList.ExecuteRButtonUserCommand(/*NULL, */m_User, ID_POPUP1_DETAILS);
	return TRUE;
}

LRESULT CMsgWin::OnBtnMuteOnOff(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	_MainDlg.ToggleMute();
	return TRUE;
}

LRESULT CMsgWin::OnBtnClearHistory(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CComBSTR2 a = "Are you sure you want to delete the history?";
	int i = ::MessageBox(m_hWnd, a.ToString(), "Delete history?", MB_ICONQUESTION | MB_YESNO);
	if (i == 6)
	{
		m_LastSay.Empty();
		unlink(m_HistoryPath);
//		if (m_ChatBox.m_Events)
//			delete m_ChatBox.m_Events;
//		m_ChatBox.m_Events = NULL;
		VARIANT v;
		v.vt = VT_ERROR;
		m_ChatBox.m_Events->m_IgnoreNavigate = TRUE;
		m_ChatBox.m_wb2ChatBox->Navigate (CComBSTR("about:"),&v, &v, &v, &v );


		m_ChatBox.AddHtml(m_EmptyBody);
		m_ChatBox.SetDefMargin();
//		m_ChatBox.m_Events = new wWebEvents(m_ChatBox.m_wb2ChatBox);

	}
	return TRUE;
}

BOOL CMsgWin::ArrangeLayout()
{

	// called only by OnInitDialog
		RECT rc;
		GetClientRect(&rc);
		int iButtonX = rc.left + 6;

		// middle toolbar elements
		HWND hFontForecolor = GetDlgItem(IDC_FONTFORECOLOR);
		HWND hFontBackcolor = GetDlgItem(IDC_FONTBACKCOLOR);
		HWND hFontSizeDown = GetDlgItem(IDC_FONTSIZEDOWN);
		HWND hFontSizeNormal = GetDlgItem(IDC_FONTSIZENORMAL);
		HWND hFontSizeUp = GetDlgItem(IDC_FONTSIZEUP);
		HWND hBold = GetDlgItem(IDC_BOLD);
		HWND hItalic= GetDlgItem(IDC_ITALIC);
		HWND hUnderline = GetDlgItem(IDC_UNDERLINE);
		HWND hHyperlink = GetDlgItem(IDC_HYPERLINK);
		HWND hEmoticons = GetDlgItem(IDC_EMOTICONS);
//		HWND hChooseEmoticon= GetDlgItem(IDC_CHOOSEEMOTICON);
		HWND hIsTyping = GetDlgItem(IDC_ISTYPING);
		
		//main toolbar elements
		HWND hSendFile = GetDlgItem(ID_PNG1_SENDFILE);
		HWND hDetails = GetDlgItem(ID_PNG1_DETAILS);
		HWND hMuteOnOff = GetDlgItem(ID_PNG1_MUTEONOFF);
		HWND hHumanHead = GetDlgItem(IDC_PERSON);
		HWND hClearHistory = GetDlgItem(ID_PNG1_CLEARHISTORY);

		HWND hContactList = GetDlgItem(IDC_MSGWIN_USERS);
		
		//main elements
		HWND hList = GetDlgItem(IDC_LIST);
		HWND hEdit = GetDlgItem(IDC_EDITOR);
		HWND hBtn = GetDlgItem(IDB_SEND);
		
		if (m_Room)
		{
			::MoveWindow(hContactList,  rc.right-100, rc.top+50, rc.right, rc.bottom - 132, TRUE);
			::MoveWindow(hList,  rc.left, rc.top+50, rc.right-100, rc.bottom - 132, TRUE);
		}
		else
			::MoveWindow(hList,  rc.left, rc.top+50, rc.right , rc.bottom - 132, TRUE);
		::MoveWindow(hEdit,  rc.left, rc.bottom-50, rc.right-50, 50, TRUE);
		::MoveWindow(hBtn,  rc.right-50, rc.bottom-50, 50, 50, TRUE);

		// main toolbar elements
		::MoveWindow(hSendFile,  rc.left + 5, rc.top+1, 48, 48, TRUE);
		::MoveWindow(hDetails,  rc.left + 53, rc.top+1, 48, 48, TRUE);
		::MoveWindow(hMuteOnOff,  rc.left + 101, rc.top+1, 48, 48, TRUE);
		::MoveWindow(hClearHistory,  rc.left + 149, rc.top+1, 48, 48, TRUE);
		::MoveWindow(hHumanHead,  rc.right-48, rc.top, 48, 48, TRUE);

		// middle toolbar elements
		::MoveWindow(hFontForecolor, iButtonX, rc.bottom-77, 22, 22, TRUE);	iButtonX+=22;
		::MoveWindow(hFontBackcolor, iButtonX, rc.bottom-77, 22, 22, TRUE);	iButtonX+=22;
		::MoveWindow(hFontSizeDown, iButtonX, rc.bottom-77, 22, 22, TRUE);	iButtonX+=22;
		::MoveWindow(hFontSizeNormal , iButtonX, rc.bottom-77, 22, 22, TRUE); iButtonX+=22;
		::MoveWindow(hFontSizeUp,  iButtonX, rc.bottom-77, 22, 22, TRUE); iButtonX+=22 + 5; // 5 pixels for separator
		::MoveWindow(hBold,  iButtonX, rc.bottom-77, 22, 22, TRUE);	iButtonX+=22;
		::MoveWindow(hItalic,  iButtonX, rc.bottom-77, 22, 22, TRUE); iButtonX+=22;
		::MoveWindow(hUnderline,  iButtonX, rc.bottom-77, 22, 22, TRUE); iButtonX+=22 + 5; // 5 pixels for separator
		::MoveWindow(hHyperlink,  iButtonX, rc.bottom-77, 22, 22, TRUE ); iButtonX+=22+5; 
		::MoveWindow(hEmoticons,  iButtonX, rc.bottom-77, 32, 22, TRUE );
//		::MoveWindow(hChooseEmoticon,  iButtonX, rc.bottom-77+22, 75, 55, TRUE );

		//::MoveWindow(hIsTyping,  iButtonX, rc.bottom-77, 22, 22, TRUE );
		::SetWindowPos(hIsTyping, NULL, rc.right - 100, rc.bottom-77, 0, 0, SWP_NOSIZE | SWP_NOZORDER);


	return TRUE;
}

#define MSGMINWIDTH	    360
#define MSGMINHEIGHT	300

LRESULT CMsgWin::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_Room)
	{
		// this is chat room, is it minimized
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);
		if (wp.showCmd & SW_SHOWMINIMIZED)
			ShowWindow(SW_HIDE);

	}

/*	HWND hEmoticons = GetDlgItem(IDC_EMOTICONS);
	RECT rc;
	::GetWindowRect(hEmoticons, &rc);
	POINT p;
	p.x = rc.left;
	p.y = rc.top;
	ScreenToClient(&p);

	HWND hChooseEmoticon = GetDlgItem(IDC_CHOOSEEMOTICON);
	::MoveWindow(hChooseEmoticon,  p.x, p.y-101, 200, 100, FALSE );
*/
	// when sizing, scroll to the end
	CComQIPtr<IHTMLElement, &IID_IHTMLElement> htmlElement;
	if (m_ChatBox.m_htmlChatBox)
	{

		m_ChatBox.m_htmlChatBox->get_body(&htmlElement);
		if (htmlElement)
		{
			IHTMLElement2 *pElement = NULL;
			if (SUCCEEDED(htmlElement->QueryInterface(IID_IHTMLElement2,(LPVOID*)&pElement)))
			{
				pElement->put_scrollTop(100000);
				pElement->Release();
				KillTimer(1);
			}
		}
	}

	bHandled = FALSE;
	return FALSE;
}
LRESULT CMsgWin::OnSizing(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	LPRECT rc = (LPRECT)lParam;
	if (rc->right-rc->left < MSGMINWIDTH)
	{
		if (wParam == WMSZ_RIGHT || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_TOPRIGHT)
			rc->right = rc->left + MSGMINWIDTH;
		else
			rc->left = rc->right-MSGMINWIDTH;
	}
	if (rc->bottom-rc->top<MSGMINHEIGHT)
	{
	
		if (wParam == WMSZ_BOTTOM || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_BOTTOMLEFT)
			rc->bottom = rc->top+MSGMINHEIGHT;
		else
			rc->top = rc->bottom-MSGMINHEIGHT;
	}

	return TRUE;
}

BOOL CMsgWin::Show(void)
{
	char buff[1024];
	if (m_User)
		sprintf(buff, "%s (%s)", m_User->m_VisibleName, m_User->m_JID);
	else
		sprintf(buff, "%s", m_Room->m_JID);
	SetWindowText(buff);
	m_Button.SubclassWindow ( GetDlgItem(IDB_SEND) );


	if (m_User && m_User->m_ChatWindowRect.right > m_User->m_ChatWindowRect.left)
		SetWindowPos(NULL, &m_User->m_ChatWindowRect, SWP_NOZORDER);
	if (m_Room && m_Room->m_ChatWindowRect.right > m_Room->m_ChatWindowRect.left)
		SetWindowPos(NULL, &m_Room->m_ChatWindowRect, SWP_NOZORDER);

	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE); 
	
	return TRUE;
}

BOOL CMsgWin::AddTimestamp(Buffer *b)
{
	if (_Settings.m_TimestampMessages)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		char buff[100];
		sprintf(buff, "%d:%2d", st.wHour, st.wMinute);
		char *a = buff;
		while (*a)
		{
			if (*a == ' ')
				*a = '0';
			a++;
		}
		b->Append(buff);
		b->Append(" ");
	}

	return TRUE;
}

BOOL CMsgWin::SaveHistory(BOOL Mine, char *Text)
{
	if (_Settings.m_ShowMessageHistory)
	{

		int handle = open(m_HistoryPath, O_BINARY | O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
		if (handle == (-1))
			return FALSE;

		lseek(handle, 0, SEEK_END);
		Buffer b;
		SYSTEMTIME st;
		GetLocalTime(&st);

		FILETIME ft;
		SystemTimeToFileTime(&st, &ft);
		WORD d1,d2;
		FileTimeToDosDateTime(&ft, &d1, &d2);			
//			struct tm t;
//			memset(&t, 0, sizeof(tm));
//			SystimeToTm(&t, &st);

		b.PutChar(Mine);
//			b.PutInt(mktime(&t));
		b.PutInt(MAKELONG(d1, d2));
		b.PutCString(Text);

		write(handle, b.Ptr(), b.Len());
		close(handle);
	}
	return TRUE;
}

BOOL CMsgWin::LoadHistory(Buffer *c)
{
	if (m_User && _Settings.m_ShowMessageHistory)
	{
		char buff[32768];
		int handle = open(m_HistoryPath, O_BINARY | O_RDONLY , S_IREAD | S_IWRITE);
		if (handle == (-1))
			return FALSE;

		Buffer b;
		while (!eof(handle))
		{
			int i = read(handle, buff, 32768);
			if (i>0)
				b.Append(buff, i);
		};
		close(handle);

		CComBSTR last;
		
		CComBSTR2 j3 = _Settings.m_JID;
		char *j1 = j3.ToString();
		char *j2 = strchr(j1, '@');
		if (j2)
			*j2 = 0;

		SYSTEMTIME st;
		while (b.Len())
		{
			BOOL mine = b.GetChar();
			unsigned long tim = b.GetInt();
			if (tim)
			{
				unsigned int l = 0;
				char *a = b.GetString(&l);
				if (a && l)
				{
					WORD d1 = HIWORD(tim);
					WORD d2 = LOWORD(tim);
					FILETIME ft;
					DosDateTimeToFileTime(d2, d1, &ft);
					FileTimeToSystemTime(&ft, &st);
					{
						char *n = (mine?j1:m_User->m_VisibleName);
						BOOL major = FALSE;

						c->Append("<table cellspacing=0 cellpadding=0><tr><td width=99% valign=top>");
						if (!(last == n))
						{
							c->Append("<font face=\"Tahoma\" size=\"2\" color=\"C0C0C0\"><br><b>");
							last = n;
							c->Append(n);
							c->Append(" said:</b></b></font><hr style=\"margin: 0; padding: 0; border: 1px dotted #C0C0C0;\" />");
							c->Append("</td><td align=right valign=top width=1%><font face=\"Verdana\" size=\"1\" color=\"C0C0C0\">(");
							sprintf(buff, "%d:%d", st.wHour, st.wMinute);
							c->Append(buff);
							c->Append(")</font></td></tr><tr><td><font face=\"Verdana\" size=\"1\" color=\"C0C0C0\">");
							major = TRUE;
						}
						else
						{
							c->Append("<font face=\"Verdana\" size=\"1\" color=\"C0C0C0\">");
						}
						
						c->Append(a);
						if (major)
						{
							c->Append("</font></td><td> </td></tr></table>");
						}
						else
						{
							c->Append("</font></td><td valign=top align=right width=1%><font face=\"Verdana\" size=\"1\" color=\"C0C0C0\">(");
							sprintf(buff, "%d:%d", st.wHour, st.wMinute);
							c->Append(buff);
							c->Append(")</font></td></tr></table>");
						}
						free(a);
					}
				}
			}
		}
		c->Append("\0", 1);
		c->ConsumeEnd(1);
	}
	return TRUE;
}

void EscapeXML(Buffer *b)
{
	Buffer c;
	
	while (b->Len())
	{
		char *d = b->Ptr();
		switch (*d)
		{
//			case '\'':
//				c.Append("&apos;");
//				break;
//
			case '&':
				c.Append("&amp;");
				break;
				
			case '<':
				c.Append("&lt;");
				break;
				
			case '>':
				c.Append("&gt;");
				break;
				
			case '"':
				c.Append("&quot;");
				break;
				
			default:
				c.Append(d, 1);
				break;
		}
		b->Consume(1);
	}

	b->Append(c.Ptr(), c.Len());
}

Buffer *CMsgWin::CreateMsg(char *User, char *Text, char *Html, char *Color, char *BackColor)
{
	Html = "";
	Buffer *b = new Buffer();
	BOOL major = FALSE;
	b->Append("<table cellspacing=0 cellpadding=0 bgcolor=\"");
	b->Append(BackColor);
	b->Append("\"><tr><td width=99% valign=top>");
	if (!(m_LastSay == User))
	{
		b->Append("<br><font face=\"Tahoma\" size=\"2\" color=\"");
		b->Append(Color);
		b->Append("\"><b>");
		m_LastSay = User;
		b->Append(User);
		b->Append(" says:</b></b></font><hr style=\"border: 1px dotted #CCCCCC;\" />");
		b->Append("</td><td align=right valign=top width=1%><font face=\"Verdana\" size=\"1\">(");
		AddTimestamp(b);
		b->Append(")</font></td></tr><tr><td><font face=\"Verdana\" size=\"1\">");
		major = TRUE;
	}
	else
	{
		b->Append("<font face=\"Verdana\" size=\"1\">");
	}

	if (Html && *Html)
	{
		b->Append(Html);
	}
	else
	{
		Buffer c1;
		c1.Append(Text);
		EscapeXML(&c1);
		CString c2(c1.Ptr());
		c2.Replace("\r","");
		c2.Replace("\n","<BR>");
		b->Append(c2.GetBuffer(0));
	}
	if (major)
	{
		b->Append("</font></td><td> </td></tr></table>");
	}
	else
	{
		b->Append("</font></td><td valign=top align=right width=1%><font face=\"Verdana\" size=\"1\">(");
		AddTimestamp(b);
		b->Append(")</font></td></tr></table>");
	}

	b->Append("\0",1);
	b->ConsumeEnd(1);

	return b;
}

BOOL CMsgWin::Incoming(char *User, BOOL IsSystem, char *text, char *Html)
{
	//todo check if IsSystem
	if (!IsSystem)
	{
		SaveHistory(FALSE, text);
		CComBSTR2 u = User;
		char *u1 = u.ToString();
		char *u2 = strchr(u1, '@');
		if (u2)
			*u2 = 0;

		Buffer *b = CreateMsg(User, text, Html, "559040", "EEEEEE");

		BOOL didplayemoticonsound = m_ChatBox.AddLine(b, FALSE);
		delete b;
		if (!IsSystem && !didplayemoticonsound && !m_Room)
			_Notify.DoEvent(NotificationMsgIn);
	}
	else
	{
		m_LastSay.Empty();
		Buffer c;
		c.Append("<table cellspacing=0 cellpadding=0><tr><td width=99% valign=top><font face=\"Tahoma\" size=\"2\" color=\"800080\">");
		c.Append(text);
		c.Append("</td><td align=right valign=top width=1%><font face=\"Verdana\" size=\"1\">(");
		AddTimestamp(&c);
		c.Append(")</font></td></tr></table>");
		m_ChatBox.AddLine(&c, FALSE);
	}
	::ShowWindow(GetDlgItem(IDC_ISTYPING),  SW_HIDE);

	return TRUE;
}

void CMsgWin::OnFinalMessage(HWND /*hWnd*/)
{
	if (m_User)
	{
		// delete user window...
		// do nothing actually
	}
	if (m_Room)
	{
		// delete this room
		m_Room->m_MessageWin = NULL;
		for (int i=0;i<_MainDlg.m_ChatRooms.size();i++)
		{
			CChatRoom *room = _MainDlg.m_ChatRooms[i];
			if (room == m_Room)
			{
				_MainDlg.m_ChatRooms.erase(_MainDlg.m_ChatRooms.begin()+i);
				delete m_Room;
				break;
			}
		}
	}
}

LRESULT CMsgWin::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	ArrangeLayout();
	DlgResize_Init(); 

	m_ThreadID = GetCurrentThreadId();

	m_hwndIEBox = NULL;
	
	SetIcon(LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MSGWINICON)));

	HWND hContactList = GetDlgItem(IDC_MSGWIN_USERS);
	if (m_Room)
		::ShowWindow(hContactList, SW_SHOW);
	else
		::ShowWindow(hContactList, SW_HIDE);

	// Send button
	m_btnSend.SetCaption("Send");
	m_btnSend.SetToolTipText("Send");
	m_btnSend.SubclassWindow(GetDlgItem(IDB_SEND));
	
	//main toolbar buttons
	m_btnSendFile.SetCaption("Send file");
	m_btnSendFile.LoadPNG(ID_PNG2_SENDFILE);
	//m_btnSendFile.m_cxImage.Resample(24,24);
//	ResampleImageIfNeeded(&m_btnSendFile.m_cxImage, 32);
	m_btnSendFile.SubclassWindow(GetDlgItem(ID_PNG1_SENDFILE));
	m_btnSendFile.SetToolTipText("Send file");

	m_btnDetails.SetCaption("Details");
	m_btnDetails.LoadPNG(ID_PNG2_DETAILS);
	//m_btnDetails.m_cxImage.Resample(24,24);
//	ResampleImageIfNeeded(&m_btnDetails.m_cxImage, 32);
	m_btnDetails.SubclassWindow(GetDlgItem(ID_PNG1_DETAILS));
	m_btnDetails.SetToolTipText("Contact information");

	m_btnMuteOnOff.SetCaption("Mute");
	int mutimg = _Settings.m_SoundOn?ID_PNG2_MUTEON:ID_PNG2_MUTEOFF;
	m_btnMuteOnOff.LoadPNG(mutimg);
	//m_btnMuteOnOff.m_cxImage.Resample(24,24);
	ResampleImageIfNeeded(&m_btnMuteOnOff.m_cxImage, 32);
	m_btnMuteOnOff.SubclassWindow(GetDlgItem(ID_PNG1_MUTEONOFF));
	m_btnMuteOnOff.SetToolTipText("Mute sound");

	m_btnClearHistory.SetCaption("Clear");
	m_btnClearHistory.LoadPNG(ID_PNG_CLEARHISTORY);
	//m_btnDetails.m_cxImage.Resample(24,24);
//	ResampleImageIfNeeded(&m_btnDetails.m_cxImage, 32);
	m_btnClearHistory.SubclassWindow(GetDlgItem(ID_PNG1_CLEARHISTORY));
	m_btnClearHistory.SetToolTipText("Clear history");


	// middle toolbar buttons
	m_btnBold.LoadPNG(IDB_BOLD);
	m_btnBold.SetToolTipText("Bold");
	m_btnBold.SubclassWindow(GetDlgItem(IDC_BOLD));
	
	m_btnItalic.LoadPNG(IDB_ITALIC);
	m_btnItalic.SetToolTipText("Italic");
	m_btnItalic.SubclassWindow(GetDlgItem(IDC_ITALIC));

	m_btnUnderline.LoadPNG(IDB_UNDERLINE);
	m_btnUnderline.SetToolTipText("Underline");
	m_btnUnderline.SubclassWindow(GetDlgItem(IDC_UNDERLINE));

	m_btnHyperlink.LoadPNG(IDB_HYPERLINK);
	m_btnHyperlink.SetToolTipText("Insert hyperlink");
	m_btnHyperlink.SubclassWindow(GetDlgItem(IDC_HYPERLINK));

	m_btnFontSizeDown.LoadPNG(IDB_FONTSIZEDOWN);
	m_btnFontSizeDown.SetToolTipText("Decrease font size");
	m_btnFontSizeDown.SubclassWindow(GetDlgItem(IDC_FONTSIZEDOWN));

	m_btnFontSizeNormal.LoadPNG(IDB_FONTSIZENORMAL);
	m_btnFontSizeNormal.SetToolTipText("Reset font size");
	m_btnFontSizeNormal.SubclassWindow(GetDlgItem(IDC_FONTSIZENORMAL));

	m_btnFontSizeUp.LoadPNG(IDB_FONTSIZEUP);
	m_btnFontSizeUp.SetToolTipText("Increase font size");
	m_btnFontSizeUp.SubclassWindow(GetDlgItem(IDC_FONTSIZEUP));
	
	m_btnFontForecolor.LoadPNG(IDB_FONTFORECOLOR);
	m_btnFontForecolor.SetToolTipText("Set Foreground color");
	m_btnFontForecolor.SubclassWindow(GetDlgItem(IDC_FONTFORECOLOR));

	m_btnFontBackcolor.LoadPNG(IDB_FONTBACKCOLOR);
	m_btnFontBackcolor.SetToolTipText("Set Background color");
	m_btnFontBackcolor.SubclassWindow(GetDlgItem(IDC_FONTBACKCOLOR));
	
	m_btnEmoticons.LoadPNG(IDB_EMOTICONS);
	m_btnEmoticons.SetToolTipText("Emoticons");
	m_btnEmoticons.SubclassWindow(GetDlgItem(IDC_EMOTICONS));
	m_btnEmoticons.SetDropDown(TRUE);
	
//	if (m_HumanHead != NULL)
//		m_btnHumanHead.LoadPNGFromBuffer(m_HumanHead);
//	else
//		m_btnHumanHead.LoadPNG(IDB_HUMAN1);
	m_btnHumanHead.LoadPNG(m_HumanHead);
	m_btnHumanHead.SetBorders(0);
	m_btnHumanHead.SetBitmapButtonExtendedStyle(BMPBTN_AUTO3D_SINGLE);
	m_btnHumanHead.SubclassWindow(GetDlgItem(IDC_PERSON));

	if (_MainDlg.m_WearSkin)
		SetWindowSkin(m_hWnd, "MainFrame" );
	if (!m_InputBox.Init(this))
		return FALSE;

	if (!m_ChatBox.Init(this))
		return FALSE;

	_Module.GetMessageLoop()->AddMessageFilter(this);

//	if (!m_InputBox.Init(this)) MessageBox(":(");
//	if (!m_ChatBox.Init(this)) MessageBox(":(");

	if (m_User)
	{
		if (m_User && m_User->m_ChatWindowRect.right > m_User->m_ChatWindowRect.left)
		{
			if (_MainDlg.m_DefChatWindowRect.right == _MainDlg.m_DefChatWindowRect.left && !_MainDlg.m_DefChatWindowRect.right)
				memcpy(&_MainDlg.m_DefChatWindowRect, &m_User->m_ChatWindowRect, sizeof(_MainDlg.m_DefChatWindowRect));

			SetWindowPos(NULL, &m_User->m_ChatWindowRect, SWP_NOZORDER);
		}
		else
			if (_MainDlg.m_DefChatWindowRect.right > _MainDlg.m_DefChatWindowRect.left)
			{
				memcpy(&m_User->m_ChatWindowRect, &_MainDlg.m_DefChatWindowRect, sizeof(_MainDlg.m_DefChatWindowRect));
				_MainDlg.m_DefChatWindowRect.left -= 40;
				_MainDlg.m_DefChatWindowRect.right -= 40;
				_MainDlg.m_DefChatWindowRect.top += 40;
				_MainDlg.m_DefChatWindowRect.bottom += 40;
				SetWindowPos(NULL, &_MainDlg.m_DefChatWindowRect, SWP_NOZORDER);
			}
	}
	if (m_Room)
	{
		if (m_Room && m_Room->m_ChatWindowRect.right > m_Room->m_ChatWindowRect.left)
		{
			
			if (_MainDlg.m_DefChatWindowRect.right == _MainDlg.m_DefChatWindowRect.left && !_MainDlg.m_DefChatWindowRect.right)
				memcpy(&_MainDlg.m_DefChatWindowRect, &m_Room->m_ChatWindowRect, sizeof(_MainDlg.m_DefChatWindowRect));
			
			SetWindowPos(NULL, &m_Room->m_ChatWindowRect, SWP_NOZORDER);
		}
		else
		if (_MainDlg.m_DefChatWindowRect.right > _MainDlg.m_DefChatWindowRect.left)
		{
			memcpy(&m_Room->m_ChatWindowRect, &_MainDlg.m_DefChatWindowRect, sizeof(_MainDlg.m_DefChatWindowRect));
			_MainDlg.m_DefChatWindowRect.left -= 40;
			_MainDlg.m_DefChatWindowRect.right -= 40;
			_MainDlg.m_DefChatWindowRect.top += 40;
			_MainDlg.m_DefChatWindowRect.bottom += 40;
			SetWindowPos(NULL, &_MainDlg.m_DefChatWindowRect, SWP_NOZORDER);
		}
	}

	HWND m_hwndEmoticonList = CreateWindowEx(WS_EX_TOOLWINDOW, "LISTBOX",NULL, WS_POPUP | WS_BORDER | LBS_OWNERDRAWFIXED | LBS_NOTIFY | LBS_MULTICOLUMN | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | WS_HSCROLL,0,0,0,0,m_hWnd,NULL,_Module.GetModuleInstance(),NULL);
	if (!m_EmoticonList)
		m_EmoticonList = new CWEmoticon(this);
	m_EmoticonList->SubclassWindow(m_hwndEmoticonList);
//	m_IconList.Attach(hwndList);


	if (m_User)
	{
		strcpy(m_HistoryPath, _Settings.m_HistoryPath);
		strcat(m_HistoryPath, m_User->m_JID); 
		strcat(m_HistoryPath, ".history");
	}

	return TRUE;
}

//IDC_MSGWIN_USERS
void CMsgWin::RemoveUserFromContactList(char *JID)
{
	char *a = strchr(JID, '/');
	if (a)
	{	
		JID = a;
		JID++;
	}
	a = strchr(JID, '@');
	if (a)
	{
		JID = a;
		JID++;
	}

	int res = SendDlgItemMessage(IDC_MSGWIN_USERS, LB_FINDSTRING, 0, (LPARAM)JID);
	if (res >= 0)
		SendDlgItemMessage(IDC_MSGWIN_USERS, LB_DELETESTRING, res, 0);

}
void CMsgWin::AddUserToContactList(char *JID, BOOL isadmin)
{
	char *a = strchr(JID, '/');
	if (a)
	{	
		JID = a;
		JID++;
	}
	a = strchr(JID, '@');
	if (a)
	{
		JID = a;
		JID++;
	}

	RemoveUserFromContactList(JID);

	int res = SendDlgItemMessage(IDC_MSGWIN_USERS, LB_ADDSTRING, 0, (LPARAM)JID);
}


LRESULT CMsgWin::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CComQIPtr<IHTMLElement, &IID_IHTMLElement> htmlElement;

	m_ChatBox.m_htmlChatBox->get_body(&htmlElement);
	if (htmlElement)
	{
		IHTMLElement2 *pElement = NULL;
		if (SUCCEEDED(htmlElement->QueryInterface(IID_IHTMLElement2,(LPVOID*)&pElement)))
		{
/*//			pElement->scroll();
			VARIANT var;
			var.vt = VT_BSTR;
			var.bstrVal = T2BSTR("s
//			pElement->doScroll();
*/
			pElement->put_scrollTop(100000);
			pElement->Release();
			KillTimer(1);
		}
	}

	return TRUE;
}

CMsgWin::CChatBox::CChatBox()
{
	m_wb2ChatBox = NULL;
	m_fDocWindow = FALSE;
	m_Events = NULL;
	m_htmlChatBox = NULL;
}

CMsgWin::CChatBox::~CChatBox()
{
	Free();

}

//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////

HRESULT CMsgWin::CChatBox::AddHtml(BSTR HtmlCode)
{
	if (!m_htmlChatBox || !m_Events || !m_Events->m_NavComplete)
	{
		// store for later on
		CComBSTR2 a = HtmlCode;
		m_AddHtml.Append(a.ToString());
		return S_OK;
	}

	HRESULT hr = S_OK;

	SAFEARRAY *sf;
	VARIANT *param;

	CComBSTR bstrHTML(HtmlCode);

	sf = SafeArrayCreateVector(VT_VARIANT, 0, 1);
			
	if (sf == NULL) 
		return E_FAIL;

	SafeArrayAccessData(sf,(LPVOID*) &param);
	param->vt = VT_BSTR;
	param->bstrVal = bstrHTML.Copy();
	SafeArrayUnaccessData(sf);

	hr = m_htmlChatBox->write(sf);
	SafeArrayDestroy(sf);


	CComQIPtr<IHTMLElement, &IID_IHTMLElement> htmlElement;

	hr = m_htmlChatBox->get_body(&htmlElement);
	if (htmlElement)
	{
		IHTMLElement2 *pElement = NULL;
		if (SUCCEEDED(htmlElement->QueryInterface(IID_IHTMLElement2,(LPVOID*)&pElement)))
		{
/*//			pElement->scroll();
		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = T2BSTR("s
//			pElement->doScroll();
*/
			pElement->put_scrollTop(100000);
			m_ParentDlg->SetTimer(1, 200);
			pElement->Release();
		}
	}
	return hr;

}

BOOL CMsgWin::CChatBox::AddLine(Buffer *Line, BOOL islocal)
{
	Buffer b1, b2;
	b1.Append(Line->Ptr(), Line->Len());
	BOOL didplayemoticonsound = FALSE;

	// emoticons
	if (_MainDlg.m_EmoticonsInstance.hInst)
//	for (int i=0;i<_MainDlg.m_EmoticonsInstance.size();i++)
	{
//		CMainDlg::EmoticonsStruct *st = (CMainDlg::EmoticonsStruct *)_MainDlg.m_EmoticonsInstance[0];
		didplayemoticonsound = _MainDlg.m_EmoticonsInstance.Transform(&b1, TRUE, 
			islocal?FALSE:(_Settings.m_SoundOn?TRUE:FALSE)); // we play only if it's local, or sound is on
//		if (_MainDlg.m_EmoticonsInstance.Transform(&b1, &b2, TRUE))
//		{
//			b1.Clear();
//			b1.Append(b2.Ptr(), b2.Len());
//		}
	}

	
	
//	b1.Append("<br>\0", 5);
//	CString str(Line);
//	str+="<br>";
//	CComBSTR bstr(str);

	b1.Append("\0");
	CComBSTR bstr(b1.Ptr());
	AddHtml(bstr);
	return didplayemoticonsound;
}

BOOL CMsgWin::CChatBox::Free()
{
	if (m_hWnd)
		UnsubclassWindow(FALSE);
	if (m_wb2ChatBox) 
	{
//		try
//		{
		m_wb2ChatBox = NULL;
//		}
//		catch(_com_error e)
//		{
//			e = e;
//			MessageBeep(-1);
//		}
//		catch(_com_error *e)
//		{
//			e = e;
//			MessageBeep(-1);
//		}
	}
/*	if (m_htmlChatBox)
	{
		m_htmlChatBox->Release();
	}
*/

	return TRUE;
}

BOOL CMsgWin::CChatBox::Init(CMsgWin *dlg)
{
	CMsgWin *d = (CMsgWin *)dlg;
	HRESULT hr;
//	IDispatch *lpDisp;

	CAxWindow wndChatBox = d->GetDlgItem(IDC_LIST);

	if (m_wb2ChatBox) m_wb2ChatBox->Release();

	hr = wndChatBox.QueryControl ( &m_wb2ChatBox);

	if (SUCCEEDED(hr))
	{
		// need to initialize it, or IHTMLDocument2 won't be created
		CComVariant v;  // empty variant
		m_wb2ChatBox->put_Resizable(VARIANT_FALSE);
	
		m_Events = new wWebEvents(this);
		m_Events->m_IgnoreNavigate = TRUE;
		m_wb2ChatBox->Navigate (CComBSTR("about:"),&v, &v, &v, &v );

		HWND hwndChatBox;

		hwndChatBox = d->GetDlgItem(IDC_LIST);
		hwndChatBox = ::GetWindow(hwndChatBox, GW_CHILD);
		hwndChatBox = ::GetWindow(hwndChatBox, GW_CHILD);
	
		if (m_hWnd) UnsubclassWindow();
		SubclassWindow(hwndChatBox);

		m_ParentDlg = dlg;
	}

	return SUCCEEDED(hr) ? TRUE : FALSE;
}

void CMsgWin::CChatBox::SetDefMargin(void)
{

		// change default font, margins, etc
		IHTMLElement *body = NULL;
		m_htmlChatBox->get_body(&body);
		
/*		if (body)
		{
			IHTMLStyle *style = NULL;
			CComVariant vfontSize(CString("1px"));
			VARIANT v0;
			v0.vt = VT_I2;
			v0.iVal = 0;
			CComBSTR2 b2 = "Verdana";

			body->get_style(&style);

			if (style)
			{
				style->put_marginTop(v0);
				style->put_marginLeft(v0);
				style->put_marginRight(v0);
				style->put_marginBottom(v0);
				style->put_fontFamily(b2);
				style->put_fontSize(vfontSize);
				style->put_lineHeight(v0);
			}
		}
*/
}



//////////////////////////////////////////////////////////////////////
// Window message handlers
//////////////////////////////////////////////////////////////////////


LRESULT CMsgWin::CChatBox::OnParentNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == WM_CREATE)
	{	
		HWND hChild = (HWND) lParam;
		char strClassName[256];

		if (::GetClassName(hChild, strClassName, 255))
		{
			strlwr(strClassName);
			if (strstr(strClassName, "internet") && strstr(strClassName, "explorer") && strstr(strClassName, "server"))
			{
				UnsubclassWindow();
				SubclassWindow(hChild);
				m_fDocWindow = TRUE;
			}
		}
		bHandled = FALSE;
	}
		return 0;
}

LRESULT CMsgWin::CChatBox::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_MainDlg.CheckIfAntiInactivityMessage(WM_CHAR);
	::CallWindowProc(m_pfnSuperWindowProc, m_hWnd, uMsg, wParam, lParam);

	IHTMLSelectionObject *sel;
	if (SUCCEEDED(m_htmlChatBox->get_selection(&sel)))
	{
		IDispatch *disp;
		if (SUCCEEDED(sel->createRange(&disp)))
		{
			IHTMLTxtRange *r;
			if (SUCCEEDED(disp->QueryInterface(IID_IHTMLTxtRange, (void **)&r)))
			{
				CComBSTR2 t;//, t2;
				r->get_text(&t);
//				r->get_htmlText(&t2);


				char *t1 = t.ToString();
				int cch = strlen(t1);

				if (OpenClipboard())
				{
					EmptyClipboard(); 

					HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (cch + 1)); 
					if (hglbCopy)
					{					
						char * lptstrCopy = (char *)GlobalLock(hglbCopy); 
						memcpy(lptstrCopy, t1, cch); 
						lptstrCopy[cch] = 0;    // null character 
						GlobalUnlock(hglbCopy); 
				        SetClipboardData(CF_TEXT, hglbCopy); 
					}
					CloseClipboard(); 
				}
				r->Release();
			}
			disp->Release();
		}
		sel->Release();
	}


//	m_wb2ChatBox->ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
	m_wb2ChatBox->ExecWB(OLECMDID_CLEARSELECTION, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
	::SetFocus(m_ParentDlg->m_hwndIEBox);
	bHandled = FALSE;
	return 0;
}

LRESULT CMsgWin::CChatBox::OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	::SetFocus(m_ParentDlg->m_hwndIEBox);
	return 0;
}



CMsgWin::CInputBox::CInputBox()
{
	m_wb2InputBox = NULL;
	m_fDocWindow = FALSE;
	m_htmlInputBox = NULL;
	m_htmlEditDesigner = NULL;
}

CMsgWin::CInputBox::~CInputBox()
{
	if (m_htmlEditDesigner) 
	{
		m_htmlEditDesigner->Detach();
		m_htmlEditDesigner->Release();

		delete m_htmlEditDesigner;
	}

	if (IsWindow()) DestroyWindow();

}


BOOL CMsgWin::CInputBox::Free()
{
	if (m_hWnd)
		UnsubclassWindow(FALSE);
	if (m_wb2InputBox) 
	{
//		try
//		{
		m_wb2InputBox = NULL;
/*		}catch(_com_error e)
		{
			e = e;
			MessageBeep(-1);
		}
		catch(_com_error *e)
		{
			e = e;
			MessageBeep(-1);
		}
*/
	}
	if (m_htmlEditDesigner) 
	{
		m_htmlEditDesigner->Detach();
		m_htmlEditDesigner->Release();

		delete m_htmlEditDesigner;
		m_htmlEditDesigner = NULL;
	}


/*	if (m_htmlChatBox)
	{
		m_htmlChatBox->Release();
	}
*/

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////


HRESULT CMsgWin::CInputBox::AddHtml(BSTR HtmlCode)
{
	HRESULT hr;

	SAFEARRAY *sf;
	VARIANT *param;

	CComBSTR bstrHTML(HtmlCode);

	sf = SafeArrayCreateVector(VT_VARIANT, 0, 1);
			
	if (sf == NULL) 
		return E_FAIL;

	SafeArrayAccessData(sf,(LPVOID*) &param);
	param->vt = VT_BSTR;
	param->bstrVal = bstrHTML;
	SafeArrayUnaccessData(sf);

	hr = m_htmlInputBox->write(sf);
	SafeArrayDestroy(sf);

	return hr;

}

BOOL CMsgWin::CInputBox::Init(CMsgWin *dlg)
{
	CMsgWin *d = (CMsgWin *)dlg;
	HRESULT hr;
	IDispatch *lpDisp;

	CAxWindow wndInputBox = d->GetDlgItem(IDC_EDITOR);

	if (m_wb2InputBox) m_wb2InputBox->Release();

	hr = wndInputBox.QueryControl ( &m_wb2InputBox);

	if (SUCCEEDED(hr))
	{
		CComBSTR bstrDesignMode("On");
		// need to initialize it, or IHTMLDocument2 won't be created
		CComVariant v;  // empty variant
		m_wb2InputBox->put_Resizable(VARIANT_FALSE);
		
		m_wb2InputBox->Navigate (CComBSTR("about:"),&v, &v, &v, &v );
		// now we get the document interface

		hr = m_wb2InputBox->get_Document(&lpDisp);
		m_htmlInputBox = (IHTMLDocument2 *)lpDisp;		

		if (m_htmlInputBox)
		{
			// let's put our HTML "document" into design mode :)
			m_htmlInputBox->put_designMode(bstrDesignMode);
		}
		else
			return FALSE;
	
		m_ParentDlg = dlg;

		AddHtml(m_ParentDlg->m_EmptyBody);
		
		// change default font, margins, etc
		IHTMLElement *body;
		if (m_htmlInputBox)
			m_htmlInputBox->get_body(&body);
		else
			return FALSE;
		
		IHTMLStyle *style;

		m_fontBackColor = "transparent";
		m_fontForeColor = "000000";

		VARIANT v0;
		v0.vt = VT_I2;
		v0.intVal = 0;

		body->get_style(&style);
	
		hr = style->put_marginTop(v0);
		hr = style->put_marginLeft(v0);
		hr = style->put_marginRight(v0);
		hr = style->put_marginBottom(v0);
		hr = style->put_fontFamily(CComBSTR("Arial"));
		hr = style->put_fontSize(CComVariant(CString("x-small")));;
		hr = style->put_backgroundColor(CComVariant(m_fontBackColor));
		hr = style->put_color(CComVariant(m_fontForeColor));
	
		if (!m_htmlEditDesigner)
		{
			m_htmlEditDesigner = new CMyHTMLEditDesigner(dlg);
			m_htmlEditDesigner->AddRef();
			m_htmlEditDesigner->Attach(m_htmlInputBox);
		}

    }
	
	return SUCCEEDED(hr) ? TRUE : FALSE;
}

HRESULT CMsgWin::CInputBox::ChangeFontBackcolor(COLORREF bgColor,/*optional*/ BOOL bTransparent)
{
	HRESULT hr;
	IHTMLSelectionObject *htmlSelection;
	CComVariant var;

	char buff[20];
	VARIANT_BOOL vbool;
	sprintf(buff,"%02X%02X%02X", GetRValue(bgColor),GetGValue(bgColor),GetBValue(bgColor));

	if (bTransparent)
		var = CString("transparent");
	else
		var = CString(buff);
	
	CMsgWin *dlg = (CMsgWin *) m_ParentDlg;

 	hr = m_htmlInputBox->get_selection(&htmlSelection);

	if (htmlSelection) // user has html code selected
	{
		CComBSTR bstrType;

		hr = htmlSelection->get_type(&bstrType);
		if (SUCCEEDED(hr))
		{
			CString str = bstrType;
			if (!str.CompareNoCase("text"))
			{
				CComQIPtr<IHTMLTxtRange, &IID_IHTMLTxtRange> htmlTxtRange;
				hr = htmlSelection->createRange((IDispatch **)&htmlTxtRange);

				if (SUCCEEDED(hr))
					hr = htmlTxtRange->execCommand(CComBSTR("BackColor"), VARIANT_FALSE, var, &vbool);
				
			}
			if (!str.CompareNoCase("none")) // no selection
				hr = m_htmlInputBox->execCommand(CComBSTR("BackColor"), VARIANT_FALSE, var, &vbool);				
		}
	}
	return hr;
}
HRESULT CMsgWin::CInputBox::ChangeFontForecolor(COLORREF fgColor)
{	
	HRESULT hr;
	IHTMLSelectionObject *htmlSelection;
	CComVariant var;

	char buff[20];
	VARIANT_BOOL vbool;
	sprintf(buff,"%02X%02X%02X", GetRValue(fgColor),GetGValue(fgColor),GetBValue(fgColor));

	var = CString(buff);

	CMsgWin *dlg = (CMsgWin *) m_ParentDlg;

 	hr = m_htmlInputBox->get_selection(&htmlSelection);

	if (htmlSelection) // user has html code selected
	{
		CComBSTR bstrType;

		hr = htmlSelection->get_type(&bstrType);
		if (SUCCEEDED(hr))
		{
			CString str = bstrType;
			if (!str.CompareNoCase("text"))
			{
				CComQIPtr<IHTMLTxtRange, &IID_IHTMLTxtRange> htmlTxtRange;
				hr = htmlSelection->createRange((IDispatch **)&htmlTxtRange);

				if (SUCCEEDED(hr))
					hr = htmlTxtRange->execCommand(CComBSTR("ForeColor"), VARIANT_FALSE, var, &vbool);		
				
			}
			if (!str.CompareNoCase("none")) // no selection
				hr = m_htmlInputBox->execCommand(CComBSTR("ForeColor"), VARIANT_FALSE, var, &vbool);	
			
		}
	}
	return hr;

}

HRESULT CMsgWin::CInputBox::ChangeFontSize(signed int dwDelta)
{
	HRESULT hr;
	IHTMLSelectionObject *htmlSelection;
	CComVariant var;

	CMsgWin *dlg = (CMsgWin *) m_ParentDlg;

 	hr = m_htmlInputBox->get_selection(&htmlSelection);

	if (htmlSelection) // user has html code selected
	{
		CComBSTR bstrType;

		hr = htmlSelection->get_type(&bstrType);
		if (SUCCEEDED(hr))
		{
			CString str = bstrType;
			if (!str.CompareNoCase("text"))
			{
				CComQIPtr<IHTMLTxtRange, &IID_IHTMLTxtRange> htmlTxtRange;
				hr = htmlSelection->createRange((IDispatch **)&htmlTxtRange);

				if (SUCCEEDED(hr))
				{
				
					hr = htmlTxtRange->queryCommandValue(CComBSTR("FontSize"), &var);
					if (hr == S_OK)
					{
							VARIANT_BOOL vbool;
							if (dwDelta != 0)
								var.intVal += dwDelta;
							else
								var.intVal = 2;
							hr = htmlTxtRange->execCommand(CComBSTR("FontSize"), VARIANT_FALSE, var, &vbool);
					}
				}
			}
			if (!str.CompareNoCase("none")) // no selection
			{	
				hr = m_htmlInputBox->queryCommandValue(CComBSTR("FontSize"), &var);
				if (hr == S_OK)
				{
					VARIANT_BOOL vbool;
					if (dwDelta != 0)
						var.intVal += dwDelta;
					else
						var.intVal = 2;
					
					hr = m_htmlInputBox->execCommand(CComBSTR("FontSize"), VARIANT_FALSE, var, &vbool);
				}
			}
		}
	}
	
	return hr;

}

HRESULT CMsgWin::CInputBox::ToggleBold()
{

	return _ToggleHtmlCommand(CComBSTR("Bold"));

}

HRESULT CMsgWin::CInputBox::ToggleItalic()
{
	return _ToggleHtmlCommand(CComBSTR("Italic"));
}

HRESULT CMsgWin::CInputBox::ToggleUnderline()
{
	return _ToggleHtmlCommand(CComBSTR("Underline"));
}

HRESULT CMsgWin::CInputBox::_ToggleHtmlCommand(BSTR Command)
{
	HRESULT hr;
	CComVariant var;
	CComQIPtr<IHTMLElement, &IID_IHTMLElement> htmlElement;
	CComQIPtr<IHTMLSelectionObject, &IID_IHTMLSelectionObject> htmlSelection;

	CMsgWin *dlg = (CMsgWin *) m_ParentDlg;

 	hr = m_htmlInputBox->get_selection(&htmlSelection);

	if (SUCCEEDED(hr)) // user has html code selected
	{
		CComBSTR bstrType;

		hr = htmlSelection->get_type(&bstrType);
		if (SUCCEEDED(hr))
		{
			CString str = bstrType;
			if (!str.CompareNoCase("text"))
			{
				CComQIPtr<IHTMLTxtRange, &IID_IHTMLTxtRange> htmlTxtRange;
				hr = htmlSelection->createRange((IDispatch **)&htmlTxtRange);

				if (SUCCEEDED(hr))
				{
				
					hr = htmlTxtRange->queryCommandValue(Command, &var);
					if (hr == S_OK)
					{
						VARIANT_BOOL vbool;
				
						if (var.vt == VT_BOOL)
						{
							if (var.boolVal = VARIANT_TRUE) 
								var.boolVal = VARIANT_FALSE;
							else
								var.boolVal = VARIANT_TRUE;
							hr = htmlTxtRange->execCommand(Command, VARIANT_FALSE, var, &vbool);
						}
					}
				}
			}
			if (!str.CompareNoCase("none")) // no selection
			{	
				hr = m_htmlInputBox->queryCommandValue(Command, &var);
				if (hr == S_OK)
				{
					VARIANT_BOOL vbool;
				
					if (var.vt == VT_BOOL)
					{
						if (var.boolVal = VARIANT_TRUE) 
							var.boolVal = VARIANT_FALSE;
						else
							var.boolVal = VARIANT_TRUE;
						hr = m_htmlInputBox->execCommand(Command, VARIANT_FALSE, var, &vbool);
					}
				}
			}
		}
	}
	return  hr;
}

HRESULT CMsgWin::CInputBox::Send()
{
	HRESULT hr;
	
	CComQIPtr<IHTMLElement, &IID_IHTMLElement> htmlElement;

	hr = m_htmlInputBox->get_activeElement(&htmlElement);
	if (htmlElement)
	{
		CComBSTR bstr,bstr2;
		hr = htmlElement->get_innerHTML(&bstr);
		hr = htmlElement->get_innerText(&bstr2);
//		CString str = bstr;
		
		if (bstr2.Length())
		{
			CString s = bstr;

			// empty lines workaround
			s.Replace("<P>","");
			s.Replace("</P>","");

			// strip backcolor if no backcolor applied (to get rid of unneeded white back)
			
			bstr = s;
			
			if (SUCCEEDED(hr) && bstr.Length())
			{

/*
				Buffer b1;
				b1.Append("<font color=#0000FF>");
//				Buffer b10;
				m_ParentDlg->AddTimestamp(&b1);
				b1.Append("<b>");
				CComBSTR2 b5 = _Settings.m_JID;
				char *b6 = b5.ToString();
				char *b7 = strchr(b6, '@');
				if (b7)
					*b7 = 0;
				b7 = strchr(b6, '#');
				if (b7)
					*b7 = 0;
				b1.Append(b6);
				b1.Append("</b></font>: <font color=#000000>");
				CComBSTR2 b10 = bstr;
				b1.Append(b10.ToString());
				b1.Append("</font>");
*/
				CComBSTR2 b5 = _Settings.m_JID;
				char *b6 = b5.ToString();
				char *b7 = strchr(b6, '@');
				if (b7)
					*b7 = 0;
				CComBSTR2 b3(bstr2);
				CComBSTR2 b2(bstr);
				Buffer *b1 = m_ParentDlg->CreateMsg(b6, b3.ToString(), b2.ToString(), "000000", "FFFFFF");

							

				if (m_ParentDlg->m_User)
				{
					m_ParentDlg->m_ChatBox.AddLine(b1, TRUE);
					m_ParentDlg->SaveHistory(TRUE, b3.ToString());
					_Jabber->Message(NULL, m_ParentDlg->m_User->m_JID, b3.ToString(), b2.ToString());
				}
				delete b1;

				if (m_ParentDlg->m_Room)
				{
					char room[1024];
					sprintf(room, "%s/%s", m_ParentDlg->m_Room->m_JID, m_ParentDlg->m_Room->m_Nick);

#ifndef _WODXMPPLIB
					WODXMPPCOMLib::IXMPPChatRoom *croom = NULL;
					WODXMPPCOMLib::IXMPPChatRooms *rs = NULL;
					_Jabber->m_Jabb->get_ChatRooms(&rs);
					if (rs)
					{
						CComBSTR n = room;
						VARIANT var;
						var.vt = VT_BSTR;
						var.bstrVal = n;
						rs->get_Room(var, &croom);
					}
#else
					void *croom = NULL;
					WODXMPPCOMLib::XMPP_GetChatRoomByName(_Jabber->m_Jabb, room, &croom);
#endif

					if (croom)
						_Jabber->ChatRoomMessage(croom, b3.ToString(), b2.ToString());

#ifndef _WODXMPPLIB
					if (croom)
						croom->Release();
#else
					WODXMPPCOMLib::XMPP_ChatRoom_Free(croom);
#endif
				
				}

				if (m_ParentDlg->m_User)
				{
					BOOL online = FALSE;
#ifndef _WODXMPPLIB
					WODXMPPCOMLib::IXMPPContact *cnt;
					WODXMPPCOMLib::IXMPPContacts *cnts;
					if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cnts)))
					{
						VARIANT var;
						var.vt = VT_BSTR;
						var.bstrVal = T2BSTR(m_ParentDlg->m_User->m_JID);
						if (SUCCEEDED(cnts->get_Item(var, &cnt)))
						{					
							WODXMPPCOMLib::StatusEnum stat;
							if (SUCCEEDED(cnt->get_Status(&stat)))
							{
								if (stat > /*WODXMPPCOMLib::StatusEnum::Offline*/ 0 && stat < /*WODXMPPCOMLib::StatusEnum::Requested*/ 6)
									online = TRUE;
							}
							cnt->Release();
						}
						cnts->Release();
					}
#else
					void *cnt = NULL;
					WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, m_ParentDlg->m_User->m_JID, &cnt);
					if (cnt)
					{
						WODXMPPCOMLib::StatusEnum stat;
						WODXMPPCOMLib::XMPP_Contact_GetStatus(cnt, &stat);
						if (stat > /*WODXMPPCOMLib::StatusEnum::Offline*/ 0 && stat < /*WODXMPPCOMLib::StatusEnum::Requested*/ 6)
							online = TRUE;
						WODXMPPCOMLib::XMPP_Contacts_Free(cnt);
					}
#endif

					if (!online)
					{
						m_ParentDlg->Incoming(NULL, TRUE, "User is currently offline.", NULL);
					}
				}
				if (!m_ParentDlg->m_Room)
					_Notify.DoEvent(NotificationMsgOut);


				CComBSTR b = m_ParentDlg->m_EmptyBody;
				bstr = b.Copy();
				IHTMLElement *elem;
				hr = m_htmlInputBox->get_body(&elem);
				if (SUCCEEDED(hr))
				{
					hr = elem->put_outerHTML(bstr);
					elem->Release();
				}
				else
					::SysFreeString(bstr);


			}
//			hr = htmlElement->put_innerHTML(bstr);
		} 
		else
		{
				CComBSTR b = m_ParentDlg->m_EmptyBody;
				bstr = b.Copy();
				IHTMLElement *elem;
				hr = m_htmlInputBox->get_body(&elem);
				if (SUCCEEDED(hr))
				{
					hr = elem->put_innerHTML(bstr);
					elem->Release();
				}
				else
					::SysFreeString(bstr);
		}
	}
	
	SetStyle();
	//m_ParentDlg->m_fontBackColorChanged = FALSE;

	return hr;
}

HRESULT CMsgWin::CInputBox::NotifyTyping()
{
	if (!m_ParentDlg->m_User)
		return S_OK;

#ifndef _WODXMPPLIB
	WODXMPPCOMLib::IXMPPContact *cnt;
	WODXMPPCOMLib::IXMPPContacts *cnts;
	if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cnts)))
	{
		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = T2BSTR(m_ParentDlg->m_User->m_JID);
		if (SUCCEEDED(cnts->get_Item(var, &cnt)))
		{					
			WODXMPPCOMLib::StatusEnum stat;
			if (SUCCEEDED(cnt->get_Status(&stat)))
			{
				if (stat > /*WODXMPPCOMLib::StatusEnum::Offline*/ 0 && stat < /*WODXMPPCOMLib::StatusEnum::Requested*/ 6)
				{
					VARIANT var;
					var.vt = VT_I4;
					var.lVal = 1;
					//HRESULT hr;
					/*hr = */cnt->raw_Notify((WODXMPPCOMLib::ContactNotifyEnum)1/*UserTyping*/, var);

//					cnt->Notify((WODXMPPCOMLib::ContactNotifyEnum)1/*UserTyping*/, var);
				}
			}
			cnt->Release();
		}
		cnts->Release();
	}
#else
	void *cnt = NULL;
	WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, m_ParentDlg->m_User->m_JID, &cnt);
	if (cnt)
	{
		WODXMPPCOMLib::StatusEnum stat;
		WODXMPPCOMLib::XMPP_Contact_GetStatus(cnt, &stat);
		if (stat > /*WODXMPPCOMLib::StatusEnum::Offline*/ 0 && stat < /*WODXMPPCOMLib::StatusEnum::Requested*/ 6)
		{
			VARIANT var;
			var.vt = VT_I4;
			var.lVal = 1;
			WODXMPPCOMLib::XMPP_Contact_Notify(cnt, (WODXMPPCOMLib::ContactNotifyEnum)1/*UserTyping*/, var);
		}
		WODXMPPCOMLib::XMPP_Contacts_Free(cnt);
	}
#endif

	//m_ParentDlg->m_fontBackColorChanged = FALSE;
	return S_OK;
}

void CMsgWin::CInputBox::SetFocus()
{
	if (m_ParentDlg)
	{
		if (m_ParentDlg->m_hwndIEBox)
		{
			::SendMessage(m_ParentDlg->m_hwndIEBox, WM_LBUTTONDOWN, 0, 0);
			::SendMessage(m_ParentDlg->m_hwndIEBox, WM_LBUTTONUP, 0, 0);
			::SetFocus(m_ParentDlg->m_hwndIEBox);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Window message handlers
//////////////////////////////////////////////////////////////////////


LRESULT CMsgWin::CInputBox::OnClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = TRUE;
	return 0;
}

LRESULT CMsgWin::CInputBox::OnCharacter(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_MainDlg.CheckIfAntiInactivityMessage(WM_CHAR);
//	CComQIPtr<IHTMLElement, &IID_IHTMLElement> htmlElement;
//	CMsgWin *dlg = (CMsgWin *) m_ParentDlg;

	switch (wParam) 
		{ 
			case 0x0D:  // Carriage return 				
				Send();
				bHandled = TRUE;
				return TRUE;
		}	
	bHandled = FALSE;
	return TRUE;
}


LRESULT CMsgWin::CInputBox::OnLostFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CMsgWin *d = (CMsgWin *)m_ParentDlg;

	if (IsWindow())
		SetFocus();
	bHandled = FALSE;

	return 0;
}


void CMsgWin::CInputBox::SetStyle()
{
	ChangeFontBackcolor(RGB(0,0,0), TRUE);
	
}

CMsgWin::CInsertHyperlinkDlg::CInsertHyperlinkDlg(CMsgWin *parent)
{
	m_parent = parent;
}

CMsgWin::CInsertHyperlinkDlg::~CInsertHyperlinkDlg()
{

}

BOOL CMsgWin::CInsertHyperlinkDlg::PreTranslateMessage(MSG* pMsg)
{
	_MainDlg.CheckIfAntiInactivityMessage(pMsg->message);
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMsgWin::CInsertHyperlinkDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMsgWin::CInsertHyperlinkDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	Buffer buf;
	BSTR hyp = NULL;
	BSTR desc = NULL;

	GetDlgItemText(IDC_EDITHYPERLINK, hyp);
	GetDlgItemText(IDC_EDITDESCRIPTION, desc);
	
	CString hyperlink = hyp;

	if (hyperlink.GetLength())
	{		
		CString description = desc;
		
		if (!strstr(hyperlink, "://"))
			hyperlink = "http://" + hyperlink;
		
		buf.Append("<A HREF=\"");
		buf.Append(hyperlink);
//		buf.Append("\" target=new>");
		buf.Append("\">");
		
		if (description.GetLength())
			buf.Append(description);
		else
			buf.Append(hyperlink);
		
		buf.Append("</A>");
		buf.Append("\0",1);
		
		m_parent->m_InputBox.AddHtml(CComBSTR(buf.Ptr()));
		
		EndDialog(IDOK);
		
	}
	return TRUE;
}

LRESULT CMsgWin::CInsertHyperlinkDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	return TRUE;
}

LRESULT CMsgWin::CInsertHyperlinkDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(m_parent->m_hWnd);
	return TRUE;
}

CMsgWin::CMyHTMLEditDesigner::CMyHTMLEditDesigner()
{
	m_Parent = NULL;
	m_pServices = NULL;
	m_fDirty = FALSE;
	m_uRefCount = 0;
}
CMsgWin::CMyHTMLEditDesigner::CMyHTMLEditDesigner(void *msg)
{
	m_Parent = msg;
	m_pServices = NULL;
	m_fDirty = FALSE;
	m_uRefCount = 0;
}
HRESULT STDMETHODCALLTYPE CMsgWin::CMyHTMLEditDesigner::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	HRESULT hrRet = S_OK;

	// Standard QI() initialization - set *ppv to NULL.
	*ppvObject = NULL;

	// If the client is requesting an interface we support, set *ppvObject.
	if (IsEqualIID(riid, IID_IUnknown))
		*ppvObject = (IUnknown *) this;
	else if (IsEqualIID(riid, IID_IHTMLEditDesigner))
		*ppvObject = (IHTMLEditDesigner *) this;
	else
		// We don't support the interface the client is asking for.
		hrRet = E_NOINTERFACE;

	// If we're returning an interface pointer, AddRef() it.
	if (S_OK == hrRet)
		((IUnknown *) *ppvObject)->AddRef();

	return hrRet;
}

ULONG STDMETHODCALLTYPE CMsgWin::CMyHTMLEditDesigner::AddRef(void)
{
	return ++m_uRefCount;
}

ULONG STDMETHODCALLTYPE CMsgWin::CMyHTMLEditDesigner::Release(void)
{
	return --m_uRefCount;
}

HRESULT STDMETHODCALLTYPE CMsgWin::CMyHTMLEditDesigner::PreHandleEvent(DISPID inEvtDispId, IHTMLEventObj *pIEventObj)
{
	CMsgWin *p = (CMsgWin *)m_Parent;
	
	
	// let's give our inputbox a focus
	if (p->m_hwndIEBox == NULL)
	{
		HWND hwnd = p->GetDlgItem(IDC_EDITOR);
	
		while (::GetWindow(hwnd, GW_CHILD) != NULL)
		{
			hwnd = ::GetWindow(hwnd, GW_CHILD);;
		}

		p->m_hwndIEBox = hwnd;
		
		::SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
		::SendMessage(hwnd, WM_LBUTTONUP, 0, 0);

		::InvalidateRect(p->m_hwndIEBox, NULL,TRUE);
	}
	// focus part end
		
	if (inEvtDispId == DISPID_HTMLDOCUMENTEVENTS2_ONKEYDOWN)
	{
		if (pIEventObj != NULL)
		{
			long key; 
			pIEventObj->get_keyCode(&key);
			if (key == VK_RETURN) 
			{

				p->m_InputBox.Send();
				::PostMessage(p->m_hWnd, WM_PURGEINPUTBOX, 0,0);
				return S_OK;
			}
			else
			{
				if (key>=32 && key<128)
				{
					p->m_InputBox.NotifyTyping();
				}
			}
		}
	}

	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CMsgWin::CMyHTMLEditDesigner::PostHandleEvent(DISPID inEvtDispId, IHTMLEventObj * pIEventObj)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CMsgWin::CMyHTMLEditDesigner::TranslateAccelerator(DISPID inEvtDispId, IHTMLEventObj * pIEventObj)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CMsgWin::CMyHTMLEditDesigner::PostEditorEventNotify(DISPID inEvtDispId, IHTMLEventObj * pIEventObj)
{
		
	
	return S_FALSE;
}

CMsgWin::CMyHTMLEditDesigner::CMSHTMLDisableDragHTMLEditDesigner()
{
	m_pServices = (IHTMLEditServices *) NULL;
	m_uRefCount = 0;
}

BOOL CMsgWin::CMyHTMLEditDesigner::Attach(IHTMLDocument2 *pDoc)
{
	if (m_pServices	!= (IHTMLEditServices *) NULL)
		m_pServices->Release();

	IServiceProvider *pTemp;

	if (pDoc ==	(IHTMLDocument2	*) NULL)
		return FALSE;

	pDoc->QueryInterface(IID_IServiceProvider, (void **) &pTemp);

	if (pTemp != (IServiceProvider *) NULL)
	{
		pTemp->QueryService(SID_SHTMLEditServices, IID_IHTMLEditServices, (void	**)	&m_pServices);

		if (m_pServices	!= (IHTMLEditServices *) NULL)
		{
			m_pServices->AddDesigner(this);
			return TRUE;
		}
	}

	return FALSE;
}

void CMsgWin::CMyHTMLEditDesigner::Detach()
{
	if (m_pServices != (IHTMLEditServices *) NULL)
		m_pServices->RemoveDesigner(this);
}

LRESULT CMsgWin::OnMeasureItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPMEASUREITEMSTRUCT lp = (LPMEASUREITEMSTRUCT)lParam;

//	if (lp->itemID >= _MainDlg.m_EmoticonsInstance.m_Image.size())
//		return FALSE;

//	lp->itemWidth = _MainDlg.m_EmoticonsInstance.m_Image[lp->itemID]->GetWidth();
//	lp->itemHeight = _MainDlg.m_EmoticonsInstance.m_Image[lp->itemID]->GetHeight();

	lp->itemWidth = 32;
	lp->itemHeight = 32;
	
	return TRUE;
}

LRESULT CMsgWin::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPDRAWITEMSTRUCT lp = (LPDRAWITEMSTRUCT)lParam;

	HBRUSH hb = (HBRUSH)((lp->itemState & ODS_SELECTED)? CreateSolidBrush(RGB(0,0,128)) : CreateSolidBrush(RGB(255,255,255)));
	::FillRect(lp->hDC, &lp->rcItem, hb);
	DeleteObject(hb);

	if (lp->itemID >= _MainDlg.m_EmoticonsInstance.m_Image.size())
		return FALSE;
	
	long xpos, ypos, xwidth, xheight;

	_CalcRect(_MainDlg.m_EmoticonsInstance.m_Image[lp->itemID]->head.biWidth, _MainDlg.m_EmoticonsInstance.m_Image[lp->itemID]->head.biHeight, 
		lp->rcItem.right - lp->rcItem.left, lp->rcItem.bottom - lp->rcItem.top,
		&xpos, &ypos, &xwidth, &xheight, 30);
	
	_MainDlg.m_EmoticonsInstance.m_Image[lp->itemID]->Draw(lp->hDC, lp->rcItem.left + xpos + 1, lp->rcItem.top + ypos + 1, xwidth, xheight, NULL, FALSE);

	return TRUE;
}


//	CResiEditDispatch
/*HRESULT STDMETHODCALLTYPE CMSHTMLDisableDragDispatch::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	HRESULT hrRet = S_OK;

	// Standard QI() initialization - set *ppv to NULL.
	*ppvObject = NULL;

	// If the client is requesting an interface we support, set *ppv.
	if (IsEqualIID(riid, IID_IUnknown))
		*ppvObject = (IUnknown *) this;
	else if (IsEqualIID(riid, IID_IDispatch))
		*ppvObject = (IDispatch *) this;
	else
		// We don't support the interface the client is asking for.
		hrRet = E_NOINTERFACE;

	// If we're returning an interface pointer, AddRef() it.
	if (S_OK == hrRet)
		((IUnknown *) *ppvObject)->AddRef();

	return hrRet;
}

ULONG STDMETHODCALLTYPE CMSHTMLDisableDragDispatch::AddRef(void)
{
	return ++m_uRefCount;
}

ULONG STDMETHODCALLTYPE CMSHTMLDisableDragDispatch::Release(void)
{
	return --m_uRefCount;
}

HRESULT STDMETHODCALLTYPE CMSHTMLDisableDragDispatch::GetTypeInfoCount(UINT *pctinfo)
{
	*pctinfo = 0;
	return S_OK;
}
*/
//HRESULT STDMETHODCALLTYPE CMSHTMLDisableDragDispatch::GetTypeInfo(UINT /*iTInfo*/, LCID /*lcid*/, ITypeInfo ** /*ppTInfo*/)
/*{
	return DISP_E_BADINDEX;
}
*/
//HRESULT STDMETHODCALLTYPE CMSHTMLDisableDragDispatch::GetIDsOfNames(REFIID /*riid*/, LPOLESTR * /*rgszNames*/, UINT /*cNames*/, LCID /*lcid*/, DISPID * /*rgDispId*/)
/*{
	return DISP_E_UNKNOWNNAME;
}
*/
//HRESULT STDMETHODCALLTYPE CMSHTMLDisableDragDispatch::Invoke(DISPID /*dispIdMember*/, REFIID /*riid*/, LCID /*lcid*/, WORD /*wFlags*/, DISPPARAMS * /*pDispParams*/, VARIANT *pVarResult, EXCEPINFO * /*pExcepInfo*/, UINT * /*puArgErr*/)
/*{
	//	If we were installed it means we should disable
	//	dragging. So set the return value to false
	pVarResult->vt = VT_BOOL;
	pVarResult->boolVal = false;
	return S_FALSE;
}
*/

/*LRESULT CMsgWin::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((HWND)lParam == m_EmoticonList->m_hWnd)
	{
		switch (HIWORD(wParam))
		{
			case LBN_DBLCLK:
				{
					int i = ::SendMessage(m_EmoticonList->m_hWnd, LB_GETCURSEL, 0, 0);
					if (i != LB_ERR)
					{
						char *t = NULL;
						_MainDlg.m_EmoticonsInstance.GetText(i, &t);
						if (t)
						{
							m_EmoticonList->ShowWindow(SW_HIDE);
							::ShowWindow(m_EmoticonList->m_BalloonHwnd, SW_HIDE);
							CComBSTR b = t;
							m_InputBox.AddHtml(b);
						}
					}
				}
				break;
		}
	}

	bHandled = FALSE;
	return FALSE;
}
*/
/*LRESULT CMsgWin::OnEmoticonCaptureKeyCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_EmoticonListCapture)
	{
		if (wParam == VK_ESCAPE)
		{
			m_EmoticonListCapture = FALSE;
			ReleaseCapture();
		}

	}
	bHandled = FALSE;
	return FALSE;
}

LRESULT CMsgWin::OnEmoticonCaptureMouseCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_EmoticonListCapture)
	{
		RECT rc;
		::GetWindowRect(m_EmoticonList->m_hWnd, &rc);
		
		POINT p;
		GetCursorPos(&p);
		if (p.x<rc.left || p.x>rc.right || p.y<rc.top || p.y>rc.bottom)
		{
			m_EmoticonListCapture = FALSE;
			ReleaseCapture();
		}
	}
	bHandled = FALSE;
	return FALSE;
}
*/
CMsgWin::CWEmoticon::CWEmoticon(CMsgWin *Owner)
{
	m_Owner = Owner;
	m_BalloonHwnd = NULL;
	m_LastBalloon = -1;
	m_Image = NULL;
	m_Font = NULL;
//	m_EmoticonListCapture = FALSE;
}
CMsgWin::CWEmoticon::~CWEmoticon()
{
	if (m_Font)
		DeleteObject(m_Font);
}

LRESULT CMsgWin::CWEmoticon::OnActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == WA_INACTIVE)
	{
		if (lParam != (LPARAM)m_BalloonHwnd )
		{
//			m_Owner->SetActiveWindow();
			::ShowWindow(m_hWnd, SW_HIDE);
			::ShowWindow(m_BalloonHwnd, SW_HIDE);
		}
	}
	bHandled = FALSE;
	return FALSE;
}


LRESULT CALLBACK CMsgWin::CWEmoticon::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CWEmoticon *em = (CWEmoticon*)::GetWindowLong(hWnd, GWL_USERDATA);
	switch (message)
	{
		case WM_CREATE:
			break;

		case WM_PAINT:
			{
				if (em->m_Image)
				{
					if (em->m_BalloonHwnd == hWnd)
					{
						CPaintDC dcPaint(hWnd);
						HGDIOBJ oldfont = ::SelectObject(dcPaint.m_hDC, em->m_Font);
	
						HBRUSH hbr = CreateSolidBrush(RGB(255,255,255));
						SIZE rect, text;
						em->CalcBalloonRect(dcPaint.m_hDC, &rect, &text);
						rect.cx+=5;
						rect.cy+=5;
						RECT rc;
						memset(&rc, 0, sizeof(rc));
						rc.bottom = rect.cy;
						rc.right = rect.cx;
						dcPaint.Rectangle(&rc);
						rc.left++;rc.right--;rc.top++;rc.bottom--;
						dcPaint.FillRect(&rc, hbr);
						DeleteObject(hbr);
						em->m_Image->Draw(dcPaint.m_hDC, (rc.right - em->m_Image->GetWidth())/2 + 1, 1, -1, -1, NULL, FALSE);		

						char *str = NULL;
						_MainDlg.m_EmoticonsInstance.GetText(em->m_LastBalloon, &str);			

//						::SetTextAlign(dcPaint.m_hDC, TA_CENTER);
						//ExtTextOut(dcPaint.m_hDC, rc.right/2+1, rc.bottom - text.cy, 0, NULL, str, strlen(str), NULL);
						rc.top += em->m_Image->GetHeight();
						DrawText(dcPaint.m_hDC, str, strlen(str), &rc, DT_CENTER);

						::SelectObject(dcPaint.m_hDC, oldfont);
					}
				}
			}
			return TRUE;

		case WM_LBUTTONDOWN:
			{
				int i = em->m_LastBalloon; /*::SendMessage(em->m_Owner->m_EmoticonList->m_hWnd, LB_GETCURSEL, 0, 0);*/
				if (i != /*LB_ERR*/ -1)
				{
					if (!_MainDlg.m_EmoticonsInstance.IsBig && i == _MainDlg.m_EmoticonsInstance.GetCount()-1)
					{
						::ShowWindow(em->m_Owner->m_EmoticonList->m_hWnd, SW_HIDE);
						::ShowWindow(em->m_Owner->m_EmoticonList->m_BalloonHwnd, SW_HIDE);

//						CBalloonTipDlg dlg(em->m_Owner->m_hWnd);
//						dlg.SetText("New emoticons will be downloaded and installed. Proceed?");
//						dlg.DoModal();
//						int j = 0;
						int j = ::MessageBox(NULL, "New emoticons will be downloaded and installed. Proceed?", "New emoticons", MB_ICONQUESTION | MB_YESNO);
						if (j == 6)
						{
							CProgressDlg *dlg = new CProgressDlg();
							dlg->Create(NULL);
							dlg->CenterWindow();
							dlg->ShowWindow(SW_SHOW);
							Buffer p1;
							char bf[128];
							sprintf(bf, "/download/Emotico2.dll.gz");
							dlg->m_Total = ((float)1.11 * 1024 * 1024);
							dlg->DownloadFile(FALSE, bf, &p1, TRUE);

							if (p1.Len()>0)
							{
								Buffer p2;
								z_stream z_str;
								memset(&z_str,0,sizeof(z_stream));
								inflateInit(&z_str);
								while (p1.Len())
								{
									int l = p1.Len();
									if (l>4096)
										l = 4096;
									BufferUncompress(z_str, p1.Ptr(), l, &p2);
									p1.Consume(l);
								}
								inflateEnd(&z_str);

								if (p2.Len())
								{
									char buff[MAX_PATH];
									strcpy(buff, _Settings.m_MyPath);
									strcat(buff, "Emotico2.dll");

									int handle = open(buff, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
									if (handle != (-1))
									{
										do
										{
											int l = p2.Len();
											if (l>65535)
												l = 65535;
											write(handle, p2.Ptr(), l);
											p2.Consume(l);
										} while (p2.Len());
										close(handle);

										em->m_Owner->m_ImagesLoaded = FALSE;
										em->m_Owner->m_ListLoaded = FALSE;
										while (_MainDlg.m_EmoticonsInstance.m_Image.size())
										{
											CxImage *img = _MainDlg.m_EmoticonsInstance.m_Image[0];
											_MainDlg.m_EmoticonsInstance.m_Image.erase(_MainDlg.m_EmoticonsInstance.m_Image.begin());
											delete img;										
										}

										while (::SendMessage(em->m_Owner->m_EmoticonList->m_hWnd, LB_GETCOUNT, NULL, NULL))
										::SendMessage(em->m_Owner->m_EmoticonList->m_hWnd, LB_DELETESTRING, 0, NULL);

										_MainDlg.ReloadEmoticons(FALSE);

										::MessageBox(NULL, "You can now click again on emoticon button to see new installed emoticons.", "Done!", MB_ICONASTERISK | MB_OK);
									}
								}
							}
							dlg->DestroyWindow();
							delete dlg;
						}
					}
					else
					{
						char *t = NULL;
						if (_MainDlg.m_EmoticonsInstance.GetHtml)
							_MainDlg.m_EmoticonsInstance.GetHtml(i, &t);
						else
							_MainDlg.m_EmoticonsInstance.GetText(i, &t);
						if (t)
						{
//							em->m_Owner->SetActiveWindow();
							//::SetWindowPos(em->m_Owner->m_EmoticonList->m_BalloonHwnd, HWND_BOTTOM, 0, 0, 0, 0, 0);
							IHTMLSelectionObject *p = NULL;
							em->m_Owner->m_InputBox.m_htmlInputBox->get_selection(&p);
							if (p)
							{
								CComQIPtr<IHTMLTxtRange, &IID_IHTMLTxtRange> htmlTxtRange;		

								HRESULT hr = p->createRange((IDispatch **)&htmlTxtRange);

								if (SUCCEEDED(hr))
								{						
									CComBSTR text = t;
									htmlTxtRange->pasteHTML(text);
//								htmlTxtRange->moveEnd(
								}
							}
						}
					}
					em->m_Owner->SetActiveWindow();
				}
			}
			return TRUE;


		case WM_ACTIVATE:
			if (wParam == WA_INACTIVE)
			{
				if (lParam != (LPARAM)em->m_hWnd)
				{
//					em->m_Owner->SetActiveWindow();
					::ShowWindow(em->m_hWnd, SW_HIDE);
					::ShowWindow(em->m_BalloonHwnd, SW_HIDE);
				}
			}
			return FALSE;

		case WM_MOUSEMOVE:
			{
				BOOL bh;
				RECT me, own;
				int xPos = GET_X_LPARAM(lParam); 
				int yPos = GET_Y_LPARAM(lParam); 
				::GetWindowRect(em->m_BalloonHwnd, &me);
				::GetWindowRect(em->m_hWnd, &own);				
				return em->OnMouseMove(WM_MOUSEMOVE, wParam, MAKELPARAM(xPos + (me.left - own.left), yPos + (me.top - own.top)), bh);
			}
			break;

/*		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			::ShowWindow(em->m_hWnd, SW_HIDE);
			::ShowWindow(em->m_BalloonHwnd, SW_HIDE);
			em->m_Owner->SendMessage(message, wParam, lParam);
			break;
*/
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void CMsgWin::CWEmoticon::CalcBalloonRect(HDC dc, SIZE *rect, SIZE *text)
{
	rect->cx = m_Image->GetWidth();
	if (rect->cx<32)
		rect->cx= 32;
	rect->cy= m_Image->GetHeight();

	if (!m_Font)
	{
		int lfh = -MulDiv(8, GetDeviceCaps(dc, LOGPIXELSY), 72);
		m_Font = CreateFont(lfh , 0, 0, 0, FW_BOLD, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial");
	}


	HGDIOBJ oldfont = ::SelectObject(dc, m_Font);
	char *str = NULL;
	_MainDlg.m_EmoticonsInstance.GetText(m_LastBalloon, &str);			

//
	RECT rc = {0};
	if (!str)
		str = "";
	DrawText(dc, str, strlen(str), &rc, DT_CALCRECT | DT_CENTER);

	text->cx = rc.right;
	text->cy = rc.bottom;

//	GetTextExtentPoint32(dc, str, strlen(str), text);
	::SelectObject(dc, oldfont);
	if (rect->cx<text->cx)
		rect->cx= text->cx;

	rect->cy+=text->cy;

	if (rect->cy<32)
		rect->cy= 32;
}

LRESULT CMsgWin::CWEmoticon::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_MainDlg.CheckIfAntiInactivityMessage(WM_CHAR);
	static const char *classname = "CWEmoticonWindowClass";

	// is class registred?
	if (!_MainDlg.m_EmoticonsInstance.ClassRegistered)
	{
	    WNDCLASS wndclass;
        memset(&wndclass, 0, sizeof(wndclass));
        wndclass.lpfnWndProc = WindowProc;
        wndclass.hInstance   = _Module.GetModuleInstance();
        wndclass.lpszClassName = classname;
		wndclass.cbWndExtra  = 4;
        RegisterClass(&wndclass);
        _MainDlg.m_EmoticonsInstance.ClassRegistered = TRUE;
	}
	// is window created?
	if (!m_BalloonHwnd)
	{
		m_BalloonHwnd = CreateWindowEx(WS_EX_TOOLWINDOW, classname, NULL,WS_POPUP | WS_CHILD,0,0,0,0,/*m_hWnd*/NULL,NULL,_Module.GetModuleInstance(),NULL);
		if(m_BalloonHwnd) ::SetWindowLong(m_BalloonHwnd, GWL_USERDATA, (LONG)this);
	}

	int xPos = GET_X_LPARAM(lParam); 
	int yPos = GET_Y_LPARAM(lParam); 

	// let's see item from point
	int bal = ::SendMessage(m_Owner->m_EmoticonList->m_hWnd, LB_ITEMFROMPOINT, NULL, lParam);
	if (HIWORD(bal))
	{
		bal = -1;
	}
	RECT rc;
	GetWindowRect(&rc);
//	ATLTRACE("Balloon over %d\r\n", bal);
	if (bal != m_LastBalloon)
	{
		m_LastBalloon = bal;

		if (m_LastBalloon == (-1))
		{
			m_Image = NULL;
			::ShowWindow(m_BalloonHwnd, SW_HIDE);
		}
		else
		{
			m_Image = _MainDlg.m_EmoticonsInstance.m_Image[m_LastBalloon];
			// let's show it
			// calculate position
			int gettop = ::SendMessage(m_Owner->m_EmoticonList->m_hWnd, LB_GETTOPINDEX, NULL, NULL);
			int percol = ::SendMessage(m_Owner->m_EmoticonList->m_hWnd, LB_GETLISTBOXINFO, NULL, NULL);
			int xp = (m_LastBalloon-gettop)/percol * 32;
			int yp = (m_LastBalloon-gettop)%percol * 32;

			HDC dc = ::GetDC(m_BalloonHwnd);
			SIZE rcbal, text;
			CalcBalloonRect(dc, &rcbal, &text);
			::ReleaseDC(m_BalloonHwnd, dc);

			::SetWindowPos(m_BalloonHwnd, HWND_TOP, xp + rc.left, yp+ rc.top, rcbal.cx+5, rcbal.cy+5, SWP_NOACTIVATE);
			::ShowWindow(m_BalloonHwnd, SW_SHOW);
			::InvalidateRect(m_BalloonHwnd, NULL, TRUE);
//			ATLTRACE("m_LastBalloon = %d\r\n", m_LastBalloon);
		}
	}
	return FALSE;
}
