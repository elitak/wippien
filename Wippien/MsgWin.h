// MsgWin.h: interface for the CMsgWin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWIN_H__6CADB9EB_B4D2_43A6_8623_5C58F4E450C1__INCLUDED_)
#define AFX_MSGWIN_H__6CADB9EB_B4D2_43A6_8623_5C58F4E450C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "PNGButton.h"
//#include "SettingsDlg.h"

#include "cwodWinsocket.h"
//#define WM_PURGEINPUTBOX			WM_USER+16
#define MINWIDTH 100
#define MINHEIGHT 200
#define SNAPWIDTH 30
 
//#define isClose(x, y)  (abs((x)-(y)) < SNAPWIDTH)

extern BOOL isClose(int x, int y);


class CUser;
class Buffer;
class wWebEvents;

class CMsgWin : public CAxDialogImpl<CMsgWin>, 
		public CMessageFilter, public CIdleHandler, public CDialogResize<CMsgWin>
{
public:
	enum { IDD = IDD_MSGDLG };
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();


	CMsgWin(CUser *m_Owner, BOOL IsMultiChat);
	virtual ~CMsgWin();
	CContainedWindow /*m_List, m_Edit, */m_Button;
	CUser *m_User;
	BOOL m_IsMultiChat;

	BOOL ArrangeLayout();
	BOOL Show(void);
	BOOL Incoming(BOOL IsSystem, char *Text, char *Html);
	BOOL SaveHistory(BOOL Mine, char *Text);
	BOOL AddTimestamp(Buffer *b);
	BOOL LoadHistory(Buffer *out);
	char m_HistoryPath[MAX_PATH*2];
	BSTR m_EmptyBody;

	HBRUSH m_BackBrush;
	int m_MoveX, m_MoveY, m_SizeX, m_SizeY;
	//Buffer *m_HumanHead;
	CxImage m_HumanHead;
	DWORD m_ThreadID;
	HWND m_hwndIEBox;
	HPEN m_hSeparatorPen;
	BOOL m_ImagesLoaded, m_ListLoaded;
//	IMAGELIST m_Image;

	// Send button
	CPNGButton m_btnSend;

	// main toolbar buttons
	CPNGButton m_btnSendFile, m_btnDetails, m_btnMuteOnOff,m_btnClearHistory;

	// middle toolbar buttons
	CPNGButton m_btnBold, m_btnItalic, m_btnUnderline, m_btnHyperlink, m_btnFontSizeDown, m_btnEmoticons, 
		m_btnFontSizeNormal, m_btnFontSizeUp, m_btnFontForecolor, m_btnFontBackcolor, m_btnHumanHead;
	
	//void ShowEditText(void);
	//Buffer *m_Text;

	class CWEmoticon: public CWindowImpl< CWindow>  
	{
	public:
		CWEmoticon(CMsgWin *Owner);
		virtual ~CWEmoticon();

		BEGIN_MSG_MAP(CWEmoticon)
			// for emoticon capture
//			MESSAGE_HANDLER(WM_LBUTTONDOWN, OnEmoticonCaptureMouseCommand)
//			MESSAGE_HANDLER(WM_RBUTTONDOWN, OnEmoticonCaptureMouseCommand)
//			MESSAGE_HANDLER(WM_LBUTTONUP, OnEmoticonCaptureMouseCommand)
//			MESSAGE_HANDLER(WM_RBUTTONUP, OnEmoticonCaptureMouseCommand)
//			MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnEmoticonCaptureMouseCommand)
//			MESSAGE_HANDLER(WM_KEYDOWN, OnEmoticonCaptureKeyCommand)
			MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
			MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
			REFLECT_NOTIFICATIONS()
		END_MSG_MAP()

		HWND m_BalloonHwnd;
		int m_LastBalloon;
		CMsgWin *m_Owner;
		CxImage *m_Image;
		HFONT m_Font;

		void CalcBalloonRect(HDC dc, SIZE *rect, SIZE *text);


//		LRESULT OnEmoticonCaptureMouseCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
//		LRESULT OnEmoticonCaptureKeyCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
		LRESULT OnActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
		LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);	
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	CWEmoticon *m_EmoticonList;


	class CChatBox : public CWindowImpl< CWindow>  
	{
	public:
		CChatBox();
		virtual ~CChatBox();
		HRESULT AddHtml(BSTR HtmlCode);
		BOOL AddLine(Buffer *Line, BOOL islocal);
		Buffer m_AddHtml;

		CMsgWin *m_ParentDlg;
		BOOL m_fDocWindow;
		BOOL Init(CMsgWin *dlg);
		BOOL Free(void);
		void SetDefMargin(void);
//		CComPtr<IWebBrowser2> m_wb2ChatBox;
//		CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> m_htmlChatBox;
		IWebBrowser2 *m_wb2ChatBox;
		IHTMLDocument2 *m_htmlChatBox;
		wWebEvents *m_Events;

		void RedrawTools(void);

		BEGIN_MSG_MAP(CChatBox)
			MESSAGE_HANDLER(WM_PARENTNOTIFY, OnParentNotify)
			MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
			MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
			REFLECT_NOTIFICATIONS()
		END_MSG_MAP()

		LRESULT OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnParentNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	};

	class CMyHTMLEditDesigner : public IHTMLEditDesigner
	{
	public:
		CMyHTMLEditDesigner();
		CMyHTMLEditDesigner(void *msg);
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
		virtual ULONG	STDMETHODCALLTYPE AddRef(void);
		virtual ULONG	STDMETHODCALLTYPE Release(void);

		virtual HRESULT STDMETHODCALLTYPE PreHandleEvent(DISPID inEvtDispId, IHTMLEventObj *pIEventObj);
		virtual HRESULT STDMETHODCALLTYPE PostHandleEvent(DISPID inEvtDispId, IHTMLEventObj *pIEventObj);
		virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(DISPID inEvtDispId, IHTMLEventObj *pIEventObj);
		virtual HRESULT STDMETHODCALLTYPE PostEditorEventNotify(DISPID inEvtDispId, IHTMLEventObj *pIEventObj);

						CMSHTMLDisableDragHTMLEditDesigner();

		BOOL			Attach(IHTMLDocument2 *pDoc);
		void			Detach();

	private:
		void *m_Parent;
		IHTMLEditServices *m_pServices;
		UINT			m_uRefCount;
		BOOL m_fDirty;
	//	CMSHTMLDisableDragDispatch m_dp;
	};
	class CInputBox : public CWindowImpl< CWindow>  
	{
	public:
		void SetStyle();
		CInputBox();
		virtual ~CInputBox();
		HRESULT ChangeFontSize(signed int dwDelta);
		HRESULT ChangeFontForecolor(COLORREF fgColor);
		HRESULT ChangeFontBackcolor(COLORREF bgColor, BOOL bTransparent = FALSE);
		HRESULT _ToggleHtmlCommand(BSTR Command);
		HRESULT ToggleBold();
		HRESULT ToggleItalic();
		HRESULT ToggleUnderline();
		HRESULT Send(void);
		HRESULT NotifyTyping(void);
		HRESULT AddHtml(BSTR HtmlCode);
		void SetFocus();
		BOOL Free(void);

		CMsgWin *m_ParentDlg;
		HWND m_hwndLastFocus;
		BOOL m_fDocWindow;
		CString m_fontBackColor;
		CString m_fontForeColor;
		int	m_fontSize;
		CMyHTMLEditDesigner *m_htmlEditDesigner;
		BOOL Init(CMsgWin *dlg);
		IWebBrowser2 *m_wb2InputBox;
		IHTMLDocument2* m_htmlInputBox;

		BEGIN_MSG_MAP(CInputBox)
			MESSAGE_HANDLER(WM_PARENTNOTIFY, OnParentNotify)
			MESSAGE_HANDLER(WM_RBUTTONUP, OnClick)
			MESSAGE_HANDLER(WM_KILLFOCUS, OnLostFocus)
			MESSAGE_HANDLER(WM_CHAR, OnCharacter)
		END_MSG_MAP()

		LRESULT OnClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		LRESULT OnParentNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnCharacter(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnLostFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	};	

	CInputBox m_InputBox;
	CChatBox m_ChatBox;



	BEGIN_MSG_MAP(CMsgWin)
//	ATLTRACE("Main %x\r\n", uMsg);
		MESSAGE_HANDLER(WM_PURGEINPUTBOX, OnPurgeInputBox)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSizeMove)
	//	MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	//	MESSAGE_HANDLER(WM_SETFOCUS, OnGotFocus)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDB_SEND, OnSendButtonClicked)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
		MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
//		MESSAGE_HANDLER(WM_COMMAND, OnCommand)

		// for emoticon capture
//		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnEmoticonCaptureMouseCommand)
//		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnEmoticonCaptureMouseCommand)
//		MESSAGE_HANDLER(WM_LBUTTONUP, OnEmoticonCaptureMouseCommand)
//		MESSAGE_HANDLER(WM_RBUTTONUP, OnEmoticonCaptureMouseCommand)
//		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnEmoticonCaptureMouseCommand)
//		MESSAGE_HANDLER(WM_KEYDOWN, OnEmoticonCaptureKeyCommand)

//		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
//		MESSAGE_HANDLER(WM_PAINT, OnPaint)
//		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
//		COMMAND_ID_HANDLER(IDAPPLY, OnApplyCmd)
		COMMAND_ID_HANDLER(IDC_PERSON, OnHumanHead)
		COMMAND_ID_HANDLER(IDC_BOLD, OnBoldBtnClicked)
		COMMAND_ID_HANDLER(IDC_ITALIC, OnItalicBtnClicked)
		COMMAND_ID_HANDLER(IDC_UNDERLINE, OnUnderlineBtnClicked)
		COMMAND_ID_HANDLER(IDC_HYPERLINK, OnHyperlinkBtnClicked)
		COMMAND_ID_HANDLER(IDC_EMOTICONS, OnEmoticonBtnClicked)
		COMMAND_ID_HANDLER(IDC_FONTSIZEDOWN, OnFontSizeDownBtnClicked)
		COMMAND_ID_HANDLER(IDC_FONTSIZENORMAL, OnFontSizeNormalBtnClicked)
		COMMAND_ID_HANDLER(IDC_FONTSIZEUP, OnFontSizeUpBtnClicked)
		COMMAND_ID_HANDLER(IDC_FONTFORECOLOR, OnFontForecolorBtnClicked)
		COMMAND_ID_HANDLER(IDC_FONTBACKCOLOR, OnFontBackcolorBtnClicked)
		COMMAND_ID_HANDLER(ID_PNG1_DETAILS, OnBtnDetails)
		COMMAND_ID_HANDLER(ID_PNG1_CLEARHISTORY, OnBtnClearHistory)
		COMMAND_ID_HANDLER(ID_PNG1_MUTEONOFF, OnBtnMuteOnOff)
		CHAIN_MSG_MAP(CDialogResize<CMsgWin>)
/*	ALT_MSG_MAP(1)	
#ifdef _DEBUGWNDMSG
	ATLTRACE("ALT 1 %x\r\n", uMsg);
#endif
		MESSAGE_HANDLER(WM_CHAR, OnButtonChar)
//		MESSAGE_HANDLER(WM_SETFOCUS, OnGotFocus)
//		MESSAGE_HANDLER(WM_CHAR, OnEditChar)
	ALT_MSG_MAP(2)	
#ifdef _DEBUGWNDMSG
	ATLTRACE("ALT 2 %x\r\n", uMsg);
#endif
		MESSAGE_HANDLER(WM_CHAR, OnEditChar)
//		MESSAGE_HANDLER(WM_PAINT, OnEditPaint)
*/
	ALT_MSG_MAP(1)
//	ATLTRACE("ALT 3 %x\r\n", uMsg);
//		MESSAGE_HANDLER(WM_CHAR, OnButtonChar)
//		MESSAGE_HANDLER(WM_SETFOCUS, OnGotFocus)
		COMMAND_ID_HANDLER(IDB_SEND, OnSendButtonClicked)

		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()


	BEGIN_DLGRESIZE_MAP(CMsgWin)
        DLGRESIZE_CONTROL(IDC_BOLD, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_ITALIC, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_UNDERLINE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_HYPERLINK, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_EMOTICONS, DLSZ_MOVE_Y)
//		DLGRESIZE_CONTROL(IDC_CHOOSEEMOTICON, DLSZ_MOVE_Y | DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_FONTSIZEDOWN, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_FONTSIZENORMAL, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_FONTSIZEUP , DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_FONTFORECOLOR, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_FONTBACKCOLOR, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_EDITOR, DLSZ_MOVE_Y | DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_LIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_MSGWIN_USERS, DLSZ_MOVE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDB_SEND, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_PERSON, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_ISTYPING, DLSZ_MOVE_X | DLSZ_MOVE_Y)
    END_DLGRESIZE_MAP()
// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSendButtonClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnGotFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT OnEditPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT OnEditChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT OnButtonChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPurgeInputBox(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
			CComBSTR bstr, b = m_EmptyBody;
			bstr = b;
			IHTMLElement *elem;
			HRESULT hr = m_InputBox.m_htmlInputBox->get_body(&elem);
			if (SUCCEEDED(hr))
			{
				hr = elem->put_innerHTML(bstr);
				hr = elem->put_outerHTML(bstr);
				elem->Release();
			}
		return TRUE;
	}
