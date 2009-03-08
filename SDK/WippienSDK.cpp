// WippienSDK.cpp : Implementation of CWippienSDK
#include "stdafx.h"
#include "SDK.h"
#include "WippienSDK.h"
#include "../../Public/Wippien/ComBSTR2.h"
#include "../../Public/Wippien/Buffer.h"


/////////////////////////////////////////////////////////////////////////////
// CWippienSDK

STDMETHODIMP CWippienSDK::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IWippienSDK
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::ATL::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

CWippienSDK::CWippienSDK()
{
	m_Link = new CSDKMessageLink(this);
	m_Link->CreateLinkWindow();

}

CWippienSDK::~CWippienSDK()
{
	delete m_Link;
}

HRESULT CWippienSDK::get_String(int Cmd, BSTR *pVal)
{
	if (!m_Link->SendCommandWaitResult(Cmd, 0))
		return E_FAIL;
	if (!m_Link->m_WaitBuffer->Len())
		return E_FAIL;

	CComBSTR b = m_Link->m_WaitBuffer->Ptr();
	*pVal = b.Copy();
	return S_OK;
}


/*STDMETHODIMP CWippienSDK::Initialize()
{
	return S_OK;
}

STDMETHODIMP CWippienSDK::Free()
{
	return S_OK;
}
*/

