// BalloonTipDlg.h: interface for the CBalloonTipDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BalloonTipDLG_H__CCE3E578_00C2_4F1A_B587_76077D630052__INCLUDED_)
#define AFX_BalloonTipDLG_H__CCE3E578_00C2_4F1A_B587_76077D630052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "BalloonDialog.h"
#include "PNGButton.h"
#include "WebBrowserEvents.h"

extern _ATL_FUNC_INFO BaloonTipBeforeNavigate2Info;


class CBalloonTipDlg : public CAxDialogImpl<CBalloonTipDlg>, public CBalloonDialog<CBalloonTipDlg>
{
public:
	enum { IDD = IDD_BALLOONTIP };

	CBalloonTipDlg(HWND OwnerHwnd);
	virtual ~CBalloonTipDlg();

	HWND m_OwnerHwnd;
	CStatic m_wndTitle;
	CFont m_fontTitle;

	int m_Result, m_Type;
	BOOL m_Down;
	POINT m_DownPoint;

	CPNGButton m_btn1, m_btn2, m_btn3;
	CWebBrowserEvents<CBalloonTipDlg, &BaloonTipBeforeNavigate2Info> *m_Events;

	IWebBrowser2 *m_pWB2;
	CComBSTR m_Text, m_Caption;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	void SetText(char *Text);
	void SetCaption(char *Text);
	void SetType(int Type);

	static int Show(HWND hWnd, char *Text, char *Caption, int Type);
	void WriteText(void);
	void DrawType(void);

	BEGIN_MSG_MAP(CBalloonTipDlg)
//		ATLTRACE("Msg = %x\r\n", uMsg);
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
//		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		if( lParam==(LPARAM)(HWND)m_wndTitle) MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnWhiteLabelCtlColorStatic)
		CHAIN_MSG_MAP( CBalloonDialog<CBalloonTipDlg> )
		REFLECT_NOTIFICATIONS()
		ALT_MSG_MAP(1)
			CHAIN_MSG_MAP_ALT( CBalloonDialog<CBalloonTipDlg>, 1 )
	END_MSG_MAP()

	BEGIN_SINK_MAP(CBalloonTipDlg)
	END_SINK_MAP()


   LRESULT OnWhiteLabelCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CDCHandle dc( (HDC) wParam );
      dc.SetTextColor(RGB(0,0,200));
      dc.SetBkColor(RGB(255,255,255)),
      dc.SetBkMode(TRANSPARENT);
      return (LRESULT) (HBRUSH) ::GetStockObject(WHITE_BRUSH);
   }

   LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      // TODO: Add validation code 
	  m_Result = IDYES;
      EndDialog(wID);
      return 0;
   }
   LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      EndDialog(wID);
      return 0;
   }

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
//	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
//	LRESULT OnRegisterCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // !defined(AFX_BalloonTipDLG_H__CCE3E578_00C2_4F1A_B587_76077D630052__INCLUDED_)
