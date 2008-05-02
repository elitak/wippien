// MainDlg.h: interface for the CMainDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__D0C409AC_A31B_48CC_BD8C_840664320038__INCLUDED_)
#define AFX_MAINDLG_H__D0C409AC_A31B_48CC_BD8C_840664320038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "PNGButton.h"
#include "UserList.h"
#include "CwodWinSocket.h"
#include "SettingsDlg.h"


#define WM_APPBRMESSAGE			WM_USER+1
#define WM_STATUSMESSAGE		WM_USER+2

#define EMOTICONS_API __declspec(dllimport)


//typedef int EMOTICONS_API (WINAPI* FdTransform)(_Buffer *inb, _Buffer *outb, BOOL IsHtml);
//typedef int EMOTICONS_API (WINAPI* FdRollback)(_Buffer *inb, _Buffer *outb, BOOL IsHtml);
typedef int EMOTICONS_API (WINAPI* FdTransform)(_Buffer *inb, BOOL IsHtml, BOOL CanPlay);
typedef int EMOTICONS_API (WINAPI* FdRollback)(_Buffer *inb, BOOL IsHtml);
typedef int EMOTICONS_API (WINAPI* FdGetCount)(void);
typedef int EMOTICONS_API (WINAPI* FdGetItem)(int Index, char **ResourceType, char **ImageType);
typedef int EMOTICONS_API (WINAPI* FdGetText)(int Index, char **Text);
typedef int EMOTICONS_API (WINAPI* FdGetHtml)(int Index, char **Text);

extern UINT WM_TASKBARCREATEDMESSAGE;


#define _DEBUGWNDMSG

class CBalloonHelp;
class CSimpleHttpRequest;
class CChatRoom;

#include <vector>
typedef std::vector<CChatRoom *> CHATROOMLIST;


class CMainDlg : public CAxDialogImpl<CMainDlg>, /*public CUpdateUI<CMainDlg>,*/
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };


	void OnFinalMessage(HWND)
{
		PostQuitMessage(-1);
//		MessageBeep(-1);
}

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	CMainDlg();
	virtual ~CMainDlg();

	CHATROOMLIST m_ChatRooms;

	CPNGButton m_btnOK, m_btnSettings, m_btnAbout;
	CPNGButton m_btnMyStatus, m_btnMyContacts, m_btnAuthDlg, m_btnSmallMute, m_btnChatRooms;
	CUserList m_UserList;
	char m_StatusMessage[256];
	//CStatic m_Status;
	CxImage m_UserImage;
	HBRUSH	m_WhiteBrush, m_LightBlueBrush;
	HFONT m_Identity_NameFont, m_Identity_IPFont, m_Identity_StatusFont;
	POINT m_TooltipPoint;
	BOOL m_CanTooltip, m_GoAppbar, m_WearSkin, m_DidGoAutoAway;
//	POINT m_ToolTipPosition;
	int m_ReconnectWait;
	CSettingsDlg *m_SettingsDlg;
	CBalloonHelp* m_pBalloon;
	RECT m_DefChatWindowRect;
	int m_LastUpdateMin;
	CxImage m_MainWndTopBannerBack, m_MainWndTopBannerBack2;
	int m_InactiveTimer;
	BOOL m_WasInactiveTimer, m_WasConnectedBeforeSleep;

//	CComPtr<WODVCHATComLib::IwodVChatCom> m_VoiceChat;

	CRITICAL_SECTION m_SocketCS;


	HICON m_IconSmall, m_Icon, m_OnlineStatus[7];

	int m_MoveX, m_MoveY, m_SizeX, m_SizeY;
	int m_AuthDlgBlinker, m_PendingVCardRequests;
	BOOL m_AppbarRegistered;
	RECT m_AppbarRect;
	BOOL m_IMove, m_IMinimized;
	HWND m_EmptyWin;
	HMODULE m_User32Module;
	CSimpleHttpRequest *m_SimpleHttpRequest;

	void ShowStatusText(char *text);
	void CheckIfAntiInactivityMessage(int msg);


	typedef struct EmoticonsStruct
	{
		HINSTANCE hInst;
		FdTransform Transform;
		FdRollback Rollback;
		FdGetItem GetItem;
		FdGetHtml GetHtml;
		FdGetCount GetCount;
		FdGetText GetText;
		IMAGELIST m_Image;
		BOOL ClassRegistered;
		BOOL IsBig;
	} EmoticonsStruct;
	EmoticonsStruct m_EmoticonsInstance;
//	std::vector<void *> m_EmoticonsInstance;


//	BEGIN_UPDATE_UI_MAP(CMainDlg)
//	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
#ifdef _DEBUGWNDMSG
//	ATLTRACE("MainDlg %x %x\r\n", uMsg, hWnd);
#endif
		MESSAGE_HANDLER(WM_POWERBROADCAST, OnPowerBroadcast)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSizeMove)
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		MESSAGE_HANDLER(WM_STATUSMESSAGE, OnStatusMessage)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
		MESSAGE_HANDLER(WM_TRAYICON, OnTrayIcon)
		MESSAGE_HANDLER(WM_TASKBARCREATEDMESSAGE, OnCreateTrayIcon)
		if (hWnd == m_hWnd)
		{
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
		}
		else
			MessageBeep(-1);
		if (hWnd == m_hWnd)
		{
			MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkGnd)
		}
		else
			MessageBeep(-1);
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_APPBRMESSAGE, OnAppBarNotify); 
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_EXIT, OnExit)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDB_SETUP, OnSetup)
		COMMAND_ID_HANDLER(IDC_MYSTATUS, OnBtnStatus)
		COMMAND_ID_HANDLER(IDC_MYCONTACTS, OnBtnContacts)
		COMMAND_ID_HANDLER(IDC_SMALLMUTE, OnBtnSmallMute)
		COMMAND_ID_HANDLER(IDC_CHATROOMS, OnBtnChatRooms)
		COMMAND_ID_HANDLER(IDC_MYAUTHDLG, OnBtnAuthDlg)
//		COMMAND_ID_HANDLER(IDC_TREELIST, m_UserList.OnListCommand)
		NOTIFY_ID_HANDLER(IDC_TREELIST, m_UserList.OnListNotify)

		REFLECT_NOTIFICATIONS()
//		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()


	void RecalcInternalItems(void);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNotifyTyping(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTrayIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreateTrayIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkGnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizeImpl(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT _OnMoving(BOOL adjustmousepos, POINT cur_pos, UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnEnterSizeMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnExitSizeMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPowerBroadcast(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetup(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnContacts(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnSmallMute(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnChatRooms(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnStatus(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnAuthDlg(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStatusMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppBarNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnWindowPosChanged(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);
	void RegisterAppBar(BOOL isregister, int style);
	void RegisterAppBar(BOOL isregister, RECT *rc, int style);
	void OnIncomingMessage(char *ChatRoom, char *Contact, char *Message, char *HtmlMessage);
	void OnIncomingNotification(char *Contact, int NotID, VARIANT Data);
	void ShellIcon(int Message, HICON Icon, char *Tip);
	LRESULT CalcSizeXSizeY(void);
	void CheckAppbar(void);
	LRESULT CalcAlignment(RECT *rcin);
	void ToggleMute(void);
	BOOL ReloadEmoticons(BOOL ReleaseOnly);


};

#endif // !defined(AFX_MAINDLG_H__D0C409AC_A31B_48CC_BD8C_840664320038__INCLUDED_)
