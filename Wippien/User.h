// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__1D9D46E0_DE56_40D8_B139_5A49DF83F8D8__INCLUDED_)
#define AFX_USER_H__1D9D46E0_DE56_40D8_B139_5A49DF83F8D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "Buffer.h"
#include "proto.h"
#include "../CxImage/CxImage/ximage.h"
#include "openssl/evp.h"
#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "Crypto_AES.h"
#include "CwodWinSocket.h"

#ifdef _WODVPNLIB
namespace WODVPNCOMLib
{
#include "\WeOnlyDo\wodVPN\Code\Win32LIB\Win32LIB.h"
}
#endif

#include <vector>
typedef std::vector<void *> PACKETLIST;
class CxImage;
class CMsgWin;
class CSettingsDlg;
class CChatRoom;

typedef enum WippienState
{
	WipWaitingInitRequest,
	WipWaitingInitResponse,
	WipDisconnected,
	WipConnecting,
	WipNegotiating,
	WipConnected,
	WipUndefined
} WippienState;

extern const char *WippienStateString[];

typedef struct IPAddressConnectionStruct
{
	BOOL Allowed;
	BOOL Ignored;
	unsigned long Address;
} IPAddressConnectionStruct;
typedef std::vector<IPAddressConnectionStruct *> IPADDRESSLIST;

class CDetectMTU;

class CUser  :	public CComObjectRoot,
#ifndef _WODVPNLIB
				public IDispatchImpl<WODVPNCOMLib::IwodVPNNotify, &__uuidof(WODVPNCOMLib::IwodVPNNotify), &WODVPNCOMLib::LIBID_WODVPNCOMLib, /* wMajor = */ 1, /* wMinor = */ 0>,
#endif
				public CwodWinSocket
{
public:
	CUser();
	virtual ~CUser();

#ifdef _WODVPNLIB
	WODVPNCOMLib::VPNEventsStruct m_VPNEvents;
	void *	m_wodVPN;

#else
	CComPtr<WODVPNCOMLib::IwodVPNCom> m_wodVPN;
   
	BEGIN_COM_MAP(CUser)
		COM_INTERFACE_ENTRY(WODVPNCOMLib::IwodVPNNotify)
		{NULL, 0, 0}}; 
		return _entries;
	}//END_COM_MAP()
	
	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		return InternalAddRef();
	}; 
	virtual ULONG STDMETHODCALLTYPE Release( void)
	{
		ULONG l = InternalRelease();
		if (l == 0)
			delete this;
		return l;
	}
	STDMETHOD(QueryInterface)(REFIID, void**p)
	{
		p = (void **)this;
		return S_OK;
	}	

	STDMETHOD(raw_Connected)(WODVPNCOMLib::IwodVPNCom * Owner, BSTR PeerID, BSTR IP, LONG Port);
	STDMETHOD(raw_Disconnected)(WODVPNCOMLib::IwodVPNCom * Owner, LONG ErrorCode, BSTR ErrorText);
	STDMETHOD(raw_SearchDone)(WODVPNCOMLib::IwodVPNCom * Owner, BSTR IP, LONG Port, LONG ErrorCode, BSTR ErrorText);
	STDMETHOD(raw_IncomingData)(WODVPNCOMLib::IwodVPNCom * Owner, VARIANT Data);
	STDMETHOD(raw_StateChange)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::StatesEnum OldState){return E_NOTIMPL;}
	STDMETHOD(raw_ChannelStop)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::IVPNChannel * Chan, LONG ErrorCode, BSTR ErrorText){return E_NOTIMPL;}
	STDMETHOD(raw_ChannelStart)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::IVPNChannel * Chan, VARIANT_BOOL * Allow){return E_NOTIMPL;}
	STDMETHOD(raw_UserConnecting)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::IVPNChannel * Chan, BSTR Hostname, LONG Port, VARIANT_BOOL * Allow){return E_NOTIMPL;}
	STDMETHOD(raw_UserConnected)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::IVPNChannel * Chan, WODVPNCOMLib::IVPNUser * User){return E_NOTIMPL;}
	STDMETHOD(raw_UserDisconnected)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::IVPNChannel * Chan, WODVPNCOMLib::IVPNUser * User, LONG ErrorCode, BSTR ErrorText){return E_NOTIMPL;}
	STDMETHOD(raw_SocksConnect)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::IVPNChannel * Chan, WODVPNCOMLib::IVPNUser * User, BSTR RemoteAddress, LONG RemotePort, BSTR Login, BSTR Password, VARIANT_BOOL * Allow){return E_NOTIMPL;}
	STDMETHOD(raw_SocksBind)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::IVPNChannel * Chan, WODVPNCOMLib::IVPNUser * User, BSTR * RemoteAddress, LONG * RemotePort, BSTR Login, BSTR Password, VARIANT_BOOL * Allow){return E_NOTIMPL;}
	STDMETHOD(raw_IncomingText)(WODVPNCOMLib::IwodVPNCom * Owner, BSTR Message){return E_NOTIMPL;}
	STDMETHOD(raw_FindMTUDone)(WODVPNCOMLib::IwodVPNCom * Owner, LONG MTU, LONG ErrorCode, BSTR ErrorText){return E_NOTIMPL;}
	STDMETHOD(raw_RelayConnected)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::IVPNRelay * Relay, BSTR IP, LONG Port, BSTR ID){return E_NOTIMPL;}
	STDMETHOD(raw_RelayDisconnected)(WODVPNCOMLib::IwodVPNCom * Owner, WODVPNCOMLib::IVPNRelay * Relay, BSTR IP, LONG Port, BSTR ID){return E_NOTIMPL;}
	STDMETHOD(raw_MediatorConnected)(WODVPNCOMLib::IwodVPNCom * Owner, BSTR IP, LONG Port, BSTR FromID, BSTR ToID, BSTR * Data, VARIANT_BOOL * Allow){return E_NOTIMPL;}
	STDMETHOD(raw_MediatorDisconnected)(WODVPNCOMLib::IwodVPNCom * Owner, BSTR FromID, BSTR ToID){return E_NOTIMPL;}
	STDMETHOD(raw_MediatorExchangeData)(WODVPNCOMLib::IwodVPNCom * Owner, BSTR FromID, BSTR ToID){return E_NOTIMPL;}
	STDMETHOD(raw_UPnPMapping)(WODVPNCOMLib::IwodVPNCom *Owner, BSTR IntIP, long IntPort, BSTR ExtIP, long ExtPort){return E_NOTIMPL;}
	STDMETHOD(raw_DownloadFile)(WODVPNCOMLib::IwodVPNCom * Owner, BSTR *LocalFile, VARIANT_BOOL *Allow){return E_NOTIMPL;}
	STDMETHOD(raw_UploadFile)(WODVPNCOMLib::IwodVPNCom * Owner, BSTR *LocalFile, VARIANT_BOOL *Allow){return E_NOTIMPL;}
	STDMETHOD(raw_TransferProgress)(WODVPNCOMLib::IwodVPNCom * Owner, long Position, long Total){return E_NOTIMPL;}
	STDMETHOD(raw_Done)(WODVPNCOMLib::IwodVPNCom * Owner, BSTR RemotePath, long ErrorCode, BSTR ErrorText){return E_NOTIMPL;}


