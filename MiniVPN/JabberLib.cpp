#include "StdAfx.h"
#include "JabberLib.h"
#include "User.h"
#include "Ethernet.h"

// m_users could be in JabberLib,
// but if global it's safer when accessed in Ethernet class
USERLIST m_Users;
extern CEthernet *_Ethernet;
void SetStatus(char *Text);

#define WIPPIENRESOURCE			"WippienIM3"
#define MYRESOURCE				"MiniVPN"
#define WIPPIENINITREQUEST		"WippienInitRequest"
#define WIPPIENINITRESPONSE		"WippienInitResponse"
#define WIPPIENCONNECT			"WippienConnect"
#define WIPPIENDISCONNECT		"WippienDisconnect"
#define WIPPIENDETAILSTHREAD	"ExchangeDetailsThread"

static const char Base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char Pad64 = '=';


#define GET_32BIT(cp) (((u_long)(u_char)(cp)[0] << 24) | \
	((u_long)(u_char)(cp)[1] << 16) | \
	((u_long)(u_char)(cp)[2] << 8) | \
((u_long)(u_char)(cp)[3]))

#define PUT_32BIT(cp, value) do { \
	(cp)[0] = (value) >> 24; \
	(cp)[1] = (value) >> 16; \
	(cp)[2] = (value) >> 8; \
(cp)[3] = (value); } while (0)

extern char *hex;
int b64_ntop(u_char const *src, size_t srclength, char *target, size_t targsize) 
{
	/*[<][>][^][v][top][bottom][index][help] */
	size_t datalength = 0;
	u_char input[3];
	u_char output[4];
	size_t i;
	
	while (2 < srclength) 
	{
		input[0] = *src++;
		input[1] = *src++;
		input[2] = *src++;
		srclength -= 3;
		
		output[0] = input[0] >> 2;
		output[1] = ((input[0] & 0x03) << 4) + (input[1] >> 4);
		output[2] = ((input[1] & 0x0f) << 2) + (input[2] >> 6);
		output[3] = input[2] & 0x3f;
		
		if (datalength + 4 > targsize)
			return (-1);
		target[datalength++] = Base64[output[0]];
		target[datalength++] = Base64[output[1]];
		target[datalength++] = Base64[output[2]];
		target[datalength++] = Base64[output[3]];
	}
    
	/* Now we worry about padding. */
	if (0 != srclength)
	{
		/* Get what's left. */
		input[0] = input[1] = input[2] = '\0';
		for (i = 0; i < srclength; i++)
			input[i] = *src++;
        
		output[0] = input[0] >> 2;
		output[1] = ((input[0] & 0x03) << 4) + (input[1] >> 4);
		output[2] = ((input[1] & 0x0f) << 2) + (input[2] >> 6);
		
		if (datalength + 4 > targsize)
			return (-1);
		target[datalength++] = Base64[output[0]];
		target[datalength++] = Base64[output[1]];
		if (srclength == 1)
			target[datalength++] = Pad64;
		else
			target[datalength++] = Base64[output[2]];
		target[datalength++] = Pad64;
	}
	if (datalength >= targsize)
		return (-1);
	target[datalength] = '\0';      /* Returned value doesn't count \0. */
	return (datalength);
}


// Function name	: uuencode
// Description	    : 
// Return type		: int 
// Argument         : unsigned char *src
// Argument         : unsigned int srclength
// Argument         : char *target
// Argument         : size_t targsize
int uuencode(unsigned char *src, unsigned int srclength, char *target, size_t targsize)
{
	return b64_ntop(src, srclength, target, targsize);
}


BSTR AllocSysString(char *text)
{
	int l = strlen(text);
#if defined(_UNICODE) || defined(OLE2ANSI)
	BSTR bstr = ::SysAllocStringLen(text, l);
	if (bstr == NULL)
		return NULL;
#else
	int nLen = MultiByteToWideChar(CP_ACP, 0, text,l, NULL, NULL);
	BSTR bstr = ::SysAllocStringLen(NULL, nLen);
	if (bstr == NULL)
		return NULL;
	MultiByteToWideChar(CP_ACP, 0, text, l,bstr, nLen);
#endif

	return bstr;
}