STDMETHODIMP CWippienSDK::put_Visible(VARIANT_BOOL newVal)
{
	int succ = FALSE;
	if (newVal)
		succ = m_Link->SendCommand(WM_WIPPIEN_VISIBLE, 1);
	else
		succ = m_Link->SendCommand(WM_WIPPIEN_VISIBLE, 0);

	if (!succ)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWippienSDK::get_Status(long *pVal)
{
	int succ = m_Link->SendCommand(WM_WIPPIEN_GETSTATUS, 0);
	if (succ)
	{
		*pVal = succ-1;
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CWippienSDK::put_Status(long newVal)
{
	if (!m_Link->SendCommand(WM_WIPPIEN_SETSTATUS, newVal))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWippienSDK::SaveSettings()
{
	if (!m_Link->SendCommand(WM_WIPPIEN_SAVESETTINGS, 1))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CWippienSDK::Die()
{
	if (!m_Link->SendCommand(WM_WIPPIEN_DIE, 1))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_Hostname(BSTR *pVal)
{
	return get_String(WM_WIPPIEN_GETHOSTNAME, pVal);
}

STDMETHODIMP CWippienSDK::put_Hostname(BSTR newVal)
{
	CComBSTR2 h = newVal;
	if (!m_Link->SendCommand(WM_WIPPIEN_SETHOSTNAME, h.ToString(), h.Length()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_Port(long *pVal)
{
	long succ = m_Link->SendCommand(WM_WIPPIEN_GETPORT, 0);
	if (!succ)
		return E_FAIL;
	*pVal = succ - 1;
	return S_OK;
}

STDMETHODIMP CWippienSDK::put_Port(long newVal)
{
	if (!m_Link->SendCommand(WM_WIPPIEN_SETPORT, newVal))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_JID(BSTR *pVal)
{
	return get_String(WM_WIPPIEN_GETJID, pVal);
}

STDMETHODIMP CWippienSDK::put_JID(BSTR newVal)
{
	CComBSTR2 h = newVal;
	if (!m_Link->SendCommand(WM_WIPPIEN_SETJID, h.ToString(), h.Length()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::OpenChatWindow(VARIANT User)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	b.PutInt(i-1);
	b.PutChar(1);
	if (!m_Link->SendCommand(WM_WIPPIEN_CHATWINDOW, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::CloseChatWindow(VARIANT User)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	b.PutInt(i-1);
	b.PutChar(0);
	if (!m_Link->SendCommand(WM_WIPPIEN_CHATWINDOW, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}
STDMETHODIMP CWippienSDK::SendMessage(VARIANT User, BSTR PlainText, BSTR HtmlText)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	b.PutInt(i-1);
	b.PutCString(PlainText);
	b.PutCString(HtmlText);
	if (!m_Link->SendCommand(WM_WIPPIEN_SENDMESSAGE, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_UserCount(long *pVal)
{
	long succ = m_Link->SendCommand(WM_WIPPIEN_GETUSERCOUNT, 0);
	if (!succ)
		return E_FAIL;
	*pVal = succ - 1;
	return S_OK;
}

int CWippienSDK::GetUserID(VARIANT User)
{
	long succ = 0;

	if (User.vt == VT_I2)
	{
		return User.iVal+1;
	}
	else
	if (User.vt == VT_I4)
	{
		return User.lVal+1;
	}
	else
	if (User.vt == (VT_I2 | VT_BYREF))
	{
		return (*User.piVal)+1;
	}
	else
	if (User.vt == (VT_I4 | VT_BYREF))
	{
		return (*User.plVal)+1;
	}
	else
	if (User.vt == VT_BSTR)
	{
		CComBSTR2 h = User.bstrVal;
		succ = m_Link->SendCommand(WM_WIPPIEN_GETUSERID, h.ToString(), h.Length());
	}
	else
	if (User.vt == (VT_BSTR | VT_BYREF))
	{
		CComBSTR2 h = *User.pbstrVal;
		succ = m_Link->SendCommand(WM_WIPPIEN_GETUSERID, h.ToString(), h.Length());
	}

	return succ;
}

HRESULT CWippienSDK::get_UserProp(VARIANT User, int msg, BSTR *pVal)
{
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	if (!m_Link->SendCommandWaitResult(msg, i-1))
		return E_FAIL;
	if (!m_Link->m_WaitBuffer->Len())
		return E_FAIL;

	CComBSTR b1 = m_Link->m_WaitBuffer->Ptr();
	*pVal = b1.Copy();
	return S_OK;
}

HRESULT CWippienSDK::put_UserProp(VARIANT User, int msg, BSTR newVal)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	b.PutInt(i-1);
	b.PutCString(newVal);
	if (!m_Link->SendCommand(msg, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_UserJID(VARIANT User, BSTR *pVal)
{
	return get_UserProp(User, WM_WIPPIEN_GETUSERJID, pVal);
}

STDMETHODIMP CWippienSDK::put_UserJID(VARIANT User, BSTR newVal)
{
	return put_UserProp(User, WM_WIPPIEN_PUTUSERJID, newVal);
}

STDMETHODIMP CWippienSDK::get_UserResource(VARIANT User, BSTR *pVal)
{
	return get_UserProp(User, WM_WIPPIEN_GETUSERRESOURCE, pVal);
}

STDMETHODIMP CWippienSDK::put_UserResource(VARIANT User, BSTR newVal)
{
	return put_UserProp(User, WM_WIPPIEN_PUTUSERRESOURCE, newVal);
}

STDMETHODIMP CWippienSDK::get_UserEmail(VARIANT User, BSTR *pVal)
{
	return get_UserProp(User, WM_WIPPIEN_GETUSEREMAIL, pVal);
}

STDMETHODIMP CWippienSDK::put_UserEmail(VARIANT User, BSTR newVal)
{
	return put_UserProp(User, WM_WIPPIEN_PUTUSEREMAIL, newVal);
}

STDMETHODIMP CWippienSDK::get_UserVisibleName(VARIANT User, BSTR *pVal)
{
	return get_UserProp(User, WM_WIPPIEN_GETUSERVISIBLENAME, pVal);
}

STDMETHODIMP CWippienSDK::put_UserVisibleName(VARIANT User, BSTR newVal)
{
	return put_UserProp(User, WM_WIPPIEN_PUTUSERVISIBLENAME, newVal);
}

STDMETHODIMP CWippienSDK::get_UserGroup(VARIANT User, BSTR *pVal)
{
	return get_UserProp(User, WM_WIPPIEN_GETUSERGROUP, pVal);
}

STDMETHODIMP CWippienSDK::put_UserGroup(VARIANT User, BSTR newVal)
{
	return put_UserProp(User, WM_WIPPIEN_PUTUSERGROUP, newVal);
}

STDMETHODIMP CWippienSDK::get_UserBlock(VARIANT User, VARIANT_BOOL *pVal)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	b.PutInt(i-1);

	i = m_Link->SendCommand(WM_WIPPIEN_GETUSERBLOCK, i-1);
	if (!i)
		return E_FAIL;
	i--;

	*pVal = i?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CWippienSDK::put_UserBlock(VARIANT User, VARIANT_BOOL newVal)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	b.PutInt(i-1);
	b.PutChar(newVal?TRUE:FALSE);
	if (!m_Link->SendCommand(WM_WIPPIEN_PUTUSERBLOCK, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_UserVPNStatus(VARIANT User, long *pVal)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	b.PutInt(i-1);

	i = m_Link->SendCommand(WM_WIPPIEN_GETUSERVPNSTATUS, i-1);
	if (!i)
		return E_FAIL;
	i--;

	*pVal = i;
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_UserPresenceStatus(VARIANT User, long *pVal)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	i = m_Link->SendCommand(WM_WIPPIEN_GETUSERPRESENCESTATUS, i-1);
	if (!i)
		return E_FAIL;
	i--;

	*pVal = i;
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_MyIP(BSTR *pVal)
{
	return get_String(WM_WIPPIEN_GETMYIP, pVal);
}

STDMETHODIMP CWippienSDK::get_MyNetmask(BSTR *pVal)
{
	return get_String(WM_WIPPIEN_GETMYNETMASK, pVal);
}

STDMETHODIMP CWippienSDK::get_UserIP(VARIANT User, BSTR *pVal)
{
	return get_UserProp(User, WM_WIPPIEN_GETUSERIP, pVal);
}

STDMETHODIMP CWippienSDK::UserConnect(VARIANT User)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	if (!m_Link->SendCommand(WM_WIPPIEN_USERCONNECT, i-1))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_UserConnectedIP(VARIANT User, BSTR *pVal)
{
	return get_UserProp(User, WM_WIPPIEN_GETUSERCONNECTEDIP, pVal);
}

STDMETHODIMP CWippienSDK::get_UserConnectedPort(VARIANT User, long *pVal)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	b.PutInt(i-1);

	i = m_Link->SendCommand(WM_WIPPIEN_GETUSERCONNECTEDPORT, i-1);
	if (!i)
		return E_FAIL;
	i--;

	*pVal = i;
	return S_OK;
}

STDMETHODIMP CWippienSDK::ChangePassword(BSTR OldPassword, BSTR NewPassword, VARIANT_BOOL ProtectAll)
{
	_Buffer b;
	b.PutCString(OldPassword);
	b.PutCString(NewPassword);
	b.PutChar(ProtectAll?TRUE:FALSE);
	if (!m_Link->SendCommand(WM_WIPPIEN_CHANGEPASSWORD, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::UserConnectIP(VARIANT User, int IP)
{
	_Buffer b;
	int i = GetUserID(User);

	if (!i)
		return E_FAIL;

	b.PutInt(i-1);
	b.PutInt(IP);
	if (!m_Link->SendCommand(WM_WIPPIEN_USERCONNECTIP, b.Ptr(), b.Len()))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CWippienSDK::AddContact(BSTR JID, VARIANT_BOOL Subscribe)
{
	_Buffer b;
	b.PutCString(JID);
	b.PutChar(Subscribe?TRUE:FALSE);
	if (!m_Link->SendCommand(WM_WIPPIEN_ADDCONTACT, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::RemoveContact(BSTR JID, VARIANT_BOOL Unsubscribe)
{
	_Buffer b;
	b.PutCString(JID);
	b.PutChar(Unsubscribe?TRUE:FALSE);
	if (!m_Link->SendCommand(WM_WIPPIEN_REMOVECONTACT, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_Sound(VARIANT_BOOL *pVal)
{
	int succ = m_Link->SendCommand(WM_WIPPIEN_GETSOUND, 0);
	if (succ)
	{
		*pVal = succ-1;
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CWippienSDK::put_Sound(VARIANT_BOOL newVal)
{
	if (!m_Link->SendCommand(WM_WIPPIEN_SETSOUND, newVal))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWippienSDK::CreateAccount(BSTR JID, BSTR Password, BSTR Hostname, int Port)
{
	_Buffer b;
	b.PutCString(JID);
	b.PutCString(Password);
	b.PutCString(Hostname);
	b.PutInt(Port);
	if (!m_Link->SendCommand(WM_WIPPIEN_CREATECONTACT, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::Connect()
{
	_Buffer b;
	b.PutInt(TRUE);
	if (!m_Link->SendCommand(WM_WIPPIEN_CONNECT, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::Disconnect()
{
	_Buffer b;
	b.PutInt(FALSE);
	if (!m_Link->SendCommand(WM_WIPPIEN_CONNECT, b.Ptr(), b.Len()))
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CWippienSDK::get_StatusText(BSTR *pVal)
{
	return get_String(WM_WIPPIEN_GETSTATUSTEXT, pVal);
}

STDMETHODIMP CWippienSDK::put_StatusText(BSTR newVal)
{
	CComBSTR2 h = newVal;
	if (!m_Link->SendCommand(WM_WIPPIEN_SETSTATUSTEXT, h.ToString(), h.Length()))
		return E_FAIL;
	
	return S_OK;
}
