// ChatRoom.cpp: implementation of the CChatRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatRoom.h"
#include "MsgWin.h"
#include "Settings.h"
#include "Jabber.h"
#include "MainDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CSettings _Settings;
extern CJabber *_Jabber;
extern CMainDlg _MainDlg;

CChatRoom::CChatRoom()
{
	m_MessageWin = NULL;
	m_JID[0] = 0;
	CComBSTR2 n = _Settings.m_Nick;
	strcpy(m_Nick, n.ToString());
	memset(&m_ChatWindowRect, 0, sizeof(m_ChatWindowRect));
	m_Block = FALSE;
}

CChatRoom::~CChatRoom()
{
	for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
	{
		CUser *us = _MainDlg.m_UserList.m_Users[i];
		if (us->m_ChatRoomPtr == this)
		{
			_MainDlg.m_UserList.m_Users.erase(_MainDlg.m_UserList.m_Users.begin() + i);
			delete us;
			i--;
		}
	}

	CComBSTR2 j = m_JID;
	char *j1 = j.ToString();
	char *j2 = strchr(j1, '@');
	if (j2)
		*j2 = 0;

	for (i=0;i<_Settings.m_Groups.size();i++)
	{
		CSettings::TreeGroup *tg = _Settings.m_Groups[i];
		if (tg->Temporary && (!strcmp(tg->Name, j1) || !strcmp(tg->Name, m_JID)))
		{
			_Settings.m_Groups.erase(_Settings.m_Groups.begin() + i);
			free(tg->Name);
			delete tg;
			break;
		}
	}

	if (m_MessageWin)
	{
		if (m_MessageWin->IsWindow())
			m_MessageWin->DestroyWindow();
		delete m_MessageWin;
	}

	::PostMessage(_MainDlg.m_UserList.m_hWnd, WM_REFRESH, 0,FALSE);
}

BOOL CChatRoom::IsMsgWindowOpen(void)
{
	if (m_MessageWin && m_MessageWin->m_hWnd && IsWindow(m_MessageWin->m_hWnd))
		return TRUE;

	return FALSE;
}

void CChatRoom::OpenMsgWindow(BOOL WithFocus)
{
	if (!m_MessageWin)
	{
		m_MessageWin = new CMsgWin(this);

	}
	if (!m_MessageWin->m_hWnd)
	{
//		LoadUserImage(m_MessageWin->m_HumanHead);
		m_MessageWin->Create(NULL);
		m_MessageWin->Show();

	}

	if (WithFocus)
	{
		SetActiveWindow(m_MessageWin->m_hWnd);
	
		if (m_MessageWin->m_InputBox.IsWindow())
			m_MessageWin->m_InputBox.SetFocus();
	}
}

void CChatRoom::CloseMsgWindow(void)
{
	if (m_MessageWin)
	{
		if (::IsWindow(m_MessageWin->m_hWnd))
			::DestroyWindow(m_MessageWin->m_hWnd);
		delete m_MessageWin;
		m_MessageWin = NULL;
	}
}

void CChatRoom::PrintMsgWindow(char *Nick, BOOL IsSystem, char *Text, char *Html)
{
	OpenMsgWindow(FALSE);
	if (!IsSystem)
		FlashWindow(m_MessageWin->m_hWnd, TRUE);

	if ((Text && *Text) || (Html && *Html))
		m_MessageWin->Incoming(Nick, IsSystem, Text, Html);
}

void CChatRoom::Leave(void)
{
#ifndef _WODXMPPLIB
#error TODO
#else
		void *chatroom = NULL;
		WODXMPPCOMLib::XMPP_GetChatRoomByName(_Jabber->m_Jabb, m_JID, &chatroom);
		if (chatroom)
		{
			WODXMPPCOMLib::XMPP_ChatRoom_Leave(chatroom);
		}	
#endif

/*		for (i=0;i<_MainDlg.m_ChatRooms.size();i++)
		{
			CChatRoom *room = _MainDlg.m_ChatRooms[i];
			if (room == this)
			{
				_MainDlg.m_ChatRooms.erase(_MainDlg.m_ChatRooms.begin()+i);
//				delete this;
				break;
			}
		}
*/
		
		::PostMessage(_MainDlg.m_UserList.m_hWnd, WM_REFRESH, 0,FALSE);
}
