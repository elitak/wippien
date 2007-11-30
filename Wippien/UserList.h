// UserList.h: interface for the CUserList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERLIST_H__797D61A2_1EE7_41B7_BE6B_5745E638BF19__INCLUDED_)
#define AFX_USERLIST_H__797D61A2_1EE7_41B7_BE6B_5745E638BF19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "VividTree.h"
#include "User.h"
#include "MenuXP.h"

class CMainDlg;

#include <vector>
typedef std::vector<CUser *> USERLIST;

class CUserList : public CVividTree
{
public:
	CUserList();
	virtual ~CUserList();

	USERLIST m_Users;
	HTREEITEM m_Dragging;


	CMainDlg *m_Owner;
	HWND m_hWndParent;
	HFONT m_ListboxFont, m_ListboxSubFont;
	CCommandBarCtrlXP *m_UserPopupMenu, *m_SetupPopupMenu, *m_AwayPopupMenu;
	void RefreshView(BOOL updateonly);
	void RefreshUser(void *cntc);

	void Init(CMainDlg *Owner, HWND Parent);
	HTREEITEM FindRoot(char *RootName);
	CUser *GetUserByJID(char *JID);
	CUser *GetUserByVirtualIP(unsigned long IP);
	CUser *GetUserByJID(BSTR JID);
	void InitIcons(void);
	void LoadIconFromResource(CxImage *img, int resid);
	BOOL ConnectIfPossible(CUser *user, BOOL perform);
	//void DisableInlineEdit();
	void AddMenuImage(int resid, int dataid);
#ifndef _WODXMPPLIB
	void OnVCard(WODXMPPCOMLib::IXMPPContact *Contact, BOOL Partial, BOOL received);
	CUser *AddNewUser(char *j, WODXMPPCOMLib::IXMPPContact *contact);
#else
	CUser *AddNewUser(char *j, void *contact);
	void OnVCard(void *Contact, BOOL Partial, BOOL received);
#endif
	void SortUsers(void);
	Buffer m_SortedUsersBuffer;
	int *m_SortedUser;

	// user actions
	LRESULT OnLButtonDblClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnListNotify(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	BOOL ExecuteRButtonCommand(/*HTREEITEM ht, */CUser *user, int Command);
	LRESULT OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	class CRenameContact : public CDialogImpl<CRenameContact>
	{
	public:
		enum { IDD = IDD_RENAMECONTACT };
		char m_VisibleName[1024];

		CRenameContact()
		{
			memset(m_VisibleName, 0, 1024);
		}

		BEGIN_MSG_MAP(CRenameContact)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDOK, OnOkCmd)
			COMMAND_ID_HANDLER(IDCANCEL, OnCancelCmd)
		END_MSG_MAP()

		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		{
			SetDlgItemText(IDC_RENAMEEDIT, m_VisibleName);
			CenterWindow(GetParent());
			return TRUE;
		}

		LRESULT OnOkCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		{
			::SendMessage(GetDlgItem(IDC_RENAMEEDIT), WM_GETTEXT, 1024, (LPARAM)m_VisibleName);
			EndDialog(wID);
			return 0;
		}	
		LRESULT OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		{
			EndDialog(wID);
			return 0;
		}	
	};
};

#endif // !defined(AFX_USERLIST_H__797D61A2_1EE7_41B7_BE6B_5745E638BF19__INCLUDED_)
