#ifndef __CJABBERLIB_H
#define __CJABBERLIB_H

#include "openssl/evp.h"
#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "resource.h"

namespace WODXMPP
{
#include "\WeOnlyDo\wodXMPP\Code\Win32LIB\Win32LIB.h"
}

#include <vector>
using namespace std;
typedef vector<void *> USERLIST;

class CUser;

class CJabberLib
{
public:
	CJabberLib();
	~CJabberLib();

	static void EventConnected(void *wodXMPP);
	static void EventDisconnected(void *wodXMPP, long ErrorCode, char *ErrorText);
	static void EventStateChange(void *wodXMPP, WODXMPP::StatesEnum OldState);
	static void EventContactStatusChange(void *wodXMPP, void  *Contact, void *ChatRoom, WODXMPP::StatusEnum NewStatus, WODXMPP::StatusEnum OldStatus);
	static void EventContactList(void *wodXMPP);
	static void EventIncomingMessage(void *wodXMPP, void  *Contact, void *ChatRoom, void  *Message);


	void Disconnect(void);
	void Connect(char *JID, char *Password);

#ifndef _WIPPIENSERVICE
	LVITEM *GetItemByJID(char *JID);
#endif
	CUser *GetUserByJID(char *JID);
	BOOL IsRemoteWippienUser(void *Contact);
	int PutBignum(BIGNUM *value, char *data);
	void ExchangeWippienDetails(CUser *user, BOOL NotifyConnect);

	WODXMPP::XMPPEventsStruct m_Events;
	void *m_Handle;
#ifndef _WIPPIENSERVICE
	LVITEM m_ContactLvItem;
#endif
	RSA *m_RSA;

	char m_JID[256];
};


#endif