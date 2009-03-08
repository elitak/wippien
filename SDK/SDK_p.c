/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Mar 08 23:21:40 2009
 */
/* Compiler settings for D:\Wippien\Public\SDK\SDK.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "SDK.h"

#define TYPE_FORMAT_STRING_SIZE   1043                              
#define PROC_FORMAT_STRING_SIZE   1427                              

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IWippienSDK, ver. 0.0,
   GUID={0xCAD56E0F,0x4F78,0x4D9B,{0xB4,0x25,0x92,0xB8,0xA5,0x36,0x1D,0x6D}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IWippienSDK_ServerInfo;

#pragma code_seg(".orpc")
/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserVPNStatus_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [retval][out] */ long __RPC_FAR *pVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[766],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[766],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&User,
                  ( unsigned char __RPC_FAR * )&pVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[766],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_OpenChatWindow_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,User);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[800],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[800],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&User);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[800],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_CloseChatWindow_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,User);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[828],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[828],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&User);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[828],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_SendMessage_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    BSTR PlainText,
    BSTR HtmlText)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,HtmlText);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[856],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[856],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&User,
                  ( unsigned char __RPC_FAR * )&PlainText,
                  ( unsigned char __RPC_FAR * )&HtmlText);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[856],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_MyIP_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[896],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[896],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[896],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_MyNetmask_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[924],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[924],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[924],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserIP_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User,
    /* [retval][out] */ BSTR __RPC_FAR *pVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[952],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[952],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&User,
                  ( unsigned char __RPC_FAR * )&pVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[952],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_UserConnect_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,User);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[986],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[986],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&User);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[986],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserConnectedIP_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User,
    /* [retval][out] */ BSTR __RPC_FAR *pVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1014],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1014],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&User,
                  ( unsigned char __RPC_FAR * )&pVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1014],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserConnectedPort_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User,
    /* [retval][out] */ long __RPC_FAR *pVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1048],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1048],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&User,
                  ( unsigned char __RPC_FAR * )&pVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1048],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_ChangePassword_Proxy( 
    IWippienSDK __RPC_FAR * This,
    BSTR OldPassword,
    BSTR NewPassword,
    VARIANT_BOOL ProtectAll)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,ProtectAll);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1082],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1082],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&OldPassword,
                  ( unsigned char __RPC_FAR * )&NewPassword,
                  ( unsigned char __RPC_FAR * )&ProtectAll);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1082],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_UserConnectIP_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User,
    int IP)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,IP);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1122],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1122],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&User,
                  ( unsigned char __RPC_FAR * )&IP);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1122],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_AddContact_Proxy( 
    IWippienSDK __RPC_FAR * This,
    BSTR JID,
    VARIANT_BOOL Subscribe)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,Subscribe);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1156],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1156],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&JID,
                  ( unsigned char __RPC_FAR * )&Subscribe);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1156],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_RemoveContact_Proxy( 
    IWippienSDK __RPC_FAR * This,
    BSTR JID,
    VARIANT_BOOL Unsubscribe)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,Unsubscribe);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1190],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1190],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&JID,
                  ( unsigned char __RPC_FAR * )&Unsubscribe);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1190],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_Sound_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1224],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1224],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1224],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_Sound_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,newVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1252],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1252],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&newVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1252],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_CreateAccount_Proxy( 
    IWippienSDK __RPC_FAR * This,
    BSTR JID,
    BSTR Password,
    BSTR Hostname,
    int Port)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,Port);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1280],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1280],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&JID,
                  ( unsigned char __RPC_FAR * )&Password,
                  ( unsigned char __RPC_FAR * )&Hostname,
                  ( unsigned char __RPC_FAR * )&Port);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1280],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_Connect_Proxy( 
    IWippienSDK __RPC_FAR * This)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,This);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1326],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1326],
                  ( unsigned char __RPC_FAR * )&This);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1326],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_Disconnect_Proxy( 
    IWippienSDK __RPC_FAR * This)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,This);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1348],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1348],
                  ( unsigned char __RPC_FAR * )&This);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1348],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_StatusText_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1370],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1370],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1370],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_StatusText_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ BSTR newVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,newVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1398],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1398],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&newVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1398],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[2];

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

static const unsigned short IWippienSDK_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    28,
    56,
    84,
    112,
    140,
    162,
    184,
    212,
    240,
    268,
    296,
    324,
    358,
    392,
    426,
    460,
    494,
    528,
    562,
    596,
    630,
    664,
    698,
    732,
    766,
    800,
    828,
    856,
    896,
    924,
    952,
    986,
    1014,
    1048,
    1082,
    1122,
    1156,
    1190,
    1224,
    1252,
    1280,
    1326,
    1348,
    1370,
    1398
    };