//	LRESULT OnEmoticonCaptureMouseCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
//	LRESULT OnEmoticonCaptureKeyCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnMoving(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
//	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnEnterSizeMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizing(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
//	LRESULT OnApplyCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHumanHead(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBoldBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnItalicBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnUnderlineBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHyperlinkBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEmoticonBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFontSizeDownBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFontSizeNormalBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFontSizeUpBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFontForecolorBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFontBackcolorBtnClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnDetails(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnClearHistory(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnMuteOnOff(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT	OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT	OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);


	class CInsertHyperlinkDlg: public CDialogImpl<CInsertHyperlinkDlg>, public CUpdateUI<CInsertHyperlinkDlg>,
			public CMessageFilter, public CIdleHandler  
	{
	public:
		enum { IDD = IDD_INSERTHYPERLINKDLG };


		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual BOOL OnIdle();

		

		BEGIN_UPDATE_UI_MAP(CInsertHyperlinkDlg)
		END_UPDATE_UI_MAP()

		BEGIN_MSG_MAP(CInsertHyperlinkDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_HANDLER(IDOK, BN_CLICKED, OnOK)
			COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnCancel)
		END_MSG_MAP()
		
		CInsertHyperlinkDlg(CMsgWin *parent);
		virtual ~CInsertHyperlinkDlg();
		
		CMsgWin *m_parent;
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	};
};

#endif // !defined(AFX_MSGWIN_H__6CADB9EB_B4D2_43A6_8623_5C58F4E450C1__INCLUDED_)
