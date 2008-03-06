// SettingsDlg.cpp: implementation of the CSettingsDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SettingsDlg.h"
#include "ComBSTR2.h"
#include "Buffer.h"
#include "Jabber.h"
#include "Settings.h"
#include "Ethernet.h"
#include "MainDlg.h"
#include "ExtWndShadow.h"
#include "ChatRoom.h"
#include "Notify.h"
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <Iphlpapi.h>
#ifdef _SKINMAGICKEY
#include "SkinMagicLib.h"
#endif
#include "DownloadSkinDlg.h"
#include "UpdateHandler.h"
#include "HideContact.h"

extern CAppModule _Module;
extern CSettings _Settings;
extern CJabber *_Jabber;
extern CMainDlg _MainDlg;
extern CEthernet _Ethernet;
extern CNotify _Notify;

void ResampleImageIfNeeded(CxImage *img, int size);
void ResampleImageIfNeeded(CxImage *img, int sizeX, int sizeY);
void PopulateChatRoomListview(void);
char *trim(char *text);


static const char Base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char Pad64 = '=';

BOOL _LoadIconFromResource(CxImage *img, char *restype, int imgformat, int resid);
int uuencode(unsigned char *src, unsigned int srclength,char *target, size_t targsize);
int uudecode(const char *src, u_char *target, size_t targsize);
int b64_pton(char const *src, u_char *target, size_t targsize);
int b64_ntop(u_char const *src, size_t srclength, char *target, size_t targsize);

#define IPS_ALLOW		"allow"
#define IPS_DENY		"deny"
#define IPS_UNKNOWN		"not specified"

#define DRAWSHADOW(x)\
	::GetWindowRect(GetDlgItem((x)), rc);\
	ScreenToClient(&rc);\
	rc.InflateRect( 2, 2, 0, 0 );\
	_shadow.Paint(dcPaint, rc,\
	5,\
	CExtWndShadow::DEF_BRIGHTNESS_MIN,\
	CExtWndShadow::DEF_BRIGHTNESS_MAX, FALSE);\


CSettingsDlg::CSettingsNetworkFirewall::CSettingsNetworkFirewall() : _CSettingsTemplate()
{
	PATH = "Network\\Firewall";
	TEXT1 = "Set up firewall rules.";
	TEXT2 = "You can set up which ports can be accessed by your contacts.";
	
}

CSettingsDlg::CSettingsNetworkFirewall::~CSettingsNetworkFirewall()
{
	
}

void CSettingsDlg::CSettingsNetworkFirewall::DoDefault(void)
{
	BOOL bh = FALSE;
	Apply();
}

LRESULT CSettingsDlg::CSettingsNetworkFirewall::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	for (int i=0;i<_Settings.m_FirewallRules.size();i++)
	{
		FirewallStruct *orgfs = (FirewallStruct *)_Settings.m_FirewallRules[i];
		FirewallStruct *fs = new FirewallStruct;
		memcpy(fs, orgfs, sizeof(FirewallStruct));
		m_FirewallRules.push_back(fs);
	}

	m_Rules.Attach(GetDlgItem(IDC_RULES));
	if (_Settings.m_FirewallDefaultAllowRule)
		::SendMessage(GetDlgItem(IDC_ALLOWTRAFFIC), BM_SETCHECK, BST_CHECKED, NULL);
	else
		::SendMessage(GetDlgItem(IDC_BLOCKTRAFFIC), BM_SETCHECK, BST_CHECKED, NULL);
	::SendMessage(GetDlgItem(IDC_TCP), BM_SETCHECK, BST_CHECKED, NULL);

	PopulateList();
	return TRUE;
}

void CSettingsDlg::CSettingsNetworkFirewall::PopulateList(void)
{
	::SendMessage(GetDlgItem(IDC_RULES), LB_RESETCONTENT, NULL, NULL);
	char buff[1024];
	for (int i=0;i<m_FirewallRules.size();i++)
	{
		FirewallStruct *fs = (FirewallStruct *)m_FirewallRules[i];
		switch (fs->Proto)
		{
			case IPPROTO_TCP:
				sprintf(buff, "tcp               %-5d", fs->Port);
				break;

			case IPPROTO_UDP:
				sprintf(buff, "udp              %-5d", fs->Port);
				break;

			case IPPROTO_ICMP:
				strcpy(buff, "* All ICMP traffic");
				break;
		}
		int j = ::SendMessage(GetDlgItem(IDC_RULES), LB_ADDSTRING, NULL, (LPARAM)buff);
		::SendMessage(GetDlgItem(IDC_RULES), LB_SETITEMDATA, j, i);
	}
}

