// ontactAuthDlg.h: interface for the ContactAuthDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONTACTAUTHDLG_H__E045397A_1865_456A_AA19_11D65AA92FF7__INCLUDED_)
#define AFX_ONTACTAUTHDLG_H__E045397A_1865_456A_AA19_11D65AA92FF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "Jabber.h"

extern CJabber *_Jabber;

class CContactAuthDlg   : public CDialogImpl<CContactAuthDlg>
{
public:
	enum { IDD = IDD_AUTHORIZATIONDIALOG };

	CContactAuthDlg();
	virtual ~CContactAuthDlg();

	CComBSTR m_JID;
	int m_Pos;
//	WODJABBERCOMLib::IJbrContact *m_CT;
	CFont DlgTitle1;
	BOOL m_bFont;
	HBRUSH m_WhiteBrush;

	BEGIN_MSG_MAP(CContactAuthDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDYES, OnYes)
		COMMAND_ID_HANDLER(IDNO, OnNo)
		COMMAND_ID_HANDLER(IDC_BACK, OnNext)
		COMMAND_ID_HANDLER(IDC_NEXT, OnBack)
		COMMAND_ID_HANDLER(IDC_DETAILS, OnDetails)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	
	void SendPresence(BOOL allow);
	BOOL UpdateWin();
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnYes(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNext(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBack(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDetails(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

/*	void OnFinalMessage(HWND hWnd)
	{
		MSG msg;
		while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		::SetWindowLong(m_hWnd, GWL_WNDPROC, NULL);
		delete this;
	}
*/
};

#endif // !defined(AFX_ONTACTAUTHDLG_H__E045397A_1865_456A_AA19_11D65AA92FF7__INCLUDED_)
