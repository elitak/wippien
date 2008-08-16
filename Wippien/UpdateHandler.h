// UpdateHandler.h: interface for the CUpdateHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATEHANDLER_H__761B97A4_227D_4B98_80C7_B30F229697CE__INCLUDED_)
#define AFX_UPDATEHANDLER_H__761B97A4_227D_4B98_80C7_B30F229697CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "Buffer.h"

#ifdef _APPUPDLIB
namespace WODAPPUPDCOMLib
{
#include "\WeOnlyDo\wodAppUpdate\Code\Win32LIB\Win32LIB.h"
}
#endif


class CProgressDlg;
class wodAppUpdateEvents;

class CUpdateHandler  
{
public:
	CUpdateHandler();
	virtual ~CUpdateHandler();

	void InitUpdater(void);
	void DownloadUpdates(BOOL silently);
	BOOL m_Silently, m_SilentCheck;
	CProgressDlg *m_Dlg;
	unsigned int m_NewLanguageFiles;

#ifndef _APPUPDLIB
	IwodAppUpdateCom *m_Update;
	wodAppUpdateEvents *m_UpdateEvents;
#else
	WODAPPUPDCOMLib::AppUpdEventsStruct m_AppUpdEvents;
	void *m_Update;
#endif

};
extern CUpdateHandler *_UpdateHandler;

#endif // !defined(AFX_UPDATEHANDLER_H__761B97A4_227D_4B98_80C7_B30F229697CE__INCLUDED_)
