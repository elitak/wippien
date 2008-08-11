#include "stdafx.h"
#include "ContactAuthDlg.h"
#include "ComBSTR2.h"
#include "Settings.h"
#include "Jabber.h"
#include "MainDlg.h"
#include "SDKMessageLink.h"

extern CSettings _Settings;
extern CJabber *_Jabber;
extern CMainDlg _MainDlg;

CContactAuthDlg *_ContactAuthDlg = NULL;
extern CSDKMessageLink *_SDK;


CContactAuthDlg::CContactAuthDlg()
{
//	m_CT = NULL;
	m_Pos = 0;
	m_bFont = FALSE;
	m_WhiteBrush = CreateSolidBrush(RGB(255,255,255));
}

CContactAuthDlg::~CContactAuthDlg()
{
	DeleteObject(m_WhiteBrush);
//	if (m_CT)
//		m_CT->Release();
//	m_CT = NULL;

}

LRESULT CContactAuthDlg::OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if ((LPARAM)GetDlgItem(IDC_CONTACTJID) == lParam)
		return (LRESULT)m_WhiteBrush;
	else
		return FALSE;
}


void CContactAuthDlg::SendPresence(BOOL allow)
{
#ifndef _WODXMPPLIB
	WODXMPPCOMLib::IXMPPContact *ct;
	WODXMPPCOMLib::IXMPPContacts *cts;
#else
	void *ct = NULL;
#endif


#ifndef _WODXMPPLIB

	if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
	{
		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = m_JID;
		if (SUCCEEDED(cts->get_Item(var, &ct)))
		{
			if (ct)
			{
				WODXMPPCOMLib::StatusEnum st = (WODXMPPCOMLib::StatusEnum)6;
#else
				CComBSTR2 j = m_JID;
				WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, j.ToString(), &ct);
				if (!ct || !m_JID.Length())
					return;

				WODXMPPCOMLib::StatusEnum st = (WODXMPPCOMLib::StatusEnum)6;
#endif
				if (allow)
				{
					// get state
					if (ct)
					{
#ifndef _WODXMPPLIB
						if (SUCCEEDED(ct->get_Status(&st)))
#else
							WODXMPPCOMLib::XMPP_Contact_GetStatus(ct, &st);
#endif
						{
						}
					}
				}


				char buff[8192];
				CComBSTR2 jid = m_JID;
				sprintf(buff, "<presence to='%s' type='%s'><priority>0</priority></presence>", jid.ToString(), allow?"subscribed":"unsubscribed");
				if (_Jabber)
				{
#ifndef _WODXMPPLIB
					CComBSTR2 b = buff;
					_Jabber->m_Jabb->raw_RawSend(b);
#else
					WODXMPPCOMLib::XMPP_RawSend(_Jabber->m_Jabb, buff);
#endif
				}


				if (!(st >=1 && st <=7)) // not online, nor requested
				{
					// we want to request authorization too
#ifndef _WODXMPPLIB
					ct->Subscribe();
#else
					WODXMPPCOMLib::XMPP_Contact_Subscribe(ct);
#endif
				}
#ifndef _WODXMPPLIB
			}
			ct->Release();
		}
	}
#else
			WODXMPPCOMLib::XMPP_Contacts_Free(ct);
#endif

}

BOOL CContactAuthDlg::UpdateWin()
{
	if (m_Pos >= _Settings.m_AuthRequests.size())
		m_Pos--;

	if (!_Settings.m_AuthRequests.size())
	{
		DestroyWindow();
		return FALSE;
	}
	if (_Settings.m_AuthRequests.size() > 1)
	{
		::EnableWindow(GetDlgItem(IDC_NEXT), TRUE);
		::EnableWindow(GetDlgItem(IDC_BACK), TRUE);
	}
	else
	{
		::EnableWindow(GetDlgItem(IDC_NEXT), FALSE);
		::EnableWindow(GetDlgItem(IDC_BACK), FALSE);
	}
	
	char buff[128];
	int sz = _Settings.m_AuthRequests.size() - 1;
	if (sz<0)
		sz = 0;
	if (sz)
	{
		sprintf(buff, "%d", sz);
		SetDlgItemText(IDC_MOREWAITING, buff);
		::ShowWindow(GetDlgItem(IDC_MOREWAITING), SW_SHOW);
		::ShowWindow(GetDlgItem(IDC_MOREREQUESTSWAITING), SW_SHOW);
	}
	else
	{
		::ShowWindow(GetDlgItem(IDC_MOREWAITING), SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_MOREREQUESTSWAITING), SW_HIDE);
	}

//	if (m_CT)
//		m_CT->Release();
//	m_CT = NULL;
	m_JID.Empty();

	CComBSTR2 jid = _Settings.m_AuthRequests[m_Pos];
	if (!jid.Length())
	{
		PostMessage(WM_COMMAND, IDNO);
		return TRUE;
	}
//	char *jd1 = jid.ToString();
//	char *jd2 = strchr(jd1, '/');
//	if (jd2)
//		*jd2 = 0;

	// find current contact
	CComBSTR2 jd;
#ifndef _WODXMPPLIB
	WODXMPPCOMLib::IXMPPContacts *cts;
	WODXMPPCOMLib::IXMPPContact *ct;

	if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
	{
		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = jid;

		if (SUCCEEDED(cts->get_Item(var, &ct)))
		{
			if (SUCCEEDED(ct->get_JID(&jd)))
			{
				char *jd1 = jd.ToString();
				char *jd2 = strchr(jd1, '/');
				if (jd2)
					*jd2 = 0;
				m_JID = jd1;
				SetDlgItemText(IDC_CONTACTJID, jd1);
			}
			ct->Release();
		}
		cts->Release();
	}
