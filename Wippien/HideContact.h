// HideContact.h: interface for the CHideContact class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HIDECONTACT_H__02937B39_FA0F_473C_AFF5_2062E0281F48__INCLUDED_)
#define AFX_HIDECONTACT_H__02937B39_FA0F_473C_AFF5_2062E0281F48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern CJabber *_Jabber;

class CHiddenContact : public CDialogImpl<CHiddenContact>
{
public:
	enum { IDD = IDD_HIDECONTACT };
	char m_SelectedContact[1024];

	CHiddenContact()
	{
		m_SelectedContact[0] = 0;
	}

	BEGIN_MSG_MAP(CHiddenContact)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOkCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancelCmd)
	END_MSG_MAP()

	void AddContacts(void)
	{
		// create new contact
#ifndef _WODXMPPLIB
		WODXMPPCOMLib::IXMPPContact *ct;
		WODXMPPCOMLib::IXMPPContacts *cts;

		if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
		{
			short count = 0;
			if (SUCCEEDED(cts->get_Count(&count)))
			{
				for (int i=0;i<count;i++)
				{
					VARIANT var;
					var.vt = VT_I4;
					var.lVal = i;
					if (SUCCEEDED(cts->get_Item(var, &ct)))
					{
						CComBSTR2 c;
						if (SUCCEEDED(ct->get_JID(&c)))
						{
							char *c1 = c.ToString();
							char *c2 = strchr(c1, '/');
							if (c2)
								*c2 = 0;
							if (!_Settings.IsHiddenContact(c1))
								::SendDlgItemMessage(m_hWnd, IDC_CONTACTLIST, CB_ADDSTRING, 0, (LPARAM)c1);
						}
						ct->Release();
					}
				}
			}
			cts->Release();
		}		
#else
		short Count = 0;
		WODXMPPCOMLib::XMPP_ContactsGetCount(_Jabber->m_Jabb, &Count);
		for (int i=0;i<Count;i++)
		{
			void *ct = NULL;
			WODXMPPCOMLib::XMPP_ContactsGetContact(_Jabber->m_Jabb, i, &ct);
			if (ct)
			{
				char buff[1024];
				int bflen = sizeof(buff);
				WODXMPPCOMLib::XMPP_Contact_GetJID(ct, buff, &bflen);
				char *c2 = strchr(buff, '/');
				if (c2)
					*c2 = 0;
				if (!_Settings.IsHiddenContact(buff))
					::SendDlgItemMessage(m_hWnd, IDC_CONTACTLIST, CB_ADDSTRING, 0, (LPARAM)buff);

				WODXMPPCOMLib::XMPP_Contacts_Free(ct);
			}
		}
#endif
	}
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		AddContacts();
		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnOkCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		GetDlgItemText(IDC_CONTACTLIST, m_SelectedContact, sizeof(m_SelectedContact));
		EndDialog(wID);
		return 0;
	}	
	LRESULT OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}	
};

#endif // !defined(AFX_HIDECONTACT_H__02937B39_FA0F_473C_AFF5_2062E0281F48__INCLUDED_)