CJabberLib::CJabberLib()
{
	memset(&m_Events, 0, sizeof(m_Events));
	m_Events.Connected = EventConnected;
	m_Events.Disconnected = EventDisconnected;
	m_Events.StateChange = EventStateChange;
	m_Events.ContactList = EventContactList;
	m_Events.ContactStatusChange = EventContactStatusChange;
	m_Events.IncomingMessage = EventIncomingMessage;

	m_Handle = WODXMPP::_XMPP_Create(&m_Events);
//	WODXMPP::XMPP_SetDebugFile(m_Handle, "c:\\jab.nfo");
	WODXMPP::XMPP_SetTag(m_Handle, this);
	m_JID[0] = 0;


	LVCOLUMN listCol;
	memset(&listCol, 0, sizeof(LVCOLUMN));
	listCol.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
	listCol.pszText = "JID";
	listCol.cx = 200;

	SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST,LVM_SETEXTENDEDLISTVIEWSTYLE,0,/*LVS_EX_BORDERSELECT | */LVS_EX_FULLROWSELECT /*| LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES*/); 

	SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST,LVM_INSERTCOLUMN,0,(LPARAM)&listCol);
	listCol.cx = 100;
	listCol.pszText="IP";                            
	SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST,LVM_INSERTCOLUMN,1,(LPARAM)&listCol);
	listCol.cx = 120;
	listCol.pszText="Status";
	SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST,LVM_INSERTCOLUMN,2,(LPARAM)&listCol);

	memset(&m_ContactLvItem, 0, sizeof(m_ContactLvItem));
	m_RSA = RSA_generate_key(1024,35,NULL,NULL);

}

CJabberLib::~CJabberLib(void)
{
	while (m_Users.size())
	{
		CUser *user = (CUser *)m_Users[0];
		delete user;
		m_Users.erase(m_Users.begin());
	}
	WODXMPP::_XMPP_Destroy(m_Handle);
	RSA_free(m_RSA);
}

// helpers
void ToHex(char *in, int len, char *out)
{
	while (len)
	{
		unsigned char b = (unsigned char)*in;
		*out = hex[b/16];
		out++;
		*out = hex[b%16];
		out++;

		in++;
		len--;
	}
	*out = 0;
}

int FromHex(char *in, char *out)
{
	char buff[3];
	buff[2] = 0;
	
	int len = 0;

	while (*in)
	{
		memcpy(buff, in, 2);
		strlwr(buff);
		
		char *s1, *s2;
		s1 = strchr(hex, buff[0]);
		s2 = strchr(hex, buff[1]);
		
		if (!s1 || !s2)
			return 0;
		
		buff[0] = (s1 - hex)*16 + (s2-hex);
		*out = buff[0];
		out++;
		len++;
		in++;
		if (*in)
			in++;
	}

	return len;
}

// events
void CJabberLib::EventConnected(void *wodXMPP)
{
	CJabberLib *me;
	if (!WODXMPP::XMPP_GetTag(wodXMPP, (void **)&me))
	{
		SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_DELETEALLITEMS, 0, 0);
	}

	// set status to 'do not disturb'
	WODXMPP::XMPP_SetStatus(wodXMPP, (WODXMPP::StatusEnum)/*DoNotDisturb*/4, "DND - I am not human");

}

void CJabberLib::EventDisconnected(void *wodXMPP, long ErrorCode, char *ErrorText)
{
	CJabberLib *me;
	if (!WODXMPP::XMPP_GetTag(wodXMPP, (void **)&me))
	{
		char buff[1024];
		sprintf(buff, "(XMPP) %s", ErrorText);
		SetStatus(buff);

		if (ErrorCode) // otherwise we clicked it...
		{
			if (ErrorCode != 30010)
			{
				HWND h = GetDlgItem(hMainWnd, IDC_CONNECT);
				::PostMessage(hMainWnd, WM_COMMAND,IDC_CONNECT,(LPARAM)h);
			}	
		}
	}
}


