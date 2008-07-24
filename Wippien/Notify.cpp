// Notify.cpp: implementation of the CNotify class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Notify.h"
#include "Settings.h"
#include "MainDlg.h"
#include "BaloonHelp.h"
#include "Buffer.h"

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

}

void CNotify::Init(CMainDlg *Owner)
{
	m_Owner = Owner;
}

void CNotify::ShowTrayNotify(char *Subject, char *Text)
{
	if (_MainDlg.m_TrayBalloon)
	{
		if (_MainDlg.m_TrayBalloon->IsWindow())
			_MainDlg.m_TrayBalloon->DestroyWindow();
		delete _MainDlg.m_TrayBalloon;
		_MainDlg.m_TrayBalloon = NULL;
	}
	
	_MainDlg.m_TrayBalloon =new CBalloonHelp;
	POINT p;
	RECT rc;
	GetClientRect(GetDesktopWindow(), &rc);
	p.x = rc.right;
	p.y	= rc.bottom;
	Buffer b;
	b.Append(Subject);
	b.Append(" ");
	b.Append(Text);
	_MainDlg.m_TrayBalloon->Create(NULL, "Wippien notificiation", b.Ptr(), NULL, &p,	
		CBalloonHelp::BallonOptions::BOCloseOnKeyDown | 
		CBalloonHelp::BallonOptions::BOShowCloseButton | 
		CBalloonHelp::BallonOptions::BOCloseOnAppDeactivate | 
		CBalloonHelp::BallonOptions::BOShowTopMost | 
		NULL,
		0);

	_MainDlg.SetTimer(114, 5000);
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
