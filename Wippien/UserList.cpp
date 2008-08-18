// UserList.cpp: implementation of the CUserList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserList.h"
#include "MainDlg.h"
#include "Settings.h"
#include "Jabber.h"
#include "Notify.h"
#include "SettingsDlg.h"
#include "MsgWin.h"
#include "ChatRoom.h"
#include "Ethernet.h"

extern CSettings _Settings;
extern CJabber *_Jabber;
extern CNotify _Notify;
extern CMainDlg _MainDlg;
extern CEthernet _Ethernet;

CRITICAL_SECTION m_UserCS;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define CONNECTQUIET	10000

int wildmat(const char *Buffer, const char *Pattern);
int uuencode(unsigned char *src, unsigned int srclength,char *target, size_t targsize);
char *trim(char *text);

BOOL __LoadIconFromResource(CxImage *img, HINSTANCE hInst, char *restype, int imgformat, int resid)
{
	BOOL res = FALSE;
	HRSRC  h = FindResource(hInst, MAKEINTRESOURCE(resid), restype);
	DWORD rsize= SizeofResource(hInst,h);
	if (rsize)
	{
		HGLOBAL hMem=::LoadResource(hInst,h);
		if (hMem)
		{
			char* lpVoid=(char*)LockResource(hMem);
			if (lpVoid)
			{
				CxMemFile fTmp((BYTE*)lpVoid,rsize);
				img->Decode(&fTmp, imgformat);
				res = TRUE;
			}

			DeleteObject(hMem);
			return res;
		}
	}

	return res;
}
BOOL _LoadIconFromResource(CxImage *img, char *restype, int imgformat, int resid)
{
	return __LoadIconFromResource(img, _Module.GetModuleInstance(), restype, imgformat, resid);
}


CUserList::CUserList()
{
	m_Dragging = 0;
	m_Owner = NULL;
	m_hWndParent = NULL;

	m_ListboxFont = NULL;
	m_ListboxSubFont = NULL;
	m_ListboxGroupFont = NULL;
	m_UserPopupMenu = NULL;
	m_GroupPopupMenu = NULL;
	m_SetupPopupMenu = NULL;
	m_ChatRoomPopupMenu = NULL;
	m_AwayPopupMenu = NULL;
	m_SortedUser = NULL;
	InitializeCriticalSection(&m_UserCS);
}

CUserList::~CUserList()
{
	while (m_Users.size())
	{
		CUser *user = m_Users[0];
		m_Users.erase(m_Users.begin());
#ifdef _WODVPNLIB
		delete user;
#else
		user->Release();
#endif						
	}
	if (m_UserPopupMenu)
		delete m_UserPopupMenu;
	if (m_GroupPopupMenu)
		delete m_GroupPopupMenu;
	if (m_SetupPopupMenu)
		delete m_SetupPopupMenu;
	if (m_ChatRoomPopupMenu)
		delete m_ChatRoomPopupMenu;
	if (m_AwayPopupMenu)
		delete m_AwayPopupMenu;
	if (m_ListboxFont)
		DeleteObject(m_ListboxFont);
	if (m_ListboxSubFont)
		DeleteObject(m_ListboxSubFont);
	if (m_ListboxGroupFont)
		DeleteObject(m_ListboxGroupFont);
	DeleteCriticalSection(&m_UserCS);
}

