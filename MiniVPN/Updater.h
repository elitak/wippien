#ifndef _UPDATER_H
#define _UPDATER_H

namespace WODAPPUPDCOMLib
{
#include "\WeOnlyDo\wodAppUpdate\Code\Win32LIB\Win32LIB.h"
}


class CUpdateHandler  
{
public:
	CUpdateHandler()
	{
		m_UpdateHandlerMsgBoxShown = FALSE;
		m_Silently = TRUE;
	}
	virtual ~CUpdateHandler()
	{
	}	
	
	void InitUpdater(void);
	void DownloadUpdates(BOOL silently);
	BOOL m_Silently;
	
	WODAPPUPDCOMLib::AppUpdEventsStruct m_AppUpdEvents;
	void *m_Update;
	BOOL m_UpdateHandlerMsgBoxShown;
};

#endif
