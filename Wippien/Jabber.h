#ifndef __WIP_JABBER_H
#define __WIP_JABBER_H

#define WIPPIENINITREQUEST		"WippienInitRequest"
#define WIPPIENINITRESPONSE		"WippienInitResponse"
#define WIPPIENCONNECT			"WippienConnect"
#define WIPPIENDISCONNECT		"WippienDisconnect"
#define WIPPIENDETAILSTHREAD	"ExchangeDetailsThread"


#ifdef _WODXMPPLIB
namespace WODXMPPCOMLib
{
#include "\WeOnlyDo\wodXMPP\Code\Win32LIB\Win32LIB.h"
}

#else

// Define the ftp events to be handled:
extern _ATL_FUNC_INFO 
ConnectedInfo, 
DisconnectedInfo,
StateChangeInfo,
ContactStatusChangeInfo,
IncomingMessageInfo,
IncomingNotificationInfo,
ContactListInfo,
ServiceRegisterInfo,
ServiceStatusChangeInfo,
ContactAuthRequestInfo,
VCardDetailsInfo;

#endif

class CJabber;


#ifndef _WODXMPPLIB
class CJabberEvents : public IDispEventSimpleImpl<1, CJabberEvents, &__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents)>
{
public:

    CJabberEvents (CJabber * ppJ);
    virtual ~CJabberEvents ();
	void __stdcall DispContactAuthRequest(WODXMPPCOMLib::IXMPPContact *Contact, WODXMPPCOMLib::XMPPActionsEnum *Action);
    void __stdcall DispConnected ();
    void __stdcall DispDisconnected (long ErrorCode, BSTR ErrorText);
    void __stdcall DispStateChange(WODXMPPCOMLib::StatesEnum OldState);
    void __stdcall DispContactStatusChange(WODXMPPCOMLib::IXMPPContact *Contact, WODXMPPCOMLib::StatusEnum OldStatus);
	void __stdcall DispIncomingNotification(WODXMPPCOMLib::IXMPPContact *Contact, WODXMPPCOMLib::ContactNotifyEnum NotifyID, VARIANT Data);
    void __stdcall DispIncomingMessage(WODXMPPCOMLib::IXMPPContact *Contact, WODXMPPCOMLib::IXMPPMessage *Message);
    void __stdcall DispContactList();
    void __stdcall DispServiceRegister (WODXMPPCOMLib::IXMPPService *Service, VARIANT_BOOL Success, BSTR Instructions);
    void __stdcall DispServiceStatusChange (WODXMPPCOMLib::IXMPPService *Service);
    void __stdcall DispVCardDetails(WODXMPPCOMLib::IXMPPContact *Contact, VARIANT_BOOL Partial);
    
    BEGIN_SINK_MAP (CJabberEvents)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),0,DispConnected,&ConnectedInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),1,DispDisconnected,&DisconnectedInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),2,DispStateChange,&StateChangeInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),5,DispContactAuthRequest,&ContactAuthRequestInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),6,DispContactStatusChange,&ContactStatusChangeInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),7,DispIncomingMessage,&IncomingMessageInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),9,DispServiceStatusChange,&ServiceStatusChangeInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),10,DispServiceRegister,&ServiceRegisterInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),11,DispIncomingNotification,&IncomingNotificationInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),12,DispContactList,&ContactListInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODXMPPCOMLib::_IwodXMPPComEvents),13,DispVCardDetails,&VCardDetailsInfo)
        END_SINK_MAP ()
private:
	CJabber * m_pJ;
};

#endif

class Buffer;

class CJabber
{
public:

#ifndef _WODXMPPLIB
	CComPtr<WODXMPPCOMLib::IwodXMPPCom> m_Jabb;
#else
	void *m_Jabb;
#endif

	CJabber();
	virtual ~CJabber();
	void Connect(char *JID, char *pass, char *hostname, int port, BOOL sslwrapped);
	void Disconnect(void);
//	void Message(char *JID, char *MessageText, char *HtmlText);
	
#ifndef _WODXMPPLIB
	void Message(WODXMPPCOMLib::IXMPPContact *Contact, char *JID, char *MessageText, char *HtmlText);
#else
	void Message(void *Contact, char *JID, char *MessateText, char *HtmlText);
#endif

#ifndef _WODXMPPLIB
	void ChatRoomMessage(WODXMPPCOMLib::IXMPPChatRoom *Contact, char *MessageText, char *HtmlText);
#else
	void ChatRoomMessage(void *ChatRoom, char *MessateText, char *HtmlText);
#endif
	
	void ShowError(void);
	HWND	m_ServiceRegisterHwnd, m_ServiceRefreshHwnd;


	void ExchangeWippienDetails(char *JID, char *Subj, Buffer *Text);

#ifndef _WODXMPPLIB
	CJabberEvents *m_Events;
#else
	WODXMPPCOMLib::XMPPEventsStruct m_Events;
#endif
	BOOL m_Initial, m_DoReconnect;
	unsigned long m_ConnectTime;
};


#endif //__WIP_JABBER_H