static const MIDL_SERVER_INFO IWippienSDK_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IWippienSDK_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IWippienSDK_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IWippienSDK_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(53) _IWippienSDKProxyVtbl = 
{
    &IWippienSDK_ProxyInfo,
    &IID_IWippienSDK,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)-1 /* IWippienSDK::get_JID */ ,
    (void *)-1 /* IWippienSDK::put_JID */ ,
    (void *)-1 /* IWippienSDK::put_Visible */ ,
    (void *)-1 /* IWippienSDK::get_Status */ ,
    (void *)-1 /* IWippienSDK::put_Status */ ,
    (void *)-1 /* IWippienSDK::SaveSettings */ ,
    (void *)-1 /* IWippienSDK::Die */ ,
    (void *)-1 /* IWippienSDK::get_Hostname */ ,
    (void *)-1 /* IWippienSDK::put_Hostname */ ,
    (void *)-1 /* IWippienSDK::get_Port */ ,
    (void *)-1 /* IWippienSDK::put_Port */ ,
    (void *)-1 /* IWippienSDK::get_UserCount */ ,
    (void *)-1 /* IWippienSDK::get_UserJID */ ,
    (void *)-1 /* IWippienSDK::put_UserJID */ ,
    (void *)-1 /* IWippienSDK::get_UserResource */ ,
    (void *)-1 /* IWippienSDK::put_UserResource */ ,
    (void *)-1 /* IWippienSDK::get_UserEmail */ ,
    (void *)-1 /* IWippienSDK::put_UserEmail */ ,
    (void *)-1 /* IWippienSDK::get_UserVisibleName */ ,
    (void *)-1 /* IWippienSDK::put_UserVisibleName */ ,
    (void *)-1 /* IWippienSDK::get_UserGroup */ ,
    (void *)-1 /* IWippienSDK::put_UserGroup */ ,
    (void *)-1 /* IWippienSDK::get_UserBlock */ ,
    (void *)-1 /* IWippienSDK::put_UserBlock */ ,
    (void *)-1 /* IWippienSDK::get_UserPresenceStatus */ ,
    IWippienSDK_get_UserVPNStatus_Proxy ,
    IWippienSDK_OpenChatWindow_Proxy ,
    IWippienSDK_CloseChatWindow_Proxy ,
    IWippienSDK_SendMessage_Proxy ,
    IWippienSDK_get_MyIP_Proxy ,
    IWippienSDK_get_MyNetmask_Proxy ,
    IWippienSDK_get_UserIP_Proxy ,
    IWippienSDK_UserConnect_Proxy ,
    IWippienSDK_get_UserConnectedIP_Proxy ,
    IWippienSDK_get_UserConnectedPort_Proxy ,
    IWippienSDK_ChangePassword_Proxy ,
    IWippienSDK_UserConnectIP_Proxy ,
    IWippienSDK_AddContact_Proxy ,
    IWippienSDK_RemoveContact_Proxy ,
    IWippienSDK_get_Sound_Proxy ,
    IWippienSDK_put_Sound_Proxy ,
    IWippienSDK_CreateAccount_Proxy ,
    IWippienSDK_Connect_Proxy ,
    IWippienSDK_Disconnect_Proxy ,
    IWippienSDK_get_StatusText_Proxy ,
    IWippienSDK_put_StatusText_Proxy
};


