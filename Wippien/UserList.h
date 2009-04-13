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
#include "Settings.h"

class CMainDlg;
extern CSettings _Settings;

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
	HFONT m_ListboxFont, m_ListboxSubFont, m_ListboxGroupFont;
	CCommandBarCtrlXP *m_UserPopupMenu, *m_SetupPopupMenu, *m_AwayPopupMenu, *m_ChatRoomPopupMenu, *m_GroupPopupMenu;
	void RefreshView(BOOL updateonly);
	void RefreshUser(void *cntc, char *chatroom);
	void InitialUserList(void);

	void Init(CMainDlg *Owner, HWND Parent);
	HTREEITEM FindRoot(char *RootName);
	HTREEITEM FindRoot(char *RootName, BOOL canaddnew);
	CUser *GetUserByJID(char *JID);
	CUser *GetUserByJID(BSTR JID);
	CUser *GetUserByJID(char *JID, BOOL WithResource);
	CUser *GetUserByJID(BSTR JID, BOOL WithResource);
	CUser *GetUserByVirtualIP(unsigned long IP);
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
	BOOL ExecuteRButtonGroupCommand(CSettings::TreeGroup *Group, int Command);
	BOOL ExecuteRButtonUserCommand(CUser *user, int Command);
	LRESULT OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	BOOL DeleteGroup(char *GroupName);

	class CRenameContact : public CDialogImpl<CRenameContact>
	{
	public:
		enum { IDD = IDD_RENAMECONTACT };
		char m_VisibleName[1024];
		BOOL m_IsGroupEdit;

		CRenameContact()
		{
			memset(m_VisibleName, 0, 1024);
			m_IsGroupEdit = FALSE;
		}

		BEGIN_MSG_MAP(CRenameContact)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDOK, OnOkCmd)
			COMMAND_ID_HANDLER(IDCANCEL, OnCancelCmd)
		END_MSG_MAP()

		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		{
			SetDlgItemText(IDC_RENAMEEDIT, m_VisibleName);
			SetDlgItemText(IDOK, _Settings.Translate("O&K"));
			SetDlgItemText(IDCANCEL, _Settings.Translate("&Cancel"));
			if (m_IsGroupEdit)
			{
				SetWindowText(_Settings.Translate("Rename group"));
				SetDlgItemText(IDC_STATICCONTACTNAME, _Settings.Translate("Enter new group name"));
			}
			else
			{
				SetWindowText(_Settings.Translate("Rename contact"));
				SetDlgItemText(IDC_STATICCONTACTNAME, _Settings.Translate("Enter new contact name"));

			}
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
