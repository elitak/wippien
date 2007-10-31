// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "versionno.h"
#include "aboutdlg.h"
#include "settings.h"
#include "Jabber.h"
#include "ComBSTR2.h"
#include "MainDlg.h"
#include "UpdateHandler.h"

BOOL _LoadIconFromResource(CxImage *img, char *restype, int imgformat, int resid);
char *trim(char *text);
extern CJabber *_Jabber;
extern CSettings _Settings;
extern CMainDlg _MainDlg;

#define ABOUT_SCROLL_TEXT	"Special thanks go to: \r\n\r\nhttp://www.afterglow.ie\r\nfor excellent set of 'forum faces' icons used in Wippien.\r\n\r\n"


CAboutDlg::CAboutDlg()
{
	DumpDebug("*CAboutDlg::CAboutDlg\r\n");
	m_WhiteBrush = CreateSolidBrush(RGB(255,255,255));
}

CAboutDlg::~CAboutDlg()
{
	DumpDebug("*CAboutDlg::~CAboutDlg\r\n");
	DeleteObject(m_WhiteBrush);
}

LRESULT CAboutDlg::OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	DumpDebug("*CAboutDlg::OnCltColorStatic\r\n");
	return (LRESULT)m_WhiteBrush;
}

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DumpDebug("*CAboutDlg::OnInitDialog\r\n");
/*	RECT rc;
	GetWindowRect(&rc);
	SetWindowPos(HWND_TOPMOST, rc.left, rc.top, 400, 300, SWP_NOSIZE | SWP_NOMOVE);
	CenterWindow(GetDesktopWindow());
*/

	RECT rcScreen;
	RECT rcMain;
	::GetWindowRect(_MainDlg.m_hWnd, &rcMain);
	POINT p;
	p.x = rcMain.left;
	p.y = rcMain.top;

	MONITORINFO mi;
	
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	HMONITOR hmon = MonitorFromPoint(p, MONITOR_DEFAULTTONEAREST);
	if (hmon && GetMonitorInfo(hmon, &mi))
	{
		memcpy(&rcScreen, &mi.rcWork, sizeof(RECT));
	}
	else
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScreen, 0);

//	::GetWindowRect(GetDesktopWindow(), &rcScreen);

	RECT rcDlg;
	GetClientRect(&rcDlg);

	rcDlg.left = (rcScreen.right-rcDlg.right+rcScreen.left+rcDlg.left )/2;
	rcDlg.top = (rcScreen.bottom-rcDlg.bottom+rcScreen.top+rcDlg.top)/2;

	SetWindowPos(NULL, &rcDlg, SWP_NOSIZE | SWP_NOZORDER);
	

	_LoadIconFromResource(&m_Image, "JPG", CXIMAGE_FORMAT_JPG, IDR_SPLASH);

	SetDlgItemText(IDC_WIPPIEN_VERSION, MYDESCTEXTVER);

	CComBSTR2 jab;
	if (_Jabber)
	{
		_Jabber->m_Jabb->get_Version(&jab);

	}
	SetDlgItemText(IDC_WODJABBER_VERSION, jab.ToString());

	if (_UpdateHandler && _UpdateHandler->m_Update)
	{
		CComBSTR2 app;
#ifndef _APPUPDLIB
		_UpdateHandler->m_Update->get_Version(&app);
		SetDlgItemText(IDC_WODAPPUPDATE_VERSION, app.ToString());
#else
		char buff[1024] = {0};
		int len = sizeof(buff);
		WODAPPUPDCOMLib::AppUpd_GetVersion(_UpdateHandler->m_Update, buff, &len);
		SetDlgItemText(IDC_WODAPPUPDATE_VERSION, buff);
#endif	

#ifndef _WODVPNLIB
		SetDlgItemText(IDC_WODVPN_VERSION, app.ToString());
#else
		len = sizeof(buff);
		void *vpn = WODVPNCOMLib::_VPN_Create(NULL);
		WODVPNCOMLib::VPN_GetVersion(vpn, buff, &len);
		WODVPNCOMLib::_VPN_Destroy(vpn);
		SetDlgItemText(IDC_WODVPN_VERSION, buff);
#endif	
	
	}

	RECT rc1 = {0};
	rc1.left = 210;
	rc1.top = 90;
	rc1.right = 390;
	rc1.bottom = 250;
	m_scroller.Create(rc1, m_hWnd);
	m_scroller.SetText(ABOUT_SCROLL_TEXT);
	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DumpDebug("*CAboutDlg::OnCloseCmd\r\n");
	EndDialog(wID);
	return 0;
}


LRESULT CAboutDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DumpDebug("*CAboutDlg::OnPaint\r\n");
	PAINTSTRUCT ps;
	HDC h = BeginPaint(&ps);
	m_Image.Draw(ps.hdc, 1, 1, 400, 300, NULL, FALSE);

	EndPaint(&ps);

	return TRUE;
}
