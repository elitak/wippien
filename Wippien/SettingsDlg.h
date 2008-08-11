// SettingsDlg.h: interface for the CSettingsDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGSDLG_H__9A74142A_2617_4BBF_BC67_7D9EC43FCA0F__INCLUDED_)
#define AFX_SETTINGSDLG_H__9A74142A_2617_4BBF_BC67_7D9EC43FCA0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#include <vector>
#include "../CxImage/CxImage/ximage.h"
#include <atlcrack.h>
#include "Buffer.h"
#include "openssl/rsa.h"
#include "ComBSTR2.h"
#include "CwodWinSocket.h"
#include "Settings.h"

extern CSettings _Settings;


#ifdef _WODXMPPLIB
namespace WODXMPPCOMLib
{
#include "\WeOnlyDo\wodXMPP\Code\Win32LIB\Win32LIB.h"
}
#endif


typedef std::vector<CxImage*> IMAGELIST;

class CSettingsDlg   : public CAxDialogImpl<CSettingsDlg>
{
public:
	enum { IDD = IDD_SETTINGS };

class _CSettingsTemplate;
typedef std::vector<_CSettingsTemplate *> DIALOGSLIST;


	CSettingsDlg(BOOL Modal);
	virtual ~CSettingsDlg();

	VARIANT_BOOL m_Modal;
	unsigned int m_DlgPos;
	BOOL m_OnInit;
	DIALOGSLIST m_Dialogs;
	CTreeViewCtrl m_Tree;
	HBRUSH m_WhiteBrush;
	CFont DlgTitle1, DlgTitle2;
	BOOL m_VCardChanged, m_NeedRestart;