void CJabberLib::EventStateChange(void *wodXMPP, WODXMPP::StatesEnum OldState)
{
	CJabberLib *me;
	if (!WODXMPP::XMPP_GetTag(wodXMPP, (void **)&me))
	{
		int state = 0;
		if (!WODXMPP::XMPP_GetState(wodXMPP, (WODXMPP::StatesEnum *)&state))
		{
			char buff[1024];
			int bflen = sizeof(buff);
			if (!WODXMPP::XMPP_GetStateText(wodXMPP, (WODXMPP::StatesEnum)state, buff, &bflen))
			{
				SetStatus(buff);
			}
		}
	}
}
void CJabberLib::EventContactStatusChange(void *wodXMPP, void  *Contact, void *ChatRoom, WODXMPP::StatusEnum NewStatus, WODXMPP::StatusEnum OldStatus)
{
	LVITEM li1 = {0};
	CJabberLib *me;
	if (!WODXMPP::XMPP_GetTag(wodXMPP, (void **)&me))
	{
		char jid[1024];
		int jidsize = sizeof(jid);
		WODXMPP::XMPP_Contact_GetJID(Contact, jid, &jidsize);

		LVITEM *li = me->GetItemByJID(jid);
		if (!li)
		{
			CJabberLib *me;
			if (!WODXMPP::XMPP_GetTag(wodXMPP, (void **)&me))
			{
				
				CUser *user = new CUser();
				strcpy(user->m_JID, jid);
				/*me->*/m_Users.push_back(user);
				char *j = strchr(user->m_JID, '/');
				if (j)
					*j =0;
				jidsize = sizeof(sizeof(user->m_Resource));
				WODXMPP::XMPP_Contact_GetResource(Contact, user->m_Resource, &jidsize);

				li1.mask = LVIF_PARAM | LVIF_TEXT;
									
				li1.lParam = (LONG)user;
				li1.pszText = jid;
				li1.cchTextMax = strlen(jid);
									
				if (SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_INSERTITEM, 0, (LPARAM)&li1) != -1)
				{
					
					li1.mask = LVIF_TEXT;
					li1.iSubItem = 2;
					sprintf(jid, "Offline");
					SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_SETITEM, 0, (LPARAM)&li1);
					
					if (me->IsRemoteWippienUser(Contact))
					{
						SetTimer(user->m_hWnd, 1, rand()%2000, NULL); // TIMER1 do something
					}					
				}
				li = &li1;
			}
		}
		
		if (li)
		{
			WODXMPP::StatusEnum st;
			if (!WODXMPP::XMPP_Contact_GetStatus(Contact, &st))
			{
				char statusbuff[1024];
				int bflen = sizeof(statusbuff);
				if (!WODXMPP::XMPP_Contact_GetStatusText(Contact, statusbuff, &bflen))
				{
					li->mask = LVIF_TEXT;
					li->pszText = statusbuff;
					li->cchTextMax = bflen;
					li->iSubItem = 2;
					SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_SETITEM, 0, (LPARAM)li);

					CUser *us = me->GetUserByJID(jid);
					if (me->IsRemoteWippienUser(Contact))
					{
						if (st != 0) // disconnected
						{
							if (us)
								SetTimer(us->m_hWnd, 1, rand()%500, NULL); // TIMER1 do something
						}
						else
						{
							if (us)
							{
								// user left
								us->m_State = WipWaitingInitRequest;
								us->m_RemoteState = WipWaitingInitRequest;
							}
						}	
					}
				}
			}
		}
	}
}
void CJabberLib::EventContactList(void *wodXMPP)
{
	SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_DELETEALLITEMS, 0, 0);

}

