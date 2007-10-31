// Licensedlg.cpp : implementation of the CLicenseDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "versionno.h"
#include "Licensedlg.h"
#include "settings.h"
#include "Jabber.h"
#include "ComBSTR2.h"
#include "MainDlg.h"


//#define COMPILE_MULTIMON_STUBS
#include "MultiMon.h"

char *trim(char *text);

extern CSettings _Settings;
extern CMainDlg _MainDlg;


CLicenseDlg::CLicenseDlg()
{
}

CLicenseDlg::~CLicenseDlg()
{
}

LRESULT CLicenseDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
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

	return TRUE;
}

LRESULT CLicenseDlg::OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CLicenseDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CLicenseDlg::OnOrderNowCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(NULL, "open", "http://wippien.com/order.php", "", "", 0);
	return 0;
}