	BEGIN_MSG_MAP(CSettingsDlg)
//		ATLTRACE("msg2 %d\r\n", uMsg);
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//		MESSAGE_HANDLER(WM_SIZE, OnSize)
//		MESSAGE_HANDLER(WM_SIZING, OnSize)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		MESSAGE_HANDLER(WM_NEEDRESTART, OnNeedRestart)
		MESSAGE_HANDLER(WM_VCARDCHANGED, OnVCardChanged)
		NOTIFY_ID_HANDLER(IDC_TREE1, OnTreeNotify)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_NEXT, OnNext)
		COMMAND_ID_HANDLER(IDC_BACK, OnBack)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnVCardChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_VCardChanged = TRUE;
		return FALSE;
	}
	LRESULT OnNeedRestart(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_NeedRestart = TRUE;
		return FALSE;
	}
	LRESULT OnTreeNotify(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNext(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBack(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	HTREEITEM AddToTree(char *Path, HTREEITEM at);
	HTREEITEM FindTree(char *Path, HTREEITEM at);
	void ShowDialog(int Index);

	class CEnterPassDialog : public CDialogImpl<CEnterPassDialog>
	{
	public:
		enum { IDD = IDD_NEEDPASS };
		char m_Password[1024];

		CEnterPassDialog()
		{
			memset(m_Password, 0, 1024);
			m_Success = FALSE;
			m_Subject.Append(_Settings.Translate("Enter Passphrase"));
			m_Text.Append(_Settings.Translate("Please enter key passphrase"));
		}

		BOOL m_Success;
		Buffer m_Subject;
		Buffer m_Text;

		BEGIN_MSG_MAP(CEnterPassDialog)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
			COMMAND_ID_HANDLER(IDCANCEL, OnCancelCmd)
		END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

		
		void SetText(char *Subject, char *Text)
		{
			m_Subject.Clear();
			m_Text.Clear();
			m_Subject.Append(Subject);
			m_Text.Append(Text);
		}
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		{
			CenterWindow(GetDesktopWindow());
			SetWindowText(m_Subject.Ptr());
			SetDlgItemText(IDC_ENTERPASSPHRASE, m_Text.Ptr());
			SetDlgItemText(IDOK, _Settings.Translate("O&K"));
			SetDlgItemText(IDCANCEL, _Settings.Translate("&Cancel"));
			return TRUE;
		}

		LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		{
			::SendMessage(GetDlgItem(IDC_PASSWORD), WM_GETTEXT, 1024, (LPARAM)m_Password);
			m_Success = TRUE;
			EndDialog(wID);
			return 0;
		}	
		LRESULT OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		{
			m_Success = FALSE;
			EndDialog(wID);
			return 0;
		}	
	};

	class _CSettingsTemplate
	{
	public:

		_CSettingsTemplate()
		{
			PATH = NULL;
			TEXT1 = NULL;
			TEXT2 = NULL;
			m_hItem = NULL;
			m_Owner = NULL;
//			m_Changed = FALSE;
			m_Starting = TRUE;
			m_NeedRestart = FALSE;
			m_VCardChanged = FALSE;
		}
		virtual ~_CSettingsTemplate()
		{
		
		}

		char *PATH;
		char *TEXT1;
		char *TEXT2;
		HTREEITEM m_hItem;
		BOOL m_Init;
		HWND m_Owner;
//		BOOL m_Changed;
		BOOL m_VCardChanged;
		BOOL m_NeedRestart;
		BOOL m_Starting;
				
		public:
		virtual void Init(HWND Owner)
		{
		}
		virtual void Show(BOOL Show, RECT *rc)
		{
		
		}
		virtual BOOL Apply(void)
		{
			return FALSE;
		}
		virtual void NeedRestart(void)
		{
			if (m_Starting)
			{
				if (m_Owner)
				::PostMessage(m_Owner, WM_NEEDRESTART, 0, 0);
			}
		}
		virtual void VCardChanged(void)
		{
			if (m_Owner)
				::PostMessage(m_Owner, WM_VCARDCHANGED, 0, 0);
		}
		virtual void DoDefault(void)
		{
		
		}
	};


	class CSettingsJID   : public CAxDialogImpl<CSettingsJID>, public _CSettingsTemplate
	{
	public:
#ifndef _WODXMPPLIB
		class CJabberEvents;
#endif
		class CJabberWiz
		{
		public:

#ifndef _WODXMPPLIB
			CComPtr<WODXMPPCOMLib::IwodXMPPCom> m_Jabb;
#else
			void *m_Jabb;
#endif
			CJabberWiz(CSettingsJID *Owner);
			~CJabberWiz();
			void Connect(char *JID, char *pass, char *hostname, int port, BOOL registernew, BOOL sslwrapped);
			void Disconnect(void);

#ifndef _WODXMPPLIB
			CJabberEvents *m_Events;
#else
			WODXMPPCOMLib::XMPPEventsStruct m_Events;
#endif
			CSettingsJID *m_Owner;
		};


	public:
		enum { IDD = IDD_SETTINGS_JID };

		CSettingsJID();
		~CSettingsJID();
		CJabberWiz *m_Jabber;
		BOOL m_TestSuccess;

		BEGIN_MSG_MAP(CSettingsJID)
//			ATLTRACE("msg1 %d\r\n", uMsg);
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			COMMAND_ID_HANDLER(IDB_BUTTON_JID1, OnBtnSearch)
			COMMAND_ID_HANDLER(IDC_EDIT_JID, OnJIDChange)
			COMMAND_ID_HANDLER(IDC_EDIT2_JID, OnJIDPassChange)
			COMMAND_ID_HANDLER(IDC_TEST_JID, OnBtnTest)
			COMMAND_ID_HANDLER(IDC_USESSLWRAPPER, OnBtnUseSSLWrapper)
			COMMAND_CODE_HANDLER(EN_CHANGE, OnChange)
		END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnBtnUseSSLWrapper(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnJIDChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnJIDPassChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};


	class CSettingsIcon   : public CDialogImpl<CSettingsIcon>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_PICTURE };

		CSettingsIcon();
		~CSettingsIcon();
		CListBox m_IconList;
		IMAGELIST m_Image;
		CxImage StaticImage;
		CxImage *SelectedImage, m_BaseImage;
		BOOL m_WasShown;

		BEGIN_MSG_MAP(CSettingsIcon)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
			MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			COMMAND_ID_HANDLER(IDC_CHOOSEICON, OnNickIcon)
			COMMAND_ID_HANDLER(IDC_ADDNEW, OnAddNewCmd)
		END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnIconChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		int FindFormat(const CString& ext);
		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void DrawNickIcon(HDC dc);

		LRESULT OnNickIcon(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnAddNewCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		void CalcRect(int imgwidth, int imgheight, int rcwidth, int rcheight, long *xpos, long *ypos, long *xwidth, long *xheight);
		LRESULT	OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT	OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void AddImage(int ImageId);
		LRESULT	OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
		void DoDefault(void);


	};
	class CSettingsEthernet   : public CDialogImpl<CSettingsEthernet>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_ETHERNET};

		CSettingsEthernet();
		~CSettingsEthernet();
		BOOL m_WasShown;

		BEGIN_MSG_MAP(CSettingsEthernet)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_CODE_HANDLER(EN_CHANGE, OnChange)
		END_MSG_MAP()


		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);


	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

		LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnEthernetChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
		void DoDefault(void);
	};
	class CSettingsNetworkFirewall   : public CDialogImpl<CSettingsNetworkFirewall>, public _CSettingsTemplate
	{
		
	public:
		enum { IDD = IDD_SETTINGS_NETWORK_FIREWALL};
		
		CSettingsNetworkFirewall();
		~CSettingsNetworkFirewall();
		CListBox m_Rules;

		typedef std::vector<FirewallStruct *> FIREWALLSTRUCT;
		FIREWALLSTRUCT m_FirewallRules;
		
		BEGIN_MSG_MAP(CSettingsNetworkFirewall)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDC_FIREWALLRULE_ADD, OnAddNewRule)
			COMMAND_ID_HANDLER(IDC_FIREWALLRULE_REMOVE, OnRemoveRule)
			COMMAND_CODE_HANDLER(LBN_SELCHANGE, OnChange)
			COMMAND_CODE_HANDLER(EN_CHANGE, OnChange)
			COMMAND_CODE_HANDLER(BN_CLICKED, OnChange)
		END_MSG_MAP()
			
			
		// Handler prototypes (uncomment arguments if needed):
		//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
		
		void PopulateList(void);
		LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnAddNewRule(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnRemoveRule(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
		void DoDefault(void);
	};
	class CSettingsMediator   : public CDialogImpl<CSettingsMediator>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_MEDIATOR};
		CListBox m_MediatorList;
		CImageList m_ImageList;


		CSettingsMediator();
		~CSettingsMediator();

		BEGIN_MSG_MAP(CSettingsMediator)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		END_MSG_MAP()



	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnMediatorChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsMTU   : public CDialogImpl<CSettingsMTU>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_MTU};

		CSettingsMTU();
		~CSettingsMTU();

		BEGIN_MSG_MAP(CSettingsMTU)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		END_MSG_MAP()



	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnMTUChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsContactsSort   : public CDialogImpl<CSettingsContactsSort>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_CONTACTS_SORT};

		CSettingsContactsSort();
		~CSettingsContactsSort();

		BEGIN_MSG_MAP(CSettingsContactsSort)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnMTUChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsVoiceChat   : public CDialogImpl<CSettingsVoiceChat>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_VOICECHAT};
		CComboBox m_Combo1, m_Combo2;

		CSettingsVoiceChat();
		~CSettingsVoiceChat();

		BEGIN_MSG_MAP(CSettingsVoiceChat)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		END_MSG_MAP()



	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnVoiceChatChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsAccounts   : public CDialogImpl<CSettingsAccounts>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_ACCOUNTS};
		CComBSTR2 m_ServiceName;
		BOOL m_ShowAllServices;
		CListBox m_ServicesList;

		char *AccRegisters[5];
		int AccRegister;
		BOOL m_RegVisible;

		CSettingsAccounts();
		~CSettingsAccounts();

		BEGIN_MSG_MAP(CSettingsAccounts)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			MESSAGE_HANDLER(WM_REFRESH, OnRefresh)
			MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
			MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			COMMAND_ID_HANDLER(IDC_LISTREGISTEREDGATEWAYS, OnListClick);
			COMMAND_ID_HANDLER(IDC_ADDNEWACCOUNT, OnAddNewAccount)
			COMMAND_ID_HANDLER(IDC_OPENNEWACCOUNT, OnOpenNewAccount)
			COMMAND_ID_HANDLER(IDC_REMOVEACCOUNT, OnRemoveAccount)
			COMMAND_ID_HANDLER(IDC_ACCOUNT_REGISTER, OnAccountRegister)
		END_MSG_MAP()


		int GetServiceType(char *t);
		LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);
		LRESULT OnRefresh(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
		void RefreshGatewaysList(void);

		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnOpenNewAccount(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnRemoveAccount(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnAccountRegister(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnAddNewAccount(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnListClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT	OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT	OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void SetAccRegisterWindowVisibility(int show);

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnAccountsChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsContactsHide   : public CDialogImpl<CSettingsContactsHide>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_CONTACTS_HIDE};
		CListBox m_HiddenContacts;

		BOOL m_Changed;

		CSettingsContactsHide();
		~CSettingsContactsHide();

		BEGIN_MSG_MAP(CSettingsContactsHide)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDC_BLOCKNEWCONTACT, OnBlockNewContact)
			COMMAND_ID_HANDLER(IDC_UNBLOCKCONTACT, OnUnblockContact)
			COMMAND_ID_HANDLER(IDC_HIDDENCONTACTS, OnListClick);
		END_MSG_MAP()


		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);
		LRESULT OnBlockNewContact(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnUnblockContact(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnListClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnAccountsChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsContactsAddRemove   : public CDialogImpl<CSettingsContactsAddRemove>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_CONTACTS_ADDREMOVE};

		CSettingsContactsAddRemove();
		~CSettingsContactsAddRemove();
		CComboBox m_Combo1, m_Combo2;

		BEGIN_MSG_MAP(CSettingsContactsAddRemove)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			COMMAND_ID_HANDLER(IDC_ADDNEWGROUP, OnAddNewGroup)
			COMMAND_ID_HANDLER(IDC_REMOVEGROUP, OnRemoveGroup)
			COMMAND_ID_HANDLER(IDC_CONTACTID, OnContactID)
			COMMAND_ID_HANDLER(IDC_ADDNEWCONTACT, OnAddNewContact)
		END_MSG_MAP()

		void RefreshGatewaysList(void);
		void RefreshGroupsList(void);

		LRESULT OnAddNewGroup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnRemoveGroup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnContactID(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnAddNewContact(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsLogging   : public CDialogImpl<CSettingsLogging>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_LOG};

		CSettingsLogging();
		~CSettingsLogging();

		BEGIN_MSG_MAP(CSettingsLogging)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDB_BROWSE, OnBrowseCmd)
			COMMAND_ID_HANDLER(IDB_BROWSE2, OnBrowse2Cmd)
			COMMAND_ID_HANDLER(IDB_BROWSE3, OnBrowse3Cmd)
			COMMAND_ID_HANDLER(IDB_BROWSE4, OnBrowse4Cmd)
		END_MSG_MAP()

		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnBrowseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnBrowse2Cmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnBrowse3Cmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnBrowse4Cmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsSound   : public CDialogImpl<CSettingsSound>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_SOUNDS};

		CSettingsSound();
		~CSettingsSound();


		char m_Sound[5][MAX_PATH];
		CListBox m_SoundList;
		BOOL m_IChange;

		BEGIN_MSG_MAP(CSettingsSound)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDC_SOUNDEVENTS, OnSoundList)
			COMMAND_ID_HANDLER(IDC_SOUND, OnEditSound)
			COMMAND_ID_HANDLER(IDB_BROWSE, OnBrowseCmd)
			COMMAND_ID_HANDLER(IDB_PLAY, OnPlayCmd)
		END_MSG_MAP()

		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnBrowseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnPlayCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnSoundList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnEditSound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsSkins   : public CDialogImpl<CSettingsSkins>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_SKINS};

		CSettingsSkins();
		~CSettingsSkins();

		CListBox m_SkinList;
		IMAGELIST m_Image;
		CxImage *SelectedImage, m_BaseImage;

		BEGIN_MSG_MAP(CSettingsSkins)
