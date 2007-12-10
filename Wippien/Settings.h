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
//typedef std::vector<CUser *> DHCPLIST;
typedef std::vector<BSTR> REQUESTLIST;
typedef std::vector<CxImage *> IMAGESLIST;

extern const char *GROUP_GENERAL;
extern const char *GROUP_OFFLINE;

#define WIPPIENIM	"WippienIM3"
#define REFRESHUSERDETAILS			21600L // 6 hours


enum DONOTSHOW
{
	DONOTSHOW_NOETHERNET = 0,
	DONOTSHOW_NOALLOWEXIT,

	MAXDONOTSHOWANYMORESETTINGS
};


class CSettings  
{
public:
	CSettings();
	virtual ~CSettings();

	CComBSTR m_JID, m_Password, m_ServerHost;
	CComBSTR m_IPMediator, m_LinkMediator;
	long	m_LinkMediatorPort, m_ObtainIPAddress;
	CComBSTR m_JabberDebugFile, m_SocketDebugFile, m_FunctionDebugFile;
	CComBSTR m_Skin, m_UpdateURL;
	BOOL m_UseIPMediator, m_AllowLinkMediatorToBeProvidedByIPMediator, m_AllowAnyMediator, m_ShowMediatorOnContacts, m_SoundOn, m_LoadSuccess;
	BOOL m_TimestampMessages, m_ShowMessageHistory, m_ShowInTaskbar, m_SnapToBorder,
			m_DeleteContactsOnStartup, m_DeleteContactsOnConnect, m_AutoConnectVPNOnNetwork, m_AutoConnectVPNOnStartup;
	BOOL	m_UseSSLWrapper;
	int m_ServerPort, m_UDPPort, m_LastOperatorMessageID;
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
	unsigned long m_MyLastNetwork, m_MyLastNetmask; // in NETWORK ORDER!!
	
//	CComBSTR	m_MediatorAddr;

	BOOL m_EnableVoiceChat;
	unsigned long m_VoiceChatRecordingDevice, m_VoiceChatPlaybackDevice;

	Buffer m_HiddenContactsBuffer;
	typedef std::vector<long> HIDDENCONTACTS;
	HIDDENCONTACTS m_HiddenContacts;

	BOOL m_ShowContactPicture, m_ShowContactName, m_ShowContactIP, m_ShowContactStatus, m_ShowContactLastOnline;
	BOOL m_ShowMyPicture, m_ShowMyName, m_ShowMyIP, m_ShowMyStatus;

	int  m_SortContacts;

	CComBSTR m_SettingsFolder;
	char m_CfgFilename[MAX_PATH];
	char m_UserImagePath[MAX_PATH];
	char m_HistoryPath[MAX_PATH];
	char m_UsrFilename[MAX_PATH];
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
//		CComBSTR2 Name;
		char *Name;
		BOOL Open;
		HTREEITEM Item;
		int Count;
		int TotalCount;
	} TreeGroup;

	typedef std::vector<TreeGroup *> GROUPS;
	typedef std::vector<MenuTool *> MENUTOOLS;
	GROUPS m_Groups;
	MENUTOOLS m_MenuTools;
//	DHCPLIST m_DHCPAddress;
//	CUser *m_DHCPAddress[256];


	int Load(void);
	int LoadTools(void);
	BOOL CSettings::Save(BOOL UserOnly);
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

	// window stuff
	BOOL m_IsAligned, m_IsTopMost, m_DoAlign;
	RECT m_RosterRect;
	char m_RosterSnap;

	BOOL m_CheckUpdate, m_CheckUpdateSilently,m_PasswordProtectAll, m_NowProtected, m_UsePowerOptions;
	BOOL m_CheckUpdateConnect, m_CheckUpdateTimed;
	int m_CheckUpdateTimedNum;
	int m_AuthContacts;

	BOOL m_FixedMTU;
	int m_FixedMTUNum;

	CComBSTR m_PasswordProtectPassword;
};

#endif // !defined(AFX_SETTINGS_H__1DE4FC9D_C300_4D67_9DE6_DF3E02864843__INCLUDED_)
