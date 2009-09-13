// DownloadSkinDlg.cpp: implementation of the CDownloadSkinDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloadSkinDlg.h"
#include "ComBSTR2.h"
//#include <wininet.h>
#include "Buffer.h"
#include "Settings.h"
//#include <io.h>
//#include <fcntl.h>
//#include <sys/stat.h>
#include "SettingsDlg.h"
#include "ProgressDlg.h"


extern CSettings _Settings;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Define the sWeb events to be handled:
  // delete pwEvents;

_ATL_FUNC_INFO SkinDlgBeforeNavigate2Info = {CC_STDCALL, VT_EMPTY, 7, {VT_DISPATCH,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_BOOL}};

CDownloadSkinDlg::CDownloadSkinDlg(HWND Owner)
{
	m_Events = NULL;
	m_OwnerHwnd = Owner;
	m_pWB2 = NULL;
}

CDownloadSkinDlg::~CDownloadSkinDlg()
{
	if (m_Events)
	{
		delete m_Events;
		m_Events = NULL;
	}
	if (m_pWB2)
	{
		m_pWB2->Release();
		m_pWB2 = NULL;
	}
}

BOOL CDownloadSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CDownloadSkinDlg::OnIdle()
{
	return FALSE;
}

LRESULT CDownloadSkinDlg::OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CDownloadSkinDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow();
	CAxWindow wndIE = GetDlgItem(IDC_EXPLORER1);
	HRESULT hr;
	hr = wndIE.QueryControl ( &m_pWB2 );

	m_Events = new CWebBrowserEvents<CDownloadSkinDlg, &SkinDlgBeforeNavigate2Info>(this, m_pWB2);


	if ( m_pWB2 )
    {
		CComVariant v;  // empty variant
 
		m_pWB2->Navigate ( CComBSTR("http://www.wippien.com/SkinUpdate.php"), &v, &v, &v, &v );
    }	


	SetWindowText(_Settings.Translate("Download more skins from Wippien website"));
	SetDlgItemText(IDC_S1, _Settings.Translate("Click on the skin to download and install it."));
	SetDlgItemText(ID_CANCEL, _Settings.Translate("&Cancel"));
	return FALSE;
}


void CWebBrowserEvents<CDownloadSkinDlg, &SkinDlgBeforeNavigate2Info>::__BeforeNavigate2(/*[in]*/ IDispatch* pDisp, /*[in]*/ VARIANT* URL, /*[in]*/ VARIANT* Flags, 
		/*[in]*/ VARIANT* TargetFrameName, /*[in]*/ VARIANT* PostData, /*[in]*/ VARIANT* Headers, 
		/*[out]*/ VARIANT_BOOL* Cancel)
{
	if (m_Initial)
	{
		*Cancel = VARIANT_FALSE;
		m_Initial = FALSE;
	}
	else
	{
		// get URL
		CComBSTR2 url;

		if (URL->vt = VT_BSTR)
			url = URL->bstrVal;
		else
		if (URL->vt == (VT_BSTR | VT_BYREF))
			url = *URL->pbstrVal;

		if (url.Length())
		{
			char *u = url.ToString();
			// go to the end
			int i = strlen(u);
			while (i>0 && u[i]!='/')
				i--;

			if (u[i]=='/')
				i++;

			// create two threads to download files
//			DWORD id;
			Buffer *b = new Buffer;

			char *a = &u[i];
			while (&a && *a != '.') a++;
			if (*a == '.')
				*a = 0;


			b->PutInt((unsigned int)m_Owner->m_OwnerHwnd);
			b->PutCString(&u[i]);
			b->PutCString(_Settings.m_MyPath);
			CProgressDlg *dlg = new CProgressDlg();
			dlg->InitDownloadSkin(b);
//			CreateThread(NULL, 0, DownloadThreadProc, b, 0, &id);
			m_Owner->EndDialog(0);


		}
		*Cancel = VARIANT_TRUE;
	}
}
