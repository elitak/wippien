/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Mar 08 23:21:40 2009
 */
/* Compiler settings for D:\Wippien\Public\SDK\SDK.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __SDK_h__
#define __SDK_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IWippienSDK_FWD_DEFINED__
#define __IWippienSDK_FWD_DEFINED__
typedef interface IWippienSDK IWippienSDK;
#endif 	/* __IWippienSDK_FWD_DEFINED__ */


#ifndef ___IWippienSDKEvents_FWD_DEFINED__
#define ___IWippienSDKEvents_FWD_DEFINED__
typedef interface _IWippienSDKEvents _IWippienSDKEvents;
#endif 	/* ___IWippienSDKEvents_FWD_DEFINED__ */


#ifndef __WippienSDK_FWD_DEFINED__
#define __WippienSDK_FWD_DEFINED__

#ifdef __cplusplus
typedef class WippienSDK WippienSDK;
#else
typedef struct WippienSDK WippienSDK;
#endif /* __cplusplus */

#endif 	/* __WippienSDK_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IWippienSDK_INTERFACE_DEFINED__
#define __IWippienSDK_INTERFACE_DEFINED__

/* interface IWippienSDK */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWippienSDK;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CAD56E0F-4F78-4D9B-B425-92B8A5361D6D")
    IWippienSDK : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_JID( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_JID( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Status( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveSettings( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Die( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Hostname( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Hostname( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Port( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Port( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserCount( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserJID( 
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UserJID( 
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserResource( 
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UserResource( 
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserEmail( 
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UserEmail( 
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserVisibleName( 
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UserVisibleName( 
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserGroup( 
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UserGroup( 
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserBlock( 
            /* [in] */ VARIANT User,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UserBlock( 
            /* [in] */ VARIANT User,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserPresenceStatus( 
            /* [in] */ VARIANT User,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserVPNStatus( 
            /* [in] */ VARIANT User,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenChatWindow( 
            /* [in] */ VARIANT User) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseChatWindow( 
            /* [in] */ VARIANT User) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendMessage( 
            /* [in] */ VARIANT User,
            BSTR PlainText,
            BSTR HtmlText) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MyIP( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MyNetmask( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserIP( 
            VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UserConnect( 
            VARIANT User) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserConnectedIP( 
            VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserConnectedPort( 
            VARIANT User,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangePassword( 
            BSTR OldPassword,
            BSTR NewPassword,
            VARIANT_BOOL ProtectAll) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UserConnectIP( 
            VARIANT User,
            int IP) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddContact( 
            BSTR JID,
            VARIANT_BOOL Subscribe) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveContact( 
            BSTR JID,
            VARIANT_BOOL Unsubscribe) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Sound( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Sound( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateAccount( 
            BSTR JID,
            BSTR Password,
            BSTR Hostname,
            int Port) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_StatusText( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_StatusText( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWippienSDKVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWippienSDK __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWippienSDK __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IWippienSDK __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_JID )( 
            IWippienSDK __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_JID )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Visible )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Status )( 
            IWippienSDK __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Status )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveSettings )( 
            IWippienSDK __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Die )( 
            IWippienSDK __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Hostname )( 
            IWippienSDK __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Hostname )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Port )( 
            IWippienSDK __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Port )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserCount )( 
            IWippienSDK __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserJID )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UserJID )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserResource )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UserResource )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserEmail )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UserEmail )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserVisibleName )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UserVisibleName )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserGroup )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UserGroup )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserBlock )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UserBlock )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserPresenceStatus )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserVPNStatus )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenChatWindow )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseChatWindow )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendMessage )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT User,
            BSTR PlainText,
            BSTR HtmlText);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MyIP )( 
            IWippienSDK __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MyNetmask )( 
            IWippienSDK __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserIP )( 
            IWippienSDK __RPC_FAR * This,
            VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UserConnect )( 
            IWippienSDK __RPC_FAR * This,
            VARIANT User);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserConnectedIP )( 
            IWippienSDK __RPC_FAR * This,
            VARIANT User,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserConnectedPort )( 
            IWippienSDK __RPC_FAR * This,
            VARIANT User,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChangePassword )( 
            IWippienSDK __RPC_FAR * This,
            BSTR OldPassword,
            BSTR NewPassword,
            VARIANT_BOOL ProtectAll);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UserConnectIP )( 
            IWippienSDK __RPC_FAR * This,
            VARIANT User,
            int IP);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddContact )( 
            IWippienSDK __RPC_FAR * This,
            BSTR JID,
            VARIANT_BOOL Subscribe);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveContact )( 
            IWippienSDK __RPC_FAR * This,
            BSTR JID,
            VARIANT_BOOL Unsubscribe);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Sound )( 
            IWippienSDK __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Sound )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateAccount )( 
            IWippienSDK __RPC_FAR * This,
            BSTR JID,
            BSTR Password,
            BSTR Hostname,
            int Port);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IWippienSDK __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IWippienSDK __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StatusText )( 
            IWippienSDK __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StatusText )( 
            IWippienSDK __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IWippienSDKVtbl;

    interface IWippienSDK
    {
        CONST_VTBL struct IWippienSDKVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWippienSDK_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWippienSDK_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWippienSDK_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWippienSDK_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWippienSDK_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWippienSDK_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWippienSDK_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWippienSDK_get_JID(This,pVal)	\
    (This)->lpVtbl -> get_JID(This,pVal)

#define IWippienSDK_put_JID(This,newVal)	\
    (This)->lpVtbl -> put_JID(This,newVal)

#define IWippienSDK_put_Visible(This,newVal)	\
    (This)->lpVtbl -> put_Visible(This,newVal)

#define IWippienSDK_get_Status(This,pVal)	\
    (This)->lpVtbl -> get_Status(This,pVal)

#define IWippienSDK_put_Status(This,newVal)	\
    (This)->lpVtbl -> put_Status(This,newVal)

#define IWippienSDK_SaveSettings(This)	\
    (This)->lpVtbl -> SaveSettings(This)

#define IWippienSDK_Die(This)	\
    (This)->lpVtbl -> Die(This)

#define IWippienSDK_get_Hostname(This,pVal)	\
    (This)->lpVtbl -> get_Hostname(This,pVal)

#define IWippienSDK_put_Hostname(This,newVal)	\
    (This)->lpVtbl -> put_Hostname(This,newVal)

#define IWippienSDK_get_Port(This,pVal)	\
    (This)->lpVtbl -> get_Port(This,pVal)

#define IWippienSDK_put_Port(This,newVal)	\
    (This)->lpVtbl -> put_Port(This,newVal)

#define IWippienSDK_get_UserCount(This,pVal)	\
    (This)->lpVtbl -> get_UserCount(This,pVal)

#define IWippienSDK_get_UserJID(This,User,pVal)	\
    (This)->lpVtbl -> get_UserJID(This,User,pVal)

#define IWippienSDK_put_UserJID(This,User,newVal)	\
    (This)->lpVtbl -> put_UserJID(This,User,newVal)

#define IWippienSDK_get_UserResource(This,User,pVal)	\
    (This)->lpVtbl -> get_UserResource(This,User,pVal)

#define IWippienSDK_put_UserResource(This,User,newVal)	\
    (This)->lpVtbl -> put_UserResource(This,User,newVal)

#define IWippienSDK_get_UserEmail(This,User,pVal)	\
    (This)->lpVtbl -> get_UserEmail(This,User,pVal)

#define IWippienSDK_put_UserEmail(This,User,newVal)	\
    (This)->lpVtbl -> put_UserEmail(This,User,newVal)

#define IWippienSDK_get_UserVisibleName(This,User,pVal)	\
    (This)->lpVtbl -> get_UserVisibleName(This,User,pVal)

#define IWippienSDK_put_UserVisibleName(This,User,newVal)	\
    (This)->lpVtbl -> put_UserVisibleName(This,User,newVal)

#define IWippienSDK_get_UserGroup(This,User,pVal)	\
    (This)->lpVtbl -> get_UserGroup(This,User,pVal)

#define IWippienSDK_put_UserGroup(This,User,newVal)	\
    (This)->lpVtbl -> put_UserGroup(This,User,newVal)

#define IWippienSDK_get_UserBlock(This,User,pVal)	\
    (This)->lpVtbl -> get_UserBlock(This,User,pVal)

#define IWippienSDK_put_UserBlock(This,User,newVal)	\
    (This)->lpVtbl -> put_UserBlock(This,User,newVal)

#define IWippienSDK_get_UserPresenceStatus(This,User,pVal)	\
    (This)->lpVtbl -> get_UserPresenceStatus(This,User,pVal)

#define IWippienSDK_get_UserVPNStatus(This,User,pVal)	\
    (This)->lpVtbl -> get_UserVPNStatus(This,User,pVal)

#define IWippienSDK_OpenChatWindow(This,User)	\
    (This)->lpVtbl -> OpenChatWindow(This,User)

#define IWippienSDK_CloseChatWindow(This,User)	\
    (This)->lpVtbl -> CloseChatWindow(This,User)

#define IWippienSDK_SendMessage(This,User,PlainText,HtmlText)	\
    (This)->lpVtbl -> SendMessage(This,User,PlainText,HtmlText)

#define IWippienSDK_get_MyIP(This,pVal)	\
    (This)->lpVtbl -> get_MyIP(This,pVal)

#define IWippienSDK_get_MyNetmask(This,pVal)	\
    (This)->lpVtbl -> get_MyNetmask(This,pVal)

#define IWippienSDK_get_UserIP(This,User,pVal)	\
    (This)->lpVtbl -> get_UserIP(This,User,pVal)

#define IWippienSDK_UserConnect(This,User)	\
    (This)->lpVtbl -> UserConnect(This,User)

#define IWippienSDK_get_UserConnectedIP(This,User,pVal)	\
    (This)->lpVtbl -> get_UserConnectedIP(This,User,pVal)

#define IWippienSDK_get_UserConnectedPort(This,User,pVal)	\
    (This)->lpVtbl -> get_UserConnectedPort(This,User,pVal)

#define IWippienSDK_ChangePassword(This,OldPassword,NewPassword,ProtectAll)	\
    (This)->lpVtbl -> ChangePassword(This,OldPassword,NewPassword,ProtectAll)

#define IWippienSDK_UserConnectIP(This,User,IP)	\
    (This)->lpVtbl -> UserConnectIP(This,User,IP)

#define IWippienSDK_AddContact(This,JID,Subscribe)	\
    (This)->lpVtbl -> AddContact(This,JID,Subscribe)

#define IWippienSDK_RemoveContact(This,JID,Unsubscribe)	\
    (This)->lpVtbl -> RemoveContact(This,JID,Unsubscribe)

#define IWippienSDK_get_Sound(This,pVal)	\
    (This)->lpVtbl -> get_Sound(This,pVal)

#define IWippienSDK_put_Sound(This,newVal)	\
    (This)->lpVtbl -> put_Sound(This,newVal)

#define IWippienSDK_CreateAccount(This,JID,Password,Hostname,Port)	\
    (This)->lpVtbl -> CreateAccount(This,JID,Password,Hostname,Port)

#define IWippienSDK_Connect(This)	\
    (This)->lpVtbl -> Connect(This)

#define IWippienSDK_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define IWippienSDK_get_StatusText(This,pVal)	\
    (This)->lpVtbl -> get_StatusText(This,pVal)

#define IWippienSDK_put_StatusText(This,newVal)	\
    (This)->lpVtbl -> put_StatusText(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_JID_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_JID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_JID_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IWippienSDK_put_JID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_Visible_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IWippienSDK_put_Visible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_Status_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_Status_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_Status_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IWippienSDK_put_Status_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_SaveSettings_Proxy( 
    IWippienSDK __RPC_FAR * This);


void __RPC_STUB IWippienSDK_SaveSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_Die_Proxy( 
    IWippienSDK __RPC_FAR * This);


void __RPC_STUB IWippienSDK_Die_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_Hostname_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_Hostname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_Hostname_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IWippienSDK_put_Hostname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_Port_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_Port_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_Port_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IWippienSDK_put_Port_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserCount_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserJID_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserJID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_UserJID_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [in] */ BSTR newVal);


void __RPC_STUB IWippienSDK_put_UserJID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserResource_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_UserResource_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [in] */ BSTR newVal);


void __RPC_STUB IWippienSDK_put_UserResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserEmail_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserEmail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_UserEmail_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [in] */ BSTR newVal);


void __RPC_STUB IWippienSDK_put_UserEmail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserVisibleName_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserVisibleName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_UserVisibleName_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [in] */ BSTR newVal);


void __RPC_STUB IWippienSDK_put_UserVisibleName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserGroup_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_UserGroup_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [in] */ BSTR newVal);