void CUserList::LoadIconFromResource(CxImage *img, int resid)
{
	_LoadIconFromResource(img, "PNG", CXIMAGE_FORMAT_PNG, resid);
}
void CUserList::InitIcons(void)
{
//	LoadIconFromResource(&m_StaticImage, IDB_HUMAN);
	LoadIconFromResource(&m_BlinkImage, IDB_BELL);
	LoadIconFromResource(&m_GroupOpened, IDB_GROUPOPENED);
	LoadIconFromResource(&m_GroupClosed, IDB_GROUPCLOSED);
	LoadIconFromResource(&m_LockContact, IDB_LOCKCONTACT);
}
void CUserList::Init(CMainDlg *Owner, HWND Parent)
{
	m_Owner = Owner;
	m_hWndParent = Parent;
	long lfHeightSub, lfHeight, lfHeightGroup;

	HDC hdc = ::GetDC(NULL);
	lfHeight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	lfHeightSub = -MulDiv(7, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	lfHeightGroup = -MulDiv(11, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	::ReleaseDC(NULL, hdc);

	m_ListboxFont = CreateFont(lfHeight, 0, 0, 0, FW_BOLD, FALSE, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
	m_ListboxSubFont = CreateFont(lfHeightSub, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial");
	m_ListboxGroupFont = CreateFont(lfHeightGroup, 0, 0, 0, FW_BOLD, FALSE, 0, 0, 0, 0, 0, 0, 0, "Sans Serif");

	/*m_Tree.*/SubclassWindow(m_hWndParent);
	/*m_Tree.*/SetItemHeight(1);
	/*m_Tree.*/SetIndent(0);


	/*m_Tree.*/SetFont(m_ListboxFont);
	/*m_Tree.*/SetSmallFont(m_ListboxSubFont);
	/*m_Tree.*/SetGroupFont(m_ListboxGroupFont);



	InitIcons();

	m_UserPopupMenu = new CCommandBarCtrlXP();
	m_UserPopupMenu->Create(/*m_Tree.*/m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

	m_GroupPopupMenu = new CCommandBarCtrlXP();
	m_GroupPopupMenu->Create(/*m_Tree.*/m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	
	m_SetupPopupMenu = new CCommandBarCtrlXP();
	m_SetupPopupMenu->Create(/*m_Tree.*/m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

	m_AwayPopupMenu = new CCommandBarCtrlXP();
	m_AwayPopupMenu->Create(/*m_Tree.*/m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

	m_ChatRoomPopupMenu = new CCommandBarCtrlXP();
	m_ChatRoomPopupMenu->Create(/*m_Tree.*/m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

}

void ResampleImageIfNeeded(CxImage *img, int sizeX, int sizeY)
{
	int w = img->GetWidth();
	int h = img->GetHeight();

	if (w>sizeX || h>sizeY)
	{
		float wf = w, wh = h, s;
		
		// does need resample. Find larger image
		if (w>h)
		{
			s = sizeX;
			wf = s/wf;
			wh = wf*wh;
			wf = s;
		}
		else
		{
			s = sizeY;
			wh = s/wh;
			wf = wh*wf;
			wh = s;
		}
		img->IncreaseBpp(24);
		img->Resample(wf, wh);
	}
}
void ResampleImageIfNeeded(CxImage *img, int size)
{
	ResampleImageIfNeeded(img, size, size);
}

#ifndef _WODXMPPLIB
CUser *CUserList::AddNewUser(char *j, WODXMPPCOMLib::IXMPPContact *contact)
#else
CUser *CUserList::AddNewUser(char *j, void *contact)
#endif
{
		//CUser *user = new CUser();
#ifdef _WODVPNLIB
		CUser *user = new CUser();
#else
		CComObject<CUser> *user;
		CComObject<CUser>::CreateInstance(&user);
		user->AddRef();
#endif	
		strcpy(user->m_JID, j);

		if (!user->m_Hidden)
			user->m_Changed = TRUE;
		user->m_Hidden = FALSE;

		CComBSTR2 g;
		if (contact)
		{
#ifndef _WODXMPPLIB
			if (SUCCEEDED(contact->get_Group(&g)))
				strcpy(user->m_Group, g.ToString());
#else
			char gb[1024];
			int gblen = sizeof(gb);
			WODXMPPCOMLib::XMPP_Contact_GetGroup(contact, gb, &gblen);
			strcpy(user->m_Group, gb);
#endif
		}

		// calculate visible name
		CComBSTR2 j1 = user->m_JID;
		char *j2 = j1.ToString();
		char *b = strchr(j2, '@');
		if (b)
		{
			*b = 0;
			b = strchr(j2, '%'); // if this is a service
			if (b)
				*b = 0;
		}
		strcpy(user->m_VisibleName, j2);

		CxImage img;
//							if (!user->m_Icon.Len())
		if (!user->LoadUserImage(&img))
		{
			// KRESOFIX, get user REAL icon
			int hm = (rand() % 37);
			HRSRC h = FindResource(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDB_HUMAN1 + hm), "PNG");
			DWORD rsize= SizeofResource(_Module.GetModuleInstance(),h);
			CxImage cx;
			if (rsize)
			{
				HGLOBAL hMem=::LoadResource(_Module.GetModuleInstance(),h);
				if (hMem)
				{
					char* lpVoid=(char*)LockResource(hMem);
					if (lpVoid)
					{
						user->SaveUserImage(lpVoid, rsize);
/*						
						CxMemFile fTmp((BYTE*)lpVoid,rsize);
						cx.Decode(&fTmp, CXIMAGE_FORMAT_PNG);

						CxMemFile fMem;
						fMem.Open();
						cx.Encode(&fMem, CXIMAGE_FORMAT_PNG);
						user->m_Icon.Append((char *)fMem.GetBuffer(), fMem.Size());
*/
					}
					DeleteObject(hMem);
				}
			}
		}


		// KRESOFIXEND
		m_SortedUsersBuffer.Clear();
		m_Users.push_back(user);

		return user;
}

void CUserList::SortUsers(void)
{
//	ATLTRACE("*** Sorting users\r\n");
	m_SortedUsersBuffer.Clear();
	char *a = NULL;
	m_SortedUsersBuffer.AppendSpace(&a, sizeof(int)*m_Users.size());
	m_SortedUser = (int *)a;
	if (a)
	{
		int i;
		for (i=0;i<m_Users.size();i++)
		{
			m_SortedUser[i] = i;
		}

		if (_Settings.m_SortContacts) // do some sort
		{
			for (i=0;i<m_Users.size();i++)
			{
				CUser *user1 = (CUser *)m_Users[m_SortedUser[i]];
				for (int j=i+1;j<m_Users.size();j++)
				{
					CUser *user2 = (CUser *)m_Users[m_SortedUser[j]];
					
					int rez = 0;
					switch (_Settings.m_SortContacts)
					{
						case 1: // visible name
							rez = stricmp(user1->m_VisibleName, user2->m_VisibleName);
							break;

						case 2: // JID
							rez = stricmp(user1->m_JID, user2->m_JID);
							break;

						case 3: // by IP bottom
//							if (user1->m_HisVirtualIP && user2->m_HisVirtualIP)
//
//								rez = 1;
							if (user1->m_HisVirtualIP && !user2->m_HisVirtualIP)
								rez = 1;
							if (!user1->m_HisVirtualIP && user2->m_HisVirtualIP)
								rez = -1;

//							else
//								rez = stricmp(user1->m_VisibleName, user2->m_VisibleName);
							break;
/*
						case 4: // by IP top
							if (!user1->m_HisVirtualIP)
							if (user1->m_HisVirtualIP &&)
							{
								if (user1->m_HisVirtualIP < user2->m_HisVirtualIP)
									rez = 1;
							}
							else
								rez = stricmp(user1->m_VisibleName, user2->m_VisibleName);
							break;
*/
					}

					if (rez>0)
					{
						int k = m_SortedUser[i];
						m_SortedUser[i] = m_SortedUser[j];
						m_SortedUser[j] = k;

						user1 = (CUser *)m_Users[m_SortedUser[i]];
						user1->m_Changed = TRUE;
						user2 = (CUser *)m_Users[m_SortedUser[j]];
						user2->m_Changed = TRUE;
					}
				}
			}
		}

/*		for (i=0;i<m_Users.size();i++)
		{
			CUser *user1 = (CUser *)m_Users[m_SortedUser[i]];
			ATLTRACE("%d %s\r\n", i, user1->m_VisibleName);
		}
		ATLTRACE("** END\r\n");
*/
	}
}

void CUserList::RefreshUser(void *cntc, char *chatroom1)
{	


#ifndef _WODXMPPLIB
	WODXMPPCOMLib::IXMPPContacts *contacts;
	if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&contacts)))
	{
		short count;
		if (SUCCEEDED(contacts->get_Count(&count)))
		{
			for (int i=0;i<(cntc?1:count);i++)
			{
				WODXMPPCOMLib::IXMPPContact *contact;
				HRESULT hr = S_OK;
				if (cntc)
					contact = (WODXMPPCOMLib::IXMPPContact *)cntc;
				else
				{
					VARIANT var;
					var.vt = VT_I2;
					var.iVal = i;
					hr = contacts->get_Item(var, &contact);
				}
				if (SUCCEEDED(hr))
				{					
					CComBSTR2 jid, jd;
					CComBSTR2 jdnew;
					if (SUCCEEDED(contact->get_JID(&jd)))
					{
						jdnew = jd;
#else
	short count = 0;
	WODXMPPCOMLib::XMPP_ContactsGetCount(_Jabber->m_Jabb, &count);
	for (int i=0;i<(cntc?1:count);i++)
	{
		void *contact = NULL;
		if (cntc)
			contact = cntc;
		else
			WODXMPPCOMLib::XMPP_ContactsGetContact(_Jabber->m_Jabb, i, &contact);
		if (contact)
		{
			{
				{
					char jdbuff[1024];
					int jdlen = sizeof(jdbuff);
					WODXMPPCOMLib::XMPP_Contact_GetJID(contact, jdbuff, &jdlen);
					CComBSTR2 jid, jd = jdbuff, jdnew = jdbuff;

					{
#endif
						char *res = NULL;
						char *jd1 = jdnew.ToString();
						char *jd2 = strchr(jd1, '/');
						if (jd2)
						{
							*jd2 = 0;
							jd2++;
							res = jd2;
						}
						jid = jd1;
//						ATLTRACE("user=%s \r\n", jd.ToString());
//						if (res)
//							ATLTRACE("res=%s \r\n", res);


						char *j = jid.ToString();						
						if (_Settings.IsHiddenContact(j))
							continue;


						BOOL found = FALSE;
						CUser *user = NULL;
						for (int k=0;!found && k<m_Users.size();k++)
						{
							user = (CUser *)m_Users[k];
							if (!stricmp(user->m_JID, j) || !stricmp(user->m_JID, jd.ToString()))
							 found = TRUE;
						}
						WODXMPPCOMLib::StatusEnum stat = (WODXMPPCOMLib::StatusEnum)0/*Offline*/;
#ifndef _WODXMPPLIB
						if (SUCCEEDED(contact->get_Status(&stat)))
						{
						}
#else
						WODXMPPCOMLib::XMPP_Contact_GetStatus(contact, &stat);
#endif

//						stat = stat;


						BOOL isuserinchatroom = FALSE;
						if (!found)
						{
							if (chatroom1 && jd2)
							{
								if (!strncmp(chatroom1, jd1, strlen(chatroom1)))
									isuserinchatroom = TRUE;
							}
							if (isuserinchatroom)
							{

								user = AddNewUser(jd.ToString(), contact);
								strcpy(user->m_VisibleName, jd2);
							}
							else
								user = AddNewUser(j, contact);
						}

						if (user)
						{
							if (chatroom1 && isuserinchatroom && !user->m_ChatRoomPtr)
							{
								for (int i=0;i<_MainDlg.m_ChatRooms.size();i++)
								{
										CChatRoom *room = _MainDlg.m_ChatRooms[i];
										if (!strcmp(room->m_JID, chatroom1))
										{					
											user->m_ChatRoomPtr = room;
											room->CreateGroup();
											user->m_Block = room->m_Block; 
											strcpy(user->m_Group, room->m_ShortName);
											break;
										}	
										
									}	
									if (!user->m_ChatRoomPtr)
										return; // should not happen										
							}	
						}
						
						if (user && !found && chatroom1 && isuserinchatroom)
						{
							// new chatroom user was added
							user->m_ChatRoomPtr->m_MessageWin->AddUserToContactList(user->m_JID, FALSE);
						}	

						if (user)
						{
							if (res && strlen(res)<sizeof(user->m_Resource))
								strcpy(user->m_Resource, res);

							// if unsubscripted
							if (stat == /*Unsubscribed*/8)
							{
								user->m_Hidden = TRUE;
								user->m_Changed = TRUE;
							}
							else
							{
								if (!user->m_Hidden)
									user->m_Changed = TRUE;
								user->m_Hidden = FALSE;
							}
						}

						if (user && !user->m_Hidden)
						{
//							WODXMPPCOMLib::StatusEnum stat;
//							if (SUCCEEDED(contact->get_Status(&stat)))
							{
//								ATLTRACE("refresh for %s\r\n", j);
								m_SortedUsersBuffer.Clear();
								if (stat > /*WODXMPPCOMLib::StatusEnum::Offline*/ 0 && stat < /*WODXMPPCOMLib::StatusEnum::Requested*/ 6)
								{
									if (!user->m_Online)
									{
										user->m_Changed = TRUE;
										user->m_ChangeNotify = TRUE;
									}
									m_SortedUsersBuffer.Clear();
									user->m_Online = TRUE;	
									time((long *)&user->m_LastOnline);
#ifdef _WODXMPPLIB
									char nickbuff[1024];
									int nickbuflen = sizeof(nickbuff);
									WODXMPPCOMLib::XMPP_Contact_GetNick(contact, nickbuff, &nickbuflen);
									if (nickbuflen)
										strcpy(user->m_VisibleName, nickbuff);

									// let's see if nickname contains IP
									if (nickbuflen>1)
									{
										if (nickbuff[nickbuflen-2] == ']')
										{
											char *nlend = NULL;
											int nl = nickbuflen-2;
											while (nl>0 && nickbuff[nl]!='[') nl--;
											if (nl)
											{
												nlend = &nickbuff[nl];
												nl++;
												nickbuff[nickbuflen-2] = 0;

												unsigned int ip[4] = {0};
												int ctr = 0;
												char *a = &nickbuff[nl];
												char *b = a;
												do 
												{
													if (*a == '.')
													{
														*a = 0;
														if (ctr<4)
														{
															ip[ctr] = atoi(b);
															a++;
															b = a;
															ctr++;
														}
														else
															break;
													}
													a++;
												} while (*a);

												if (ctr && !*a)
												{
													if (nlend)
													{
														*nlend = 0;
														strcpy(user->m_VisibleName, trim(nickbuff));
													}

													if (!user->m_IsWippien || strcmp(user->m_IsWippien->Ptr(), jdnew.ToString()))
													{
															if (user->m_IsWippien)
																delete user->m_IsWippien;
															user->m_IsWippien = new Buffer();
															user->m_IsWippien->Append(jdnew.ToString());
															user->m_IsAlienWippien = TRUE;
													}
													if (user->m_IsWippien && _Ethernet.m_Available)
													{
															// send presence notification to user
															Buffer raw;
															raw.Append("<presence to='");
															CComBSTR2 e = jdnew.ToString();
															char *e1 = e.ToString();
															char *e2 = strchr(e1, '/');
															if (e2)
																*e2 = 0;
															e2 = strchr(e1, '@');
															if (e2)
															{
																e2++;
																raw.Append(e2);
															}
															else
																raw.Append(e1);
															raw.Append("'><x xmlns='vcard-temp:x:update'><nickname>");
															CComBSTR2 j = _Settings.m_JID;
															char *j1 = j.ToString();
															char *j2 = strchr(j1, '@');
															if (j2)
																*j2 = 0;
															raw.Append(j1);
															raw.Append(" [");
															struct  in_addr sin_addr;
															sin_addr.S_un.S_addr = _Settings.m_MyLastNetwork;
															raw.Append(inet_ntoa(sin_addr));
															raw.Append("]</nickname></x></presence>");
															WODXMPPCOMLib::XMPP_RawSend(_Jabber->m_Jabb, raw.Ptr());
														
															user->m_WippienState = WipDisconnected;
															user->SetTimer(rand()%10 * 500, 3);
													}
												}
											}

										}
									}
#endif
									user->m_IsAway = FALSE;
									user->SetSubtext();
									if (stat >= 2 && stat <=5)
										user->m_IsAway = TRUE;
									
									if (user->m_Changed)
									{
										// is this wippien dude?
										CComBSTR2 r;
										BOOL isWippien = FALSE;


#ifndef _WODXMPPLIB
										if (SUCCEEDED(contact->get_Capabilities(&r)))
#else
										char rsbuf[1024] = {0};
										int rsbuflen = sizeof(rsbuf);
										WODXMPPCOMLib::XMPP_Contact_GetCapabilities(contact, rsbuf, &rsbuflen);
										r = rsbuf;
#endif
										if (strstr(r.ToString(), WIPPIENIM))
										{
											if (user->m_IsWippien)
												delete user->m_IsWippien;
											user->m_IsWippien = new Buffer();
											user->m_IsWippien->Append(user->m_JID);
											user->m_IsWippien->Append("/");
											user->m_IsWippien->Append(WIPPIENIM);
										}

#ifndef _WODXMPPLIB
										CComBSTR2 gpr3;
										contact->get_Group(&gpr3);
										char *grp = gpr3.ToString();
#else
										// if group isn't set remotely, but is locally, set it
										char grp[1024];
										int grplen = sizeof(grp);
										WODXMPPCOMLib::XMPP_Contact_GetGroup(contact, grp, &grplen);
#endif
										
										if (grp[0]) // if server provides group, override ours
											strcpy(user->m_Group, grp);
										else
											if (user->m_Group[0] && strcmp(user->m_Group, GROUP_GENERAL)) // set remote group for future
#ifndef _WODXMPPLIB
											{
												CComBSTR gpr4 = user->m_Group;
												contact->put_Group(gpr4);
											}
#else
												WODXMPPCOMLib::XMPP_Contact_SetGroup(contact, user->m_Group);											
#endif

										// also check by resource (obsolete!)
										if (!isWippien)
										{
#ifndef _WODXMPPLIB
											if (SUCCEEDED(contact->get_Resource(&r)))
#else
											rsbuflen = sizeof(rsbuf);
											rsbuf[0] = 0;
											WODXMPPCOMLib::XMPP_Contact_GetResource(contact, rsbuf, &rsbuflen);
											r = rsbuf;
#endif
											{
												Buffer b;
												b.Append(r.ToString());
												b.Append("\r\n");
												char *line;
												do 
												{
													line = b.GetNextLine();
													if (line && !strncmp(line, WIPPIENIM, strlen(WIPPIENIM)))
													{
														// yes he is, request init details
														user->SetTimer(rand()%1000, 3);
														break;
													}
												}while (line);	
											}	
										}

										if (isWippien)
										{
											// yes he is, request init details
											user->SetTimer(rand()%1000, 3);
										}
									}
								}
								else
								{
									if (user->m_Online)
									{

										user->m_Changed = TRUE;
										user->m_ChangeNotify = TRUE;
									}
									m_SortedUsersBuffer.Clear();
									user->m_Online = FALSE;

									if (!chatroom1)
										user->ReInit(FALSE/*TRUE*/);
								}
							
							}
							CComBSTR2 s;							
#ifndef _WODXMPPLIB
							if (SUCCEEDED(contact->get_StatusText(&s)))
#else
							char stbuf[1024];
							int stlen = sizeof(stbuf);
							WODXMPPCOMLib::XMPP_Contact_GetStatusText(contact, stbuf, &stlen);
							s = stbuf;
#endif
							{
								char *s1 = _Settings.Translate(s.ToString());
								if (strcmp(user->m_StatusText, s1))
								{
									user->m_Changed = TRUE;
									if (strlen(s1)> sizeof(user->m_StatusText))
										s1[sizeof(user->m_StatusText)-1] = 0;								
									strcpy(user->m_StatusText, s1);
									user->SetSubtext();
								}
							}
							if (cntc)
								user->m_Changed = TRUE;
						}

						if (chatroom1 && cntc)
						{
							// let's remove this user 
							if (!user->m_Online)
							{
								// and remove contact from list of users
								for (int i=0;i<m_Users.size();i++)
								{
									CUser *us = m_Users[i];
									if (us == user)
									{
										if (user->m_ChatRoomPtr)
											user->m_ChatRoomPtr->m_MessageWin->RemoveUserFromContactList(user->m_JID);
										m_Users.erase(m_Users.begin() + i);
										delete user;
										break;
									}
								}
//								m_SortedUsersBuffer.Clear();
//								PostMessage(WM_REFRESH, NULL, FALSE);
							}
						}

					}
#ifndef _WODXMPPLIB
					if (!cntc)
						contact->Release();
#else
					if (!cntc && contact)
						WODXMPPCOMLib::XMPP_Contacts_Free(contact);
#endif
				}
				if (cntc)
					break; // go outsize for loop
			}
		}
#ifndef _WODXMPPLIB
		contacts->Release();
#endif
	}

	::PostMessage(m_hWnd, WM_REFRESH, 0, cntc?TRUE:FALSE);

}

void CUserList::RefreshView(BOOL updateonly)
{
//	::LockWindowUpdate(m_hWndParent);
	SendMessage(WM_SETREDRAW, FALSE, 0);

	TV_INSERTSTRUCT TreeItem;
	TreeItem.hParent = NULL; 
	TreeItem.hInsertAfter = TVI_LAST ;
	TreeItem.itemex.mask = TVIF_TEXT | TVIF_CHILDREN | TVIF_INTEGRAL; 
	TreeItem.itemex.iIntegral=30;
	TreeItem.itemex.cChildren = 1;

	// if initial populate, add groups
	if (!updateonly)
	{
		/*m_Tree.*/DeleteAllItems();


		for (int i=0;i<_Settings.m_Groups.size();i++)
		{
			CSettings::TreeGroup *tg = _Settings.m_Groups[i];
			TreeItem.itemex.pszText = tg->Name;
			tg->Item = InsertItem(&TreeItem);
		}
		for (i=0;i<m_Users.size();i++)
		{
			m_Users[i]->m_BlinkTimerCounter = 0;
			m_Users[i]->m_TreeItem = 0;
		}
	}
//	else
//		OnVCard((WODXMPPCOMLib::IXMPPContact *)cntc, FALSE, FALSE);
	if (_Settings.m_ShowContactPicture)
		TreeItem.itemex.iIntegral=34;
	else
		if (_Settings.m_ShowContactStatus)
			TreeItem.itemex.iIntegral=30;
		else
			TreeItem.itemex.iIntegral=15;

	if (m_SortedUsersBuffer.Len()/sizeof(int) != m_Users.size())
	{
//		updateonly = FALSE;
		SortUsers();
	}
	if (m_SortedUsersBuffer.Len())
	{
		int integralsize = TreeItem.itemex.iIntegral;
		// finally show users
		for (int i=0;i<m_Users.size();i++)
		{
			CUser *p = m_Users[m_SortedUser[i]];
			if (p->m_ChatRoomPtr)
				TreeItem.itemex.iIntegral = 17;
			else
				TreeItem.itemex.iIntegral = integralsize;
	//		ATLTRACE("Testing %s: tree=%x, changed=%d\r\n", p->m_JID, p->m_TreeItem, p->m_Changed);
			if (!p->m_TreeItem || p->m_Changed)
			{
				// this one is new
				memset(&TreeItem.item, 0, sizeof(TVITEM));
				TreeItem.item.mask = TVIF_TEXT |/* TVIF_IMAGE | */TVIF_PARAM /*| TVIF_STATE*/ | TVIF_INTEGRAL;

				if (p->m_TreeItem) // this is old user
				{
					TreeItem.item.hItem = p->m_TreeItem;
					GetItem(&TreeItem.item);
				}

	//			TreeItem.item.pszText = "12345678901234567890";
	//			TreeItem.item.cchTextMax = 32;

				BOOL doblink = FALSE;
				BOOL isblocked = _Settings.IsHiddenContact(p->m_JID);
				if (!p->m_Hidden && !isblocked)
				{
					p->SetSubtext();
					BOOL ison = p->m_Online;
					if (!ison && p->m_WippienState==WipConnected)
						ison = TRUE;

					if (!ison && p->m_WippienState!=WipConnected)
					{
						if (!p->m_TreeItem)
						{
								TreeItem.hParent = FindRoot((char *)GROUP_OFFLINE);
								TreeItem.hInsertAfter = TVI_LAST;
						}
						if (p->m_TreeItem && updateonly) // only for old users that go offline
						{
							TreeItem.hParent = FindRoot((char *)GROUP_OFFLINE);
							if (GetTickCount() - _Jabber->m_ConnectTime  > CONNECTQUIET)
							{
								if (p->m_ChangeNotify)
								{
									doblink = TRUE;
									if (p->IsMsgWindowOpen())
									{
										CComBSTR k = _Settings.Translate("Contact");
										k += " ";
										k += _Settings.Translate("is now offline");										
										CComBSTR2 k2 = k;
										p->PrintMsgWindow(TRUE, k2.ToString(), NULL);
									}
									if (_Settings.m_ShowNotificationPopup)
										_Notify.ShowTrayNotify(p->m_VisibleName, _Settings.Translate("is now offline"));
									_Notify.DoEvent(NotificationOffline);
								}
							}
						}
					}
					else
					{				
						// set initial group
						if (!p->m_TreeItem)
						{
								TreeItem.hParent = FindRoot(p->m_Group, FALSE);
								if (!TreeItem.hParent)
								{
									::PostMessage(m_hWnd, WM_REFRESH, 0, FALSE);
									if (p->m_ChatRoomPtr)
									{
										p->m_ChatRoomPtr->CreateGroup();
										strcpy(p->m_Group, p->m_ChatRoomPtr->m_ShortName);
									}
									TreeItem.hParent = FindRoot(p->m_Group);

								}
								TreeItem.hInsertAfter = TVI_LAST;

/*
								// let's find this group
								for (int it = 0; it < _Settings.m_Groups.size(); it++)
								{
									CSettings::TreeGroup *tg = (CSettings::TreeGroup *)_Settings.m_Groups[it];
									if (!stricmp(tg->Name, p->m_Group))
									{
										if (p->m_ChatRoomPtr)
											tg->Temporary = TRUE;
									}
								}
*/
						}
						
						if (p->m_TreeItem && updateonly) // only for old users that go offline
						{
							TreeItem.hInsertAfter = TVI_FIRST;
							TreeItem.hParent = FindRoot(p->m_Group);
							if (GetTickCount() - _Jabber->m_ConnectTime  > CONNECTQUIET)
							{
								if (p->m_ChangeNotify)
								{
									// fix for going offline but VPN leave connected
									if (p->m_Online)
									{
										if (_Settings.m_ShowNotificationPopup)
											_Notify.ShowTrayNotify(p->m_VisibleName, _Settings.Translate("is now online"));
										_Notify.DoEvent(NotificationOnline);
										if (p->IsMsgWindowOpen())
										{
											CComBSTR k = _Settings.Translate("Contact");
											k += " ";
											k += _Settings.Translate("is now online");										
											CComBSTR2 k2 = k;
											p->PrintMsgWindow(TRUE, k2.ToString(), NULL);
										}
									}
									else
									{
										if (p->IsMsgWindowOpen())
										{
											CComBSTR k = _Settings.Translate("Contact");
											k += " ";
											k += _Settings.Translate("is now offline");										
											CComBSTR2 k2 = k;
											p->PrintMsgWindow(TRUE, k2.ToString(), NULL);
										}
										if (_Settings.m_ShowNotificationPopup)
											_Notify.ShowTrayNotify(p->m_VisibleName, _Settings.Translate("is now offline"));
										_Notify.DoEvent(NotificationOffline);
									}
									doblink = TRUE;
								}
							}
						}
					
					}

					TreeItem.item.pszText = "1234567890";
					TreeItem.item.cchTextMax = 10;		


					if (p->m_Image)
					{
						delete p->m_Image;
						p->m_Image = NULL;
					}
					if (!p->m_Image/* && p->m_Icon.Len()*/)
					{
						CxImage img;
						if (p->LoadUserImage(&img))
						{
							p->m_Image = new CxImage(img);
	//						p->m_Image->Copy(img);
							ResampleImageIfNeeded(p->m_Image, 32);
						}
	//					p->m_Image = new CxImage();
	//					p->m_Image->Decode((unsigned char *)p->m_Icon.Ptr(), p->m_Icon.Len(), CXIMAGE_FORMAT_PNG);
					}
				}

				TreeItem.item.lParam = (long)p;
	//			ATLTRACE("changed to %d %s\r\n", p->m_Online, p->m_JID);
				if (p->m_Changed && p->m_TreeItem)
				{
	//				if (TreeItem.hParent != (HTREEITEM)SendMessage(TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)TreeItem.item.hItem) || p->m_Hidden)
					{
						p->m_BlinkTimerCounter = 0;
	//					ATLTRACE("Deleting %s\r\n", p->m_JID);
						DeleteItem(p->m_TreeItem);
						p->m_TreeItem = 0;
					}
				}
				if (!p->m_Hidden && !isblocked)
				{
					if (!p->m_TreeItem) // this is new user
					{
						p->m_BlinkTimerCounter = 0;
						TreeItem.hInsertAfter = TVI_FIRST;
	//					if (p->m_Online)
	//						ATLTRACE("%s goes first\r\n", p->m_JID);
						for (int jk=0;jk<m_Users.size() && m_Users[m_SortedUser[jk]]!=p;jk++)
						{
							CUser *us1 = (CUser *)m_Users[m_SortedUser[jk]];
							if (!stricmp((*us1->m_Group)?us1->m_Group:GROUP_GENERAL, (*p->m_Group)?p->m_Group:GROUP_GENERAL) && p->m_Online == us1->m_Online)
							{
	//							if (p->m_Online)
	//								ATLTRACE("%s goes after %s\r\n", p->m_JID, us1->m_JID);
								TreeItem.hInsertAfter = us1->m_TreeItem;
							}
						}
	//					if (p->m_Online)
	//						ATLTRACE("Inserting item %s\r\n", p->m_JID);
						p->m_TreeItem = /*m_Tree.*/InsertItem(&TreeItem);
					}
					else
					{
	//					ATLTRACE("Setting item %s\r\n", p->m_JID);
						/*m_Tree.*/SetItem(&TreeItem.item); // this is old user
					}
					if (doblink && updateonly && p->m_TreeItem && (GetTickCount() - _Jabber->m_ConnectTime > CONNECTQUIET))
						BlinkItem(p->m_TreeItem);
				}
			}	
		}


		// expand
		for (i=0;i<_Settings.m_Groups.size();i++)
		{
			CSettings::TreeGroup *tg = _Settings.m_Groups[i];
			if (tg->Open && tg->Item)
				/*m_Tree.*/Expand(tg->Item);
		}

		_Settings.SaveUsers();


		for (int j=0;j<_Settings.m_Groups.size();j++)
		{
			CSettings::TreeGroup *tg = _Settings.m_Groups[j];
			tg->Count = 0;
			tg->TotalCount = 0;
		}


		// now just enumerate users in groups
		int offlinecount = 0;
		for (i=0;i<m_Users.size();i++)
		{
			CUser *user = (CUser *)m_Users[i];
			for (int j=0;j<_Settings.m_Groups.size();j++)
			{
				CSettings::TreeGroup *tg = _Settings.m_Groups[j];
				if (!strcmp(tg->Name, user->m_Group) || (!*user->m_Group && !strcmp(tg->Name, GROUP_GENERAL)))
				{
					if (tg->Block)
						user->m_Block = TRUE;

					if (!user->m_Hidden && !_Settings.IsHiddenContact(user->m_JID))
					{
						tg->TotalCount++;
						BOOL ison = user->m_Online;
						if (!ison && user->m_WippienState==WipConnected)
							ison = TRUE;
						if (ison)
							tg->Count++;
						else
							offlinecount++;

					}
					j = _Settings.m_Groups.size()+1;
				}
			}
		}

		for (j=0;j<_Settings.m_Groups.size();j++)
		{
	//		char buff[1024];
			CSettings::TreeGroup *tg = _Settings.m_Groups[j];

			memset(&TreeItem.item, 0, sizeof(TVITEM));
			TreeItem.item.mask = TVIF_TEXT;

			if (tg->Item) // this is old user
			{
				TreeItem.item.hItem = tg->Item;
				if (strcmp(tg->Name, GROUP_OFFLINE))
					sprintf(tg->CountBuff, "%d/%d", tg->Count, tg->TotalCount);
				else
					sprintf(tg->CountBuff, "%d", offlinecount);

				TreeItem.item.pszText = tg->Name;
	//			TreeItem.item.cchTextMax = strlen(buff);
				SetItem(&TreeItem.item);
			}
		}


		// clear out...
		for (i=0;i<m_Users.size();i++)
		{
			CUser *user = m_Users[i];
			user->m_Changed = FALSE;
			user->m_ChangeNotify = FALSE;
		}
	}
	SendMessage(WM_SETREDRAW, TRUE, 0);
//	::LockWindowUpdate(NULL);
}

HTREEITEM CUserList::FindRoot(char *RootName)
{
	return FindRoot(RootName, TRUE);
}

HTREEITEM CUserList::FindRoot(char *RootName, BOOL canaddnew)
{
	if (!RootName || !*RootName)
		RootName = (char *)GROUP_GENERAL;

	// find this item, or create it if it doesn't exist
	HTREEITEM root = /*m_Tree.*/GetRootItem();
	do
	{
		BSTR t = NULL;
		CComBSTR2 t1;
		/*m_Tree.*/GetItemText(root, t);
		t1.Empty();
		t1 = t;
		::SysFreeString(t);

		if (strstr(t1.ToString(), RootName))
			return root;
	
		root = /*m_Tree.*/GetNextItem(root, TVGN_NEXT);
	} while (root);

	if (!canaddnew)
		return NULL;

	// otherwise, add this to root
	TV_INSERTSTRUCT TreeItem;
	TreeItem.hParent = NULL; 
	TreeItem.hInsertAfter = TVI_LAST ;
	TreeItem.itemex.mask = TVIF_TEXT | TVIF_CHILDREN | TVIF_INTEGRAL;
	TreeItem.itemex.iIntegral=20;
	TreeItem.itemex.cChildren = 1;

	TreeItem.itemex.pszText = RootName;
	HTREEITEM hi = InsertItem(&TreeItem);	

	if (hi)
	{
		for (int i = 0; i < _Settings.m_Groups.size(); i++)
		{
			CSettings::TreeGroup *tg = (CSettings::TreeGroup *)_Settings.m_Groups[i];
			if (!strcmp(tg->Name, RootName))
				return hi;
		}

		// add it to groups collection
		CSettings::TreeGroup *tg = new CSettings::TreeGroup;
		tg->Item = hi;
		tg->Open = TRUE;
		tg->Temporary = FALSE;
		tg->Block = FALSE;
		tg->CountBuff[0] = 0;
		char *a = (char *)malloc(strlen(RootName)+1);
		memset(a, 0, strlen(RootName)+1);
		memcpy(a, RootName, strlen(RootName));
		tg->Name = a;
		_Settings.PushGroupSorted(tg);
//		_Settings.m_Groups.push_back(tg);
	}

	return hi;

}

CUser *CUserList::GetUserByJID(BSTR JID)
{
	CComBSTR2 j = JID;
	return GetUserByJID(j.ToString());
}

CUser *CUserList::GetUserByJID(char *JID)
{
	CComBSTR2 j = JID;
	char *a1 = j.ToString();
	char *a2 = strchr(a1, '/');
	if (a2)
		*a2 = 0;

	for (int i=0;i<m_Users.size();i++)
	{
		CUser *user = (CUser *)m_Users[i];
		if (!stricmp(user->m_JID, a1) || !stricmp(user->m_JID, JID))
			return user;
	}

	return NULL;
}

CUser *CUserList::GetUserByVirtualIP(unsigned long IP)
{
	for (int i=0;i<m_Users.size();i++)
	{
		CUser *user = (CUser *)m_Users[i];
		if (user->m_HisVirtualIP == IP)
			return user;
	}

	return NULL;
}

BOOL CUserList::ConnectIfPossible(CUser *user, BOOL perform)
{
	if (user->m_Online)
	{
		// do we have this user's details?
		if (user->m_RSA && user->m_WippienState != WipConnected)
		{
			if (perform)
			{
				// send connection request
				user->SendConnectionRequest(TRUE);
			}
			return TRUE;
		}
		else
		{
			if (user->m_IsWippien)
				user->ExchangeWippienDetails();
		}
	}
	return FALSE;
}

LRESULT CUserList::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_MainDlg.CheckIfAntiInactivityMessage(WM_CHAR);
	TVHITTESTINFO ht;
	ht.pt.x = GET_X_LPARAM(lParam); 
	ht.pt.y = GET_Y_LPARAM(lParam); 

	SendMessage(TVM_HITTEST, 0, (LPARAM)&ht);

	LRESULT res = CVividTree::OnLButtonDown(uMsg, wParam, lParam, bHandled);

	if (!ht.hItem)
		return FALSE;

	CUser *user = (CUser *)GetItemData(ht.hItem);
	if (!user)
	{
		BOOL save = FALSE;
		for (int i=0;i<_Settings.m_Groups.size();i++)
		{
			CSettings::TreeGroup *tg = (CSettings::TreeGroup *)_Settings.m_Groups[i];
			if (tg->Item == ht.hItem)
			{
				BOOL found = FALSE;
				for (int o=0;o<_MainDlg.m_ChatRooms.size();o++)
				{
					CChatRoom *room = _MainDlg.m_ChatRooms[o];
					if (!strcmp(tg->Name, room->m_ShortName))
					{
						found = !(::ShowWindow(room->m_MessageWin->m_hWnd, SW_SHOWNORMAL));
						room->OpenMsgWindow(TRUE);
					}
				}
				if (!found)
				{
					if (SendMessage(TVM_GETITEMSTATE, (WPARAM)ht.hItem, TVIS_EXPANDED) & TVIS_EXPANDED)
						tg->Open = TRUE;
					else
						tg->Open = FALSE;
				}
			}
		}
		if (save)
		{
			_Settings.SaveConfig();
			_Settings.SaveUsers();
		}
		bHandled = TRUE;
		return TRUE;
	}
	else
	{
		SendMessage(TVM_SELECTITEM,TVGN_CARET,(LPARAM)ht.hItem); 
	}
	bHandled = FALSE;
	return FALSE;


	return TRUE;
}


LRESULT CUserList::OnLButtonDblClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	_MainDlg.CheckIfAntiInactivityMessage(WM_CHAR);
/*	if (m_InlineEdit)
	{
		DisableInlineEdit();
	}
*/
	TVHITTESTINFO ht;
	ht.pt.x = GET_X_LPARAM(lParam); 
	ht.pt.y = GET_Y_LPARAM(lParam); 

	SendMessage(TVM_HITTEST, 0, (LPARAM)&ht);

	if (!ht.hItem)
		return FALSE;

	CUser *user = (CUser *)GetItemData(ht.hItem);
	if (!user)
		return FALSE;

	if (user->m_Block)
	{	
		_Notify.DoEvent(NotificationError);
		_MainDlg.ShowStatusText(_Settings.Translate("Contact is blocked."));

	}
	else
	{

		if (!user->m_Online)
		{
			_Notify.DoEvent(NotificationError);
			_MainDlg.ShowStatusText(_Settings.Translate("Contact is offline."));
			return FALSE;
		}

		if (user->m_WippienState != WipConnected)
		{
			if (!ConnectIfPossible(user, TRUE))
			{
				_Notify.DoEvent(NotificationError);
			}
			else
				return TRUE;
		}
	}
	
	user->OpenMsgWindow(TRUE);
	if (user->m_MessageWin && ::IsWindow(user->m_MessageWin->m_hWnd))
		::ShowWindow(user->m_MessageWin->m_hWnd, SW_SHOWNORMAL);

	return TRUE;
}

LRESULT CUserList::OnListNotify(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NMTREEVIEW *pnmtv;
	int i;
	HIMAGELIST hImg;

//	ATLTRACE("%d \r\n", pnmh->code);


	i = TVN_ITEMEXPANDED;
	i = pnmh->code;

	pnmtv = (NMTREEVIEW *)pnmh;
	switch (pnmh->code)
	{
		
		case TVN_ITEMEXPANDING:
			return OnTvnItemexpanding(idCtrl, pnmh, bHandled);

		case TVN_ITEMEXPANDED:
			BOOL save;
			save = FALSE;
			for (i=0;i<_Settings.m_Groups.size();i++)
			{
				CSettings::TreeGroup *tg = (CSettings::TreeGroup *)_Settings.m_Groups[i];
				if (tg->Item == pnmtv->itemNew.hItem)
				{
					save = TRUE;
					if (pnmtv->itemNew.state & TVIS_EXPANDED)
						tg->Open = TRUE;
					else
						tg->Open = FALSE;
				}
			}
			if (save)
				_Settings.SaveConfig();
			return TRUE;

		case TVN_BEGINDRAG:
			// tree view message info
			hImg = ImageList_Create(32, 32, ILC_COLOR32  | ILC_MASK, 1, 1);

			HBITMAP hbm;
			hbm = LoadBitmap(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDB_DRAGDROP));
			ImageList_AddMasked(hImg, hbm, RGB(255,255,255));
			DeleteObject(hbm);
			

//			hImg=TreeView_CreateDragImage(m_hWndParent, pnmtv->itemNew.hItem); 
			ImageList_BeginDrag(hImg, 0, 0, 0); // start drag effect
			ImageList_DragEnter(m_hWndParent, pnmtv->ptDrag.x,pnmtv->ptDrag.y-70); // where to start
			ShowCursor(FALSE); // no need mouse cursor
			::SetCapture(m_Owner->m_hWnd);  // snap mouse & window
			m_Dragging = pnmtv->itemNew.hItem; // we are dragging
			break;
	}
	
	return 0;
}

DWORD WINAPI ExplorerThreadFunc(LPVOID lpParam) 
{ 
	in_addr ar;
	ar.S_un.S_addr = (unsigned long)lpParam;

	char buff[1024];
	sprintf(buff, "\\\\%s", inet_ntoa(ar));
	ShellExecute(NULL, "open", buff, "", "", SW_SHOW);

	return 0;
}

void CUserList::AddMenuImage(int resid, int dataid)
{
	// load the icon if it isn't there
	BOOL found = FALSE;
	for (int x=0;!found && x<_Settings.m_MenuImages.size();x++)
	{
		CxImage *img = (CxImage *)_Settings.m_MenuImages[x];
		if (img->pUserData == (void *)dataid)
			found = TRUE;
	}
	if (!found)
	{
		CxImage *img = new CxImage();
		_LoadIconFromResource(img, "PNG", CXIMAGE_FORMAT_PNG, resid);
		img->pUserData = (void *)dataid;
		_Settings.m_MenuImages.push_back(img);
	}
}

LRESULT CUserList::OnRButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
/*	if (m_InlineEdit)
	{
		DisableInlineEdit();
	}
*/

//	SendMessage(WM_LBUTTONDOWN, wParam, lParam);
//	SendMessage(WM_LBUTTONUP, wParam, lParam);

	TVHITTESTINFO ht;
	ht.pt.x = GET_X_LPARAM(lParam); 
	ht.pt.y = GET_Y_LPARAM(lParam); 

	SendMessage(TVM_HITTEST, 0, (LPARAM)&ht);

	if (!ht.hItem)
		return FALSE;

	SendMessage(TVM_SELECTITEM,TVGN_CARET, (LPARAM)ht.hItem);
	ClientToScreen(&ht.pt);

	CUser *user = (CUser *)GetItemData(ht.hItem);
	if (user) // clicked on user
	{

		CSettings::TreeGroup *tg = NULL;

		BOOL save = FALSE;
		for (int i=0;i<_Settings.m_Groups.size();i++)
		{
			CSettings::TreeGroup *tg1 = (CSettings::TreeGroup *)_Settings.m_Groups[i];
			if (!strcmp(tg1->Name, user->m_Group))
			{
				tg = tg1;
				break;
			}
		}

		MENUITEMINFO lpmii = {0};
		lpmii.cbSize = sizeof(lpmii);
		lpmii.fMask = MIIM_STATE;

		HMENU hm = LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_USERLISTPOPUP));
		HMENU h = GetSubMenu(hm, 0);

		int ctr = 0;
		for (i=0;i<_Settings.m_MenuTools.size();i++)
		{
			CSettings::MenuTool *mt = _Settings.m_MenuTools[i];
			BOOL add = TRUE;
			if (mt->State == 1 && user->m_Online) // only offline
				add = FALSE;
			if (mt->State == 2 && !user->m_Online && user->m_WippienState == WipConnected) // only online
				add = FALSE;		
			if (mt->State == 3 && !user->m_Online) // online or connected
				add = FALSE;		
			if (mt->State == 4 && (!user->m_Online || !user->m_HisVirtualIP)) // online or connected
				add = FALSE;
			if (mt->State == 5 && user->m_WippienState != WipConnected) // online or connected
				add = FALSE;

	//m_HisDHCPAddress
		
			if (mt->FilterGroup)
				if (!wildmat(user->m_Group, mt->FilterGroup))
					add = FALSE;
			if (mt->FilterJID)
				if (!wildmat(user->m_JID, mt->FilterJID))
					add = FALSE;
			if (mt->FilterVisibleName)
				if (!wildmat(user->m_VisibleName, mt->FilterVisibleName))
					add = FALSE;

			if (add)
			{
				MENUITEMINFO mit;
				memset(&mit, 0, sizeof(mit));
				mit.cbSize = sizeof(mit);
				mit.fMask = MIIM_STRING | MIIM_ID;
				mit.dwTypeData = mt->Menu;
				mit.cch = strlen(mt->Menu);
				mit.fType = MFT_STRING;
				mit.wID = (UINT)mt;
				if (*mt->IconPath)
				{
					Buffer *b = user->ExpandArgs(mt->IconPath);
					HICON hic = ExtractIcon(_Module.GetModuleInstance(), b->Ptr(), mt->IconID);
					delete b;
					if (hic && (int)hic!=1)
					{			
						CxImage *img = new CxImage();
						if (img->CreateFromHICON(hic))
						{
							ResampleImageIfNeeded(img, 16);
	//						img->Resample(16, 16);
							img->pUserData = (void *)mt;
							_Settings.m_MenuImages.push_back(img);
						}
						else
							delete img;
						DestroyIcon(hic);
					}
				}
				InsertMenuItem(h, 2+ctr, 2+ctr, &mit);
	//			InsertMenu(h, 4, MF_BYPOSITION, (UINT)mt, mt->Menu);
				ctr++;
			}
		} 
		if (ctr)
			InsertMenu(h, 2+ctr, MF_BYPOSITION | MF_SEPARATOR, NULL, NULL);

		BOOL canconnect = (user->m_WippienState == WipConnected?TRUE:ConnectIfPossible(user, FALSE));

		lpmii.fMask = MIIM_STATE;
		GetMenuItemInfo(h, ID_POPUP1_COPYADDRESS, FALSE, &lpmii);
		if (canconnect)
			lpmii.fState = MFS_ENABLED;
		else
			lpmii.fState = MFS_DISABLED;
		lpmii.dwTypeData = _Settings.Translate("&Copy IP");
		lpmii.fMask = MIIM_STRING | MIIM_DATA | MIIM_STATE;
		SetMenuItemInfo(h, ID_POPUP1_COPYADDRESS, FALSE, &lpmii);
		AddMenuImage(ID_PNG1_COPYADDRESS, ID_POPUP1_COPYADDRESS);


		lpmii.fMask = MIIM_STATE;
		GetMenuItemInfo(h, ID_POPUP1_BLOCK, FALSE, &lpmii);
		if (user->m_Block)
			lpmii.dwTypeData = _Settings.Translate("Unblock");
		else
			lpmii.dwTypeData = _Settings.Translate("Block");
		if (tg && tg->Block) // disable block/unblock
			lpmii.fState = MFS_DISABLED;
		lpmii.fMask = MIIM_STRING | MIIM_DATA | MIIM_STATE;
		SetMenuItemInfo(h, ID_POPUP1_BLOCK, FALSE, &lpmii);
		AddMenuImage(ID_PNG1_BLOCK, ID_POPUP1_BLOCK);

		// nije dovrseno
		lpmii.fMask = MIIM_STATE;
		GetMenuItemInfo(h, ID_POPUP1_SENDFILE, FALSE, &lpmii);
		lpmii.dwTypeData = _Settings.Translate("Send &File");
		lpmii.fState = MFS_DISABLED;
		lpmii.fMask = MIIM_STRING | MIIM_DATA | MIIM_STATE;
		SetMenuItemInfo(h, ID_POPUP1_SENDFILE, FALSE, &lpmii);
		AddMenuImage(ID_PNG1_SENDFILE, ID_POPUP1_SENDFILE);

		// nije dovrseno
		lpmii.fMask = MIIM_STATE;
		GetMenuItemInfo(h, ID_POPUP1_VOICECHAT, FALSE, &lpmii);
		lpmii.dwTypeData = _Settings.Translate("&Voice Chat");
		lpmii.fState = MFS_DISABLED;
		lpmii.fMask = MIIM_STRING | MIIM_DATA | MIIM_STATE;
		SetMenuItemInfo(h, ID_POPUP1_VOICECHAT, FALSE, &lpmii);
	//	AddMenuImage(ID_PNG1_SENDFILE, ID_POPUP1_VOICECHAT);

		lpmii.fMask = MIIM_STATE;
		GetMenuItemInfo(h, ID_POPUP1_SENDEMAIL, FALSE, &lpmii);
		lpmii.dwTypeData = _Settings.Translate("Send E&mail");
		if (*user->m_Email)
			lpmii.fState = MFS_ENABLED;
		else
			lpmii.fState = MFS_DISABLED;
		lpmii.fMask = MIIM_STRING | MIIM_DATA | MIIM_STATE;
		SetMenuItemInfo(h, ID_POPUP1_SENDEMAIL, FALSE, &lpmii);
		AddMenuImage(ID_PNG1_SENDEMAIL, ID_POPUP1_SENDEMAIL);
		
		AddMenuImage(ID_PNG1_DETAILS, ID_POPUP1_DETAILS);
		lpmii.fMask = MIIM_DATA | MIIM_STRING;
//		GetMenuItemInfo(h, ID_POPUP1_DETAILS, FALSE, &lpmii);
		lpmii.dwTypeData = _Settings.Translate("De&tails");
		SetMenuItemInfo(h, ID_POPUP1_DETAILS, FALSE, &lpmii);

		AddMenuImage(ID_PNG1_CHAT, ID_POPUP1_CHAT);
//		GetMenuItemInfo(h, ID_POPUP1_CHAT, FALSE, &lpmii);
		lpmii.dwTypeData = _Settings.Translate("C&hat");
		SetMenuItemInfo(h, ID_POPUP1_CHAT, FALSE, &lpmii);
		AddMenuImage(ID_PNG1_RENAME, ID_POPUP1_RENAME);
//		GetMenuItemInfo(h, ID_POPUP1_RENAME, FALSE, &lpmii);
		lpmii.dwTypeData = _Settings.Translate("&Rename");
		SetMenuItemInfo(h, ID_POPUP1_RENAME, FALSE, &lpmii);
		AddMenuImage(ID_PNG1_DELETE, ID_POPUP1_DELETE);
//		GetMenuItemInfo(h, ID_POPUP1_DELETE, FALSE, &lpmii);
		lpmii.dwTypeData = _Settings.Translate("&Delete");
		SetMenuItemInfo(h, ID_POPUP1_DELETE, FALSE, &lpmii);


		m_UserPopupMenu->AttachMenu(hm);
		_MainDlg.m_CanTooltip = FALSE;
		i = m_UserPopupMenu->TrackPopupMenu(h, TPM_LEFTALIGN | TPM_RETURNCMD, ht.pt.x, ht.pt.y, 0);
		_MainDlg.m_CanTooltip = TRUE;
		DestroyMenu(hm);

		return ExecuteRButtonUserCommand(/*ht.hItem, */user, i);
	}
	else
	{
		BOOL save = FALSE;
		for (int i=0;i<_Settings.m_Groups.size();i++)
		{
			CSettings::TreeGroup *tg = (CSettings::TreeGroup *)_Settings.m_Groups[i];
			if (tg->Item == ht.hItem)
			{

				MENUITEMINFO lpmii = {0};
				lpmii.cbSize = sizeof(lpmii);
				lpmii.fMask = MIIM_STATE;
				
				HMENU hm = LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_GROUPLISTPOPUP));
				HMENU h = GetSubMenu(hm, 0);
				// gotcha! open popup menu
				lpmii.fMask = MIIM_STRING | MIIM_DATA;
				GetMenuItemInfo(h, ID_POPUP1_BLOCK, FALSE, &lpmii);
				if (tg->Block)
					lpmii.dwTypeData = _Settings.Translate("Unblock");
				else
					lpmii.dwTypeData = _Settings.Translate("Block");
				SetMenuItemInfo(h, ID_POPUP1_BLOCK, FALSE, &lpmii);
				AddMenuImage(ID_PNG1_BLOCK, ID_POPUP1_BLOCK);


				if (!strcmp(tg->Name, GROUP_GENERAL) || !strcmp(tg->Name, GROUP_OFFLINE))
				{
					lpmii.fMask = MIIM_STATE;
					GetMenuItemInfo(h, ID_POPUP1_RENAME, FALSE, &lpmii);
					lpmii.fState = MFS_DISABLED;
					SetMenuItemInfo(h, ID_POPUP1_RENAME, FALSE, &lpmii);

					lpmii.fMask = MIIM_STATE;
					GetMenuItemInfo(h, ID_POPUP1_DELETE, FALSE, &lpmii);
					lpmii.fState = MFS_DISABLED;
					SetMenuItemInfo(h, ID_POPUP1_DELETE, FALSE, &lpmii);
				
				}

				AddMenuImage(ID_PNG1_RENAME, ID_POPUP1_RENAME);
				AddMenuImage(ID_PNG1_DELETE, ID_POPUP1_DELETE);

				m_GroupPopupMenu->AttachMenu(hm);
				_MainDlg.m_CanTooltip = FALSE;
				i = m_GroupPopupMenu->TrackPopupMenu(h, TPM_LEFTALIGN | TPM_RETURNCMD, ht.pt.x, ht.pt.y, 0);
				_MainDlg.m_CanTooltip = TRUE;
				DestroyMenu(hm);

				return ExecuteRButtonGroupCommand(tg, i);
			}
		}
	}
	return 0;
}

BOOL CUserList::ExecuteRButtonGroupCommand(CSettings::TreeGroup *tg, int Command)
{
	switch (Command)
	{
	case ID_POPUP1_RENAME:
		{
			CRenameContact ndlg;
			ndlg.m_IsGroupEdit = TRUE;
			strcpy(ndlg.m_VisibleName, tg->Name);
			ndlg.DoModal();
			if (ndlg.m_VisibleName[0])
			{
				free(tg->Name);
				tg->Name = (char *)malloc(strlen(ndlg.m_VisibleName)+1);
				strcpy(tg->Name, ndlg.m_VisibleName);
				_Settings.Save();
				PostMessage(WM_REFRESH, NULL, TRUE);
			}
		}
		break;
		
	case ID_POPUP1_DELETE:
		{
			char buff[1024];
			sprintf(buff, _Settings.Translate("Are you sure you want to remove group %s"), tg->Name);
			int i = MessageBox(buff, _Settings.Translate("Remove group?"), MB_YESNO | MB_ICONQUESTION);
			if (i == IDYES)
			{
				DeleteGroup(tg->Name);
				PostMessage(WM_REFRESH, NULL, FALSE);
			}
		}
		break;
		
	case ID_POPUP1_BLOCK:
		{
			
			tg->Block = !tg->Block;

			// loop through all users and (un)block them
			for (int j=0;j<_MainDlg.m_UserList.m_Users.size();j++)
			{
				CUser *user = _MainDlg.m_UserList.m_Users[j];
				if (!stricmp(user->m_Group, tg->Name))
				{
					user->m_Block = tg->Block;
				}
			}
			PostMessage(WM_REFRESH, NULL, FALSE);
		}
		break;
	}
	return TRUE;
}

BOOL CUserList::DeleteGroup(char *GroupName)
{

	CSettings::TreeGroup *tg = NULL;
	BOOL found = FALSE;
	// do we have this already?
	for (int i=0;i<_Settings.m_Groups.size();i++)
	{
		CSettings::TreeGroup *tg = _Settings.m_Groups[i];
		if (!stricmp(tg->Name, GroupName))
		{
			found = TRUE;
			// delete it
			// loop through all users and move them to General
			for (int j=0;j<_MainDlg.m_UserList.m_Users.size();j++)
			{
				CUser *user = _MainDlg.m_UserList.m_Users[j];
				if (!stricmp(user->m_Group, tg->Name))
				{
					// move user to different group
					user->m_Group[0] = 0;
					//						strcpy(user->m_Group, GROUP_GENERAL);
					
#ifndef _WODXMPPLIB
					WODXMPPCOMLib::IXMPPContacts *cts;
					WODXMPPCOMLib::IXMPPContact *ct = NULL;
					
					if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
					{
						VARIANT var;
						var.vt = VT_BSTR;
						CComBSTR j = user->m_JID;
						var.bstrVal = j;
						
						cts->get_Item(var, &ct);
						{
							CComBSTR g = "";
							ct->put_Group(g);
							ct->Release();
						}
						cts->Release();
					}
#else
					void *ct = NULL;
					WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, user->m_JID, &ct);
					if (ct)
					{
						WODXMPPCOMLib::XMPP_Contact_SetGroup(ct, "");
						WODXMPPCOMLib::XMPP_Contacts_Free(ct);
					}
#endif

				}
			}
			free(tg->Name);
			delete tg;
			_Settings.m_Groups.erase(_Settings.m_Groups.begin() + i);
			return TRUE;
		}
	}
	return FALSE;
}
		
BOOL CUserList::ExecuteRButtonUserCommand(/*HTREEITEM ht, */CUser *user, int Command)
{
	char buff[1024];
	in_addr ar;
	ar.S_un.S_addr = user->m_HisVirtualIP;
//	HWND h;
	CRect rc, rc2;

	switch (Command)
	{
		case ID_POPUP1_COPYADDRESS:
			ConnectIfPossible(user, FALSE);
			if (OpenClipboard())
			{
				EmptyClipboard();
				Buffer b;
				b.Append(inet_ntoa(ar));
				b.Append("\0",1);
				HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE,  (b.Len()) * sizeof(TCHAR)); 
			    if (hglbCopy == NULL) 
				{ 
					CloseClipboard(); 
			        return FALSE; 
				}

				char *lptstrCopy = (char *)GlobalLock(hglbCopy); 
			    memcpy(lptstrCopy, b.Ptr(), b.Len() * sizeof(TCHAR)); 
			    GlobalUnlock(hglbCopy); 
			    SetClipboardData(CF_TEXT, hglbCopy); 
				GlobalFree(hglbCopy);
				CloseClipboard(); 
			}
			break;

		case ID_POPUP1_CHAT:
			user->OpenMsgWindow(TRUE);
			break;

		case ID_POPUP1_VOICECHAT:
			MessageBeep(-1);
			break;

		case ID_POPUP1_SENDEMAIL:
			{
				char buff[1024];
				sprintf(buff, "mailto:%s", user->m_Email);
				ShellExecute(NULL, "open", buff, "", "", SW_SHOW);
			}
			break;

		case ID_POPUP1_RENAME:
			{
				CRenameContact ndlg;
				strcpy(ndlg.m_VisibleName, user->m_VisibleName);
				ndlg.DoModal();
				if (ndlg.m_VisibleName[0])
				{
					strcpy(user->m_VisibleName, ndlg.m_VisibleName);
					user->m_Changed = TRUE;
//					user->m_TreeItem = (HTREEITEM)-1;
					m_SortedUsersBuffer.Clear();
					_Settings.SaveUsers();
					PostMessage(WM_REFRESH, NULL, TRUE);
				}
			}
			break;

		case ID_POPUP1_DELETE:
			{
				CComBSTR2 b1 = user->m_JID;
				sprintf(buff, _Settings.Translate("Are you sure you want to remove contact %s"), b1.ToString());
				int i = MessageBox(buff, _Settings.Translate("Remove contact?"), MB_YESNO | MB_ICONQUESTION);
				if (i == IDYES)
				{
#ifndef _WODXMPPLIB
					// locate this user 
					WODXMPPCOMLib::IXMPPContact *ct;
					WODXMPPCOMLib::IXMPPContacts *cts;

					if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
					{
						VARIANT var;
						var.vt = VT_BSTR;
						var.bstrVal = b1;
						if (SUCCEEDED(cts->get_Item(var, &ct)))
						{
							ct->raw_Unsubscribe();
//							ct->Remove();

							var.vt = VT_DISPATCH;
							var.pdispVal = ct;
							cts->raw_Remove(var);
							ct->Release();
						}
						cts->Release();
					}
#else
					void *ct = NULL;
					WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, user->m_JID, &ct);
					if (ct)
					{
						WODXMPPCOMLib::XMPP_Contact_Unsubscribe(ct);
						WODXMPPCOMLib::XMPP_ContactsRemove(_Jabber->m_Jabb, ct);
						WODXMPPCOMLib::XMPP_Contacts_Free(ct);
					}
#endif


					// and remove contact from list of users
					for (int i=0;i<m_Users.size();i++)
					{
						CUser *us = m_Users[i];
						if (us == user)
						{
							m_Users.erase(m_Users.begin() + i);
//							delete user;
							//Refresh(NULL);
							_Settings.SaveUsers();
							PostMessage(WM_REFRESH, NULL, FALSE);
							return TRUE;
						}
					}
					m_SortedUsersBuffer.Clear();
					PostMessage(WM_REFRESH, NULL, FALSE);
				}
			}
			break;

		case ID_POPUP1_BLOCK:
			{

				m_SortedUsersBuffer.Clear();
				user->m_Block = !user->m_Block;
				user->NotifyBlock();
//				if (user->m_WippienState >= WipConnecting)
//					user->ReInit();
			}
			break;

		case ID_POPUP1_DETAILS:
			{
				CComBSTR2 b1 = user->m_JID;
#ifndef _WODXMPPLIB
				WODXMPPCOMLib::IXMPPContact *ct;
				WODXMPPCOMLib::IXMPPContacts *cts;

				if (SUCCEEDED(_Jabber->m_Jabb->get_Contacts(&cts)))
				{
					VARIANT var;
					var.vt = VT_BSTR;
					var.bstrVal = b1;
					if (SUCCEEDED(cts->get_Item(var, &ct)))
					{

						// create window
						WODXMPPCOMLib::IXMPPVCard *vcard = NULL;
						ct->get_VCard(&vcard);
#else
				void *ct = NULL;
				void *vcard = NULL;
				WODXMPPCOMLib::XMPP_ContactsGetContactByJID(_Jabber->m_Jabb, user->m_JID, &ct);
				if (ct)
				{
					WODXMPPCOMLib::XMPP_Contact_GetVCard(ct, &vcard);
					if (vcard)
					{
#endif

						CSettingsDlg::CSettingsUser1 *us1 = NULL;
						CSettingsDlg::CSettingsUser2 *us2 = NULL;
						CSettingsDlg::CSettingsUser3 *us3 = NULL;
						CSettingsDlg::CSettingsUser4 *us4 = NULL;

						if (user->m_SettingsContactsDlg)
						{
							if (!user->m_SettingsContactsDlg->IsWindow())
								delete user->m_SettingsContactsDlg;
							user->m_SettingsContactsDlg = NULL;
						}

						if (user->m_SettingsContactsDlg)
						{
							us1 = (CSettingsDlg::CSettingsUser1 *)user->m_SettingsContactsDlg->m_Dialogs[0];
							us2 = (CSettingsDlg::CSettingsUser2 *)user->m_SettingsContactsDlg->m_Dialogs[1];
							us3 = (CSettingsDlg::CSettingsUser3 *)user->m_SettingsContactsDlg->m_Dialogs[2];
							us4 = (CSettingsDlg::CSettingsUser4 *)user->m_SettingsContactsDlg->m_Dialogs[3];

							us1->m_IsContact = TRUE;
							us2->m_IsContact = TRUE;
							us3->m_IsContact = TRUE;

							user->m_SettingsContactsDlg->ShowWindow(SW_SHOW);
						}
						else
						{
							user->m_SettingsContactsDlg = new CSettingsDlg(FALSE);

							us1 = new CSettingsDlg::CSettingsUser1();
							CSettingsDlg::_CSettingsTemplate *pg = us1;
							strcpy(us1->m_Text2, b1.ToString());
							user->m_SettingsContactsDlg->m_Dialogs.push_back((CSettingsDlg::_CSettingsTemplate *)pg);
																			
							us2 = new CSettingsDlg::CSettingsUser2();
							pg = us2;
							strcpy(us2->m_Text2, b1.ToString());
							user->m_SettingsContactsDlg->m_Dialogs.push_back((CSettingsDlg::_CSettingsTemplate *)pg);
																			
							us3 = new CSettingsDlg::CSettingsUser3();
							pg = us3;
							strcpy(us3->m_Text2, b1.ToString());
							user->m_SettingsContactsDlg->m_Dialogs.push_back((CSettingsDlg::_CSettingsTemplate *)pg);
																			
							us4 = new CSettingsDlg::CSettingsUser4();
							pg = us4;
							strcpy(us4->m_Text2, b1.ToString());
							user->m_SettingsContactsDlg->m_Dialogs.push_back((CSettingsDlg::_CSettingsTemplate *)pg);
																			
							user->m_SettingsContactsDlg->Create(NULL, NULL);
							::SetWindowText(user->m_SettingsContactsDlg->m_hWnd, _Settings.Translate("Contact details"));

							
							us1->Lock(TRUE);
							us2->Lock(TRUE);
							us3->Lock(TRUE);
							user->m_SettingsContactsDlg->ShowWindow(SW_SHOW);
						}

						if (vcard)
						{
							us1->InitData(vcard);
							us2->InitData(vcard);
							us3->InitData(vcard);
#ifndef _WODXMPPLIB
							vcard->Receive();
							vcard->Release();
#else
							WODXMPPCOMLib::XMPP_Contact_VCard_Receive(ct);
							WODXMPPCOMLib::XMPP_VCard_Free(vcard);
#endif

						}
						us4->InitData();
							
#ifndef _WODXMPPLIB
						ct->Release();
#else
						WODXMPPCOMLib::XMPP_Contacts_Free(ct);
#endif
					}
#ifndef _WODXMPPLIB
					cts->Release();
#endif
				}
			}
			break;

		default:
			for (int i=0;i<_Settings.m_MenuTools.size();i++)
			{
				CSettings::MenuTool *mt = _Settings.m_MenuTools[i];
				if (Command == (int)mt)
				{
					Buffer *b = user->ExpandArgs(mt->Exec);
					if (b)
					{
						STARTUPINFO si;
						PROCESS_INFORMATION pi;

						ZeroMemory( &si, sizeof(si) );
						si.cb = sizeof(si);
						ZeroMemory( &pi, sizeof(pi) );
						CreateProcess(NULL, b->Ptr(),NULL,NULL,FALSE,0,NULL,NULL,&si, &pi);
						delete b;
					}
				}
			}
			break;

	}

	return TRUE;
}

LRESULT CUserList::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_MainDlg.CheckIfAntiInactivityMessage(WM_CHAR);
	return _MainDlg.OnMouseMove(m_hWnd, uMsg, wParam, lParam, bHandled);
}