static const PRPC_STUB_FUNCTION IWippienSDK_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IWippienSDKStubVtbl =
{
    &IID_IWippienSDK,
    &IWippienSDK_ServerInfo,
    53,
    &IWippienSDK_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[2] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute, more than 32 methods in the interface.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure get_JID */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/*  8 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 16 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 18 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 20 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 22 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 24 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 26 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_JID */

/* 28 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 30 */	NdrFcLong( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 36 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 38 */	NdrFcShort( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0x8 ),	/* 8 */
/* 42 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 44 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 46 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 48 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 50 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 52 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 54 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Visible */

/* 56 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 58 */	NdrFcLong( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
/* 64 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 66 */	NdrFcShort( 0x6 ),	/* 6 */
/* 68 */	NdrFcShort( 0x8 ),	/* 8 */
/* 70 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 72 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 74 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 76 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 80 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Status */

/* 84 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
/* 92 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	NdrFcShort( 0x10 ),	/* 16 */
/* 98 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 100 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 102 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 104 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 106 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 108 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 110 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Status */

/* 112 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 114 */	NdrFcLong( 0x0 ),	/* 0 */
/* 118 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 120 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 122 */	NdrFcShort( 0x8 ),	/* 8 */
/* 124 */	NdrFcShort( 0x8 ),	/* 8 */
/* 126 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 128 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 130 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 132 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 134 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 136 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SaveSettings */

/* 140 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 142 */	NdrFcLong( 0x0 ),	/* 0 */
/* 146 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 148 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 154 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 156 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 158 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 160 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Die */

/* 162 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 164 */	NdrFcLong( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 170 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0x8 ),	/* 8 */
/* 176 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 178 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 180 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 182 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Hostname */

/* 184 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 186 */	NdrFcLong( 0x0 ),	/* 0 */
/* 190 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 192 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 196 */	NdrFcShort( 0x8 ),	/* 8 */
/* 198 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 200 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 202 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 204 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 206 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 208 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 210 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Hostname */

/* 212 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 214 */	NdrFcLong( 0x0 ),	/* 0 */
/* 218 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 220 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 222 */	NdrFcShort( 0x0 ),	/* 0 */
/* 224 */	NdrFcShort( 0x8 ),	/* 8 */
/* 226 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 228 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 230 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 232 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 234 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 236 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 238 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Port */

/* 240 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 242 */	NdrFcLong( 0x0 ),	/* 0 */
/* 246 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 248 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */
/* 252 */	NdrFcShort( 0x10 ),	/* 16 */
/* 254 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 256 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 258 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 260 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 262 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 264 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 266 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Port */

/* 268 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 270 */	NdrFcLong( 0x0 ),	/* 0 */
/* 274 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 276 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 278 */	NdrFcShort( 0x8 ),	/* 8 */
/* 280 */	NdrFcShort( 0x8 ),	/* 8 */
/* 282 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 284 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 286 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 288 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 290 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 292 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 294 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserCount */

/* 296 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 298 */	NdrFcLong( 0x0 ),	/* 0 */
/* 302 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 304 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 308 */	NdrFcShort( 0x10 ),	/* 16 */
/* 310 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 312 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 314 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 320 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserJID */

/* 324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0x13 ),	/* 19 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 332 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 338 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 340 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 342 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 344 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 346 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 348 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 350 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 352 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 354 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 356 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_UserJID */

/* 358 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 360 */	NdrFcLong( 0x0 ),	/* 0 */
/* 364 */	NdrFcShort( 0x14 ),	/* 20 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 366 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 370 */	NdrFcShort( 0x8 ),	/* 8 */
/* 372 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 374 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 376 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 378 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter newVal */

/* 380 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 382 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 384 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 386 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 388 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 390 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserResource */

/* 392 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 394 */	NdrFcLong( 0x0 ),	/* 0 */
/* 398 */	NdrFcShort( 0x15 ),	/* 21 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 400 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 404 */	NdrFcShort( 0x8 ),	/* 8 */
/* 406 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 408 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 410 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 412 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 414 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 416 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 418 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 420 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 422 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 424 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_UserResource */

/* 426 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 428 */	NdrFcLong( 0x0 ),	/* 0 */
/* 432 */	NdrFcShort( 0x16 ),	/* 22 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 434 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0x8 ),	/* 8 */
/* 440 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 442 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 444 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 446 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter newVal */

/* 448 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 450 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 452 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 454 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 456 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 458 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserEmail */

/* 460 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 462 */	NdrFcLong( 0x0 ),	/* 0 */
/* 466 */	NdrFcShort( 0x17 ),	/* 23 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 468 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 472 */	NdrFcShort( 0x8 ),	/* 8 */
/* 474 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 476 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 478 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 480 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 482 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 484 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 486 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 488 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 490 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 492 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_UserEmail */

/* 494 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 496 */	NdrFcLong( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0x18 ),	/* 24 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 502 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 506 */	NdrFcShort( 0x8 ),	/* 8 */
/* 508 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 510 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 512 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 514 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter newVal */

/* 516 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 518 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 520 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 522 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 524 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 526 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserVisibleName */

/* 528 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 530 */	NdrFcLong( 0x0 ),	/* 0 */
/* 534 */	NdrFcShort( 0x19 ),	/* 25 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 536 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 540 */	NdrFcShort( 0x8 ),	/* 8 */
/* 542 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 544 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 546 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 548 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 550 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 552 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 554 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 556 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 558 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 560 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_UserVisibleName */

/* 562 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 564 */	NdrFcLong( 0x0 ),	/* 0 */
/* 568 */	NdrFcShort( 0x1a ),	/* 26 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 570 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0x8 ),	/* 8 */
/* 576 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 578 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 580 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 582 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter newVal */

/* 584 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 586 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 588 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 590 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 592 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 594 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserGroup */

/* 596 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 598 */	NdrFcLong( 0x0 ),	/* 0 */
/* 602 */	NdrFcShort( 0x1b ),	/* 27 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 604 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 608 */	NdrFcShort( 0x8 ),	/* 8 */
/* 610 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 612 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 614 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 616 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 618 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 620 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 622 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 624 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 626 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 628 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_UserGroup */

/* 630 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 632 */	NdrFcLong( 0x0 ),	/* 0 */
/* 636 */	NdrFcShort( 0x1c ),	/* 28 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 638 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0x8 ),	/* 8 */
/* 644 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 646 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 648 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 650 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter newVal */

/* 652 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 654 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 656 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 658 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 660 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 662 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserBlock */

/* 664 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 666 */	NdrFcLong( 0x0 ),	/* 0 */
/* 670 */	NdrFcShort( 0x1d ),	/* 29 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 672 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0xe ),	/* 14 */
/* 678 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 680 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 682 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 684 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 686 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 688 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 690 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 692 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 694 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 696 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_UserBlock */

/* 698 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 700 */	NdrFcLong( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x1e ),	/* 30 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 706 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 708 */	NdrFcShort( 0x6 ),	/* 6 */
/* 710 */	NdrFcShort( 0x8 ),	/* 8 */
/* 712 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 714 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 716 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 718 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter newVal */

/* 720 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 722 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 724 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 726 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 728 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 730 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserPresenceStatus */

/* 732 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 734 */	NdrFcLong( 0x0 ),	/* 0 */
/* 738 */	NdrFcShort( 0x1f ),	/* 31 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 740 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x10 ),	/* 16 */
/* 746 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 748 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 750 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 752 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 754 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 756 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 758 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 760 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 762 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 764 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserVPNStatus */

/* 766 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 768 */	NdrFcLong( 0x0 ),	/* 0 */
/* 772 */	NdrFcShort( 0x20 ),	/* 32 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 774 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 778 */	NdrFcShort( 0x10 ),	/* 16 */
/* 780 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 782 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 784 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 786 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 788 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 790 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 792 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 794 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 796 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 798 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OpenChatWindow */

/* 800 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 802 */	NdrFcLong( 0x0 ),	/* 0 */
/* 806 */	NdrFcShort( 0x21 ),	/* 33 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 808 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 812 */	NdrFcShort( 0x8 ),	/* 8 */
/* 814 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter User */

/* 816 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 818 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 820 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Return value */

/* 822 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 824 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 826 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CloseChatWindow */

/* 828 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 830 */	NdrFcLong( 0x0 ),	/* 0 */
/* 834 */	NdrFcShort( 0x22 ),	/* 34 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 836 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 840 */	NdrFcShort( 0x8 ),	/* 8 */
/* 842 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter User */

/* 844 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 846 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 848 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Return value */

/* 850 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 852 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 854 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendMessage */

/* 856 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 858 */	NdrFcLong( 0x0 ),	/* 0 */
/* 862 */	NdrFcShort( 0x23 ),	/* 35 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 864 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
#else
			NdrFcShort( 0x24 ),	/* MIPS & PPC Stack size/offset = 36 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 866 */	NdrFcShort( 0x0 ),	/* 0 */
/* 868 */	NdrFcShort( 0x8 ),	/* 8 */
/* 870 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter User */

/* 872 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 874 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 876 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter PlainText */

/* 878 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 880 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 882 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter HtmlText */

/* 884 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 886 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 888 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 890 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 892 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 894 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_MyIP */

/* 896 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 898 */	NdrFcLong( 0x0 ),	/* 0 */
/* 902 */	NdrFcShort( 0x24 ),	/* 36 */
#ifndef _ALPHA_
/* 904 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 908 */	NdrFcShort( 0x8 ),	/* 8 */
/* 910 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 912 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 914 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 916 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 918 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 920 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 922 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_MyNetmask */

/* 924 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 926 */	NdrFcLong( 0x0 ),	/* 0 */
/* 930 */	NdrFcShort( 0x25 ),	/* 37 */
#ifndef _ALPHA_
/* 932 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 934 */	NdrFcShort( 0x0 ),	/* 0 */
/* 936 */	NdrFcShort( 0x8 ),	/* 8 */
/* 938 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 940 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 942 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 944 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 946 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 948 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 950 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserIP */

/* 952 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 954 */	NdrFcLong( 0x0 ),	/* 0 */
/* 958 */	NdrFcShort( 0x26 ),	/* 38 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 960 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 964 */	NdrFcShort( 0x8 ),	/* 8 */
/* 966 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 968 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 970 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 972 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 974 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 976 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 978 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 980 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 982 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 984 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UserConnect */

/* 986 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 988 */	NdrFcLong( 0x0 ),	/* 0 */
/* 992 */	NdrFcShort( 0x27 ),	/* 39 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 994 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 996 */	NdrFcShort( 0x0 ),	/* 0 */
/* 998 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1000 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter User */

/* 1002 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1004 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1006 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Return value */

/* 1008 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1010 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1012 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserConnectedIP */

/* 1014 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1016 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1020 */	NdrFcShort( 0x28 ),	/* 40 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1022 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1024 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1026 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1028 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 1030 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1032 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1034 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 1036 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1038 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1040 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 1042 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1044 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1046 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserConnectedPort */

/* 1048 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1050 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1054 */	NdrFcShort( 0x29 ),	/* 41 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1056 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1060 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1062 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 1064 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1066 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1068 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter pVal */

/* 1070 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1072 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1074 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1076 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1078 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1080 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ChangePassword */

/* 1082 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1084 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1088 */	NdrFcShort( 0x2a ),	/* 42 */
#ifndef _ALPHA_
/* 1090 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1092 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1094 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1096 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter OldPassword */

/* 1098 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1100 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1102 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter NewPassword */

/* 1104 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1106 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1108 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter ProtectAll */

/* 1110 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1112 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1114 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1116 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1118 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1120 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UserConnectIP */

/* 1122 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1124 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1128 */	NdrFcShort( 0x2b ),	/* 43 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1130 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1132 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1134 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1136 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter User */

/* 1138 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1140 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1142 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Parameter IP */

/* 1144 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1146 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1148 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1150 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1152 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1154 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddContact */

/* 1156 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1158 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1162 */	NdrFcShort( 0x2c ),	/* 44 */
#ifndef _ALPHA_
/* 1164 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1166 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1168 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1170 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter JID */

/* 1172 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1174 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1176 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter Subscribe */

/* 1178 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1180 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1182 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1184 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1186 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1188 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveContact */

/* 1190 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1192 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1196 */	NdrFcShort( 0x2d ),	/* 45 */
#ifndef _ALPHA_
/* 1198 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1200 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1202 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1204 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter JID */

/* 1206 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1208 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1210 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter Unsubscribe */

/* 1212 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1214 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1216 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1218 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1220 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1222 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Sound */

/* 1224 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1230 */	NdrFcShort( 0x2e ),	/* 46 */
#ifndef _ALPHA_
/* 1232 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1236 */	NdrFcShort( 0xe ),	/* 14 */
/* 1238 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 1240 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1242 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1244 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1248 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Sound */

/* 1252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1258 */	NdrFcShort( 0x2f ),	/* 47 */
#ifndef _ALPHA_
/* 1260 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1262 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1264 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1266 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 1268 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1270 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1272 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1274 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1276 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1278 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateAccount */

/* 1280 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1282 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1286 */	NdrFcShort( 0x30 ),	/* 48 */
#ifndef _ALPHA_
/* 1288 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1290 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1292 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1294 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter JID */

/* 1296 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1298 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1300 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter Password */

/* 1302 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1304 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1306 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter Hostname */

/* 1308 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1310 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1312 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter Port */

/* 1314 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1316 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1318 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1320 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1322 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1324 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Connect */

/* 1326 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1328 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1332 */	NdrFcShort( 0x31 ),	/* 49 */
#ifndef _ALPHA_
/* 1334 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1336 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1338 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1340 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 1342 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1344 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1346 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Disconnect */

/* 1348 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1350 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1354 */	NdrFcShort( 0x32 ),	/* 50 */
#ifndef _ALPHA_
/* 1356 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1358 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1360 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1362 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 1364 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1366 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1368 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_StatusText */

/* 1370 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1372 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1376 */	NdrFcShort( 0x33 ),	/* 51 */
#ifndef _ALPHA_
/* 1378 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1382 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1384 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 1386 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1388 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1390 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Return value */

/* 1392 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1394 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1396 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_StatusText */

/* 1398 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1400 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1404 */	NdrFcShort( 0x34 ),	/* 52 */
#ifndef _ALPHA_
/* 1406 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1410 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1412 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 1414 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1416 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1418 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 1420 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1422 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1424 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  4 */	NdrFcShort( 0x1a ),	/* Offset= 26 (30) */
/*  6 */	
			0x13, 0x0,	/* FC_OP */
/*  8 */	NdrFcShort( 0xc ),	/* Offset= 12 (20) */
/* 10 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* 2 */
/* 14 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 16 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 18 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 20 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 22 */	NdrFcShort( 0x8 ),	/* 8 */
/* 24 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (10) */
/* 26 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 28 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 30 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 32 */	NdrFcShort( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0x4 ),	/* 4 */
/* 36 */	NdrFcShort( 0x0 ),	/* 0 */
/* 38 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (6) */
/* 40 */	
			0x12, 0x0,	/* FC_UP */
/* 42 */	NdrFcShort( 0xffffffea ),	/* Offset= -22 (20) */
/* 44 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x4 ),	/* 4 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (40) */
/* 54 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 56 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 58 */	
			0x12, 0x0,	/* FC_UP */
/* 60 */	NdrFcShort( 0x3b4 ),	/* Offset= 948 (1008) */
/* 62 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 64 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 66 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 68 */	NdrFcShort( 0x2 ),	/* Offset= 2 (70) */
/* 70 */	NdrFcShort( 0x10 ),	/* 16 */
/* 72 */	NdrFcShort( 0x2f ),	/* 47 */
/* 74 */	NdrFcLong( 0x14 ),	/* 20 */
/* 78 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 80 */	NdrFcLong( 0x3 ),	/* 3 */
/* 84 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 86 */	NdrFcLong( 0x11 ),	/* 17 */
/* 90 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 92 */	NdrFcLong( 0x2 ),	/* 2 */
/* 96 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 98 */	NdrFcLong( 0x4 ),	/* 4 */
/* 102 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 104 */	NdrFcLong( 0x5 ),	/* 5 */
/* 108 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 110 */	NdrFcLong( 0xb ),	/* 11 */
/* 114 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 116 */	NdrFcLong( 0xa ),	/* 10 */
/* 120 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 122 */	NdrFcLong( 0x6 ),	/* 6 */
/* 126 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (358) */
/* 128 */	NdrFcLong( 0x7 ),	/* 7 */
/* 132 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 134 */	NdrFcLong( 0x8 ),	/* 8 */
/* 138 */	NdrFcShort( 0xffffff9e ),	/* Offset= -98 (40) */
/* 140 */	NdrFcLong( 0xd ),	/* 13 */
/* 144 */	NdrFcShort( 0xdc ),	/* Offset= 220 (364) */
/* 146 */	NdrFcLong( 0x9 ),	/* 9 */
/* 150 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (382) */
/* 152 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 156 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (400) */
/* 158 */	NdrFcLong( 0x24 ),	/* 36 */
/* 162 */	NdrFcShort( 0x302 ),	/* Offset= 770 (932) */
/* 164 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 168 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (932) */
/* 170 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 174 */	NdrFcShort( 0x2fa ),	/* Offset= 762 (936) */
/* 176 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 180 */	NdrFcShort( 0x2f8 ),	/* Offset= 760 (940) */
/* 182 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 186 */	NdrFcShort( 0x2f6 ),	/* Offset= 758 (944) */
/* 188 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 192 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (948) */
/* 194 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 198 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (952) */
/* 200 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 204 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (956) */
/* 206 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 210 */	NdrFcShort( 0x2da ),	/* Offset= 730 (940) */
/* 212 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 216 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (944) */
/* 218 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 222 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (960) */
/* 224 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 228 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (956) */
/* 230 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 234 */	NdrFcShort( 0x2da ),	/* Offset= 730 (964) */
/* 236 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 240 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (968) */
/* 242 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 246 */	NdrFcShort( 0x2d6 ),	/* Offset= 726 (972) */
/* 248 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 252 */	NdrFcShort( 0x2d4 ),	/* Offset= 724 (976) */
/* 254 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 258 */	NdrFcShort( 0x2d2 ),	/* Offset= 722 (980) */
/* 260 */	NdrFcLong( 0x10 ),	/* 16 */
/* 264 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 266 */	NdrFcLong( 0x12 ),	/* 18 */
/* 270 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 272 */	NdrFcLong( 0x13 ),	/* 19 */
/* 276 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 278 */	NdrFcLong( 0x15 ),	/* 21 */
/* 282 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 284 */	NdrFcLong( 0x16 ),	/* 22 */
/* 288 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 290 */	NdrFcLong( 0x17 ),	/* 23 */
/* 294 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 296 */	NdrFcLong( 0xe ),	/* 14 */
/* 300 */	NdrFcShort( 0x2b0 ),	/* Offset= 688 (988) */
/* 302 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 306 */	NdrFcShort( 0x2b6 ),	/* Offset= 694 (1000) */
/* 308 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 312 */	NdrFcShort( 0x2b4 ),	/* Offset= 692 (1004) */
/* 314 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 318 */	NdrFcShort( 0x26e ),	/* Offset= 622 (940) */
/* 320 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 324 */	NdrFcShort( 0x26c ),	/* Offset= 620 (944) */
/* 326 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 330 */	NdrFcShort( 0x26a ),	/* Offset= 618 (948) */
/* 332 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 336 */	NdrFcShort( 0x260 ),	/* Offset= 608 (944) */
/* 338 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 342 */	NdrFcShort( 0x25a ),	/* Offset= 602 (944) */
/* 344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x0 ),	/* Offset= 0 (348) */
/* 350 */	NdrFcLong( 0x1 ),	/* 1 */
/* 354 */	NdrFcShort( 0x0 ),	/* Offset= 0 (354) */
/* 356 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (355) */
/* 358 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 360 */	NdrFcShort( 0x8 ),	/* 8 */
/* 362 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 364 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 366 */	NdrFcLong( 0x0 ),	/* 0 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x0 ),	/* 0 */
/* 374 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 376 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 378 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 380 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 382 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 384 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 392 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 394 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 396 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 398 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 400 */	
			0x12, 0x10,	/* FC_UP */
/* 402 */	NdrFcShort( 0x2 ),	/* Offset= 2 (404) */
/* 404 */	
			0x12, 0x0,	/* FC_UP */
/* 406 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (914) */
/* 408 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 410 */	NdrFcShort( 0x18 ),	/* 24 */
/* 412 */	NdrFcShort( 0xa ),	/* 10 */
/* 414 */	NdrFcLong( 0x8 ),	/* 8 */
/* 418 */	NdrFcShort( 0x58 ),	/* Offset= 88 (506) */
/* 420 */	NdrFcLong( 0xd ),	/* 13 */
/* 424 */	NdrFcShort( 0x78 ),	/* Offset= 120 (544) */
/* 426 */	NdrFcLong( 0x9 ),	/* 9 */
/* 430 */	NdrFcShort( 0x94 ),	/* Offset= 148 (578) */
/* 432 */	NdrFcLong( 0xc ),	/* 12 */
/* 436 */	NdrFcShort( 0xbc ),	/* Offset= 188 (624) */
/* 438 */	NdrFcLong( 0x24 ),	/* 36 */
/* 442 */	NdrFcShort( 0x114 ),	/* Offset= 276 (718) */
/* 444 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 448 */	NdrFcShort( 0x130 ),	/* Offset= 304 (752) */
/* 450 */	NdrFcLong( 0x10 ),	/* 16 */
/* 454 */	NdrFcShort( 0x148 ),	/* Offset= 328 (782) */
/* 456 */	NdrFcLong( 0x2 ),	/* 2 */
/* 460 */	NdrFcShort( 0x160 ),	/* Offset= 352 (812) */
/* 462 */	NdrFcLong( 0x3 ),	/* 3 */
/* 466 */	NdrFcShort( 0x178 ),	/* Offset= 376 (842) */
/* 468 */	NdrFcLong( 0x14 ),	/* 20 */
/* 472 */	NdrFcShort( 0x190 ),	/* Offset= 400 (872) */
/* 474 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (473) */
/* 476 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 478 */	NdrFcShort( 0x4 ),	/* 4 */
/* 480 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 482 */	NdrFcShort( 0x0 ),	/* 0 */
/* 484 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 486 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 488 */	NdrFcShort( 0x4 ),	/* 4 */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x1 ),	/* 1 */
/* 494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 498 */	0x12, 0x0,	/* FC_UP */
/* 500 */	NdrFcShort( 0xfffffe20 ),	/* Offset= -480 (20) */
/* 502 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 504 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 506 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 508 */	NdrFcShort( 0x8 ),	/* 8 */
/* 510 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 512 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 514 */	NdrFcShort( 0x4 ),	/* 4 */
/* 516 */	NdrFcShort( 0x4 ),	/* 4 */
/* 518 */	0x11, 0x0,	/* FC_RP */
/* 520 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (476) */
/* 522 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 524 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 526 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 528 */	NdrFcShort( 0x0 ),	/* 0 */
/* 530 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 534 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 538 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 540 */	NdrFcShort( 0xffffff50 ),	/* Offset= -176 (364) */
/* 542 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 544 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 546 */	NdrFcShort( 0x8 ),	/* 8 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	NdrFcShort( 0x6 ),	/* Offset= 6 (556) */
/* 552 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 554 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 556 */	
			0x11, 0x0,	/* FC_RP */
/* 558 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (526) */
/* 560 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 562 */	NdrFcShort( 0x0 ),	/* 0 */
/* 564 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 566 */	NdrFcShort( 0x0 ),	/* 0 */
/* 568 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 572 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 574 */	NdrFcShort( 0xffffff40 ),	/* Offset= -192 (382) */
/* 576 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 578 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 580 */	NdrFcShort( 0x8 ),	/* 8 */
/* 582 */	NdrFcShort( 0x0 ),	/* 0 */
/* 584 */	NdrFcShort( 0x6 ),	/* Offset= 6 (590) */
/* 586 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 588 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 590 */	
			0x11, 0x0,	/* FC_RP */
/* 592 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (560) */
/* 594 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 596 */	NdrFcShort( 0x4 ),	/* 4 */
/* 598 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 602 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 604 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 606 */	NdrFcShort( 0x4 ),	/* 4 */
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x1 ),	/* 1 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 616 */	0x12, 0x0,	/* FC_UP */
/* 618 */	NdrFcShort( 0xfffffd96 ),	/* Offset= -618 (0) */
/* 620 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 622 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 624 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 626 */	NdrFcShort( 0x8 ),	/* 8 */
/* 628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 630 */	NdrFcShort( 0x6 ),	/* Offset= 6 (636) */
/* 632 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 634 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 636 */	
			0x11, 0x0,	/* FC_RP */
/* 638 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (594) */
/* 640 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 642 */	NdrFcLong( 0x2f ),	/* 47 */
/* 646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 652 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 654 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 656 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 658 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 660 */	NdrFcShort( 0x1 ),	/* 1 */
/* 662 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 664 */	NdrFcShort( 0x4 ),	/* 4 */
/* 666 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 668 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 670 */	NdrFcShort( 0x10 ),	/* 16 */
/* 672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 674 */	NdrFcShort( 0xa ),	/* Offset= 10 (684) */
/* 676 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 678 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 680 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (640) */
/* 682 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 684 */	
			0x12, 0x0,	/* FC_UP */
/* 686 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (658) */
/* 688 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 690 */	NdrFcShort( 0x4 ),	/* 4 */
/* 692 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 696 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 698 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 700 */	NdrFcShort( 0x4 ),	/* 4 */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x1 ),	/* 1 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */
/* 708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 710 */	0x12, 0x0,	/* FC_UP */
/* 712 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (668) */
/* 714 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 716 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 718 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 720 */	NdrFcShort( 0x8 ),	/* 8 */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	NdrFcShort( 0x6 ),	/* Offset= 6 (730) */
/* 726 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 728 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 730 */	
			0x11, 0x0,	/* FC_RP */