#endif //WODVPNLIB

	char m_JID[256];
	char m_Resource[256], m_LastResource[256];;
	char m_VisibleName[256];
	char m_Email[128];
	char m_SubText[256];
	char m_IPText[128];
	char m_Group[128];
	BOOL m_IsAway;
	char m_MyMediatorOffer[256], m_HisMediatorOffer[256];
	int m_MyMediatorPort, m_HisMediatorPort;
	int m_MyMediatorChoice, m_HisMediatorChoice;
	RECT m_ChatWindowRect;
	BOOL m_Block;//, m_StaticIP;
	unsigned long m_GotVCard;
//	Buffer m_Icon;
	CxImage *m_Image;
	_cryptoAES128 m_AES;
	int m_MTU;
	CDetectMTU *m_DetectMTU;
	BOOL m_Saved;
	unsigned long m_LastSent, m_LastReceive, m_TotalReceived, m_TotalSent;
	int m_DisconnectedTimer;

	char m_StatusText[128];
	WippienState m_WippienState, m_RemoteWippienState;
	char m_MyKey[16], m_SharedKey[16];
	MACADDR m_MAC;
	CSettingsDlg *m_SettingsContactsDlg;
	CRITICAL_SECTION m_CritCS;
	int m_BlinkTimerCounter, m_BlinkConnectingCounter;
	HTREEITEM m_TreeItem;
	unsigned long m_HisVirtualIP;

	BOOL m_Changed, m_ChangeNotify, m_Online, m_Hidden;
	unsigned long m_LastOnline;
	CChatRoom *m_ChatRoomPtr;
	//Buffer *m_IsWippien;
	BOOL m_IsUsingWippien;
	BOOL m_IsAlienWippien;


	void SetSubtext(void);
	void SetDebugLogFile(void);
	void DumpToFileFixed(char *text);
	void DumpToFile(char *text,...);


	CComBSTR m_RemoteAddr, m_Log;
	int m_RemotePort;//, /*m_LocalPort, */;
	RSA *m_RSA;
	Buffer m_RemoteIPList;
	int m_MyRandom, m_HisRandom;
	
	IPADDRESSLIST m_AllowedRemoteIPs;
	BOOL m_AllowedRemoteAny, m_AllowedRemoteMediator;
	
	
	BOOL LoadUserImage(CxImage *img);
	BOOL SaveUserImage(CxImage &img);
	BOOL SaveUserImage(char *data, int len);
	BOOL IsIPAllowed(unsigned long IP);
	void NotifyUserIsTyping(BOOL IsTyping);
	void ReInit(BOOL WithDirect);
	Buffer *ExpandArgs(char *Text);
	static Buffer *ExpandSystemArgs(Buffer *in);
	void SendConnectionRequest(BOOL Notify);
	BOOL ExchangeWippienDetails(void);
	void FdTimer(int timerid);
	void FdMTU(int MTU);
	BOOL SendNetworkPacket(char *data, int len);
	void ProcessNetworkData(char *buff, int len);
	void NotifyBlock(void);
	void NotifyConnect(void);
	void NotifyDisconnect(void);


	// GUI
	CMsgWin *m_MessageWin;
	void OpenMsgWindow(BOOL WithFocus);
	BOOL IsMsgWindowOpen(void);
	void CloseMsgWindow(void);
	void PrintMsgWindow(BOOL IsSystem, char *Text, char *Html);
};

#endif // !defined(AFX_USER_H__1D9D46E0_DE56_40D8_B139_5A49DF83F8D8__INCLUDED_)