//		ATLTRACE("SettingsSkinMsg = %x\r\n", uMsg);
//			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_REFRESH, OnRefresh)
			MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
			MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
			COMMAND_ID_HANDLER(IDC_SKINLIST, OnSkinList)
			COMMAND_ID_HANDLER(IDB_MORESKINS, OnMoreSkins)
			COMMAND_ID_HANDLER(IDB_APPLY, OnApplyBtn)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		END_MSG_MAP()

		LRESULT	OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT	OnRefresh(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT	OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

//		void ShowSkinBitmap(char *filename);
//		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnSkinList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnMoreSkins(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnApplyBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsSystem   : public CDialogImpl<CSettingsSystem>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_SYSTEM};

		CSettingsSystem();
		~CSettingsSystem();

		BEGIN_MSG_MAP(CSettingsSystem)
//			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		END_MSG_MAP()

//		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsSystemUpdate   : public CDialogImpl<CSettingsSystemUpdate>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_SYSTEM_UPDATE};

		CSettingsSystemUpdate();
		~CSettingsSystemUpdate();

		BEGIN_MSG_MAP(CSettingsSystemUpdate)
//			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDC_UPDATE_CHECKNOW, OnCheckNow)
		END_MSG_MAP()

//		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnCheckNow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsAppearance   : public CDialogImpl<CSettingsAppearance>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_APPEARANCE};

		CSettingsAppearance();
		~CSettingsAppearance();

		BEGIN_MSG_MAP(CSettingsAppearance)
//			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDC_UPDATE_CHECKNOW, OnCheckNow)
		END_MSG_MAP()

//		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnCheckNow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsContacts   : public CDialogImpl<CSettingsContacts>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_CONTACTS};

		CSettingsContacts();
		~CSettingsContacts();

		BEGIN_MSG_MAP(CSettingsContacts)
//			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		END_MSG_MAP()

//		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsChatRooms   : public CDialogImpl<CSettingsChatRooms>, public _CSettingsTemplate
	{
		
	public:
		enum { IDD = IDD_SETTINGS_CHATROOMS};
		
		CSettingsChatRooms();
		~CSettingsChatRooms();
		
		CComboBox m_ServicesList, m_NewRoomServicesList;
		CListBox m_RoomList;

		BEGIN_MSG_MAP(CSettingsChatRooms)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDC_CHATROOM_GATEWAYLIST, OnGatewayListClick);
			COMMAND_ID_HANDLER(IDC_CHATROOM_GATEWAY2, OnChatRoom2Change);
			COMMAND_HANDLER(IDC_CHATROOM_ROOMNAME, EN_CHANGE, OnChange);
			NOTIFY_ID_HANDLER(IDC_CHATROOM_ROOMLIST, OmRoomList)				
			COMMAND_ID_HANDLER(IDC_CHATROOM_JOIN, OnButtonClick);
		END_MSG_MAP()
			
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnGatewayListClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnChatRoom2Change(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OmRoomList(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsAutoAway   : public CDialogImpl<CSettingsAutoAway>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_AUTOAWAY};

		CSettingsAutoAway();
		~CSettingsAutoAway();

		BEGIN_MSG_MAP(CSettingsAutoAway)
//			MESSAGE_HANDLER(WM_PAINT, OnPaint)
//			COMMAND_ID_HANDLER(IDC_SKINLIST, OnSkinList)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		END_MSG_MAP()

		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//		LRESULT OnSkinList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsMsgWin   : public CDialogImpl<CSettingsMsgWin>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_MSGWIN};

		CSettingsMsgWin();
		~CSettingsMsgWin();

		BEGIN_MSG_MAP(CSettingsMsgWin)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		END_MSG_MAP()



	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnMsgWinChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsUser1   : public CDialogImpl<CSettingsUser1>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_USER_1};

		CSettingsUser1();
		~CSettingsUser1();

		BEGIN_MSG_MAP(CSettingsUser1)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			COMMAND_CODE_HANDLER(EN_CHANGE, OnChange)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
		END_MSG_MAP()

		
		char m_Text2[256], m_Path[256];
#ifndef _WODXMPPLIB
		void InitData(WODXMPPCOMLib::IXMPPVCard *card);
#else
		void InitData(void  *card);
#endif
		void Lock(BOOL);
		CxImage m_Image;
		BOOL m_IsContact;


	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnMsgWinChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsUser2   : public CDialogImpl<CSettingsUser2>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_USER_2};

		CSettingsUser2();
		~CSettingsUser2();

		BEGIN_MSG_MAP(CSettingsUser2)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_CODE_HANDLER(EN_CHANGE, OnChange)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
		END_MSG_MAP()

		char m_Text2[256], m_Path[256];
