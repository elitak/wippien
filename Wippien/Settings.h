// Settings.h: interface for the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGS_H__1DE4FC9D_C300_4D67_9DE6_DF3E02864843__INCLUDED_)
#define AFX_SETTINGS_H__1DE4FC9D_C300_4D67_9DE6_DF3E02864843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Buffer.h"
#include "proto.h"
#include "ComBSTR2.h"
#include "openssl/evp.h"
#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "Crypto_AES.h"
#include "../CxImage/CxImage/ximage.h"

#include <vector>

class CUser;
class CXmlEntity;
//typedef std::vector<CUser *> DHCPLIST;
typedef std::vector<BSTR> REQUESTLIST;
typedef std::vector<CxImage *> IMAGESLIST;

extern char *GROUP_GENERAL;
extern char *GROUP_OFFLINE;

#define WIPPIENIM3	"WippienIM3"
#define WIPPIENIM4	"WippienIM4"
#define REFRESHUSERDETAILS			21600L // 6 hours


enum DONOTSHOW
{
	DONOTSHOW_NOETHERNET = 0,
	DONOTSHOW_NOALLOWEXIT,

	MAXDONOTSHOWANYMORESETTINGS
};


typedef struct FirewallStruct
{
	int Proto;
	int Port;
} FirewallStruct;

class CSettings  
{
public:
	CSettings();
	virtual ~CSettings();

	CComBSTR m_JID, m_Password, m_ServerHost, m_Nick, m_Resource, m_StaticResource;
	long	m_ObtainIPAddress;
	CComBSTR m_JabberDebugFile, m_SocketDebugFile, m_VPNSocketDebugFolder, m_FunctionDebugFile;
	CComBSTR m_Skin, m_UpdateURL, m_IPProviderURL, m_Language;
	int m_LanguageFileVersion, m_LanguageEngFileVersion;
	BOOL m_AllowAnyMediator, m_SoundOn;
	BOOL m_TimestampMessages, m_ShowMessageHistory, m_ShowInTaskbar, m_SnapToBorder,
			m_DeleteContactsOnStartup, m_DeleteContactsOnConnect, m_AutoConnectVPNOnNetwork, m_AutoConnectVPNOnStartup;
	BOOL m_ShowNotificationPopup;
	BOOL	m_UseSSLWrapper, m_AutoHide, m_DisconnectEthernetOnExit;
	int m_AutoHideSeconds;
	int m_ServerPort, /*m_UDPPort, */m_LastOperatorMessageID;
	long m_DeleteFunctionLogMb;
	Buffer m_Icon;
	RSA *m_RSA;
	MACADDR m_MAC;
	REQUESTLIST m_AuthRequests;
	IMAGESLIST m_MenuImages;
	_cryptoAES128 m_AES;

	unsigned long m_AutoAwayMinutes, m_ExtendedAwayMinutes, m_AutoDisconnectMinutes;
	CComBSTR m_AutoAwayMessage, m_ExtendedAwayMessage;
	BOOL m_AutoSetBack;
	unsigned long m_MyLastNetwork, m_MyLastNetmask, m_Gateway; // in NETWORK ORDER!!
	BOOL m_UseGateway;
	
	Buffer m_HiddenContactsBuffer;
	typedef std::vector<long> HIDDENCONTACTS;
	HIDDENCONTACTS m_HiddenContacts;
	typedef std::vector<FirewallStruct *> FIREWALLSTRUCT;
	FIREWALLSTRUCT m_FirewallRules;
	BOOL m_FirewallDefaultAllowRule;

	BOOL m_ShowContactPicture, m_ShowContactName, m_ShowContactIP, m_ShowContactStatus, m_ShowContactLastOnline, m_ShowContactActivity;
	BOOL m_ShowMyPicture, m_ShowMyName, m_ShowMyIP, m_ShowMyStatus;

	int  m_SortContacts;

	Buffer m_LanguageEnglish;
	Buffer *m_LanguageOther;
	Buffer m_LanguageEnglishIndex, m_LanguageOtherIndex;
	unsigned int m_LanguageEnglishTotal;


