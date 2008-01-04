// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Jabber.h"
#include "Settings.h"
#include "Buffer.h"
#include "ComBSTR2.h"
#include "MainDlg.h"
#include "Ethernet.h"
#include "ContactAuthDlg.h"
#include "SDKMessageLink.h"

#ifdef _WODVPNLIB
namespace WODVPNCOMLib
{
#include "\WeOnlyDo\wodVPN\Code\Win32LIB\Win32LIB.h"
}
#endif
#ifdef _WODXMPPLIB
namespace WODXMPPCOMLib
{
#include "\WeOnlyDo\wodXMPP\Code\Win32LIB\Win32LIB.h"
}
#endif


CJabber *_Jabber = NULL;
extern CMainDlg _MainDlg;
extern CSettings _Settings;
extern CEthernet _Ethernet;
extern CContactAuthDlg *_ContactAuthDlg;
extern CSDKMessageLink *_SDK;

int uuencode(unsigned char *src, unsigned int srclength,char *target, size_t targsize);

#ifndef _WODXMPPLIB

// Define the jabber events to be handled:
_ATL_FUNC_INFO ConnectedInfo = {CC_STDCALL, VT_EMPTY, 0};
_ATL_FUNC_INFO DisconnectedInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_BSTR}};
_ATL_FUNC_INFO StateChangeInfo = {CC_STDCALL, VT_EMPTY, 1, {VT_I4}};
_ATL_FUNC_INFO ContactStatusChangeInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_I4}};
_ATL_FUNC_INFO IncomingMessageInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_DISPATCH}};
_ATL_FUNC_INFO IncomingNotificationInfo = {CC_STDCALL, VT_EMPTY, 3, {VT_DISPATCH, VT_I4, VT_VARIANT}};
_ATL_FUNC_INFO ContactListInfo = {CC_STDCALL, VT_EMPTY, 0};
_ATL_FUNC_INFO ServiceRegisterInfo = {CC_STDCALL, VT_EMPTY, 3, {VT_DISPATCH, VT_BOOL, VT_BSTR}};
_ATL_FUNC_INFO ServiceStatusChangeInfo = {CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH}};
_ATL_FUNC_INFO ContactAuthRequestInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_I4|VT_BYREF}};
_ATL_FUNC_INFO VCardDetailsInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH,VT_BOOL}};

CJabberEvents::CJabberEvents (CJabber * ppJ)
{
    m_pJ = ppJ;
    DispEventAdvise ( (IUnknown*)m_pJ->m_Jabb);
}

CJabberEvents::~CJabberEvents ()
{
    DispEventUnadvise ( (IUnknown*)m_pJ->m_Jabb);
}
#endif


#ifdef _WODXMPPLIB
void XMPPContactAuthRequest(void *wodXMPP, void  *Contact, WODXMPPCOMLib::XMPPActionsEnum *Action)
#else
void __stdcall CJabberEvents::DispContactAuthRequest(WODXMPPCOMLib::IXMPPContact *Contact, WODXMPPCOMLib::XMPPActionsEnum *Action)
#endif
{
	// add to list of auth requests
	CComBSTR2 j;

#ifndef _WODXMPPLIB
	if (SUCCEEDED(Contact->get_JID(&j)))
#else
	char bf[1024];
	int bfl = sizeof(bf);
	WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, bf, &bfl);
	j = bf;
#endif
	{
		char *jd1 = j.ToString();
		char *jd2 = strchr(jd1, '/');
		if (jd2)
			*jd2 = 0;
		
		BOOL found = FALSE;
		for (int i=0;!found && i<_Settings.m_AuthRequests.size();i++)
		{
			CComBSTR2 b = _Settings.m_AuthRequests[i];
			if (!stricmp(b.ToString(), jd1)) // if they are same
				found = TRUE;
		}

		if (!found)
		{
			CComBSTR j3 = jd1;
			_Settings.m_AuthRequests.push_back(j3.Copy());
		}
	}

	BOOL b;
	_MainDlg.OnBtnAuthDlg(NULL, NULL, NULL, b);

/*		// and now, if needed, reopen authdlg...
	if (!_ContactAuthDlg)
	{
		CContactAuthDlg *_ContactAuthDlg = new CContactAuthDlg();
		RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
		_ContactAuthDlg->Create(NULL, rcDefault, NULL);
	}
	if (_ContactAuthDlg)
		_ContactAuthDlg->ShowWindow(SW_SHOW);
*/

	*Action = (WODXMPPCOMLib::XMPPActionsEnum)2;//::SilentDeny);
}

