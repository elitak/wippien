// WippienSDK.h : Declaration of the CWippienSDK

#ifndef __WIPPIENSDK_H_
#define __WIPPIENSDK_H_

#include "resource.h"       // main symbols
#include "SDKMessageLink.h"
#include "SDKCP.h"

/////////////////////////////////////////////////////////////////////////////
// CWippienSDK
class ATL_NO_VTABLE CWippienSDK : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWippienSDK, &CLSID_WippienSDK>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CWippienSDK>,
	public IDispatchImpl<IWippienSDK, &IID_IWippienSDK, &LIBID_WIPPIENSDKCOMLib>,
	public CProxy_IWippienSDKEvents< CWippienSDK >
{
public:
	CWippienSDK();
	~CWippienSDK();

	CSDKMessageLink *m_Link;

	HRESULT get_String(int Cmd, BSTR *pVal);
	int GetUserID(VARIANT User);
	HRESULT get_UserProp(VARIANT User, int msg, BSTR *pVal);	
	HRESULT put_UserProp(VARIANT User, int msg, BSTR newVal);


DECLARE_REGISTRY_RESOURCEID(IDR_WIPPIENSDK)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CWippienSDK)
	COM_INTERFACE_ENTRY(IWippienSDK)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CWippienSDK)
CONNECTION_POINT_ENTRY(DIID__IWippienSDKEvents)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IWippienSDK
public:
	STDMETHOD(get_StatusText)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_StatusText)(/*[in]*/ BSTR newVal);
	STDMETHOD(Disconnect)();
	STDMETHOD(Connect)();
	STDMETHOD(CreateAccount)(BSTR JID, BSTR Password, BSTR Hostname, int Port);
	STDMETHOD(get_Sound)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Sound)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(RemoveContact)(BSTR JID, VARIANT_BOOL Unsubscribe);
	STDMETHOD(AddContact)(BSTR JID, VARIANT_BOOL Subscribe);
	STDMETHOD(UserConnectIP)(VARIANT User, int IP);
	STDMETHOD(ChangePassword)(BSTR OldPassword, BSTR NewPassword, VARIANT_BOOL ProtectAll);
	STDMETHOD(get_UserConnectedPort)(VARIANT User, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_UserConnectedIP)(VARIANT User, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(UserConnect)(VARIANT User);
	STDMETHOD(get_UserIP)(VARIANT User, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_MyNetmask)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_MyIP)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Port)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Port)(/*[in]*/ long newVal);
	STDMETHOD(get_Hostname)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Hostname)(/*[in]*/ BSTR newVal);
	STDMETHOD(Die)();
	STDMETHOD(SaveSettings)();
	STDMETHOD(get_Status)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Status)(/*[in]*/ long newVal);
	STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_JID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_JID)(/*[in]*/ BSTR newVal);
//	STDMETHOD(Free)();
//	STDMETHOD(Initialize)();

	STDMETHOD(get_UserCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(SendMessage)(VARIANT User, BSTR PlainText, BSTR HtmlText);
	STDMETHOD(CloseChatWindow)(VARIANT User);
	STDMETHOD(OpenChatWindow)(VARIANT User);
	STDMETHOD(get_UserVPNStatus)(VARIANT User, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_UserPresenceStatus)(VARIANT User, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_UserBlock)(VARIANT User, /*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_UserBlock)(VARIANT User, /*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_UserGroup)(VARIANT User, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UserGroup)(VARIANT User, /*[in]*/ BSTR newVal);
	STDMETHOD(get_UserVisibleName)(VARIANT User, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UserVisibleName)(VARIANT User, /*[in]*/ BSTR newVal);
	STDMETHOD(get_UserEmail)(VARIANT User, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UserEmail)(VARIANT User, /*[in]*/ BSTR newVal);
	STDMETHOD(get_UserResource)(VARIANT User, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UserResource)(VARIANT User, /*[in]*/ BSTR newVal);
	STDMETHOD(get_UserJID)(VARIANT User, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UserJID)(VARIANT User, /*[in]*/ BSTR newVal);



// events:
	BOOL FireStatusChange(long OldState, long NewState)
	{
		long olddwRef;
		this->AddRef();

		Fire_StatusChange(OldState, NewState);
	
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireUserPresenceChange(BSTR JID, long NewPresence, long OldPresence)
	{
		long olddwRef;
		this->AddRef();

		Fire_UserPresenceChange(JID, NewPresence, OldPresence);
	
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireUserVPNChange(BSTR JID, long NewPresence, long OldPresence)
	{
		long olddwRef;
		this->AddRef();

		Fire_UserVPNChange(JID, NewPresence, OldPresence);
	
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireIncomingMessage(BSTR JID, BSTR *PlainText, BSTR *HtmlText)
	{
		long olddwRef;
		this->AddRef();

		Fire_IncomingMessage(JID, PlainText, HtmlText);
	
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireConnected(void)
	{
		long olddwRef;
		this->AddRef();

		Fire_Connected();
	
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireDisconnected(BSTR Error)
	{
		long olddwRef;
		this->AddRef();

		Fire_Disconnected(Error);
	
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireMediatorMessage(BSTR JID, BSTR *Text)
	{
		long olddwRef;
		this->AddRef();

		Fire_MediatorMessage(JID, Text);
	
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireAccountCreated(BSTR Error)
	{
		long olddwRef;
		this->AddRef();

		Fire_AccountCreated(Error);
	
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireAuthRequest(BSTR JID)
	{
		long olddwRef;
		this->AddRef();

		Fire_AuthRequest(JID);
	
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireDie(void)
	{
		long olddwRef;
		this->AddRef();
		
		Fire_Die();
		
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;
		
		return TRUE;
	}
};

#endif //__WIPPIENSDK_H_
