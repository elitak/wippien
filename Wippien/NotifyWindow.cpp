// NotifyWindow.cpp: implementation of the CNotifyWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "atlgdix.h"
#include "ShellApi.h"
#include "NotifyWindow.h"
#include "Buffer.h"
#include "MainDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL _LoadIconFromResource(CxImage *img, char *restype, int imgformat, int resid);
extern CMainDlg _MainDlg;

CNotifyWindow::CNotifyWindow()
{
	m_Image = NULL;
	m_State = 0;
	m_Timer = 0;
	m_SubjectFont = m_TextFont = NULL;
	m_Subject = NULL;
	m_Subtext = NULL;
	m_Text = NULL;
}

CNotifyWindow::~CNotifyWindow()
{
	if (m_Image)
		delete m_Image;
	if (m_SubjectFont)
		DeleteObject(m_SubjectFont);
	if (m_TextFont)
		DeleteObject(m_TextFont);
	if (m_Subject)
		delete m_Subject;
	if (m_Text)
		delete m_Text;
	if (m_Subtext)
		delete m_Subtext;
}

BOOL CNotifyWindow::Create(char *Subject, char *Subtext, char *Text)
{
	DWORD dwExStyle=WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	
	CRect rcPos(0,0,0,0);
	if(!WindowBase::Create(NULL,rcPos,"",WS_POPUP,dwExStyle))
		return FALSE;

	m_Image = new CxImage();
	_LoadIconFromResource(m_Image, "PNG", CXIMAGE_FORMAT_PNG, IDB_NOTIFYWINDOWBACK);


    HDC hdc;
    long h1, h2;
    
    hdc = ::GetDC(NULL);
    h1 = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    h2 = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ::ReleaseDC(NULL, hdc);
    m_SubjectFont = CreateFont(h1, 0, 0, 0, FW_BOLD, FALSE, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");
    m_TextFont = CreateFont(h2, 0, 0, 0, FW_NORMAL, FALSE, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Tahoma");

	m_Subject = new Buffer();
	m_Subject->Append(Subject);
	m_Text = new Buffer();
	m_Text->Append(Text);
	m_Subtext = new Buffer();
	if (*Subtext)
	{
		m_Subtext->Append("(");
		m_Subtext->Append(Subtext);
		m_Subtext->Append(")");
	}

	APPBARDATA	abd;
	memset(&abd, 0, sizeof(APPBARDATA));
	abd.cbSize = sizeof(APPBARDATA);	
	SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

	::GetWindowRect(GetDesktopWindow(), &m_Rect);
	if (abd.uEdge == ABE_BOTTOM)
		m_Rect.bottom -= (abd.rc.bottom - abd.rc.top);
	if (abd.uEdge == ABE_RIGHT)
		m_Rect.right -= (abd.rc.right - abd.rc.left);

	m_Rect.top = m_Rect.bottom - m_Image->GetHeight();
	m_Rect.left = m_Rect.right - m_Image->GetWidth();

	memcpy(&m_ShowRect, &m_Rect, sizeof(RECT));
	m_ShowRect.top = m_Rect.bottom;
	SetWindowPos(NULL, &m_ShowRect, SWP_SHOWWINDOW | SWP_NOACTIVATE);

	SetTimer(1, 100);
	return TRUE;
}

void CNotifyWindow::OnPaint(HDC dc)
{
	CPaintDC pdc = CPaintDC(m_hWnd);
	CMemDC mc = CMemDC(pdc.m_hDC);

	m_Image->Draw(mc.m_hDC, 0, 0);
	RECT rc = {50,10,m_Image->GetWidth(), m_Image->GetHeight()};
	HGDIOBJ font = ::SelectObject(mc.m_hDC, m_SubjectFont);
	::SetBkMode(mc.m_hDC, TRANSPARENT);
	::DrawText(mc.m_hDC, m_Subject->Ptr(), m_Subject->Len(), &rc, DT_CENTER);
	rc.top += 20;
	::SelectObject(mc.m_hDC, m_TextFont);
	::DrawText(mc.m_hDC, m_Subtext->Ptr(), m_Subtext->Len(), &rc, DT_CENTER);
	rc.top += 20;
	rc.left -= 30;
	::DrawText(mc.m_hDC, m_Text->Ptr(), m_Text->Len(), &rc, DT_CENTER);
//	rc.left -= 30;
	::SelectObject(mc.m_hDC, font);
	::BitBlt(pdc.m_hDC, 0, 0, m_Image->GetWidth(), m_Image->GetHeight(), mc.m_hDC, 0, 0, SRCCOPY);
}

void CNotifyWindow::OnTimer(UINT id)
{
	switch (m_State)
	{
		case 0:
			if (m_ShowRect.top != m_Rect.top)
			{
				m_ShowRect.top-=10;
				if (m_ShowRect.top < m_Rect.top)
					m_ShowRect.top = m_Rect.top;
				MoveWindow(&m_ShowRect);
//				SetWindowPos(NULL, &m_ShowRect, SWP_NOACTIVATE);
			}
			else
			{
				m_Timer = 0;
				m_State = 1;
			}
			break;

		case 1:
			m_Timer ++;
			if (m_Timer>50)
			{
				m_Timer = 0;
				m_State = 2;
			}
			break;

		case 2:
			if (m_ShowRect.top != m_Rect.bottom)
			{
				m_ShowRect.top+=20;
				if (m_ShowRect.top > m_Rect.bottom)
					m_ShowRect.top = m_Rect.bottom;
				MoveWindow(&m_ShowRect);
			}
			else
			{
				m_Timer = 0;
				m_State = 3;
			}
			break;

		default:
			ShowWindow(SW_HIDE);
			KillTimer(id);
			break;
	}
}

void CNotifyWindow::OnMouseMove(UINT wParam, CPoint p)
{
	_MainDlg.CheckIfAntiInactivityMessage(WM_CHAR);
	ShowWindow(SW_HIDE);
	m_State = 4;

}