void CJabberLib::EventIncomingMessage(void *wodXMPP, void  *Contact, void *ChatRoom, void  *Message)
{
	CJabberLib *me;
	if (!WODXMPP::XMPP_GetTag(wodXMPP, (void **)&me))
	{
		int type = 0;
		if (!WODXMPP::XMPP_Message_GetType(Message, (WODXMPP::MessageTypesEnum *)&type))
		{
			if (type == 3) // this is how we exchange messages
			{
				if (me->IsRemoteWippienUser(Contact))
				{
					char jid[1024];
					int jidsize = sizeof(jid);
					WODXMPP::XMPP_Contact_GetJID(Contact, jid, &jidsize);

					// let's locate this user
					CUser *us = me->GetUserByJID(jid);
					if (!us)
						return;

					// get JID
					char subjbuff[16384], data[16384];
					char dbg[1024];
					int bflen = sizeof(subjbuff);
					if (!WODXMPP::XMPP_Message_GetSubject(Message, subjbuff, &bflen))
					{
						if (!strcmp(subjbuff, WIPPIENINITREQUEST))
						{
							sprintf(dbg, "got initrequest from %s\r\n", us->m_JID);
							OutputDebugString(dbg);

							bflen = sizeof(subjbuff);
							if (!WODXMPP::XMPP_Message_GetText(Message, subjbuff, &bflen))
							{
								int len = FromHex(subjbuff, data);
								if (len)
								{

									unsigned long rip;
									memcpy(&rip, data, 4);
									us->m_RemoteIP = htonl(rip);
									memcpy(&us->m_MAC, data+4, 6);

									if (us->m_RSA)
										RSA_free(us->m_RSA);
									us->m_RSA = RSA_new();
									us->m_RSA->e = BN_new();
									us->m_RSA->n = BN_new();

									char *d = data + 10;
									len -= 10;
									unsigned long l = GET_32BIT(d);
									len -= 4;
									if (l && l<= (signed)len)
									{
										d += 4;
										BN_bin2bn((unsigned char *)d, l, us->m_RSA->e);
										d += l;
										len -= l;
									}
									
									l = GET_32BIT(d);
									len -= 4;
									if (l && l<=(signed)len)
									{
										d += 4;
										BN_bin2bn((unsigned char *)d, l, us->m_RSA->n);
										d += l;
										len -= l;
									}

									if (len>0)
									{
										us->m_RemoteState = (WippienState)(char)(*d);
										len--;
										d++;
									}


									if (len>0)
									{
										// ignore mediator, use only ours
										unsigned long l = GET_32BIT(d);
										len -= l;
										d+=l;
										len-=4;
										d+=4;
									}

									if (len>0)
									{
										us->m_HisRandom = GET_32BIT(d);
									}


									LVITEM *li = me->GetItemByJID(jid);
									if (li)
									{
										struct  in_addr sa1;
										sa1.S_un.S_addr = us->m_RemoteIP;
										li->pszText = inet_ntoa(sa1);
										li->cchTextMax = strlen(li->pszText);
										li->iSubItem = 1;
										li->mask = LVIF_TEXT;
										SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_SETITEM, 0, (LPARAM)li);
									}

									us->m_State = WipWaitingInitResponse;
									SetTimer(us->m_hWnd, 1, rand()%100, NULL);
								}
							}
						}
						else
						if (!strcmp(subjbuff, WIPPIENINITRESPONSE))
						{
							GetDlgItemText(hMainWnd, IDC_MEDIATOR, us->m_MediatorHost, sizeof(us->m_MediatorHost));
							us->m_MediatorPort = 8000;

							char hidmediatorhost[1024];
							int hismediatorport = 0;
							sprintf(dbg, "got initresponse from %s\r\n", us->m_JID);
							bflen = sizeof(subjbuff);
							if (!WODXMPP::XMPP_Message_GetText(Message, subjbuff, &bflen))
							{
								int len = FromHex(subjbuff, data);
								if (len>128)
									us->m_RemoteState = (WippienState)(char)data[128];
								if (len>=131)
								{
									char *d = data+129;
									int l = len-129;
									if (l)
									{
										int l1 = GET_32BIT(d);
										if (l1)
										{
											d+=4;
											memset(hidmediatorhost, 0, sizeof(hidmediatorhost));
											memcpy(hidmediatorhost, d, l1);
											d += l1;
											hismediatorport = GET_32BIT(d);
											d += 4;

											int hischoice = GET_32BIT(d);


											int ch = 0;
											if ((hischoice) % 2 != 0)
											{
												strcpy(us->m_MediatorHost, hidmediatorhost);
												us->m_MediatorPort = hismediatorport;
											}

										}
									}
								}

								if (len)
								{
									if (us->m_State == WipWaitingInitResponse)
									{
										char src[8192], dst[8192];
										memcpy(src, data, 128);
										if (us->m_State == WipWaitingInitResponse)
										{
											int ij = RSA_private_decrypt(128, (unsigned char *)src, (unsigned char *)dst, me->m_RSA, RSA_PKCS1_PADDING);
											if (ij < 0)
											{
												// something's wrong.. start all over
												us->m_State = WipWaitingInitRequest;
												us->m_RemoteState = WipWaitingInitRequest;
												return;
											}
										}

										// and XOR with ours
										for (int i = 0; i < 16; i++)
											us->m_SharedKey[i] = us->m_MyKey[i] ^ dst[i + 24];

										
										int klen = uuencode((unsigned char *)us->m_SharedKey, 16, dst, 8192);
										src[klen] = 0;

	//											EnterCriticalSection(&user->m_CritCS);
										WODVPN::VPN_Stop(us->m_Handle);
										WODVPN::VPN_SetPassword(us->m_Handle, dst);
	//											LeaveCriticalSection(&user->m_CritCS);

										us->m_State = WipDisconnected;
										SetTimer(us->m_hWnd, 1, rand()%100, NULL);
									}
									else
										me->ExchangeWippienDetails(us, FALSE);
								}
							}
						}
						else
						if (!strcmp(subjbuff, WIPPIENCONNECT))
						{
							sprintf(dbg, "got connect from %s\r\n", us->m_JID);
							bflen = sizeof(subjbuff);
							if (!WODXMPP::XMPP_Message_GetText(Message, subjbuff, &bflen))
							{
								int len = FromHex(subjbuff, data);
								if (len)
								{
									us->m_RemoteState = (WippienState)(char)data[0];
								}
								SetTimer(us->m_hWnd, 1, rand()%300, NULL);
							}
						}
						else
						if (!strcmp(subjbuff, WIPPIENDISCONNECT))
						{
							sprintf(dbg, "got disconnect from %s\r\n", us->m_JID);
							us->m_State = WipWaitingInitRequest;
							us->m_RemoteState = WipWaitingInitRequest;

//							SetTimer(us->m_hWnd, 1, rand()%300, NULL);
						}
					}
				}
			}
		}
	}
}