#ifdef _WODXMPPLIB
void XMPPStateChange(void *wodXMPP, WODXMPPCOMLib::StatesEnum OldState)
#else
void __stdcall CJabberEvents::DispStateChange(WODXMPPCOMLib::StatesEnum OldState)
#endif
{
	int newst = 0;
#ifndef _WODXMPPLIB
	if (SUCCEEDED(_Jabber->m_Jabb->get_State((WODXMPPCOMLib::StatesEnum *)&newst)))
#else
	WODXMPPCOMLib::XMPP_GetState(wodXMPP, (WODXMPPCOMLib::StatesEnum *)&newst);
#endif

	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	WODXMPPCOMLib::StatusEnum newstatus;

#ifndef _WODXMPPLIB
	_Jabber->m_Jabb->get_Status(&newstatus);
	CComBSTR2 j;
	VARIANT var;
	var.vt = VT_ERROR;
	if (SUCCEEDED(_Jabber->m_Jabb->get_StatusText(var, &j)))
	{
		char buff[1024];
		strcpy(buff, j.ToString());
		j.Empty();
		if (SUCCEEDED(_Jabber->m_Jabb->get_StateText(var, &j)))
		{
			strcat(buff, " - ");
			strcat(buff, j.ToString());
		}
		_MainDlg.ShowStatusText(buff);
	}
#else
	WODXMPPCOMLib::XMPP_GetStatus(wodXMPP, &newstatus);
	char stat[1024], stet[1024];
	int slen = 1024;
	WODXMPPCOMLib::XMPP_GetStatusText(wodXMPP, stat, &slen);

	if (_MainDlg.IsWindow())
	{
		if (newstatus<7)
		{
			_MainDlg.ShellIcon(NIM_MODIFY, _MainDlg.m_OnlineStatus[newstatus], stat);
		}
		_MainDlg.Invalidate(FALSE);
	}

	strcat(stat, " - ");
	slen = 1024;
	WODXMPPCOMLib::XMPP_GetStateText(wodXMPP, (WODXMPPCOMLib::StatesEnum)newst, stet, &slen);
	strcat(stat, stet);
	_MainDlg.ShowStatusText(stat);

#endif

	// let's integrate SDK too
	if (_SDK)
	{
		{
			Buffer b;
			b.PutInt((int)OldState);
			b.PutInt((int)newst);
			if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_STATECHANGE, b.Ptr(), b.Len()))
				return;
		}
	}
	
}

#ifdef _WODXMPPLIB
void XMPPDisconnected(void *wodXMPP, long ErrorCode, char *ErrorText)
#else
void __stdcall CJabberEvents::DispDisconnected (long ErrorCode, BSTR ErrorText)
#endif
{
	
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	_Jabber->m_Initial = TRUE;
	
	if (_SDK)
	{
		Buffer b;
		b.PutCString(ErrorText);
		
		if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_DISCONNECTED, b.Ptr(), b.Len()))
			return;
	}
	
	
	CComBSTR2 b;
	if (ErrorCode)
	{
		b = ErrorText;
		_MainDlg.ShowStatusText(b.ToString());
		
	}
	else
		_MainDlg.ShowStatusText("Disconnected.");
	
	if (_Jabber->m_DoReconnect)
	{
		_MainDlg.m_ReconnectWait = 15;
		if (_MainDlg.IsWindow())
			_MainDlg.SetTimer(106,1000);
	}
	
	
	
	if (_MainDlg.IsWindow())
	{
		for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
		{
			CUser *us = (CUser *)_MainDlg.m_UserList.m_Users[i];
			us->m_Online = FALSE;
			us->m_Changed = TRUE;
		}
		_MainDlg.m_UserList.RefreshUser(NULL);
		//		_MainDlg.m_UserList.PostMessage(WM_REFRESH, NULL, FALSE);
	}
	_MainDlg.ShellIcon(NIM_MODIFY, _MainDlg.m_OnlineStatus[0], "Disconnected"); //offline
	
	
#ifdef _WODXMPPLIB
	XMPPStateChange(wodXMPP, (WODXMPPCOMLib::StatesEnum)1);
#else
	DispStateChange((WODXMPPCOMLib::StatesEnum)1)
#endif
}

#ifdef _WODXMPPLIB
void XMPPConnected(void *wodXMPP)
#else
void __stdcall CJabberEvents::DispConnected ()
#endif
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	_Jabber->m_ConnectTime = GetTickCount();
//		_MainDlg.ShowStatusText("Connected.");
#ifndef _WODXMPPLIB
/*	WODXMPPCOMLib::IXMPPServices *serv = NULL;
	if (SUCCEEDED(_Jabber->m_Jabb->get_Services(&serv)))
	{
		serv->Discover(TRUE);
		serv->Release();
	}
*/
	// and request vcard
	WODXMPPCOMLib::IXMPPVCard *vc;
	if (SUCCEEDED(_Jabber->m_Jabb->get_VCard(&vc)))
	{
		vc->Receive();
		vc->Release();
	}
	DispStateChange((WODXMPPCOMLib::StatesEnum)0);
