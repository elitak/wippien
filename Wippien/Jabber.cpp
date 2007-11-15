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


CJabber *_Jabber = NULL;
extern CMainDlg _MainDlg;
extern CSettings _Settings;
extern CEthernet _Ethernet;
extern CContactAuthDlg *_ContactAuthDlg;
extern CSDKMessageLink *_SDK;

int uuencode(unsigned char *src, unsigned int srclength,char *target, size_t targsize);


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

void __stdcall CJabberEvents::DispContactAuthRequest(WODJABBERCOMLib::IJbrContact *Contact, WODJABBERCOMLib::JabberActionsEnum *Action)
{
	// add to list of auth requests
	CComBSTR2 j;

	if (SUCCEEDED(Contact->get_JID(&j)))
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
			_Settings.m_AuthRequests.push_back(j.Copy());
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

	*Action = (WODJABBERCOMLib::JabberActionsEnum)2;//::SilentDeny);
}

void __stdcall CJabberEvents::DispConnected ()
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	_Jabber->m_ConnectTime = GetTickCount();
//		_MainDlg.ShowStatusText("Connected.");
	WODJABBERCOMLib::IJbrServices *serv = NULL;
	if (SUCCEEDED(_Jabber->m_Jabb->get_Services(&serv)))
	{
		serv->Discover(TRUE);
		serv->Release();
	}

	// and request vcard
	WODJABBERCOMLib::IJbrVCard *vc;
	if (SUCCEEDED(_Jabber->m_Jabb->get_VCard(&vc)))
	{
		vc->Receive();
		vc->Release();
	}
	DispStateChange((WODJABBERCOMLib::StatesEnum)0);

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

	
	if (_SDK)
	{
		if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_CONNECTED,""))
			return;
	}

}
void __stdcall CJabberEvents::DispDisconnected (long ErrorCode, BSTR ErrorText)
{

	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	m_pJ->m_Initial = TRUE;

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

	if (m_pJ->m_DoReconnect)
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

}
void __stdcall CJabberEvents::DispStateChange(WODJABBERCOMLib::StatesEnum OldState)
{

	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();

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
	
	// let's integrate SDK too
	if (_SDK)
	{
		int newst = 0;
		if (SUCCEEDED(_Jabber->m_Jabb->get_State((WODJABBERCOMLib::StatesEnum *)&newst)))
		{
			Buffer b;
			b.PutInt((int)OldState);
			b.PutInt((int)newst);
			if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_STATECHANGE, b.Ptr(), b.Len()))
				return;
		}
	}
	
}
void __stdcall CJabberEvents::DispContactStatusChange(WODJABBERCOMLib::IJbrContact *Contact, WODJABBERCOMLib::StatusEnum OldStatus)
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	if (!m_pJ->m_Initial)
	{

		if (_SDK)
		{
			CComBSTR j;
			Contact->get_JID(&j);

			WODJABBERCOMLib::StatusEnum st;
			Contact->get_Status(&st);

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
void __stdcall CJabberEvents::DispIncomingNotification(WODJABBERCOMLib::IJbrContact *Contact, WODJABBERCOMLib::ContactNotifyEnum NotifyID, VARIANT Data)
{
	CComBSTR2 j;
	Contact->get_JID(&j);

	char *jd1 = j.ToString();
	char *jd2 = strchr(jd1, '/');
	if (jd2)
		*jd2 = 0;

	_MainDlg.OnIncomingNotification(jd1, (int)NotifyID, Data);
}
void __stdcall CJabberEvents::DispIncomingMessage(WODJABBERCOMLib::IJbrContact *Contact, WODJABBERCOMLib::IJbrMessage *Message)
{
	if (Contact)
	{
		WODJABBERCOMLib::MessageTypesEnum msgtype = (WODJABBERCOMLib::MessageTypesEnum)0;
		if (Message)
			Message->get_Type(&msgtype);

		// is it from mediator?
		CComBSTR jid;
		CComBSTR2 jd;
		if (SUCCEEDED(Contact->get_JID(&jd)))
		{
			char *jd1 = jd.ToString();
			if (_Settings.IsHiddenContact(jd1))
				return;

			char *jd2 = strchr(jd1, '/');
			if (jd2)
				*jd2 = 0;
			jid = jd1;

			
			if (jid == _Settings.m_IPMediator)
			{
				if (!_Ethernet.m_Available) return; // ignore if ethernet is not available
				if (!_Settings.m_UseIPMediator) return; //ingore if user said so
				if (msgtype == /*MsgError*/4) return; // ignore errors from mediator


				// yes, store mediator info
				CComBSTR2 text;
				if (SUCCEEDED(Message->get_Text(&text)))
				{
					char *t = text.ToString();

					if (_SDK)
					{
						Buffer b;
						b.PutCString(jd1);
						b.PutCString(t);

						if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_MEDIATORMESSAGE, b.Ptr(), b.Len()))
							return;
					}

					
					if (strstr(t, "<Listen>"))
					{
						char *t1 = strstr(t, "<Virtual>");
						// locate IP
						char *ip = strstr(t, "<IP>");
						if (ip)
						{
							ip += 4;

							// locate port
							char *port = strstr(ip, "<UDPPort>");
							if (port)
								port += 9;

							// NULL ip/port
							char *c = strchr(ip, '<');
							if (c)
								*c = 0;

							c = strchr(port, '<');
							if (c)
							{
								*c = 0;
								c++;
							}

							unsigned long mylastnetwork = 0, mylastnetmask = 0;
							if (t1)
							{
								c = strstr(t1, "<Virtual>");
								if (c)
								{
									c = strstr(c, "<IP>");
									if (c)
									{
										c += 4;
										char *d = strstr(c, "</");
										if (d)
										{
											*d = 0;
											d+=2;
											
											char *e = strstr(d, "<Mask>");
											if (e)
											{
												e += 6;
												char *f = strstr(e, "</Mask>");
												if (f)
												{
													*f = 0;

													// start with IP and netmask - in case it's not started yet
													mylastnetwork = htonl(atol(c));
													mylastnetmask = htonl(atol(e));
												}
											}
										}
									}
								}
							}

							if (_Settings.m_ObtainIPAddress == 1)
							{
								if (mylastnetwork && mylastnetmask)
								{
									_Settings.m_MyLastNetwork = 0;
									_Settings.m_MyLastNetmask = 0;
									_Ethernet.Start(mylastnetwork, mylastnetmask);
								}
							}

//							struct  in_addr inaddr;
//							inaddr.s_addr = inet_addr(ip);
//							if (inaddr.s_addr != INADDR_NONE)
							{
								_Settings.m_LinkMediator = ip;
								_Settings.m_LinkMediatorPort = atol(port);
								_Settings.Save(FALSE);
							}
							_MainDlg.ShowStatusText("Mediator is available.");
							_MainDlg.Invalidate();
						}						
					}
					else
					if (strstr(t, "<Restart>"))
					{
						m_pJ->m_DoReconnect = TRUE;
						m_pJ->m_Jabb->Disconnect();
//						CComBSTR b("Restarted by mediator");
//						DispDisconnected(-1,b);
					}
				}
				return;
			}
		}
		
		// is it headline
//		WODJABBERCOMLib::MessageTypesEnum ms;

//		if (SUCCEEDED(Message->get_Type(&ms)))
		{
			if (msgtype == (WODJABBERCOMLib::MessageTypesEnum)3)//::MsgHeadline)
			{
				ATLTRACE("Got message from remote peer\r\n");
				CComBSTR subj;
				if (SUCCEEDED(Message->get_Subject(&subj)))
				{
					if (subj == WIPPIENINITREQUEST && Contact)
					{
						if (!_Ethernet.m_Available) return; // ignore if ethernet is not available

						Buffer in, out;
						CComBSTR2 r;
						if (SUCCEEDED(Message->get_Text(&r)))
						{
							char *r1 = r.ToString();
							in.Append(r1);

							_Settings.FromHex(&in, &out);

							// save this for user
							CComBSTR j;
							if (SUCCEEDED(Contact->get_JID(&j)))
							{
								CUser *user = _MainDlg.m_UserList.GetUserByJID(j);
								if (user)
								{
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
						ATLTRACE("Got WIPPIENINITRESPONSE\r\n");
						if (!_Ethernet.m_Available) return; // ignore if ethernet is not available

						Buffer in, out;
						CComBSTR2 r;
						if (SUCCEEDED(Message->get_Text(&r)))
						{
							char *r1 = r.ToString();
							in.Append(r1);

							_Settings.FromHex(&in, &out);

							// save this for user
							CComBSTR j;
							if (SUCCEEDED(Contact->get_JID(&j)))
							{
								CUser *user = _MainDlg.m_UserList.GetUserByJID(j);
								if (user)
								{
									char src[8192], dst[8192];
									memcpy(src, out.Ptr(), 128);
									if (RSA_private_decrypt(128, (unsigned char *)src, (unsigned char *)dst,  _Settings.m_RSA, RSA_PKCS1_PADDING) < 0)
									{
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
						if (SUCCEEDED(Contact->get_JID(&j)))
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
						ATLTRACE("Got CONNECT message\r\n");
						// reinit...
						CComBSTR j;
						if (SUCCEEDED(Contact->get_JID(&j)))
						{
							CUser *user = _MainDlg.m_UserList.GetUserByJID(j);
							if (user)
							{
								Buffer in, out;
								CComBSTR2 r;
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
				Contact->get_JID(&j);
				if (msgtype == (WODJABBERCOMLib::MessageTypesEnum)4)//::error)
				{
					if (Message)
					{
						BOOL show = TRUE;
						CComBSTR2 j1;
						Message->get_Thread(&j1);
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
					Message->get_Text(&t);
					CComBSTR2 ht;
					Message->get_HTMLText(&ht);
					_MainDlg.OnIncomingMessage(j.ToString(), t.ToString(), ht.ToString());
				}
			}
		}
		
	}
}
void __stdcall CJabberEvents::DispContactList()
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	BOOL init = m_pJ->m_Initial;
	m_pJ->m_Initial = FALSE;

	if (_Settings.m_IPMediator.Length() && _Settings.m_UseIPMediator)
	{
		// ask mediator for his details
		WODJABBERCOMLib::IJbrContacts *contacts;
		if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&contacts)))
		{
			
			WODJABBERCOMLib::IJbrContact *contact;
			VARIANT var;
			var.vt = VT_BSTR;
			var.bstrVal = _Settings.m_IPMediator;
			HRESULT hr = contacts->get_Item(var, &contact);
			if (FAILED(hr))
				hr = contacts->raw_Add(_Settings.m_IPMediator, &contact);

			if (SUCCEEDED(hr))
			{
				_Jabber->Message(contact, NULL, "HELO", "");
				contact->Release();
			}
			contacts->Release();
		}
	}

//	if (_MainDlg.IsWindow())
//		_MainDlg.m_UserList.PostMessage(WM_REFRESH, NULL, TRUE);
//		_MainDlg.m_UserList.Refresh(NULL);
	_MainDlg.m_UserList.RefreshUser(NULL);
}
void __stdcall CJabberEvents::DispServiceRegister (WODJABBERCOMLib::IJbrService *Service, VARIANT_BOOL Success, BSTR Instructions)
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	CComBSTR2 i = Instructions;
	if (_Jabber->m_ServiceRegisterHwnd)
	{
		SetWindowText(_Jabber->m_ServiceRegisterHwnd, i.ToString());
	}
}
void __stdcall CJabberEvents::DispServiceStatusChange (WODJABBERCOMLib::IJbrService *Service)
{
	_MainDlg.m_UserList.m_SortedUsersBuffer.Clear();
	if (_Jabber->m_ServiceRefreshHwnd)
	{
		PostMessage(_Jabber->m_ServiceRefreshHwnd, WM_REFRESH, 0, 0);
	}
}
void __stdcall CJabberEvents::DispVCardDetails(WODJABBERCOMLib::IJbrContact *Contact, VARIANT_BOOL Partial)
{
//	if (Contact)
//	{
		_MainDlg.m_UserList.OnVCard(Contact, Partial?TRUE:FALSE, TRUE);
//	}
}

CJabber::CJabber(void)
{
	m_Jabb.CoCreateInstance(__uuidof(WODJABBERCOMLib::wodJabberCom));

#ifdef WODJABBER_LICENSE_KEY
	CComBSTR blic(WODJABBER_LICENSE_KEY);
	m_Jabb->put_LicenseKey(blic);
#endif	
	m_Initial = TRUE;
	m_Events = NULL;
	m_ServiceRegisterHwnd = NULL;
	m_ServiceRefreshHwnd = NULL;
	m_DoReconnect = TRUE;
}

CJabber::~CJabber()
{
	Disconnect();
	m_Jabb.Release();
}

void CJabber::Connect(char *JID, char *pass, char *hostname, int port)
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

	m_Jabb->put_Login(l);
	m_Jabb->put_Password(p);
	if (prt)
		m_Jabb->put_Port(prt);

	VARIANT var;
	if (h.Length())
	{
		var.vt = VT_BSTR;
		var.bstrVal = h;
	}
	else
		var.vt = VT_ERROR;

	m_Initial = TRUE;
	if (m_Events)
		delete m_Events;
	m_Events = new CJabberEvents(this);
	

	m_Jabb->put_DebugFile(_Settings.m_JabberDebugFile);
//	CComBSTR b = "c:\\jabber.nfo";
//	m_Jabb->put_DebugFile(b);
//	CComBSTR blic("6VRR-2G5P-Q42G-9GRA");
//	m_Jabb->put_LicenseKey(blic);

	HRESULT hr;
	hr = m_Jabb->raw_Connect(var);
	if (FAILED(hr))
		ShowError();
}

void CJabber::Disconnect(void)
{
	m_Jabb->Disconnect();
	if (m_Events)
		delete m_Events;
	m_Events = NULL;
}

void CJabber::Message(WODJABBERCOMLib::IJbrContact *Contact, char *JID, char *MessageText, char *HtmlText)
{
	CComPtr<WODJABBERCOMLib::IJbrMessage> msg;
	msg.CoCreateInstance(__uuidof(WODJABBERCOMLib::JbrMessage));

	BOOL sendhtml = FALSE;

	CComBSTR2 res;

	WODJABBERCOMLib::IJbrContact *c = Contact;
	WODJABBERCOMLib::IJbrContact *ct = NULL;
	if (!c)
	{
		CComBSTR j = JID;
		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = j;
		WODJABBERCOMLib::IJbrContacts *cts = NULL;
		WODJABBERCOMLib::IJbrContact *ct = NULL;

		if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
		{
			if (SUCCEEDED(cts->get_Item(var, &ct)))
			{
				c = ct;
			}
			cts->Release();
		}
	}
	if (c)
	{
		c->get_Resource(&res);
		if (strstr(res.ToString(), WIPPIENIM))
			sendhtml = TRUE;

	}
	if (ct)
		ct->Release();
	
	msg->put_Type((WODJABBERCOMLib::MessageTypesEnum)1/*MsgChat*/);
	CComBSTR t = MessageText;
	msg->put_Text(t);
	if (sendhtml)
	{
		CComBSTR ht = HtmlText;
		msg->put_HTMLText(ht);
	}

	HRESULT hr = S_OK;
	if (Contact)
	{
		hr = Contact->raw_SendMessage(msg);
	}
	else
	{
		CComBSTR j(JID);
		hr = m_Jabb->raw_SendMessage(j, msg);
	}

	if (FAILED(hr))
	{
		ShowError();
	}
}

void CJabber::ExchangeWippienDetails(char *JID, char *Subj, Buffer *Text)
{
	CComBSTR j = JID;
	VARIANT var;
	var.vt = VT_BSTR;
	var.bstrVal = j;
	WODJABBERCOMLib::IJbrContacts *cts = NULL;
	WODJABBERCOMLib::IJbrContact *ct = NULL;

	if (SUCCEEDED(m_Jabb->get_Contacts(&cts)))
	{
		if (SUCCEEDED(cts->get_Item(var, &ct)))
		{
			ExchangeWippienDetails(ct, Subj, Text);
			ct->Release();
		}
		cts->Release();
	}
}


void CJabber::ExchangeWippienDetails(WODJABBERCOMLib::IJbrContact *Contact, char *Subj, Buffer *Text)
{
	CComPtr<WODJABBERCOMLib::IJbrMessage> msg;
	msg.CoCreateInstance(__uuidof(WODJABBERCOMLib::JbrMessage));

	msg->put_Type((WODJABBERCOMLib::MessageTypesEnum)/*WODJABBERCOMLib::MessageTypesEnum::MsgHeadline*/3);

	CComBSTR t = Subj;
	msg->put_Subject(t);

	CComBSTR thr = "ExchangeDetailsThread";
	msg->put_Thread(thr);

	if (Text && Text->Len())
	{
		Buffer out;
		_Settings.ToHex(Text, &out);
		out.Append("\0", 1);
		CComBSTR t1 = out.Ptr();
		msg->put_Text(t1);
	}

//	Contact->SendMessage(msg);
	HRESULT hr = Contact->raw_SendMessage(msg);
	if (FAILED(hr))
	{
		ShowError();
	}
}

void CJabber::ShowError(void)
{
	CComBSTR2 text;
	_Jabber->m_Jabb->get_LastErrorText(&text);
	if (text.Length())
	{
		_MainDlg.ShowStatusText(text.ToString());
	}

}