int CJabberLib::PutBignum(BIGNUM *value, char *data)
{
	int bytes = BN_num_bytes(value) + 1;
	unsigned char *buf = (unsigned char *)malloc(bytes);
	int oi;
	int hasnohigh = 0;
	buf[0] = '\0';

	oi = BN_bn2bin(value, buf+1);
	if (oi != bytes-1)
		return 0;

	hasnohigh = (buf[1] & 0x80) ? 0 : 1;
	if (value->neg) 
	{
		int i, carry;
		unsigned char *uc = buf;
		//		log("negativ!");
		for(i = bytes-1, carry = 1; i>=0; i--) 
		{
			uc[i] ^= 0xff;
			if(carry)
				carry = !++uc[i];
		}
	}
	PUT_32BIT(data, (bytes-hasnohigh));
	data += 4;
	memcpy(data, buf+hasnohigh, bytes-hasnohigh);
	memset(buf, 0, bytes);
	free(buf);

	return bytes-hasnohigh + 4;
}

void CJabberLib::ExchangeWippienDetails(CUser *us, BOOL NotifyConnect)
{
//	CUser *us = GetUserByContact(Contact);
	if (!us)
		return;
	
	char dbg[1024];
	char mediatorbuff[1024];

	char *subject = NULL;
	void *Message = NULL;
	char out[16384];
	if (us->m_State < WipDisconnected || us->m_RemoteState < WipDisconnected)
	{
		if (!us->m_RSA || us->m_State < WipWaitingInitResponse || us->m_RemoteState < WipWaitingInitResponse) // send WIPPIENINITREQUEST
		{
			char buff[16384];
			char *d = buff;
			unsigned long myip = htonl(_Ethernet->m_MyIP);
//			myip = 83886345;
			memcpy(d, &myip, 4);
			d+=4;
			memcpy(d, _Ethernet->m_MAC, 6);
			d+=6;
			if (!m_RSA->e) m_RSA->e = BN_new();
			if (!m_RSA->n) m_RSA->n = BN_new();
			d += PutBignum(m_RSA->e, d);
			d += PutBignum(m_RSA->n, d);
			*d++ = (char)us->m_State;

			GetDlgItemText(hMainWnd, IDC_MEDIATOR, mediatorbuff, sizeof(mediatorbuff));
			PUT_32BIT(d, 8+strlen(mediatorbuff));
			d+=4;
			PUT_32BIT(d, strlen(mediatorbuff));
			d+=4;
			memcpy(d, mediatorbuff, strlen(mediatorbuff));
			d+=strlen(mediatorbuff);
			int p = 8000;
			PUT_32BIT(d, p);
			d+=4;
			PUT_32BIT(d, us->m_MyRandom);
			d+=4;

			ToHex(buff, d-buff, out);
			subject = WIPPIENINITREQUEST;	
			sprintf(dbg, "sending initrequest to %s\r\n", us->m_JID);
			OutputDebugString(dbg);
		}
		else // send WIPPIENINITRESPONSE
		{
			if (us->m_State < WipDisconnected || us->m_RemoteState < WipDisconnected)
			{
				int i;
				char src[128 - RSA_PKCS1_PADDING_SIZE], dst[1024];
				for (i=0;i<sizeof(src);i++)
					src[i]=rand();
				memcpy(src + 24, us->m_MyKey, 16); // this is stupid, ok?...
				RSA_public_encrypt(128 - RSA_PKCS1_PADDING_SIZE, (unsigned char *)src, (unsigned char *)dst, us->m_RSA, RSA_PKCS1_PADDING);
				dst[128] = (char)us->m_State;
				char *d = dst+129;
				GetDlgItemText(hMainWnd, IDC_MEDIATOR, mediatorbuff, sizeof(mediatorbuff));
				PUT_32BIT(d, strlen(mediatorbuff));
				d += 4;
				memcpy(d, mediatorbuff, strlen(mediatorbuff));
				d += strlen(mediatorbuff);
				i = 8000;
				PUT_32BIT(d, i);
				d += 4;
				i = 0;
				PUT_32BIT(d, i);
				d += 4;

					
				ToHex(dst, d-dst, out);
				subject = WIPPIENINITRESPONSE;
				sprintf(dbg, "sending initresponse to %s\r\n", us->m_JID);
				OutputDebugString(dbg);
			}
		}
	}
	else
	{
		// let's try to connect

//		EnterCriticalSection(&m_CritCS);
		WODVPN::StatesEnum st;
		WODVPN::VPN_GetState(us->m_Handle, &st);
//		LeaveCriticalSection(&m_CritCS);
		
		if (st <= (WODVPN::StatesEnum)1/*::Listening*/ /*&& _Settings.m_MediatorAddr.Length()*/)
		{
			if (us->m_State < WipConnecting)
			{
				us->m_State = WipConnecting;

				us->m_BlinkCounter = 0;
				SetTimer(us->m_hWnd, 2, 200, NULL);
				
//				EnterCriticalSection(&m_CritCS);
				WODVPN::VPN_Stop(us->m_Handle);

				char myid[1024];
				strcpy(out, us->m_JID);
				char *slash = strchr(out, '/');
				if (slash)
					*slash = 0;

				if (us->m_MyRandom && us->m_HisRandom)
				{
					sprintf(myid, "%s_%s_%u_%s_%s_%u", m_JID, MYRESOURCE, us->m_MyRandom, out, WIPPIENRESOURCE, us->m_HisRandom);
				}
				else
				{
					sprintf(myid, "%s_%s", m_JID, out);
				}
				strlwr(myid);

				char hisid[1024];
				if (us->m_MyRandom && us->m_HisRandom)
				{
					sprintf(hisid, "%s_%s_%u_%s_%s_%u", out, WIPPIENRESOURCE, us->m_HisRandom, m_JID, MYRESOURCE, us->m_MyRandom);
				}
				else
				{
					sprintf(hisid, "%s_%s", out, m_JID);
				}
				strlwr(hisid);

				WODVPN::VPN_SetMyID(us->m_Handle, myid);



				VARIANT varhost;
				varhost.vt = VT_BSTR;
				varhost.bstrVal = AllocSysString(us->m_MediatorHost);

				VARIANT varport;
				varport.vt = VT_I4;
				varport.lVal = us->m_MediatorPort;
				
				VARIANT varempty;
				varempty.vt = VT_ERROR;
				
				char intbuff[1024] = {0};
				int ilen = sizeof(intbuff);
				intbuff[0] = ' ';
				ilen--;
				WODVPN::VPN_GetBindIP(us->m_Handle, intbuff, &ilen);
				if (ilen)
				{
					char net[1024];
					struct in_addr ina;
					ina.s_addr = _Ethernet->m_MyIP;
					strcpy(net, inet_ntoa(ina));
					// does it contain my IP inside
					char *c = strstr(intbuff, net);
					if (c)
					{
						char *d = c + strlen(net);
						if (*d == ' ')
							d++;
						MoveMemory(c, d, strlen(d)+1);
						WODVPN::VPN_SetBindIP(us->m_Handle, intbuff);
					}
				}

				WODVPN::VPN_SetRetryCount(us->m_Handle, 10);
				WODVPN::VPN_SetRetryWait(us->m_Handle, 1000);
				WODVPN::VPN_SetThreads(us->m_Handle, TRUE);
				WODVPN::VPN_Start(us->m_Handle, 0);
				WODVPN::VPN_Search(us->m_Handle, (WODVPN::SearchEnum)0, hisid, varhost, varport, varempty);

				::SysFreeString(varhost.bstrVal);
//				LeaveCriticalSection(&m_CritCS);

				if (NotifyConnect)
				{
					subject = WIPPIENCONNECT;
					char dst = (char)us->m_State;
					ToHex(&dst, 1, out);
				}
			}
		}
	}

	if (subject)
	{
		// get contact out

		Message = WODXMPP::XMPP_Message_New();
		WODXMPP::XMPP_Message_SetType(Message, (WODXMPP::MessageTypesEnum)3);
		WODXMPP::XMPP_Message_SetThread(Message, WIPPIENDETAILSTHREAD);
		WODXMPP::XMPP_Message_SetSubject(Message, subject);
		if (*out)
			WODXMPP::XMPP_Message_SetText(Message, out);
	
		
		WODXMPP::XMPP_SendMessage(m_Handle, us->m_JID, Message);
		WODXMPP::XMPP_Message_Free(Message);
	}

}