#else 
//	WODXMPPCOMLib::XMPP_ServicesDiscover(_Jabber->m_Jabb, TRUE);
	WODXMPPCOMLib::XMPP_VCardReceive(_Jabber->m_Jabb);
#endif

	if (_Settings.m_DeleteContactsOnConnect)
	{
		_MainDlg.m_UserList.DeleteAllItems();
		// we should now delete all our contacts
		while (_MainDlg.m_UserList.m_Users.size())
		{
			CUser *user = _MainDlg.m_UserList.m_Users[0];
			_MainDlg.m_UserList.m_Users.erase(_MainDlg.m_UserList.m_Users.begin());
#ifdef _WODVPNLIB
			delete user;
#else
			user->Release();
#endif						
		}
	}

	
	_MainDlg.ShellIcon(NIM_MODIFY, _MainDlg.m_OnlineStatus[1], "Connected"); //online
	if (_SDK)
	{
		if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_CONNECTED,""))
			return;
	}


#ifdef _WODXMPPLIB
	XMPPStateChange(wodXMPP, (WODXMPPCOMLib::StatesEnum)0);
#else
	DispStateChange((WODXMPPCOMLib::StatesEnum)0)
#endif

}
#ifdef _WODXMPPLIB
void XMPPContactStatusChange(void *wodXMPP, void  *Contact, WODXMPPCOMLib::StatusEnum OldStatus)
#else
void __stdcall CJabberEvents::DispContactStatusChange(WODXMPPCOMLib::IXMPPContact *Contact, WODXMPPCOMLib::StatusEnum OldStatus)
#endif
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	if (!_Jabber->m_Initial)
	{

		if (_SDK)
		{
			CComBSTR j;

			WODXMPPCOMLib::StatusEnum st;
#ifndef _WODXMPPLIB
			Contact->get_JID(&j);
			Contact->get_Status(&st);
#else
			char bf[1024];
			int bflen = sizeof(bf);
			WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, bf, &bflen);
			j = bf;
			WODXMPPCOMLib::XMPP_Contact_GetStatus(Contact, &st);
#endif

			Buffer b;
			b.PutCString(j);
			b.PutInt((int)st);
			b.PutInt((int)OldStatus);
			if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_USERPRESENCECHANGE, b.Ptr(), b.Len()))
				return;
		}
		
		_MainDlg.m_UserList.RefreshUser(Contact);
//		_MainDlg.m_UserList.PostMessage(WM_REFRESH, 0, (LPARAM)Contact);
//		m_UserList.PostMessage(, NULL, 0);
	}
//		m_pJ->m_Owner->ParseUsers(m_pJ->m_Jabb);
}
#ifdef _WODXMPPLIB
void XMPPIncomingNotification(void *wodXMPP, void  *Contact, WODXMPPCOMLib::ContactNotifyEnum NotifyID, VARIANT Data)
#else
void __stdcall CJabberEvents::DispIncomingNotification(WODXMPPCOMLib::IXMPPContact *Contact, WODXMPPCOMLib::ContactNotifyEnum NotifyID, VARIANT Data)
#endif
{
#ifndef _WODXMPPLIB
	CComBSTR2 j;
	Contact->get_JID(&j);

	char *jd1 = j.ToString();
	char *jd2 = strchr(jd1, '/');
	if (jd2)
		*jd2 = 0;
#else
	char jd1[1024];
	int jl = sizeof(jd1);
	WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, jd1, &jl);
