#include "StdAfx.h"
#include "Updater.h"
#include "JabberLib.h"

extern CJabberLib *_Jabber;

	void AppUpd_CloseApp(void *wodAppUpd)
	{
		if (::IsWindow(hMainWnd))
		{
			::PostMessage(hMainWnd, WM_COMMAND, IDCANCEL, 0);
		}
	}
	void AppUpd_CheckDone(void *wodAppUpd, long NewFiles, long ErrorCode, char *ErrorText)
	{
		VARIANT tag;
		WODAPPUPDCOMLib::AppUpd_GetTag(wodAppUpd, &tag);
		CUpdateHandler *m_Owner = (CUpdateHandler *)tag.plVal;
		if (!ErrorCode)
		{	
			if (NewFiles)
			{
				int answer = IDNO;
				WODAPPUPDCOMLib::AppUpd_SetVisible(wodAppUpd, m_Owner->m_Silently?VARIANT_FALSE:VARIANT_TRUE);
				if (m_Owner->m_Silently)
					answer = IDYES;
				else
				{
					if (m_Owner->m_UpdateHandlerMsgBoxShown)
						return;

					m_Owner->m_UpdateHandlerMsgBoxShown = TRUE;
					answer = MessageBox(NULL, "New version of Wippien is available. Download?\r\n\r\nClick on Help to see what's new.", "Wippien update", MB_YESNO | MB_HELP | MB_ICONQUESTION);
					m_Owner->m_UpdateHandlerMsgBoxShown = FALSE;
				}

				if (answer == IDYES)
				{
					WODAPPUPDCOMLib::AppUpd_SetVisible(wodAppUpd, /*m_Owner->m_Silently?VARIANT_FALSE:VARIANT_TRUE*/VARIANT_TRUE);
					WODAPPUPDCOMLib::AppUpd_Download(wodAppUpd);
				}

			}
			else
			{
				if (!m_Owner->m_Silently)
				{
					if (m_Owner->m_UpdateHandlerMsgBoxShown)
						return;
					m_Owner->m_UpdateHandlerMsgBoxShown = TRUE;
					MessageBox(NULL, "Your version of Wippien is up-to-date.", "Wippien update", MB_OK);
					m_Owner->m_UpdateHandlerMsgBoxShown = FALSE;
				}
			}
		}
		else
			if (!m_Owner->m_Silently)
			{
				if (m_Owner->m_UpdateHandlerMsgBoxShown)
					return;
				m_Owner->m_UpdateHandlerMsgBoxShown = TRUE;
				MessageBox(NULL, "Error connecting to remote server.", "Wippien update", MB_ICONERROR);
				m_Owner->m_UpdateHandlerMsgBoxShown = FALSE;
			}
	}

	void AppUpd_DownloadDone(void *wodAppUpd, long ErrorCode, char *ErrorText)
	{
		VARIANT tag;
		WODAPPUPDCOMLib::AppUpd_GetTag(wodAppUpd, &tag);
		CUpdateHandler *m_Owner = (CUpdateHandler *)tag.plVal;
		if (!ErrorCode)
		{
			if (m_Owner->m_UpdateHandlerMsgBoxShown)
				return;
			m_Owner->m_UpdateHandlerMsgBoxShown = TRUE;
			int i = 0;
			if (m_Owner->m_Silently)
				i = 6;
			else
				i = ::MessageBox(NULL, "Download successful. Replace now?", "Wippien update", MB_ICONQUESTION | MB_YESNO);
			m_Owner->m_UpdateHandlerMsgBoxShown = FALSE;
			if (i == 6)
			{
				m_Owner->m_Silently = TRUE;
				WODAPPUPDCOMLib::AppUpd_Update(wodAppUpd);
			}
		}
		else
		{
			if (!m_Owner->m_Silently)
			{
				MessageBox(NULL, ErrorText, "Wippien update error!", MB_ICONERROR);
			}

		}
	}

	void AppUpd_PrevDetected(void *wodAppUpd)		
	{
		VARIANT tag;
		WODAPPUPDCOMLib::AppUpd_GetTag(wodAppUpd, &tag);
		CUpdateHandler *m_Owner = (CUpdateHandler *)tag.plVal;
		if (m_Owner->m_UpdateHandlerMsgBoxShown)
			return;

		m_Owner->m_UpdateHandlerMsgBoxShown = TRUE;
		int i = 6;
		if (!m_Owner->m_Silently)
			i = ::MessageBox(NULL, "New version found localy. Replace?", "New version", MB_ICONQUESTION | MB_YESNO);
		m_Owner->m_UpdateHandlerMsgBoxShown = FALSE;
		if (i==6)
		{
			WODAPPUPDCOMLib::AppUpd_Update(wodAppUpd);
			if (::IsWindow(hMainWnd))
			{
				::PostMessage(hMainWnd, WM_COMMAND, IDCANCEL, 0);
			}
		}
		else
			WODAPPUPDCOMLib::AppUpd_Reset(wodAppUpd);
	}    

void CUpdateHandler::InitUpdater(void)
{
	if (!m_Update)
	{
		memset (&m_AppUpdEvents, 0, sizeof(m_AppUpdEvents));
		m_AppUpdEvents.CheckDone = AppUpd_CheckDone;
		m_AppUpdEvents.CloseApp = AppUpd_CloseApp;
		m_AppUpdEvents.DownloadDone = AppUpd_DownloadDone;
		m_AppUpdEvents.PrevDetected = AppUpd_PrevDetected;
		
		m_Update = WODAPPUPDCOMLib::_AppUpd_Create(&m_AppUpdEvents);
		VARIANT var;
		var.vt = VT_I4;
		var.lVal = (LONG)this;
		
		const char *dialognoteurl = "http://www.weonlydo.com/index.asp?showform=AppUpdate&from=MiniVPN";
		const char *updtername="MiniVPNUpdater";
		
		WODAPPUPDCOMLib::AppUpd_SetTag(m_Update, var);
		WODAPPUPDCOMLib::AppUpd_SetUpdaterName(m_Update, (char *)updtername);
		WODAPPUPDCOMLib::AppUpd_SetDialogNoteURL(m_Update, (char *)dialognoteurl);
		WODAPPUPDCOMLib::AppUpd_SetReplaceRule(m_Update, (WODAPPUPDCOMLib::ReplaceRulesEnum)/*ReplaceIfVersion*/1);
		WODAPPUPDCOMLib::AppUpd_SetReplaceRule(m_Update, (WODAPPUPDCOMLib::ReplaceRulesEnum)/*ReplaceIfVersion*/1);
		WODAPPUPDCOMLib::AppUpd_SetRequireAdmin(m_Update, (BOOL)TRUE);
	}
}

void CUpdateHandler::DownloadUpdates(BOOL silently)
{
	if (m_UpdateHandlerMsgBoxShown)
		return;
	
	InitUpdater();
	
	// let's see if we have newer versions
	char buff[1024];
	sprintf(buff, "http://wippien.com/Download/miniupdate.php?JID=%s", _Jabber->m_JID);

	
	WODAPPUPDCOMLib::AppUpd_SetVisible(m_Update, silently?VARIANT_FALSE:VARIANT_TRUE);
	m_Silently = silently;
	WODAPPUPDCOMLib::AppUpd_SetURL(m_Update, buff);
	WODAPPUPDCOMLib::AppUpd_SetReplaceRule(m_Update, (WODAPPUPDCOMLib::ReplaceRulesEnum)1);
	WODAPPUPDCOMLib::AppUpd_Check(m_Update);
}