void __RPC_STUB IWippienSDK_put_UserGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserBlock_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserBlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_UserBlock_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IWippienSDK_put_UserBlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserPresenceStatus_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserPresenceStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserVPNStatus_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserVPNStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_OpenChatWindow_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User);


void __RPC_STUB IWippienSDK_OpenChatWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_CloseChatWindow_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User);


void __RPC_STUB IWippienSDK_CloseChatWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_SendMessage_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT User,
    BSTR PlainText,
    BSTR HtmlText);


void __RPC_STUB IWippienSDK_SendMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_MyIP_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_MyIP_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_MyNetmask_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_MyNetmask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserIP_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserIP_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_UserConnect_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User);


void __RPC_STUB IWippienSDK_UserConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserConnectedIP_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserConnectedIP_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_UserConnectedPort_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_UserConnectedPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_ChangePassword_Proxy( 
    IWippienSDK __RPC_FAR * This,
    BSTR OldPassword,
    BSTR NewPassword,
    VARIANT_BOOL ProtectAll);


void __RPC_STUB IWippienSDK_ChangePassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_UserConnectIP_Proxy( 
    IWippienSDK __RPC_FAR * This,
    VARIANT User,
    int IP);


void __RPC_STUB IWippienSDK_UserConnectIP_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_AddContact_Proxy( 
    IWippienSDK __RPC_FAR * This,
    BSTR JID,
    VARIANT_BOOL Subscribe);


