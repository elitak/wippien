// BalloonTipDlg.cpp: implementation of the CBalloonTipDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaloonTip.h"
#include "ComBSTR2.h"
#include "Buffer.h"
#include "Settings.h"
#include "MainDlg.h"

extern CSettings _Settings;
extern CMainDlg _MainDlg;

_ATL_FUNC_INFO BaloonTipBeforeNavigate2Info = {CC_STDCALL, VT_EMPTY, 7, {VT_DISPATCH,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_BOOL}};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
CBalloonTipDlg::CBalloonTipDlg(HWND OwnerHwnd)
{
	m_pWB2 = NULL;
	m_OwnerHwnd = OwnerHwnd;
	if (!m_OwnerHwnd)
		m_OwnerHwnd = GetDesktopWindow();
	m_Result = IDCANCEL;
	m_Down = FALSE;
	m_Type = MB_OK;
	m_Events = NULL;
}

CBalloonTipDlg::~CBalloonTipDlg()
{
	if (m_Events)
	{
		delete m_Events;
		m_Events = NULL;
	}
	if (m_pWB2)
		m_pWB2->Release();
}

BOOL CBalloonTipDlg::PreTranslateMessage(MSG* pMsg)
{
	_MainDlg.CheckIfAntiInactivityMessage(pMsg->message);
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CBalloonTipDlg::OnIdle()
{
	return FALSE;
}

/*LRESULT CBalloonTipDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPaintDC dc(m_hWnd);
	{
		CMemDC dcMem(dc);
		RECT rc = {5,5,69,69};
		::FillRect(dc, &rc, CreateSolidBrush(RGB(255,255,255)));
		CxImage *img = new CxImage();
		img->CreateFromHICON(LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDC_BALLOONTIPICON)));
		img->Draw(dc, rc.left, rc.top, rc.right, rc.bottom, NULL, FALSE);
	}
	return TRUE;
}*/


LRESULT CBalloonTipDlg::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	_MainDlg.m_InactiveTimer = 0;
//	m_Down = TRUE;
//	GetCursorPos(&m_DownPoint);
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	return TRUE;
}

LRESULT CBalloonTipDlg::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	_MainDlg.m_InactiveTimer = 0;
//	m_Down = UP;
	return TRUE;
}

LRESULT CBalloonTipDlg::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	_MainDlg.m_InactiveTimer = 0;
	if (m_Down)
	{
		POINT p;
		memcpy(&p, &m_DownPoint, sizeof(POINT));
		GetCursorPos(&m_DownPoint);

//		SetWindowPos(NULL, 
	}
	m_Down = TRUE;
	return TRUE;
}

LRESULT CBalloonTipDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CenterWindow(m_OwnerHwnd);

	HFONT hFont = GetFont();
	LOGFONT lf;
	::GetObject(hFont, sizeof(LOGFONT), &lf);
	lf.lfHeight -= 5;
	lf.lfWeight = FW_BOLD;
	m_fontTitle.CreateFontIndirect(&lf),

	m_wndTitle = GetDlgItem(IDC_BALLOONTIPTEXT);
	m_wndTitle.SetFont(m_fontTitle);

//	m_btnYes.SetCaption("&Yes");
	m_btn1.SetStyle(PBS_UNDERLINE);
	m_btn1.SubclassWindow(GetDlgItem(IDOK));

//	m_btnNo.SetCaption("&No");
	m_btn2.SetStyle(PBS_UNDERLINE);
	m_btn2.SubclassWindow(GetDlgItem(IDCANCEL));

	m_btn3.SetStyle(PBS_UNDERLINE);
	m_btn3.SubclassWindow(GetDlgItem(IDCANCEL2));


	CAxWindow wndIE = GetDlgItem(IDC_BALLOONTIPMESSAGE);
	HRESULT hr;
	hr = wndIE.QueryControl ( &m_pWB2 );

//	m_Events = new CWebBrowserEvents<CBalloonTipDlg>(this, m_pWB2);

	m_Events = new CWebBrowserEvents<CBalloonTipDlg, &BaloonTipBeforeNavigate2Info>(this, m_pWB2);


	if ( m_pWB2 )
    {
		WriteText();
    }	
	bHandled = FALSE;
	return FALSE;
}

void CBalloonTipDlg::SetText(char *Text)
{
	m_Text.Empty();
	m_Text = Text;
	if (m_pWB2)
		WriteText();
}

void CBalloonTipDlg::SetCaption(char *Text)
{
	m_Caption.Empty();
	m_Caption = Text;
	if (m_pWB2)
		SetDlgItemText(IDC_BALLOONTIPTEXT, Text);
}

void CBalloonTipDlg::SetType(int Type)
{
	m_Type = Type;
	if (m_pWB2)
		DrawType();
}

