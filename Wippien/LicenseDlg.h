// Licensedlg.h : interface of the CLicenseDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LicenseDLG_H__ECD566A9_0AF1_4606_833F_348B7C6BB2B1__INCLUDED_)
#define AFX_LicenseDLG_H__ECD566A9_0AF1_4606_833F_348B7C6BB2B1__INCLUDED_

#include "../CxImage/CxImage/ximage.h"
#include "ScrollerCtrl.h"


class CLicenseDlg : public CDialogImpl<CLicenseDlg>
{
public:
	enum { IDD = IDD_LICENSE };

	CLicenseDlg();
	virtual ~CLicenseDlg();

	BEGIN_MSG_MAP(CLicenseDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_ORDERNOW, OnOrderNowCmd)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOrderNowCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // !defined(AFX_LicenseDLG_H__ECD566A9_0AF1_4606_833F_348B7C6BB2B1__INCLUDED_)
