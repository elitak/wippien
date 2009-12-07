// NotifyWindow.h: interface for the CNotifyWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTIFYWINDOW_H__650CB038_7315_4E59_8993_01006A962AC1__INCLUDED_)
#define AFX_NOTIFYWINDOW_H__650CB038_7315_4E59_8993_01006A962AC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include <atlcrack.h>
#include "../CxImage/CxImage/ximage.h"

class Buffer;
class CNotifyWindow: public CWindowImpl<CNotifyWindow>
{
private:
	
	typedef CNotifyWindow thisClass;
	typedef CWindowImpl<CNotifyWindow>		WindowBase;
	
public:
	
	DECLARE_WND_CLASS_EX(_T("XPNotifyWindow"),CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS|CS_DBLCLKS,COLOR_WINDOW)
		
	CNotifyWindow();
	virtual ~CNotifyWindow();

	CxImage *m_Image;

	BOOL Create(char *Subject, char *Subtext, char *Text);

	RECT m_Rect, m_ShowRect;
	HFONT m_SubjectFont, m_TextFont;
	int m_State;
	int m_Timer;
	void OnPaint(HDC dc);
	void OnTimer(UINT id, TIMERPROC);
	void OnMouseMove(UINT wParam, CPoint p);
	Buffer *m_Subject, *m_Text, *m_Subtext;

	BEGIN_MSG_MAP_EX(thisClass)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_MOUSEMOVE(OnMouseMove)
	END_MSG_MAP()

};

#endif // !defined(AFX_NOTIFYWINDOW_H__650CB038_7315_4E59_8993_01006A962AC1__INCLUDED_)
