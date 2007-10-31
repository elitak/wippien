// DownloadSkinDlg.h: interface for the CDownloadSkinDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADSKINDLG_H__CCE3E578_00C2_4F1A_B587_76077D630052__INCLUDED_)
#define AFX_DOWNLOADSKINDLG_H__CCE3E578_00C2_4F1A_B587_76077D630052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "WebBrowserEvents.h"


extern _ATL_FUNC_INFO SkinDlgBeforeNavigate2Info;



class CDownloadSkinDlg : public CAxDialogImpl<CDownloadSkinDlg>, public CMessageFilter, public CIdleHandler

{
public:
	enum { IDD = IDD_DOWNLOADSKIN };

	CDownloadSkinDlg(HWND Owner);
	virtual ~CDownloadSkinDlg();
	HWND m_OwnerHwnd;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	CWebBrowserEvents<CDownloadSkinDlg, &SkinDlgBeforeNavigate2Info> *m_Events;
	IWebBrowser2 *m_pWB2;

	BEGIN_MSG_MAP(CDownloadSkinDlg)
//		ATLTRACE("Msg = %x\r\n", uMsg);
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//		MESSAGE_HANDLER(WM_PAINT, OnPaint)
//		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
//		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorStatic)
		COMMAND_ID_HANDLER(ID_CANCEL, OnCancelCmd)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
//	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
//	LRESULT OnRegisterCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // !defined(AFX_DOWNLOADSKINDLG_H__CCE3E578_00C2_4F1A_B587_76077D630052__INCLUDED_)