#ifndef _WODXMPPLIB
		void InitData(WODXMPPCOMLib::IXMPPVCard *card);
#else
		void InitData(void *card);
#endif
		void Lock(BOOL);
		BOOL m_IsContact;



	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnMsgWinChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};
	class CSettingsUser3   : public CDialogImpl<CSettingsUser3>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_USER_3};

		CSettingsUser3();
		~CSettingsUser3();

		BEGIN_MSG_MAP(CSettingsUser3)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_CODE_HANDLER(EN_CHANGE, OnChange)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
		END_MSG_MAP()


		char m_Text2[256], m_Path[256];
#ifndef _WODXMPPLIB
		void InitData(WODXMPPCOMLib::IXMPPVCard *card);
#else
		void InitData(void *card);
#endif
		void Lock(BOOL);
		BOOL m_IsContact;

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnMsgWinChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};

	class CSettingsUser4   : public CDialogImpl<CSettingsUser4>, public _CSettingsTemplate
	{

	public:
		enum { IDD = IDD_SETTINGS_USER_4};

		CSettingsUser4();
		~CSettingsUser4();

		CFont m_BoldFont;
		
		BEGIN_MSG_MAP(CSettingsUser4)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			NOTIFY_ID_HANDLER(IDC_INTERFACELIST, OnInterfaceList)
			COMMAND_ID_HANDLER(IDC_SETTINGS_USER4_RESETALL, OnBtnResetAll)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
			MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
		END_MSG_MAP()

		char m_Text2[256];
		void InitData(void);
		void Lock(BOOL);


	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);		

		
		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnInterfaceList(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
		LRESULT OnBtnResetAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnBtnSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//		LRESULT OnMsgWinChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		void Show(BOOL Show, RECT *rc);
		void Init(HWND Owner);
		BOOL Apply(void);
	};

};

#endif // !defined(AFX_SETTINGSDLG_H__9A74142A_2617_4BBF_BC67_7D9EC43FCA0F__INCLUDED_)
