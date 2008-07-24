// Notify.cpp: implementation of the CNotify class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Notify.h"
#include "Settings.h"
#include "MainDlg.h"
#include "BaloonHelp.h"
#include "Buffer.h"
#include "NotifyWindow.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNotify _Notify;
extern CSettings _Settings;
extern CMainDlg _MainDlg;

CNotify::CNotify()
{
	m_Owner = NULL;	
	m_LastTick = 0;
}

CNotify::~CNotify()
{
	if (m_NotifyWindow)
	{
		if (m_NotifyWindow->IsWindow())
			m_NotifyWindow->DestroyWindow();
		delete m_NotifyWindow;
		m_NotifyWindow = NULL;
	}
}

void CNotify::Init(CMainDlg *Owner)
{
	m_Owner = Owner;
}

void CNotify::ShowTrayNotify(char *Subject, char *Text)
{
	if (m_NotifyWindow)
	{
		if (m_NotifyWindow->IsWindow())
			m_NotifyWindow->DestroyWindow();
		delete m_NotifyWindow;
		m_NotifyWindow = NULL;
	}

	m_NotifyWindow = new CNotifyWindow();
	m_NotifyWindow->Create(Subject, Text);
}

void CNotify::DoEvent(NotificationsEnum Event)
{
	switch (Event)
	{
		case NotificationOnline:

			Beep(m_Online, IDR_SOUNDON, FALSE);
			break;

		case NotificationOffline:
			Beep(m_Offline, IDR_SOUNDOFF, FALSE);
			break;


		case NotificationMsgIn:
			Beep(m_MsgIn, IDR_SOUNDMSGIN, TRUE);
			break;

		case NotificationMsgOut:
			Beep(m_MsgOut, IDR_SOUNDMSGOUT, TRUE);
			break;
	
		case NotificationError:
			Beep(m_Error, IDR_SOUNDNONO, TRUE);
			break;
	
	}
}

void CNotify::Beep(BSTR Sound, int resID, BOOL Must)
{
	unsigned long t = GetTickCount();
	if (_Settings.m_SoundOn)
	{
		if (Must || (t - m_LastTick > 2000)) // more than 2 seconds
		{
			CComBSTR2 s;
			if (Sound)
				s = Sound;
			if (s.Length())
			{
				sndPlaySound(s.ToString(), SND_ASYNC);
			}
			else
			{
				PlaySound(MAKEINTRESOURCE(resID), _Module.GetModuleInstance() ,SND_RESOURCE | SND_ASYNC | SND_NODEFAULT | SND_NOWAIT);
			}
		}
	}
	m_LastTick = t;
}