/* 732 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (688) */
/* 734 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 736 */	NdrFcShort( 0x8 ),	/* 8 */
/* 738 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 740 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 742 */	NdrFcShort( 0x10 ),	/* 16 */
/* 744 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 746 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 748 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (734) */
			0x5b,		/* FC_END */
/* 752 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 754 */	NdrFcShort( 0x18 ),	/* 24 */
/* 756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 758 */	NdrFcShort( 0xa ),	/* Offset= 10 (768) */
/* 760 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 762 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 764 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (740) */
/* 766 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 768 */	
			0x11, 0x0,	/* FC_RP */
/* 770 */	NdrFcShort( 0xffffff0c ),	/* Offset= -244 (526) */
/* 772 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 774 */	NdrFcShort( 0x1 ),	/* 1 */
/* 776 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 782 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 784 */	NdrFcShort( 0x8 ),	/* 8 */
/* 786 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 788 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 790 */	NdrFcShort( 0x4 ),	/* 4 */
/* 792 */	NdrFcShort( 0x4 ),	/* 4 */
/* 794 */	0x12, 0x0,	/* FC_UP */
/* 796 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (772) */
/* 798 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 800 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 802 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 804 */	NdrFcShort( 0x2 ),	/* 2 */
/* 806 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 810 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 812 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 814 */	NdrFcShort( 0x8 ),	/* 8 */
/* 816 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 818 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 820 */	NdrFcShort( 0x4 ),	/* 4 */
/* 822 */	NdrFcShort( 0x4 ),	/* 4 */
/* 824 */	0x12, 0x0,	/* FC_UP */
/* 826 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (802) */
/* 828 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 830 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 832 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 834 */	NdrFcShort( 0x4 ),	/* 4 */
/* 836 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 840 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 842 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 844 */	NdrFcShort( 0x8 ),	/* 8 */
/* 846 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 848 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 850 */	NdrFcShort( 0x4 ),	/* 4 */
/* 852 */	NdrFcShort( 0x4 ),	/* 4 */
/* 854 */	0x12, 0x0,	/* FC_UP */
/* 856 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (832) */
/* 858 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 860 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 862 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 864 */	NdrFcShort( 0x8 ),	/* 8 */
/* 866 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 868 */	NdrFcShort( 0x0 ),	/* 0 */
/* 870 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 872 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 874 */	NdrFcShort( 0x8 ),	/* 8 */
/* 876 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 878 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 880 */	NdrFcShort( 0x4 ),	/* 4 */
/* 882 */	NdrFcShort( 0x4 ),	/* 4 */
/* 884 */	0x12, 0x0,	/* FC_UP */
/* 886 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (862) */
/* 888 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 890 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 892 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 894 */	NdrFcShort( 0x8 ),	/* 8 */
/* 896 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 898 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 900 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 902 */	NdrFcShort( 0x8 ),	/* 8 */
/* 904 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 906 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 908 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 910 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (892) */
/* 912 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 914 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 916 */	NdrFcShort( 0x28 ),	/* 40 */
/* 918 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (900) */
/* 920 */	NdrFcShort( 0x0 ),	/* Offset= 0 (920) */
/* 922 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 924 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 926 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 928 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffdf7 ),	/* Offset= -521 (408) */
			0x5b,		/* FC_END */