#else
	void *ct;
	if (SUCCEEDED(WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, jid.ToString(), &ct)))
	{
		if (ct)
		{
			char jfb[1024];
			int jlen = sizeof(jfb);
			WODXMPPCOMLib::XMPP_Contact_GetJID(ct, jfb, &jlen);
			char *jd2 = strchr(jfb, '/');
			if (jd2)
				*jd2 = 0;
			m_JID = jfb;
			WODXMPPCOMLib::XMPP_Contacts_Free(ct);
			SetDlgItemText(IDC_CONTACTJID, jfb);
		}
	}
#endif


	if (_Settings.m_AuthContacts == 0)
		PostMessage(WM_COMMAND, IDNO);
	else
	if (_Settings.m_AuthContacts == 2)
		PostMessage(WM_COMMAND, IDYES);
	else
	if (_SDK)
	{
		Buffer b;
		b.PutCString(m_JID);
		if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_AUTHREQUEST, b.Ptr(), b.Len()))
			return FALSE;
	}

	return TRUE;
}

LRESULT CContactAuthDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	m_hWnd = NULL;
	return TRUE;
}
LRESULT CContactAuthDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	DestroyWindow();
	
	bHandled = TRUE;
	return TRUE;
}

LRESULT CContactAuthDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!m_bFont)
	{
		m_bFont = TRUE;
		CFontHandle DlgFont = GetFont();
		CClientDC dcScreen(NULL);
		
		LOGFONT LogFont = {0};
		DlgFont.GetLogFont(&LogFont);

		_tcscpy(LogFont.lfFaceName, _T("arial bold"));
		LogFont.lfWeight = FW_BOLD;
		LogFont.lfHeight = -::MulDiv(8, dcScreen.GetDeviceCaps(LOGPIXELSY), 72);
		DlgTitle1.CreateFontIndirect(&LogFont);
	}
	CWindow w1 = GetDlgItem(IDC_CONTACTJID);
	w1.SetFont(DlgTitle1);

	
	m_Pos = 0;
	CenterWindow(GetParent());
	SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	UpdateWin();

	SetWindowText(_Settings.Translate("Authorization request"));
	SetDlgItemText(IDC_AUTHSTATICTEXT, _Settings.Translate("Do you allow"));
	SetDlgItemText(IDC_AUTHSTATICTEXT2, _Settings.Translate("to add you to his contact list?"));
	SetDlgItemText(IDYES, _Settings.Translate("&Yes"));
	SetDlgItemText(IDNO, _Settings.Translate("&No"));
	SetDlgItemText(IDC_DETAILS, _Settings.Translate("&Details"));
	SetDlgItemText(IDC_MOREREQUESTSWAITING, _Settings.Translate("more requests waiting..."));
	SetDlgItemText(IDC_APPLYTOALL, _Settings.Translate("&Apply to all"));
	SetDlgItemText(IDC_BLOCKUSER, _Settings.Translate("&Block user(s)"));

	return TRUE;
}

LRESULT CContactAuthDlg::OnDetails(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CComBSTR2 mj = m_JID;
	char *m = mj.ToString();
	for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
	{
		CUser *us = (CUser *)_MainDlg.m_UserList.m_Users[i];
		if (!strcmp(us->m_JID, m))
		{
			_MainDlg.m_UserList.ExecuteRButtonUserCommand(/*NULL, */us, ID_POPUP1_DETAILS);
			break;
		}
	}

	return TRUE;

}
LRESULT CContactAuthDlg::OnYes(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// send 'subscribed' presence to remote side
	SendPresence(TRUE);


	// and remove this from the list
	_Settings.m_AuthRequests.erase(_Settings.m_AuthRequests.begin() + m_Pos);

	CUser *user = _MainDlg.m_UserList.GetUserByJID(m_JID);
	if (user)
	{
		BOOL toall = FALSE;
		if (::SendMessage(GetDlgItem(IDC_BLOCKUSER), BM_GETSTATE, NULL, NULL))
			user->m_Block = TRUE;
		else
			user->m_Block = FALSE;
	}


	BOOL toall = FALSE;
	if (::SendMessage(GetDlgItem(IDC_APPLYTOALL), BM_GETSTATE, NULL, NULL))
		toall = TRUE;

	UpdateWin();
	if (toall && _Settings.m_AuthRequests.size())
	{
		BOOL b;
		OnYes(NULL, NULL, NULL, b);
	}
	return 0;
}	
LRESULT CContactAuthDlg::OnNo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SendPresence(FALSE);

	// and remove this from the list
	_Settings.m_AuthRequests.erase(_Settings.m_AuthRequests.begin() + m_Pos);

	BOOL toall = FALSE;
	if (::SendMessage(GetDlgItem(IDC_APPLYTOALL), BM_GETSTATE, NULL, NULL))
		toall = TRUE;

	UpdateWin();
	if (toall && _Settings.m_AuthRequests.size())
	{
		BOOL b;
		OnYes(NULL, NULL, NULL, b);
	}
	return 0;
}	
LRESULT CContactAuthDlg::OnNext(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Pos++;
	if (m_Pos >= _Settings.m_AuthRequests.size())
		m_Pos = 0;

	UpdateWin();
	return 0;
}	
LRESULT CContactAuthDlg::OnBack(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Pos--;
	if (m_Pos <0)
		m_Pos = _Settings.m_AuthRequests.size() - 1;

	UpdateWin();
	return 0;
}	