#endif

	_MainDlg.OnIncomingNotification(jd1, (int)NotifyID, Data);
}
#ifdef _WODXMPPLIB
void XMPPIncomingMessage(void *wodXMPP, void  *Contact, void  *Message)
#else
void __stdcall CJabberEvents::DispIncomingMessage(WODXMPPCOMLib::IXMPPContact *Contact, WODXMPPCOMLib::IXMPPMessage *Message)
#endif
{
	if (Contact)
	{
		WODXMPPCOMLib::MessageTypesEnum msgtype = (WODXMPPCOMLib::MessageTypesEnum)0;
		if (Message)
#ifndef _WODXMPPLIB
			Message->get_Type(&msgtype);
#else
		WODXMPPCOMLib::XMPP_Message_GetType(Message, &msgtype);
#endif

		// is it from mediator?
		CComBSTR jid;
		CComBSTR2 jd;
#ifndef _WODXMPPLIB
		if (SUCCEEDED(Contact->get_JID(&jd)))
#else
		char jdbf[1024];
		int jdbflen = sizeof(jdbf);
		WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, jdbf, &jdbflen);
		jd = jdbf;
#endif
		{
			char *jd1 = jd.ToString();
			if (_Settings.IsHiddenContact(jd1))
				return;

			char *jd2 = strchr(jd1, '/');
			if (jd2)
				*jd2 = 0;
			jid = jd1;

		}
		
		// is it headline
//		WODXMPPCOMLib::MessageTypesEnum ms;

//		if (SUCCEEDED(Message->get_Type(&ms)))
		{
			if (msgtype == (WODXMPPCOMLib::MessageTypesEnum)3)//::MsgHeadline)
			{
				ATLTRACE("Got message from remote peer\r\n");
				CComBSTR subj;
#ifndef _WODXMPPLIB
				if (SUCCEEDED(Message->get_Subject(&subj)))
#else
				char subjbuff[1024];
				int subjbufflen = sizeof(subjbuff);
				WODXMPPCOMLib::XMPP_Message_GetSubject(Message, subjbuff, &subjbufflen);
				subj = subjbuff;
#endif
				{
					if (subj == WIPPIENINITREQUEST && Contact)
					{
						if (!_Ethernet.m_Available) return; // ignore if ethernet is not available

						Buffer in, out;
						CComBSTR2 r;
#ifndef _WODXMPPLIB
						if (SUCCEEDED(Message->get_Text(&r)))
#else
						subjbufflen = sizeof(subjbuff);
						WODXMPPCOMLib::XMPP_Message_GetText(Message, subjbuff, &subjbufflen);
						r = subjbuff;
#endif
						{
							char *r1 = r.ToString();
							in.Append(r1);

							_Settings.FromHex(&in, &out);

							// save this for user
							CComBSTR j;
#ifndef _WODXMPPLIB
							if (SUCCEEDED(Contact->get_JID(&j)))
#else
							subjbufflen = sizeof(subjbuff);
							WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, subjbuff, &subjbufflen);
							j = subjbuff;
#endif
							{
								CUser *user = _MainDlg.m_UserList.GetUserByJID(j);
								if (user)
								{
									ATLTRACE("Got WIPPIENINITREQUEST from %s\r\n", user->m_JID);
									Buffer b;
									b.Append(user->m_Resource);
									b.Append("\r\n");
									char *line;
									do 
									{
										line = b.GetNextLine();
										if (line && !strncmp(line, WIPPIENIM, strlen(WIPPIENIM)))
										{	
											if (out.Len()>=4)
											{
												user->m_HisVirtualIP = out.GetInt();
												if (out.Len()>=6)
												{
													memcpy(user->m_MAC, out.Ptr(), 6);
													out.Consume(6);
												}
												else
												{
													// calculate MAC too
													if (!memcmp(user->m_MAC, "\0\0\0\0\0\0", 6))
													{
														user->m_MAC[0] = _Settings.m_MAC[0];
														user->m_MAC[1] = _Settings.m_MAC[1];
														memcpy(&user->m_MAC[2], &user->m_HisVirtualIP, 4);
													}
												}

												//  also private key
												if (user->m_RSA)
													RSA_free(user->m_RSA);
												user->m_RSA = RSA_new();

												user->m_RSA->e = BN_new();
												out.GetBignum2(user->m_RSA->e);
												user->m_RSA->n = BN_new();
												out.GetBignum2(user->m_RSA->n);

												if (out.Len())
													user->m_RemoteWippienState = (WippienState)out.GetChar();

												user->m_Changed = TRUE;

											}
											user->m_WippienState = WipWaitingInitResponse;
											user->SetTimer(rand()%100, 3);
											break;
										}	
									} while (line);
								}
							}
						}
					}
					if (subj == WIPPIENINITRESPONSE && Contact)
					{
						if (!_Ethernet.m_Available) return; // ignore if ethernet is not available

						Buffer in, out;
						CComBSTR2 r;
#ifndef _WODXMPPLIB
						if (SUCCEEDED(Message->get_Text(&r)))
#else
						char textbuff[1024];
						int tblen = sizeof(textbuff);
						WODXMPPCOMLib::XMPP_Message_GetText(Message, textbuff, &tblen);
						r = textbuff;
#endif
						{
							char *r1 = r.ToString();
							in.Append(r1);

							_Settings.FromHex(&in, &out);

							// save this for user
							CComBSTR j;
#ifndef _WODXMPPLIB
							if (SUCCEEDED(Contact->get_JID(&j)))
#else
							tblen = sizeof(textbuff);
							WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, textbuff, &tblen);
							j = textbuff;
#endif
							{
								CUser *user = _MainDlg.m_UserList.GetUserByJID(j);
								if (user)
								{
									ATLTRACE("Got WIPPIENINITRESPONSE from %s\r\n", user->m_JID);
									char src[8192], dst[8192];
									memcpy(src, out.Ptr(), 128);
									if (RSA_private_decrypt(128, (unsigned char *)src, (unsigned char *)dst,  _Settings.m_RSA, RSA_PKCS1_PADDING) < 0)
									{
										user->m_WippienState = WipDisconnected;
										user->NotifyDisconnect();
										return;
									}

									out.Consume(128);
									if (out.Len())
										user->m_RemoteWippienState = (WippienState)out.GetChar();

									// and XOR with ours
									for (int i = 0; i < 16; i++)
										user->m_SharedKey[i] = user->m_MyKey[i] ^ dst[i + 24];

									int klen = uuencode((unsigned char *)user->m_SharedKey, 16, dst, 8192);
									src[klen] = 0;
									EnterCriticalSection(&user->m_CritCS);
#ifdef _WODVPNLIB
									WODVPNCOMLib::VPN_Stop(user->m_wodVPN);
									WODVPNCOMLib::VPN_SetPassword(user->m_wodVPN, dst);
#else
									user->m_wodVPN->raw_Stop();
									user->m_wodVPN->Password = dst;
//									user->m_wodVPN->Start(0);
#endif
									LeaveCriticalSection(&user->m_CritCS);

									user->m_WippienState = WipDisconnected;
									user->SetTimer(rand()%100, 3);
									if (_Settings.m_AutoConnectVPNOnStartup)
										user->SendConnectionRequest(TRUE);

								}
							}
						}
					}
					if (subj == WIPPIENDISCONNECT && Contact)
					{
						ATLTRACE("Got DISCONNECT message\r\n");
						// reinit...
						CComBSTR j;
#ifndef _WODXMPPLIB
						if (SUCCEEDED(Contact->get_JID(&j)))
#else
						char tb[1024];
						int tblen = sizeof(tb);
						WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, tb, &tblen);
						j = tb;
#endif
						{
							CUser *user = _MainDlg.m_UserList.GetUserByJID(j);
							if (user)
							{
								user->ReInit(TRUE);
							}
						}
					}
					if (subj == WIPPIENCONNECT && Contact)
					{
						// reinit...
						CComBSTR j;
#ifndef _WODXMPPLIB
						if (SUCCEEDED(Contact->get_JID(&j)))
#else
						char tb[1024];
						int tblen = sizeof(tb);
						WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, tb, &tblen);
						j = tb;
#endif
						{
							CUser *user = _MainDlg.m_UserList.GetUserByJID(j);
							if (user)
							{
								ATLTRACE("Got CONNECT message from %s\r\n", user->m_JID);
								Buffer in, out;
								CComBSTR2 r;
#ifndef _WODXMPPLIB
								if (SUCCEEDED(Message->get_Text(&r)))
#else
								tblen = sizeof(tb);
								WODXMPPCOMLib::XMPP_Message_GetText(Message, tb, &tblen);
								r = tb;
#endif
								{
									char *r1 = r.ToString();
									if (r1)
									{
										in.Append(r1);
										if (in.Len())
										{		
											_Settings.FromHex(&in, &out);
											if (out.Len())
											{
												user->m_RemoteWippienState = (WippienState)out.GetChar();
											}
										}
									}
								}
							}
							if (user && !user->m_Block)
							{
								user->SendConnectionRequest(FALSE);

/*								switch (user->m_WippienState)
								{	
									case WipDisconnected:	
										if (user->m_DidSendRequest && user->m_DidSendResponse)
										{
											user->SendConnectionRequest(FALSE);
											break;
										} // intentionally left break

									case WipWaitingInitRequest:
									case WipWaitingInitResponse:
										user->SetTimer(rand()%100, 3);
										break;
										
									case WipConnecting:
										break; // in the process

									case WipConnected:
										break;

//									default:
//										MessageBeep(-1);
								}
*/
							}
						}
					}			
				}
			}
			else
			{
				CComBSTR2 j;
#ifndef _WODXMPPLIB
				Contact->get_JID(&j);
#else
				char tb[1024];
				int tblen = sizeof(tb);
				WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, tb, &tblen);
				j = tb;