/* 932 */	
			0x12, 0x0,	/* FC_UP */
/* 934 */	NdrFcShort( 0xfffffef6 ),	/* Offset= -266 (668) */
/* 936 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 938 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 940 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 942 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 944 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 946 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 948 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 950 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 952 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 954 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 956 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 958 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 960 */	
			0x12, 0x0,	/* FC_UP */
/* 962 */	NdrFcShort( 0xfffffda4 ),	/* Offset= -604 (358) */
/* 964 */	
			0x12, 0x10,	/* FC_UP */
/* 966 */	NdrFcShort( 0xfffffc62 ),	/* Offset= -926 (40) */
/* 968 */	
			0x12, 0x10,	/* FC_UP */
/* 970 */	NdrFcShort( 0xfffffda2 ),	/* Offset= -606 (364) */
/* 972 */	
			0x12, 0x10,	/* FC_UP */
/* 974 */	NdrFcShort( 0xfffffdb0 ),	/* Offset= -592 (382) */
/* 976 */	
			0x12, 0x10,	/* FC_UP */
/* 978 */	NdrFcShort( 0xfffffdbe ),	/* Offset= -578 (400) */
/* 980 */	
			0x12, 0x10,	/* FC_UP */
/* 982 */	NdrFcShort( 0x2 ),	/* Offset= 2 (984) */
/* 984 */	
			0x12, 0x0,	/* FC_UP */
