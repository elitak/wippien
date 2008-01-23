// ChatRoom.cpp: implementation of the CChatRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatRoom.h"
#include "MsgWin.h"
#include "Settings.h"
#include "Jabber.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CSettings _Settings;
extern CJabber *_Jabber;

CChatRoom::CChatRoom()
{
	m_MessageWin = NULL;
	m_JID[0] = 0;
	CComBSTR2 n = _Settings.m_Nick;
	strcpy(m_Nick, n.ToString());
	memset(&m_ChatWindowRect, 0, sizeof(m_ChatWindowRect));
}

CChatRoom::~CChatRoom()
{
	if (m_MessageWin)
	{
		if (m_MessageWin->m_hWnd)
			::DestroyWindow(m_MessageWin->m_hWnd);
		delete m_MessageWin;
	}
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


}

