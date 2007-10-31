#ifndef __WIP_JABBER_H
#define __WIP_JABBER_H

#define WIPPIENINITREQUEST		"WippienInitRequest"
#define WIPPIENINITRESPONSE		"WippienInitResponse"
#define WIPPIENCONNECT			"WippienConnect"
#define WIPPIENDISCONNECT		"WippienDisconnect"
#define WIPPIENDETAILSTHREAD	"ExchangeDetailsThread"

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

class CJabber;


class CJabberEvents : public IDispEventSimpleImpl<1, CJabberEvents, &__uuidof(WODJABBERCOMLib::_IwodJabberComEvents)>
{
public:

    CJabberEvents (CJabber * ppJ);
    virtual ~CJabberEvents ();
	void __stdcall DispContactAuthRequest(WODJABBERCOMLib::IJbrContact *Contact, WODJABBERCOMLib::JabberActionsEnum *Action);
    void __stdcall DispConnected ();
    void __stdcall DispDisconnected (long ErrorCode, BSTR ErrorText);
    void __stdcall DispStateChange(WODJABBERCOMLib::StatesEnum OldState);
    void __stdcall DispContactStatusChange(WODJABBERCOMLib::IJbrContact *Contact, WODJABBERCOMLib::StatusEnum OldStatus);
	void __stdcall DispIncomingNotification(WODJABBERCOMLib::IJbrContact *Contact, WODJABBERCOMLib::ContactNotifyEnum NotifyID, VARIANT Data);
    void __stdcall DispIncomingMessage(WODJABBERCOMLib::IJbrContact *Contact, WODJABBERCOMLib::IJbrMessage *Message);
    void __stdcall DispContactList();
    void __stdcall DispServiceRegister (WODJABBERCOMLib::IJbrService *Service, VARIANT_BOOL Success, BSTR Instructions);
    void __stdcall DispServiceStatusChange (WODJABBERCOMLib::IJbrService *Service);
    void __stdcall DispVCardDetails(WODJABBERCOMLib::IJbrContact *Contact, VARIANT_BOOL Partial);
    
    BEGIN_SINK_MAP (CJabberEvents)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),0,DispConnected,&ConnectedInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),1,DispDisconnected,&DisconnectedInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),2,DispStateChange,&StateChangeInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),5,DispContactAuthRequest,&ContactAuthRequestInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),6,DispContactStatusChange,&ContactStatusChangeInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),7,DispIncomingMessage,&IncomingMessageInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),9,DispServiceStatusChange,&ServiceStatusChangeInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),10,DispServiceRegister,&ServiceRegisterInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),11,DispIncomingNotification,&IncomingNotificationInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),12,DispContactList,&ContactListInfo)
        SINK_ENTRY_INFO (1,__uuidof(WODJABBERCOMLib::_IwodJabberComEvents),13,DispVCardDetails,&VCardDetailsInfo)
        END_SINK_MAP ()
private:
	CJabber * m_pJ;
};


class Buffer;

class CJabber
{
public:

	CComPtr<WODJABBERCOMLib::IwodJabberCom> m_Jabb;

	CJabber();
	virtual ~CJabber();
	void Connect(char *JID, char *pass, char *hostname, int port);
	void Disconnect(void);
//	void Message(char *JID, char *MessageText, char *HtmlText);
	void Message(WODJABBERCOMLib::IJbrContact *Contact, char *JID, char *MessageText, char *HtmlText);

	void ShowError(void);
	HWND	m_ServiceRegisterHwnd, m_ServiceRefreshHwnd;


	void ExchangeWippienDetails(char *JID, char *Subj, Buffer *Text);
	void ExchangeWippienDetails(WODJABBERCOMLib::IJbrContact *Contact, char *Subj, Buffer *Text);

	CJabberEvents *m_Events;
	BOOL m_Initial, m_DoReconnect;
	unsigned long m_ConnectTime;
};


#endif //__WIP_JABBER_H