/* 986 */	NdrFcShort( 0xfffffc26 ),	/* Offset= -986 (0) */
/* 988 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 990 */	NdrFcShort( 0x10 ),	/* 16 */
/* 992 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 994 */	0x1,		/* FC_BYTE */
			0x38,		/* FC_ALIGNM4 */
/* 996 */	0x8,		/* FC_LONG */
			0x39,		/* FC_ALIGNM8 */
/* 998 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1000 */	
			0x12, 0x0,	/* FC_UP */
/* 1002 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (988) */
/* 1004 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1006 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1008 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1010 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1012 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1014 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1014) */
/* 1016 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1018 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1020 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1022 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1024 */	NdrFcShort( 0xfffffc3e ),	/* Offset= -962 (62) */
/* 1026 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1028 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1030 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1032 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1034 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1036 */	NdrFcShort( 0xfffffc2e ),	/* Offset= -978 (58) */
/* 1038 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1040 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

const CInterfaceProxyVtbl * _SDK_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IWippienSDKProxyVtbl,
    0
};

const CInterfaceStubVtbl * _SDK_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IWippienSDKStubVtbl,
    0
};

PCInterfaceName const _SDK_InterfaceNamesList[] = 
{
    "IWippienSDK",
    0
};

const IID *  _SDK_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _SDK_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _SDK, pIID, n)

int __stdcall _SDK_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_SDK_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo SDK_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _SDK_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _SDK_StubVtblList,
    (const PCInterfaceName * ) & _SDK_InterfaceNamesList,
    (const IID ** ) & _SDK_BaseIIDList,
    & _SDK_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