LVITEM *CJabberLib::GetItemByJID(char *jid)
{
	char jidbuff[1024], outbuff[1024];
	strcpy(jidbuff, jid);
//	char *s = strchr(jidbuff, '/');
//	if (s) *s = 0;

	if (!IsWindow(hMainWnd))
		return NULL;

	int count = SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_GETITEMCOUNT, 0, 0);
	if (count>0)
	{
		for (int i=0;i<count;i++)
		{
			memset(&m_ContactLvItem, 0, sizeof(m_ContactLvItem));
			m_ContactLvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_STATE;
			m_ContactLvItem.iItem = i;
			m_ContactLvItem.pszText = outbuff;
			m_ContactLvItem.cchTextMax = sizeof(outbuff);
			outbuff[0] = 0;
			if (SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_GETITEM, 0, (LPARAM)&m_ContactLvItem))
			{
				if (m_ContactLvItem.pszText && *m_ContactLvItem.pszText)
				{
					if (!stricmp(m_ContactLvItem.pszText, jidbuff))
						return &m_ContactLvItem;
				}
			}
		}
	}

	return NULL;
}

CUser *CJabberLib::GetUserByJID(char *jid)
{
	char jidbuff[1024];
	strcpy(jidbuff, jid);
	char *s = strchr(jidbuff, '/');
	if (s) *s = 0;

	// let's locate this user
	CUser *us = NULL;
	for (unsigned int i=0;i<m_Users.size();i++)
	{
		char *j = ((CUser *)m_Users[i])->m_JID;
		if (!stricmp( j, jidbuff))
		{
			us = (CUser *)m_Users[i];
			break;
		}
	}
	return us;
}