#endif
				if (msgtype == (WODXMPPCOMLib::MessageTypesEnum)4)//::error)
				{
					if (Message)
					{
						BOOL show = TRUE;
						CComBSTR2 j1;
#ifndef _WODXMPPLIB
						Message->get_Thread(&j1);
#else
						tblen = sizeof(tb);
						WODXMPPCOMLib::XMPP_Message_GetThread(Message, tb, &tblen);
						j1 = tb;
#endif
						if (j1.Length())
						{
							char *j2 = j1.ToString();
							if (strcmp(j2, WIPPIENDETAILSTHREAD))
								_MainDlg.OnIncomingMessage(j.ToString(), "Could not deliver message.", "");
						}
					}
				}
				else
				{
					CComBSTR2 t;
#ifndef _WODXMPPLIB
					Message->get_Text(&t);
#else
					tblen = sizeof(tb);
					WODXMPPCOMLib::XMPP_Message_GetText(Message, tb, &tblen);
					t = tb;
#endif
					CComBSTR2 ht;
#ifndef _WODXMPPLIB
					Message->get_HTMLText(&ht);
#else
					tblen = sizeof(tb);
					WODXMPPCOMLib::XMPP_Message_GetHTMLText(Message, tb, &tblen);
					ht = tb;
#endif
					_MainDlg.OnIncomingMessage(j.ToString(), t.ToString(), ht.ToString());
				}
			}
		}
		
	}
}
#ifdef _WODXMPPLIB
void XMPPContactList(void *wodXMPP)
#else
void __stdcall CJabberEvents::DispContactList()
#endif
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	_Jabber->m_Initial = FALSE;
	_MainDlg.m_UserList.RefreshUser(NULL);
}
#ifdef _WODXMPPLIB
void XMPPServiceRegister(void *wodXMPP, void  *Service, BOOL Success, char *Instructions)
#else
void __stdcall CJabberEvents::DispServiceRegister (WODXMPPCOMLib::IXMPPService *Service, VARIANT_BOOL Success, BSTR Instructions)
#endif
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	CComBSTR2 i = Instructions;
	if (_Jabber->m_ServiceRegisterHwnd)
	{
		SetWindowText(_Jabber->m_ServiceRegisterHwnd, i.ToString());
	}
}
#ifdef _WODXMPPLIB
void XMPPServiceStatusChange(void *wodXMPP, void  *Service)
#else
void __stdcall CJabberEvents::DispServiceStatusChange (WODXMPPCOMLib::IXMPPService *Service)
#endif
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	if (_Jabber->m_ServiceRefreshHwnd)
	{
		PostMessage(_Jabber->m_ServiceRefreshHwnd, WM_REFRESH, 0, 0);
	}
}
#ifdef _WODXMPPLIB
void XMPPVCardDetails(void *wodXMPP, void  *Contact, BOOL Partial)
#else
void __stdcall CJabberEvents::DispVCardDetails(WODXMPPCOMLib::IXMPPContact *Contact, VARIANT_BOOL Partial)
#endif
{
//	if (Contact)
//	{
		_MainDlg.m_UserList.OnVCard(Contact, Partial?TRUE:FALSE, TRUE);
//	}
}