BOOL CSettingsDlg::CSettingsNetworkFirewall::Apply(void)
{
	while (_Settings.m_FirewallRules.size())
	{
		FirewallStruct *fs = (FirewallStruct *)_Settings.m_FirewallRules[0];
		delete fs;
		_Settings.m_FirewallRules.erase(_Settings.m_FirewallRules.begin());
	}

	for (int i=0;i<m_FirewallRules.size();i++)
	{
		FirewallStruct *orgfs = (FirewallStruct *)m_FirewallRules[i];
		FirewallStruct *fs = new FirewallStruct;
		memcpy(fs, orgfs, sizeof(FirewallStruct));
		_Settings.m_FirewallRules.push_back(fs);
	}

	if (::SendMessage(GetDlgItem(IDC_ALLOWTRAFFIC), BM_GETSTATE, NULL, NULL))
		_Settings.m_FirewallDefaultAllowRule = TRUE;
	else
		_Settings.m_FirewallDefaultAllowRule = FALSE;

	_Ethernet.m_FirewallRulesChanged = TRUE;
	return TRUE;
}
void CSettingsDlg::CSettingsNetworkFirewall::Init(HWND Owner)
{
	m_Owner = Owner;
	Create(Owner);
}

void CSettingsDlg::CSettingsNetworkFirewall::Show(BOOL Show, RECT *rc)
{
	if (IsWindow())
	{
		if (Show)
		{
			::SetWindowPos(m_hWnd, NULL, rc->left, rc->top, rc->right, rc->bottom, SWP_NOZORDER);
			ShowWindow(SW_SHOW);
			SetFocus();
		}
		else
			ShowWindow(SW_HIDE);
	}
}

LRESULT CSettingsDlg::CSettingsNetworkFirewall::OnAddNewRule(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buff[1024];
	buff[0] = 0;
	GetDlgItemText(IDC_EDIT1, buff, 1024);
	int proto = 0;

	if (::SendMessage(GetDlgItem(IDC_TCP), BM_GETSTATE, NULL, NULL)) 
		proto = IPPROTO_TCP;
	if (::SendMessage(GetDlgItem(IDC_UDP), BM_GETSTATE, NULL, NULL)) 
		proto = IPPROTO_UDP;
	if (::SendMessage(GetDlgItem(IDC_ICMP), BM_GETSTATE, NULL, NULL)) 
		proto = IPPROTO_ICMP;
	
	if (*buff || proto == IPPROTO_ICMP)
	{
		int port = atol(buff);
		if (proto)
		{
			BOOL found = FALSE;
			for (int i=0;!found && i<m_FirewallRules.size();i++)
			{
				FirewallStruct *fs = (FirewallStruct *)m_FirewallRules[i];
				if (fs->Proto == proto)
				{
					if (proto == IPPROTO_ICMP)
						found = TRUE;
					else
						if (fs->Port == port)
							found = TRUE;
				}
			}
			if (!found)
			{
				FirewallStruct *fs = new FirewallStruct;
				fs->Port = port;
				fs->Proto = proto;
				m_FirewallRules.push_back(fs);
				PopulateList();
			}
		}
	}
	SetDlgItemText(IDC_EDIT1, "");
	
	
	return TRUE;
}

LRESULT CSettingsDlg::CSettingsNetworkFirewall::OnRemoveRule(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int i = ::SendMessage(GetDlgItem(IDC_RULES), LB_GETCURSEL, 0, 0);
	if (i != LB_ERR)
	{
		i = ::SendMessage(GetDlgItem(IDC_RULES), LB_GETITEMDATA, i, 0);
		if (i>=0 && i<m_FirewallRules.size())
		{
			m_FirewallRules.erase(m_FirewallRules.begin()+i);
			PopulateList();
		}
	}
	else
		OnChange(0, IDC_RULES, 0, bHandled);

	return TRUE;
}

LRESULT CSettingsDlg::CSettingsNetworkFirewall::OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (::SendMessage(GetDlgItem(IDC_RULES), LB_GETCURSEL, 0, 0) == LB_ERR)
		::EnableWindow(GetDlgItem(IDC_FIREWALLRULE_REMOVE), FALSE);
	else
		::EnableWindow(GetDlgItem(IDC_FIREWALLRULE_REMOVE), TRUE);

	char buff[1024] = {0};
	GetDlgItemText(IDC_EDIT1, buff, 1024);
	if (*buff || ::SendMessage(GetDlgItem(IDC_ICMP), BM_GETSTATE, NULL, NULL))
		::EnableWindow(GetDlgItem(IDC_FIREWALLRULE_ADD), TRUE);
	else
		::EnableWindow(GetDlgItem(IDC_FIREWALLRULE_ADD), FALSE);

	if (::SendMessage(GetDlgItem(IDC_ALLOWTRAFFIC), BM_GETSTATE, NULL, NULL))
		SetDlgItemText(IDC_STATICBLOCK, "But block these ports:");
	else
		SetDlgItemText(IDC_STATICBLOCK, "But allow these ports:");
	
	
	return TRUE;
}