void __RPC_STUB IWippienSDK_AddContact_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_RemoveContact_Proxy( 
    IWippienSDK __RPC_FAR * This,
    BSTR JID,
    VARIANT_BOOL Unsubscribe);


void __RPC_STUB IWippienSDK_RemoveContact_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_Sound_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_Sound_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_Sound_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IWippienSDK_put_Sound_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_CreateAccount_Proxy( 
    IWippienSDK __RPC_FAR * This,
    BSTR JID,
    BSTR Password,
    BSTR Hostname,
    int Port);


void __RPC_STUB IWippienSDK_CreateAccount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_Connect_Proxy( 
    IWippienSDK __RPC_FAR * This);


void __RPC_STUB IWippienSDK_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWippienSDK_Disconnect_Proxy( 
    IWippienSDK __RPC_FAR * This);


void __RPC_STUB IWippienSDK_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWippienSDK_get_StatusText_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWippienSDK_get_StatusText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWippienSDK_put_StatusText_Proxy( 
    IWippienSDK __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IWippienSDK_put_StatusText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWippienSDK_INTERFACE_DEFINED__ */



#ifndef __WIPPIENSDKCOMLib_LIBRARY_DEFINED__
#define __WIPPIENSDKCOMLib_LIBRARY_DEFINED__

/* library WIPPIENSDKCOMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_WIPPIENSDKCOMLib;

#ifndef ___IWippienSDKEvents_DISPINTERFACE_DEFINED__
#define ___IWippienSDKEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IWippienSDKEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IWippienSDKEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BC5DC3B4-24B8-441A-B24C-211CF280D31A")
    _IWippienSDKEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IWippienSDKEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IWippienSDKEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IWippienSDKEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IWippienSDKEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IWippienSDKEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IWippienSDKEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IWippienSDKEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IWippienSDKEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IWippienSDKEventsVtbl;

    interface _IWippienSDKEvents
    {
        CONST_VTBL struct _IWippienSDKEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IWippienSDKEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IWippienSDKEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IWippienSDKEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IWippienSDKEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IWippienSDKEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IWippienSDKEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IWippienSDKEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IWippienSDKEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_WippienSDK;

#ifdef __cplusplus

class DECLSPEC_UUID("34AE1617-F394-4631-8F89-C291691D824F")
WippienSDK;
#endif
#endif /* __WIPPIENSDKCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