CJabber::CJabber(void)
{
#ifdef _WODXMPPLIB
	memset(&m_Events, 0, sizeof(m_Events));

	m_Events.Connected = XMPPConnected;
	m_Events.Disconnected = XMPPDisconnected;
	m_Events.StateChange = XMPPStateChange;
	m_Events.ContactStatusChange = XMPPContactStatusChange;
	m_Events.IncomingMessage = XMPPIncomingMessage;
	m_Events.IncomingNotification = XMPPIncomingNotification;
	m_Events.ContactList = XMPPContactList;
	m_Events.ServiceRegister = XMPPServiceRegister;
	m_Events.ServiceStatusChange = XMPPServiceStatusChange;
	m_Events.ContactAuthRequest = XMPPContactAuthRequest;
	m_Events.VCardDetails = XMPPVCardDetails;

		
	m_Jabb = WODXMPPCOMLib::_XMPP_Create(&m_Events);
#else
	m_Jabb.CoCreateInstance(__uuidof(WODXMPPCOMLib::wodXMPPCom));
	m_Events = NULL;
#endif

#ifdef WODXMPP_LICENSE_KEY
	CComBSTR blic(WODXMPP_LICENSE_KEY);
	m_Jabb->put_LicenseKey(blic);
#endif	

	m_Initial = TRUE;
	m_ServiceRegisterHwnd = NULL;
	m_ServiceRefreshHwnd = NULL;
	m_DoReconnect = TRUE;
}

CJabber::~CJabber()
{
	Disconnect();
#ifndef _WODXMPPLIB
	m_Jabb.Release();
#else
	WODXMPPCOMLib::_XMPP_Destroy(m_Jabb);
#endif
}