LRESULT CUserList::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_MainDlg.CheckIfAntiInactivityMessage(WM_CHAR);
	if (wParam == VK_DELETE)
	{
		HTREEITEM hitem = GetSelectedItem();
		if (hitem)
		{
			CUser *user = (CUser *)GetItemData(hitem);
			if (user)
				ExecuteRButtonUserCommand(/*NULL, */user, ID_POPUP1_DELETE);
		}
		// this is like REMOVE
	}
	else
		bHandled = FALSE;
	return 0;
}

LRESULT CUserList::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RefreshView((BOOL)lParam);
	return 0;
}

int GetPhotoType(Buffer *PhotoData)
{
	if (!strncmp(PhotoData->Ptr(), "\x89PNG", 4))
		return CXIMAGE_FORMAT_PNG;
	else
	if (!strncmp(PhotoData->Ptr(), "GIF", 3))
		return CXIMAGE_FORMAT_GIF;
	else
	if (!strncmp(PhotoData->Ptr(), "BM", 2))
		return CXIMAGE_FORMAT_BMP;

	return CXIMAGE_FORMAT_JPG;
	
}


#ifdef _WODXMPPLIB
void CUserList::OnVCard(void *Contact, BOOL Partial, BOOL received)
#else
void CUserList::OnVCard(WODXMPPCOMLib::IXMPPContact *Contact, BOOL Partial, BOOL received)
#endif
{
	time_t now;
	time( &now );
	
	if (Contact)
	{
		CComBSTR j;
#ifndef _WODXMPPLIB
		if (SUCCEEDED(Contact->get_JID(&j)))
#else
		char jdbuf[1024];
		int jdlen = sizeof(jdbuf);
		WODXMPPCOMLib::XMPP_Contact_GetJID(Contact, jdbuf, &jdlen);
		j = jdbuf;
#endif
		{
			CUser *user = _MainDlg.m_UserList.GetUserByJID(j);
			if (user)
			{
				if (received)
				{
					if (user->m_SettingsContactsDlg)
					{
#ifndef _WODXMPPLIB
						WODXMPPCOMLib::IXMPPVCard *vc;
						if (SUCCEEDED(Contact->get_VCard(&vc)))
#else
						void *vc = NULL;
						WODXMPPCOMLib::XMPP_Contact_GetVCard(Contact, &vc);
						if (vc)
#endif
						{
							CSettingsDlg::CSettingsUser1 *us1 = (CSettingsDlg::CSettingsUser1 *)user->m_SettingsContactsDlg->m_Dialogs[0];
							CSettingsDlg::CSettingsUser2 *us2 = (CSettingsDlg::CSettingsUser2 *)user->m_SettingsContactsDlg->m_Dialogs[1];
							CSettingsDlg::CSettingsUser3 *us3 = (CSettingsDlg::CSettingsUser3 *)user->m_SettingsContactsDlg->m_Dialogs[2];
//							CSettingsDlg::CSettingsUser4 *us4 = (CSettingsDlg::CSettingsUser4 *)user->m_SettingsContactsDlg->m_Dialogs[3];

							us1->InitData(vc);
							us2->InitData(vc);
							us3->InitData(vc);
//							us4->InitData();

#ifndef _WODXMPPLIB
							vc->Release();
#else
							WODXMPPCOMLib::XMPP_VCard_Free(vc);
#endif
						}
					}
//					unsigned long uservcard = user->m_GotVCard;
//					BOOL uservcard = user->m_GotVCard;
//					if (!user->m_GotVCard)
					{
						user->m_GotVCard = now;
#ifndef _WODXMPPLIB
						WODXMPPCOMLib::IXMPPVCard *vc;
						if (SUCCEEDED(Contact->get_VCard(&vc)))
#else
						void *vc = NULL;
						WODXMPPCOMLib::XMPP_Contact_GetVCard(Contact, &vc);
						char vcbuf[1024];
						int vclen;
						if (vc)
#endif
						{
							CComBSTR2 vis;
#ifndef _WODXMPPLIB
							if (SUCCEEDED(vc->get_NickName(&vis)))
#else
							vclen = sizeof(vcbuf);
							WODXMPPCOMLib::XMPP_VCard_GetNickName(vc, vcbuf, &vclen);
							vis = vcbuf;
#endif
							{
								if (vis.Length())
								{
									if (vis.Length()<sizeof(user->m_VisibleName))
									{
										strcpy(user->m_VisibleName , vis.ToString());
									}
								}
							}
							CComBSTR2 e;
#ifndef _WODXMPPLIB
							if (SUCCEEDED(vc->get_Email(&e)))
#else
							vclen = sizeof(vcbuf);
							WODXMPPCOMLib::XMPP_VCard_GetEmail(vc, vcbuf, &vclen);
							vis = vcbuf;
#endif
							{
								if (e.Length())
								{
									if (e.Length()<sizeof(user->m_Email))
									{
										strcpy(user->m_Email , e.ToString());
									}
								}
							}

							if (!user->m_Image)
								user->m_Image = new CxImage();
							else
								user->m_Image->Clear();


#ifndef _WODXMPPLIB
														SAFEARRAY * psa = NULL;
														if (SUCCEEDED(vc->get_PhotoData(&psa)))
														{
															Buffer b;

															char HUGEP *data;
															unsigned long ubound, lbound;
															SafeArrayGetLBound(psa,1,(long *)&lbound);
															SafeArrayGetUBound(psa,1,(long *)&ubound);
															unsigned long count = ubound-lbound+1;
															SafeArrayAccessData(psa, (void HUGEP**) &data);
															b.Append(data, count);
															SafeArrayUnaccessData(psa);
															SafeArrayDestroy(psa);

#else
														char photobuff[65535];
														int pblen = sizeof(photobuff);
														if (SUCCEEDED(WODXMPPCOMLib::XMPP_VCard_GetPhotoData(vc, photobuff, &pblen)))
														{
															Buffer b;
															b.Append(photobuff, pblen);
#endif

															CxMemFile fTmp((BYTE*)b.Ptr(),b.Len());
															user->m_Image->Decode(&fTmp, GetPhotoType(&b));
//															user->m_Image->IncreaseBpp(24);

//															RGBQUAD r = {255,0,255,0};
//															user->m_Image->SetTransColor(r);
//															user->m_Image->SetTransIndex(0);
																		
															CxMemFile fMem;
															fMem.Open();
															user->m_Image->Encode(&fMem, CXIMAGE_FORMAT_PNG);
//															user->m_Icon.Clear();
//															user->m_Icon.Append((

//															user->m_Icon.Clear();
//															user->m_Icon.Append((char *)fMem.GetBuffer(), fMem.Size());
															user->SaveUserImage((char *)fMem.GetBuffer(), fMem.Size());
															//user->m_Image->Resample(32, 32);
															ResampleImageIfNeeded(user->m_Image, 32);

															if (user->m_SettingsContactsDlg)
															{
																CSettingsDlg::CSettingsUser1 *us1 = (CSettingsDlg::CSettingsUser1 *)user->m_SettingsContactsDlg->m_Dialogs[0];
																us1->InitData(vc);
															}
							}
						}
						RefreshUser(Contact, NULL);
					}
				}
				else
				{
					if (now - REFRESHUSERDETAILS > user->m_GotVCard)
					{
#ifndef _WODXMPPLIB
						WODXMPPCOMLib::IXMPPVCard *vc;
						if (SUCCEEDED(Contact->get_VCard(&vc)))
						{
							vc->Receive();
							vc->Release();
						}
#else
						WODXMPPCOMLib::XMPP_Contact_VCard_Receive(Contact);
#endif
					}
				}
			}
		}
	}
}