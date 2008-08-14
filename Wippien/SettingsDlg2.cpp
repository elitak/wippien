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

extern const char *IPS_ALLOW;
extern const char *IPS_DENY;
extern const char *IPS_UNKNOWN;

CSettingsDlg::CSettingsNetworkFirewall::CSettingsNetworkFirewall() : _CSettingsTemplate()
{
	CComBSTR mp = _Settings.Translate("Network");
	mp += "\\";
	mp += _Settings.Translate("Firewall");
	mPATH = mp;
	PATH = mPATH.ToString();
	TEXT1 = _Settings.Translate("Set up firewall rules.");
	TEXT2 = _Settings.Translate("You can set up which ports can be accessed by your contacts.");
	
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
	SetDlgItemText(IDC_S1, _Settings.Translate("Default rule"));
	SetDlgItemText(IDC_ALLOWTRAFFIC, _Settings.Translate("Allow all traffic"));
	SetDlgItemText(IDC_BLOCKTRAFFIC, _Settings.Translate("Block all traffic"));
	SetDlgItemText(IDC_STATICBLOCK, _Settings.Translate("But block these ports"));
	SetDlgItemText(IDC_TCP, _Settings.Translate("TCP"));
	SetDlgItemText(IDC_UDP, _Settings.Translate("UDP"));
	SetDlgItemText(IDC_ICMP, _Settings.Translate("ICMP"));
	SetDlgItemText(IDC_FIREWALLRULE_ADD, _Settings.Translate("&Add"));
	SetDlgItemText(IDC_FIREWALLRULE_REMOVE, _Settings.Translate("&Remove"));
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
				strcpy(buff, _Settings.Translate("* All ICMP traffic"));
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
		SetDlgItemText(IDC_STATICBLOCK, _Settings.Translate("But block these ports"));
	else
		SetDlgItemText(IDC_STATICBLOCK, _Settings.Translate("But allow these ports"));
	
	
	return TRUE;
}

CSettingsDlg::CSettingsLanguages::CSettingsLanguages() : _CSettingsTemplate()
{
	CComBSTR mp = _Settings.Translate("System");
	mp += "\\";
	mp += _Settings.Translate("Languages");
	mPATH = mp;
	PATH = mPATH.ToString();
	TEXT1 = _Settings.Translate("Setup language used in all Wippien texts.");
	TEXT2 = _Settings.Translate("New languages may appear periodically.");
}

CSettingsDlg::CSettingsLanguages::~CSettingsLanguages()
{

}


LRESULT CSettingsDlg::CSettingsLanguages::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPaintDC dcPaint(m_hWnd);
	return TRUE;
}