void CJabber::Connect(char *JID, char *pass, char *hostname, int port, BOOL usessl)
{
	m_ConnectTime = GetTickCount();
	char *a = strchr(JID, '/');
	if (a)
		*a = 0;

	CComBSTR l = JID,p = pass,h = hostname;
	int prt = port;
	
	l += "/";
	l += WIPPIENIM;

//	char buff[1024];
//	srand((unsigned)time( NULL ));
//	buff[0] = 0;
//	for(int i=0;i<3;i++)
//		sprintf(&buff[strlen(buff)], "%x", rand());
//	l += buff;

#ifndef _WODXMPPLIB
	m_Jabb->put_Login(l);
	m_Jabb->put_Password(p);
	if (prt)
		m_Jabb->put_Port(prt);
	if (usessl)
		m_Jabb->put_Security((WODXMPPCOMLib::SecurityEnum)3);
	else
		m_Jabb->put_Security((WODXMPPCOMLib::SecurityEnum)1);

#else
	CComBSTR2 l1 = l;
	WODXMPPCOMLib::XMPP_SetLogin(m_Jabb, l1.ToString());
	WODXMPPCOMLib::XMPP_SetPassword(m_Jabb, pass);
	if (prt)
		WODXMPPCOMLib::XMPP_SetPort(m_Jabb, prt);
	if (usessl)
		WODXMPPCOMLib::XMPP_SetSecurity(m_Jabb, (WODXMPPCOMLib::SecurityEnum)3); // security implicit
	else
		WODXMPPCOMLib::XMPP_SetSecurity(m_Jabb, (WODXMPPCOMLib::SecurityEnum)1); // security allowed

//		m_Jabb->put_Protocol = 3;

#endif
		


	m_Initial = TRUE;
#ifndef _WODXMPPLIB
	VARIANT var;
	if (h.Length())
	{
		var.vt = VT_BSTR;
		var.bstrVal = h;
	}
	else
		var.vt = VT_ERROR;
	
	if (m_Events)
		delete m_Events;
	m_Events = new CJabberEvents(this);

	m_Jabb->put_DebugFile(_Settings.m_JabberDebugFile);
	HRESULT hr;
	hr = m_Jabb->raw_Connect(var);
#else
	long hr;
	CComBSTR2 df = _Settings.m_JabberDebugFile;
	WODXMPPCOMLib::XMPP_SetDebugFile(m_Jabb, df.ToString());
	hr = WODXMPPCOMLib::XMPP_Connect(m_Jabb, hostname);
#endif

	if (FAILED(hr))
		ShowError();
}

void CJabber::Disconnect(void)
{
#ifndef _WODXMPPLIB
	m_Jabb->Disconnect();

	if (m_Events)
		delete m_Events;
	m_Events = NULL;
#else
	WODXMPPCOMLib::XMPP_Disconnect(m_Jabb);
#endif
}

#ifndef _WODXMPPLIB
void CJabber::Message(WODXMPPCOMLib::IXMPPContact *Contact, char *JID, char *MessageText, char *HtmlText)
#else
void CJabber::Message(void *Contact, char *JID, char *MessageText, char *HtmlText)
#endif
{
#ifndef _WODXMPPLIB
	CComPtr<WODXMPPCOMLib::IXMPPMessage> msg;
	msg.CoCreateInstance(__uuidof(WODXMPPCOMLib::XMPPMessage));
#else
	void *msg = WODXMPPCOMLib::XMPP_Message_New();

#endif
	BOOL sendhtml = FALSE;

	CComBSTR2 res;

#ifndef _WODXMPPLIB
	WODXMPPCOMLib::IXMPPContact *c = Contact;
	WODXMPPCOMLib::IXMPPContact *ct = NULL;
	if (!c)
	{
		CComBSTR j = JID;
		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = j;
		WODXMPPCOMLib::IXMPPContacts *cts = NULL;
		WODXMPPCOMLib::IXMPPContact *ct = NULL;

		if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
		{
			if (SUCCEEDED(cts->get_Item(var, &ct)))
			{
				c = ct;
			}
			cts->Release();
		}
	}
#else
	void *c = NULL;
	WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, JID, &c);
#endif
	if (c)
	{
#ifndef _WODXMPPLIB
		c->get_Resource(&res);
		if (strstr(res.ToString(), WIPPIENIM))
			sendhtml = TRUE;
#else
		char tb[1024];
		int tblen = sizeof(tb);
		WODXMPPCOMLib::XMPP_Contact_GetResource(c, tb, &tblen);
		if (strstr(tb, WIPPIENIM))
			sendhtml = TRUE;
		WODXMPPCOMLib::XMPP_Contacts_Free(c);
#endif
	}
#ifndef _WODXMPPLIB
	if (ct)
		ct->Release();
#endif
	
#ifndef _WODXMPPLIB
	CComBSTR t = MessageText;
	msg->put_Type((WODXMPPCOMLib::MessageTypesEnum)1/*MsgChat*/);
	msg->put_Text(t);
	if (sendhtml)
	{
		CComBSTR ht = HtmlText;
		msg->put_HTMLText(ht);
	}
