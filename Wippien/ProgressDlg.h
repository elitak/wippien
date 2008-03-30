// progressdlg.h : interface of the CprogressDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROGRESSDLG_H__ECD566A9_0AF1_4606_833F_348B7C6BB2B1__INCLUDED_)
#define AFX_PROGRESSDLG_H__ECD566A9_0AF1_4606_833F_348B7C6BB2B1__INCLUDED_

class Buffer;

class CProgressDlg : public CAxDialogImpl<CProgressDlg>, public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_PROGRESS };

	CProgressDlg();
	virtual ~CProgressDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();


	// skins
	BOOL DownloadFile(BOOL issecure, char *URL, Buffer *data, BOOL updateprogress);
	static DWORD WINAPI DownloadSkinThreadProc(LPVOID lpParam);
	BOOL InitDownloadSkin(Buffer *data);
	Buffer *m_SkinBuffer;

	void DoEvents(void);
	int m_Value, m_Total;
	BOOL m_Canceled;
//	HBRUSH m_Brush, m_Back;

	BEGIN_MSG_MAP(CProgressDlg)
//		ATLTRACE("Msg = %x\r\n", uMsg);
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//		MESSAGE_HANDLER(WM_PAINT, OnPaint)
//	    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
//		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)

//		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
//		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorStatic)
//		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//    LRESULT OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//    LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnRegisterCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

#endif // !defined(AFX_ABOUTDLG_H__ECD566A9_0AF1_4606_833F_348B7C6BB2B1__INCLUDED_)