void CSettingsDlg::CSettingsLanguages::EnumerateLocalLanguageFiles(void)
{
	char buff[32768];
	strcpy(buff, _Settings.m_MyPath);
	strcat(buff, "Language\\*.txt");
	
	WIN32_FIND_DATA FileData;

	HANDLE hSearch = FindFirstFile(buff, &FileData); 
	CComBSTR2 Author, Version;

	if (hSearch != INVALID_HANDLE_VALUE) 
	{ 
		BOOL fFinished = FALSE;
		while (!fFinished) 
		{ 
			Author.Empty();
			Version.Empty();

			if (stricmp(FileData.cFileName, "english.txt"))
			{

				strcpy(buff, _Settings.m_MyPath);
				strcat(buff, "Language\\");
				strcat(buff, FileData.cFileName); 
				Buffer temp;
				if (_Settings.LoadLanguageFile(buff, &temp))
				{
					char *a = temp.GetNextLine(); // ignore first line
					do 
					{
						a = temp.GetNextLine();
						if (a && *a=='#')
						{
							char *b = strstr(a, "Author:");
							if (b)
							{
								Author = trim(b+7);
							}
							else
							{
								b = strstr(a, "Version:");
								if (b)
								{
									Version = trim(b+8);
								}
							}
						}
						else
							a = NULL;
					} while (a);


					a = strchr(FileData.cFileName, '.');
					if (a)
						*a = 0;
					{
						LVITEM it = {0};
						it.mask = LVIF_TEXT;
						CComBSTR2 bf = FileData.cFileName;
						it.pszText = bf.ToString();
						*it.pszText = toupper(*it.pszText);
						it.cchTextMax = strlen(it.pszText);
						
						int res = SendMessage(GetDlgItem(IDC_LOCALLANGUAGES), LVM_INSERTITEM, 0, (LPARAM)&it);
						it.iItem = res;
						it.iSubItem = 1;
						it.mask = LVIF_TEXT;
						it.pszText = Author.ToString();
						it.cchTextMax = strlen(it.pszText);					
						SendMessage(GetDlgItem(IDC_LOCALLANGUAGES), LVM_SETITEM, 0, (LPARAM)&it);
						it.iItem = res;
						it.iSubItem = 2;
						it.mask = LVIF_TEXT;
						it.pszText = Version.ToString();
						it.cchTextMax = strlen(it.pszText);					
						SendMessage(GetDlgItem(IDC_LOCALLANGUAGES), LVM_SETITEM, 0, (LPARAM)&it);
					}
				}
			}
			
			if (!FindNextFile(hSearch, &FileData)) 
				fFinished = TRUE; 
		} 
		
		// Close the search handle. 		
		FindClose(hSearch);
	}
}
LRESULT CSettingsDlg::CSettingsLanguages::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_Languages.Attach(GetDlgItem(IDC_LOCALLANGUAGES));

	SendDlgItemMessage(IDC_LOCALLANGUAGES,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_BORDERSELECT | LVS_EX_FULLROWSELECT); 
	LV_COLUMN lvcol = {0};
	lvcol.mask=LVCF_TEXT | LVCF_WIDTH;
	lvcol.pszText=_Settings.Translate("Language");
	lvcol.cx = 150;	
	SendDlgItemMessage(IDC_LOCALLANGUAGES,LVM_INSERTCOLUMN,0,(LPARAM)&lvcol); 
	lvcol.pszText = _Settings.Translate("Author");
	lvcol.cx = 150;
	SendDlgItemMessage(IDC_LOCALLANGUAGES,LVM_INSERTCOLUMN,1,(LPARAM)&lvcol); 
	lvcol.pszText = _Settings.Translate("Version");
	lvcol.cx = 60;
	SendDlgItemMessage(IDC_LOCALLANGUAGES,LVM_INSERTCOLUMN,2,(LPARAM)&lvcol); 

	int lcid = GetUserDefaultLCID();
	char buff[8192];
	if (GetLocaleInfo(lcid, LOCALE_SENGLANGUAGE, buff, sizeof(buff)))
		SetDlgItemText(IDC_S3, buff);

	EnumerateLocalLanguageFiles();


	SetDlgItemText(IDC_S1, _Settings.Translate("Following languages are found as available on local computer"));
	SetDlgItemText(IDC_CHANGE_LANGUAGE, _Settings.Translate("&Change"));
	SetDlgItemText(IDC_RESET_ENGLISH, _Settings.Translate("&Reset to English"));
	SetDlgItemText(IDC_S2, _Settings.Translate("Locale used on local computer"));
	SetDlgItemText(IDC_S5, _Settings.Translate("Currently used language"));
	SetDlgItemText(IDC_S7, _Settings.Translate("Changes will take affect on 'as needed' basis. You should restart Wippien if you want to see full change."));

	CComBSTR2 l = _Settings.m_Language;
	SetDlgItemText(IDC_S6, l.ToString());

	return TRUE;
}

BOOL CSettingsDlg::CSettingsLanguages::Apply(void)
{
	char buff[16384];
	*buff = 0;
	::SendMessage(GetDlgItem(IDC_S6), WM_GETTEXT, 16384, (LPARAM)buff);
	if (buff[0])
	{
		CComBSTR2 l = _Settings.m_Language;
		if (strcmp(l.ToString(), buff))
		{
			_Settings.m_Language = buff;
			_Settings.m_LanguageFileVersion = 0;
			_Settings.LoadLanguage(buff);
		}
	}
	return TRUE;
}

void CSettingsDlg::CSettingsLanguages::Init(HWND Owner)
{
	m_Owner = Owner;
	Create(Owner);
}

void CSettingsDlg::CSettingsLanguages::Show(BOOL Show, RECT *rc)
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
LRESULT CSettingsDlg::CSettingsLanguages::OnLanguageChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	NMLISTVIEW *nm = (NMLISTVIEW *)pnmh;
	
	switch (pnmh->code)
	{
	case NM_CLICK:
	case NM_DBLCLK:
		{
			int i = SendDlgItemMessage(IDC_LOCALLANGUAGES,LVM_GETNEXTITEM, -1,LVNI_SELECTED); // return item selected
			if (i!=LB_ERR)
				::EnableWindow(GetDlgItem(IDC_CHANGE_LANGUAGE), TRUE);
			else
				::EnableWindow(GetDlgItem(IDC_CHANGE_LANGUAGE), FALSE);
		}
		break;
	}
	return 0;
}

LRESULT CSettingsDlg::CSettingsLanguages::OnButtonChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
	int i = SendDlgItemMessage(IDC_LOCALLANGUAGES,LVM_GETNEXTITEM, -1,LVNI_SELECTED); // return item selected
	if (i != LB_ERR)
	{
		char lang[1024] = {0};
		LVITEM lv = {0};
		lv.mask = LVIF_TEXT;
		lv.pszText = lang;
		lv.cchTextMax = sizeof(lang);
		lv.iItem = i;
		SendDlgItemMessage(IDC_LOCALLANGUAGES,LVM_GETITEM, 0,(LPARAM)(LPLVITEM)&lv);
		SetDlgItemText(IDC_S6, lang);
	}
	::EnableWindow(GetDlgItem(IDC_CHANGE_LANGUAGE), FALSE);
	return TRUE;
}

LRESULT CSettingsDlg::CSettingsLanguages::OnButtonReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
	SetDlgItemText(IDC_S6, "English");
	return TRUE;
}