	CComBSTR m_SettingsFolder;
	char m_CfgFilename[MAX_PATH];
	char m_LanguagePath[MAX_PATH];
	char m_UserImagePath[MAX_PATH];
	char m_HistoryPath[MAX_PATH];
	char m_UsrFilename[MAX_PATH];
	char m_ChatRoomFilename[MAX_PATH];
	char m_MyPath[MAX_PATH];
	char m_DoNotShow[MAXDONOTSHOWANYMORESETTINGS+1];

	typedef struct MenuTool
	{
		char *Menu;
		char *Exec;
		char *IconPath;
		int IconID;
		char *FilterGroup, *FilterJID, *FilterVisibleName;
//		PCHARLIST FilterGroup, FilterJID, FilterVisibleName;
		int State;
	} MenuTool;
	typedef struct TreeGroup
	{
		char *Name;
		char CountBuff[128];
		char *VisibleName;
		BOOL Open;
		BOOL Block;
		HTREEITEM Item;
		int Count;
		int TotalCount;
		BOOL Temporary;
	} TreeGroup;
	typedef struct LinkMediatorStruct
	{
		char *Host;
		int Port;
		unsigned long LastCheck;
		unsigned long LastSuccess;		
		BOOL Permanent;
	};

	typedef std::vector<TreeGroup *> GROUPS;
	typedef std::vector<MenuTool *> MENUTOOLS;
	typedef std::vector<LinkMediatorStruct *> LINKMEDIATORS;
	GROUPS m_Groups;
	MENUTOOLS m_MenuTools;
	LINKMEDIATORS m_LinkMediators;

	int LoadConfig(void);
	int LoadUsers(void);
	int LoadRooms(void);
	int LoadTools(void);
	BOOL SaveConfig(void);
	BOOL SaveUsers(void);
	BOOL SaveRooms(void);
	BOOL Save(void);
	void ToHex(Buffer *in, Buffer *out);
	void FromHex(Buffer *in, Buffer *out);
	void KeyToBlob(Buffer *out, BOOL withprivate);
//	void KeyFromBlob(Buffer *in);
	void PushGroupSorted(TreeGroup *grp);
	BOOL CheckPasswordProtect(void);
	void AESEncrypt(Buffer *in, Buffer *out);
	void AESDecrypt(Buffer *in, Buffer *out);
	void AESWrite(Buffer *out);
	void AESRead(Buffer *out);
	BOOL IsHiddenContact(char *contact);
	LinkMediatorStruct *AddLinkMediator(char *text, int Port);
	CSettings::TreeGroup *CSettings::GetGroupByName(char *Name);
	BOOL LoadLanguage(char *Language);
	BOOL LoadLanguageFile(char *Language, Buffer *temp);
	char *Translate(char *text);
	void FixCFormatting(char *in, char *out);


	// window stuff
	BOOL m_IsAligned, m_IsTopMost, m_DoAlign;
	RECT m_RosterRect;
	int m_RosterSnap;

	BOOL m_CheckUpdate, m_CheckUpdateSilently,m_PasswordProtectAll, m_NowProtected, m_UsePowerOptions;
	BOOL m_CheckUpdateConnect, m_CheckUpdateTimed, m_ShowUpdaterMessages;
	int m_CheckUpdateTimedNum;
	int m_AuthContacts;

	BOOL m_FixedMTU;
	int m_FixedMTUNum;

	CComBSTR m_PasswordProtectPassword;

	CXmlEntity *ReadSettingsCfg(CXmlEntity *own, char *Name, BOOL *Value, BOOL default_value);
	CXmlEntity *ReadSettingsCfg(CXmlEntity *own, char *Name, long *Value, long default_value);
	CXmlEntity *ReadSettingsCfg(CXmlEntity *own, char *Name, unsigned long *Value, unsigned long default_value);
	CXmlEntity *ReadSettingsCfg(CXmlEntity *own, char *Name, CComBSTR &Value, char *default_value);
	CXmlEntity *ReadSettingsCfg(CXmlEntity *own, char *Name, Buffer *Value);
	CXmlEntity *ReadSettingsCfg(CXmlEntity *own, char *Name, char *Value, char *default_value);

};

#endif // !defined(AFX_SETTINGS_H__1DE4FC9D_C300_4D67_9DE6_DF3E02864843__INCLUDED_)
