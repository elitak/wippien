// Notify.h: interface for the CNotify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTIFY_H__45596673_978B_4297_8EC4_53265AD152D4__INCLUDED_)
#define AFX_NOTIFY_H__45596673_978B_4297_8EC4_53265AD152D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

typedef enum NotificationsEnum
{
	NotificationOnline = 1,
	NotificationOffline = 2, 
	NotificationMsgIn = 3,
	NotificationMsgOut = 4,
	NotificationError = 5
} NotificationsEnum;

class CMainDlg;
class CNotifyWindow;

class CNotify  
{
public:
	CNotify();
	virtual ~CNotify();

	CNotifyWindow *m_NotifyWindow;

	CComBSTR m_Online, m_Offline, m_MsgIn, m_MsgOut, m_Error;
	void Init(CMainDlg *Owner);
	void DoEvent(NotificationsEnum Event);
	unsigned long m_LastTick;
	void Beep(BSTR Sound, int resID, BOOL Must);
	void ShowTrayNotify(char *Subject, char *Text);

private:
	CMainDlg *m_Owner;

};

#endif // !defined(AFX_NOTIFY_H__45596673_978B_4297_8EC4_53265AD152D4__INCLUDED_)