#else
	WODXMPPCOMLib::XMPP_Message_SetType(msg, (WODXMPPCOMLib::MessageTypesEnum)1);
	WODXMPPCOMLib::XMPP_Message_SetText(msg, MessageText);
	if (/*sendhtml*/0)
	{
		WODXMPPCOMLib::XMPP_Message_SetHTMLText(msg, HtmlText);
	}
#endif

	HRESULT hr = S_OK;
	if (Contact)
	{
#ifndef _WODXMPPLIB
		hr = Contact->raw_SendMessage(msg);
#else
		hr = WODXMPPCOMLib::XMPP_Contact_SendMessage(Contact, msg);
#endif
	}
	else
	{
#ifndef _WODXMPPLIB
		CComBSTR j(JID);
		hr = m_Jabb->raw_SendMessage(j, msg);
#else
		hr = WODXMPPCOMLib::XMPP_SendMessage(m_Jabb, JID, msg);
#endif
	}

	if (FAILED(hr))
	{
		ShowError();
	}
#ifdef _WODXMPPLIB
	WODXMPPCOMLib::XMPP_Message_Free(msg);
#endif
}

void CJabber::ExchangeWippienDetails(char *JID, char *Subj, Buffer *Text)
{
#ifndef _WODXMPPLIB
	CComBSTR j = JID;
	VARIANT var;
	var.vt = VT_BSTR;
	var.bstrVal = j;
	WODXMPPCOMLib::IXMPPContacts *cts = NULL;
	WODXMPPCOMLib::IXMPPContact *ct = NULL;

	if (SUCCEEDED(m_Jabb->get_Contacts(&cts)))
	{
		if (SUCCEEDED(cts->get_Item(var, &ct)))
		{
			ExchangeWippienDetails(ct, Subj, Text);
			ct->Release();
		}
		cts->Release();
	}
#else
	void *ct = NULL;
	WODXMPPCOMLib::XMPP_ContactsGetContactByJID(m_Jabb, JID, &ct);
	ExchangeWippienDetails(ct, Subj, Text);
	WODXMPPCOMLib::XMPP_Contacts_Free(ct);
#endif
}


#ifndef _WODXMPPLIB
void CJabber::ExchangeWippienDetails(WODXMPPCOMLib::IXMPPContact *Contact, char *Subj, Buffer *Text)
#else
void CJabber::ExchangeWippienDetails(void *Contact, char *Subj, Buffer *Text)
#endif
{

#ifndef _WODXMPPLIB
	CComPtr<WODXMPPCOMLib::IXMPPMessage> msg;
	msg.CoCreateInstance(__uuidof(WODXMPPCOMLib::XMPPMessage));
#else
	void *msg = WODXMPPCOMLib::XMPP_Message_New();
#endif


#ifndef _WODXMPPLIB
	msg->put_Type((WODXMPPCOMLib::MessageTypesEnum)/*WODXMPPCOMLib::MessageTypesEnum::MsgHeadline*/3);
	CComBSTR t = Subj;
	msg->put_Subject(t);
	CComBSTR thr = "ExchangeDetailsThread";
	msg->put_Thread(thr);
#else
	WODXMPPCOMLib::XMPP_Message_SetType(msg, (WODXMPPCOMLib::MessageTypesEnum)3);
	WODXMPPCOMLib::XMPP_Message_SetSubject(msg, Subj);
	WODXMPPCOMLib::XMPP_Message_SetThread(msg, "ExchangeDetailsThread");
#endif



	if (Text && Text->Len())
	{
		Buffer out;
		_Settings.ToHex(Text, &out);
		out.Append("\0", 1);
#ifndef _WODXMPPLIB
		CComBSTR t1 = out.Ptr();
		msg->put_Text(t1);
#else
		WODXMPPCOMLib::XMPP_Message_SetText(msg, out.Ptr());
#endif
	}

//	Contact->SendMessage(msg);
#ifndef _WODXMPPLIB
	HRESULT hr = Contact->raw_SendMessage(msg);
#else
	HRESULT hr = WODXMPPCOMLib::XMPP_Contact_SendMessage(Contact, msg);
#endif
	if (FAILED(hr))
	{
		ShowError();
	}

#ifdef _WODXMPPLIB
	WODXMPPCOMLib::XMPP_Message_Free(msg);
#endif

}

void CJabber::ShowError(void)
{
#ifndef _WODXMPPLIB
	CComBSTR2 text;
	_Jabber->m_Jabb->get_LastErrorText(&text);
	if (text.Length())
	{
		_MainDlg.ShowStatusText(text.ToString());
	}
#else
	char t[1024];
	int tlen = sizeof(t);
	WODXMPPCOMLib::XMPP_GetLastErrorText(_Jabber->m_Jabb, t, &tlen);
	if (tlen)
	{
		_MainDlg.ShowStatusText(t);
	}
#endif

}