BOOL CJabberLib::IsRemoteWippienUser(void *Contact)
{
	// get JID
	char subjbuff[16384];
	int bflen = sizeof(subjbuff);
	subjbuff[0] = 0;
	if (!WODXMPP::XMPP_Contact_GetResource(Contact, subjbuff, &bflen))
	{
		if (!strcmp(subjbuff, WIPPIENRESOURCE))
		{
			return TRUE;
		}
	}
	bflen = sizeof(subjbuff);
	if (!WODXMPP::XMPP_Contact_GetCapabilities(Contact, subjbuff, &bflen))
	{
		if (strstr(subjbuff, WIPPIENRESOURCE))
		{
			return TRUE;
		}
	}
	return FALSE;
}

// methods
void CJabberLib::Disconnect(void)
{
	WODXMPP::XMPP_Disconnect(m_Handle);
}

void CJabberLib::Connect(char *JID, char *Password)
{
	int err;
	char buff[1024];
	buff[0] = 0;
	int bflen = sizeof(buff);
	if (!WODXMPP::XMPP_GetCapabilities(m_Handle, buff, &bflen))
	{
		if (!strstr(buff, WIPPIENRESOURCE))
		{
			strcat(buff, " ");
			strcat(buff, WIPPIENRESOURCE);
			WODXMPP::XMPP_SetCapabilities(m_Handle, buff);
		}
	}
	WODXMPP::XMPP_SetCombineResources(m_Handle, FALSE);


	sprintf(buff, "%s/%s", JID, MYRESOURCE);
	err = WODXMPP::XMPP_SetLogin(m_Handle, buff);

	strcpy(m_JID, JID);
	
	if (!err)
		err = WODXMPP::XMPP_SetPassword(m_Handle, Password);
	
	char *host = strchr(JID, '@');
	if (!host)
	{
		SetStatus("Invalid JID");
	}
	else
	{
		if (host)
		{
			host++;
			if (!stricmp(host, "gmail.com"))
				host = "talk.google.com";
			if (!err)
				err = WODXMPP::XMPP_Connect(m_Handle, host);
		}
		if (err)
		{
			int s = sizeof(buff);
			WODXMPP::XMPP_GetErrorText(m_Handle, err, buff, &s);
			SetStatus(buff);
			HWND h = ::GetDlgItem(hMainWnd, IDC_DISCONNECT);
			::PostMessage(hMainWnd, WM_COMMAND,IDC_DISCONNECT,(LPARAM)h);
		}
	}
}