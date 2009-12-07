// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3309BE60_19E1_4742_86E1_B7DE97B25517__INCLUDED_)
#define AFX_STDAFX_H__3309BE60_19E1_4742_86E1_B7DE97B25517__INCLUDED_

#define _HAS_ITERATOR_DEBUGGING 0
#define _SECURE_SCL 0

// Change these values to use different versions
#define WINVER		0x0500

#define _WIN32_IE	0x0500
#define _WIN32_WINNT 0x0501
#define _RICHEDIT_VER	0x0100

#define _WTL_NEW_PAGE_NOTIFY_HANDLERS


#define LASTOPERATORMSGID	0

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlmisc.h>

#ifndef _WODXMPPLIB
//#define WODXMPP_LICENSE_KEY "put_licensekey_here"
#import "C:\WINDOWS\SYSTEM32\wodXMPP.dll" exclude("LanguageEnum")
#endif

#ifndef _APPUPDLIB
//#define WODAPPUPDATE_LICENSE_KEY "put_licensekey_here"
#import "C:\WINDOWS\SYSTEM32\wodAppUp.dll" no_namespace named_guids exclude("LanguageEnum")
#endif

#ifndef _WODVPNLIB
//#define WODVPN_LICENSE_KEY "put_licensekey_here"
#import "C:\WINDOWS\SYSTEM32\wodVPN.dll" named_guids exclude("LanguageEnum")
//#include "Debug\wodvpn.tlh"
#endif

//#define _SKINMAGICKEY "put_licensekey_here"


void DumpDebug(char *text,...);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3309BE60_19E1_4742_86E1_B7DE97B25517__INCLUDED_)