void CBalloonTipDlg::DrawType(void)
{
//	POINT pos = {675, 651};
	POINT pos = {100, 220};


	switch (m_Type & 7)
	{
	case MB_OKCANCEL:
		m_btn1.SetCaption("&Ok");
		m_btn2.SetCaption("&Cancel");
		::ShowWindow(GetDlgItem(IDOK), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL2), SW_HIDE);
		::MoveWindow(GetDlgItem(IDOK), pos.x+80, pos.y, 75, 23, TRUE);
		::MoveWindow(GetDlgItem(IDCANCEL), pos.x+160, pos.y, 75, 23, TRUE);
		break;

	case MB_ABORTRETRYIGNORE:
		m_btn1.SetCaption("&Abort");
		m_btn2.SetCaption("&Retry");
		m_btn3.SetCaption("&Ignore");
		::ShowWindow(GetDlgItem(IDOK), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL2), SW_SHOW);
		::MoveWindow(GetDlgItem(IDOK), pos.x, pos.y, 75, 23, TRUE);
		::MoveWindow(GetDlgItem(IDCANCEL), pos.x+80, pos.y, 75, 23, TRUE);
		::MoveWindow(GetDlgItem(IDCANCEL2), pos.x+160, pos.y, 75, 23, TRUE);
		break;

	case MB_YESNOCANCEL:
		m_btn1.SetCaption("&Yes");
		m_btn2.SetCaption("&No");
		m_btn3.SetCaption("&Cancel");
		::ShowWindow(GetDlgItem(IDOK), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL2), SW_SHOW);
		::MoveWindow(GetDlgItem(IDOK), pos.x, pos.y, 75, 23, TRUE);
		::MoveWindow(GetDlgItem(IDCANCEL), pos.x+80, pos.y, 75, 23, TRUE);
		::MoveWindow(GetDlgItem(IDCANCEL2), pos.x+160, pos.y, 75, 23, TRUE);
		break;

	case MB_YESNO:
		m_btn1.SetCaption("&Yes");
		m_btn2.SetCaption("&No");
		::ShowWindow(GetDlgItem(IDOK), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL2), SW_HIDE);
		::MoveWindow(GetDlgItem(IDOK), pos.x+80, pos.y, 75, 23, TRUE);
		::MoveWindow(GetDlgItem(IDCANCEL), pos.x+160, pos.y, 75, 23, TRUE);
		break;

	case MB_RETRYCANCEL:
		m_btn1.SetCaption("&Retry");
		m_btn2.SetCaption("&Cancel");
		::ShowWindow(GetDlgItem(IDOK), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL2), SW_HIDE);
		::MoveWindow(GetDlgItem(IDOK), pos.x+80, pos.y, 75, 23, TRUE);
		::MoveWindow(GetDlgItem(IDCANCEL), pos.x+160, pos.y, 75, 23, TRUE);
		break;

	default: //MB_OK
		m_btn1.SetCaption("&Ok");
//		SetDlgItemText(IDCANCEL, "&Cancel");
		::ShowWindow(GetDlgItem(IDOK), SW_SHOW);
		::ShowWindow(GetDlgItem(IDCANCEL), SW_HIDE);
		::ShowWindow(GetDlgItem(IDCANCEL2), SW_HIDE);
		::MoveWindow(GetDlgItem(IDOK), pos.x+160, pos.y, 75, 23, TRUE);
		break;
	
	}
}

void CBalloonTipDlg::WriteText(void)
{
	CComBSTR2 mt = m_Caption;
	SetDlgItemText(IDC_BALLOONTIPTEXT, mt.ToString());
	SetWindowText(mt.ToString());

	DrawType();

	CComVariant v;  // empty variant
 	m_pWB2->Navigate (CComBSTR("about:"),&v, &v, &v, &v );

	HRESULT hr = S_OK;

	SAFEARRAY *sf;
	VARIANT *param;

	CComBSTR bstrHTML("<body bgcolor=#e7f0fe>");
	bstrHTML += m_Text;
	bstrHTML += "</body>";

	sf = SafeArrayCreateVector(VT_VARIANT, 0, 1);
			
	if (sf == NULL) 
		return;

	SafeArrayAccessData(sf,(LPVOID*) &param);
	param->vt = VT_BSTR;
	param->bstrVal = bstrHTML;
	SafeArrayUnaccessData(sf);

	IDispatch *lpDisp;
	hr = m_pWB2->get_Document(&lpDisp);
	IHTMLDocument2 *htd = (IHTMLDocument2 *)lpDisp;

	if (htd)
	{
		hr = htd->write(sf);
		// change default font, margins, etc
		IHTMLElement *body;
		htd->get_body(&body);
	
		IHTMLStyle *style;
		CComVariant vfontSize(CString("9pt"));
		VARIANT v0;
		v0.vt = VT_I2;
		v0.intVal = 0;
		CComBSTR2 b2 = "Arial";

		body->get_style(&style);

		style->put_marginTop(v0);
		style->put_marginLeft(v0);
		style->put_marginRight(v0);
		style->put_marginBottom(v0);
		style->put_fontFamily(b2);
		style->put_fontSize(vfontSize);	

		htd->Release();
	}
	m_Events->m_Cancel = TRUE;
}

int CBalloonTipDlg::Show(HWND hWnd, char *Text, char *Caption, int Type)
{
	CBalloonTipDlg dlg(hWnd);
	dlg.SetText(Text);
	dlg.SetCaption(Caption);
	dlg.SetType(Type);
	dlg.SetType(Type);
	dlg.DoModal();

	return dlg.m_Result;
}

void CWebBrowserEvents<CBalloonTipDlg, &BaloonTipBeforeNavigate2Info>::__BeforeNavigate2(/*[in]*/ IDispatch* pDisp, /*[in]*/ VARIANT* URL, /*[in]*/ VARIANT* Flags, 
		/*[in]*/ VARIANT* TargetFrameName, /*[in]*/ VARIANT* PostData, /*[in]*/ VARIANT* Headers, 
		/*[out]*/ VARIANT_BOOL* Cancel)
{
	if (m_Cancel)
	{
		if (Cancel)
			*Cancel = VARIANT_TRUE;

		if (URL)
		{
			CComBSTR2 url;	

			if (URL->vt == VT_BSTR)
				url = URL->bstrVal;
			else
			if (URL->vt == (VT_BSTR | VT_BYREF))
				url = *URL->pbstrVal;

			if (url.Length())
			{
				ShellExecute(NULL, "open", url.ToString(), "", "", 0);
			}
		}
	}
}
