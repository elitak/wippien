// ChatRoomWindow.h: interface for the CChatRoomWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATROOMWINDOW_H__3E627B65_FBBF_4C75_8DEB_BEE3BA96E2AE__INCLUDED_)
#define AFX_CHATROOMWINDOW_H__3E627B65_FBBF_4C75_8DEB_BEE3BA96E2AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMsgWin;

class CChatRoom
{
public:
	CChatRoom();
	virtual ~CChatRoom();

	// GUI
	CMsgWin *m_MessageWin;
	void OpenMsgWindow(BOOL WithFocus);
	BOOL IsMsgWindowOpen(void);
	void CloseMsgWindow(void);
	void PrintMsgWindow(BOOL IsSystem, char *Text, char *Html);

	char m_JID[1024];
	char m_Nick[1024];
	RECT m_ChatWindowRect;

};

#endif // !defined(AFX_CHATROOMWINDOW_H__3E627B65_FBBF_4C75_8DEB_BEE3BA96E2AE__INCLUDED_)
