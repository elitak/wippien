// UpdateHandler.cpp: implementation of the CUpdateHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpdateHandler.h"
#include "../CxImage/zlib/zlib.h"


int uuencode(unsigned char *src, unsigned int srclength,char *target, size_t targsize);
BOOL m_UpdateHandlerMsgBoxShown = FALSE;
CUpdateHandler *_UpdateHandler;

DWORD WINAPI ShowMessageThreadProc(void *d)
{
	Buffer *data = (Buffer *)d;

	char *text = data->Ptr();
	char *caption = text;
	caption+= strlen(text);
	caption++;

	m_UpdateHandlerMsgBoxShown = TRUE;
	CBalloonTipDlg::Show(NULL, text, caption, MB_OK);
	m_UpdateHandlerMsgBoxShown = FALSE;
	delete data;
	return 0;
}

	void AppUpd_CloseApp(void *wodAppUpd)
	{
		if (::IsWindow(_MainDlg.m_hWnd))
		{
			::PostMessage(_MainDlg.m_hWnd, WM_COMMAND, ID_EXIT, 0);
		}
	}
	void AppUpd_CheckDone(void *wodAppUpd, long NewFiles, long ErrorCode, char *ErrorText)
	{
		VARIANT tag;
		WODAPPUPDCOMLib::AppUpd_GetTag(wodAppUpd, &tag);
		CUpdateHandler *m_Owner = (CUpdateHandler *)tag.plVal;
		if (!ErrorCode)
		{	
			short count = 0;
#ifndef _APPUPDLIB
			IUpdMessages *msgs = NULL;
			if (SUCCEEDED(m_Owner->m_Update->get_Messages(&msgs)))
			{
				if (SUCCEEDED(msgs->get_Count(&count)))
				{
					IUpdMessage *msg = NULL, *showmsg = NULL;
#else
					void *msg = NULL, *showmsg = NULL;
					WODAPPUPDCOMLib::AppUpd_Messages_GetCount(wodAppUpd, &count);
#endif
					short min = 32000;
					for (int i=0;i<count;i++)
					{
#ifndef _APPUPDLIB
						if (SUCCEEDED(msgs->get_Item(i, &msg)))
#else
						if (!WODAPPUPDCOMLib::AppUpd_Messages_GetMessage(wodAppUpd, i, &msg) && msg)

#endif
						{

							long ID = 0;
#ifndef _APPUPDLIB
							msg->get_ID(&ID);
#else
							WODAPPUPDCOMLib::AppUpd_Message_GetID(msg, &ID);
#endif

							if (ID < min && ID>_Settings.m_LastOperatorMessageID)
							{
								min = ID;
								if (showmsg)
#ifndef _APPUPDLIB
									showmsg->Release();
#else
									WODAPPUPDCOMLib::AppUpd_Messages_Free(showmsg);
#endif
								showmsg = msg;
							}
							else
#ifndef _APPUPDLIB
								msg->Release();
#else
							WODAPPUPDCOMLib::AppUpd_Messages_Free(msg);
#endif

						}
					}	
					
					if (showmsg)
					{
						if (!m_UpdateHandlerMsgBoxShown)
						{
							long ID;
#ifndef _APPUPDLIB
							showmsg->get_ID(&ID);
#else
							WODAPPUPDCOMLib::AppUpd_Message_GetID(showmsg, &ID);
#endif
							_Settings.m_LastOperatorMessageID = ID;
							_Settings.Save(FALSE);

							// spawn new thread to show the message
#ifndef _APPUPDLIB
							CComBSTR2 text, caption;
							showmsg->get_Text(&text);
							showmsg->get_Caption(&caption);
							showmsg->Release();
#else
							char text[32768] = {0}, caption[32768] = {0};
							int len = sizeof(text);
							WODAPPUPDCOMLib::AppUpd_Message_GetText(msg, text, &len);
							len = sizeof(caption);
							WODAPPUPDCOMLib::AppUpd_Message_GetCaption(msg, caption, &len);
							WODAPPUPDCOMLib::AppUpd_Messages_Free(showmsg);
#endif

							Buffer *b = new Buffer;
#ifndef _APPUPDLIB
							b->Append(text.ToString());
							b->Append("\0",1);
							b->Append(caption.ToString());
							b->Append("\0",1);
#else
							b->Append(text);
							b->Append("\0",1);
							b->Append(caption);
							b->Append("\0",1);
#endif
							DWORD id = 0;
							HANDLE h = CreateThread(NULL, 0, ShowMessageThreadProc, b, 0, &id);
							CloseHandle(h);
						}
#ifndef _APPUPDLIB
					}
				}
				msgs->Release();
#endif
			}


			if (NewFiles)
			{
				int answer = IDNO;
#ifndef _APPUPDLIB
				m_Owner->m_Update->put_Visible(m_Owner->m_Silently?VARIANT_FALSE:VARIANT_TRUE);
#else
				WODAPPUPDCOMLib::AppUpd_SetVisible(wodAppUpd, m_Owner->m_Silently?VARIANT_FALSE:VARIANT_TRUE);
#endif
				if (m_Owner->m_SilentCheck && m_Owner->m_Silently)
					answer = IDYES;
				else
				{
					if (m_UpdateHandlerMsgBoxShown)
						return;

					m_UpdateHandlerMsgBoxShown = TRUE;
					answer = CBalloonTipDlg::Show(NULL, "<font face=Verdana size=2>New version of Wippien is available. Download?<br>\r\n<br>\r\n<a href=\"http://wippien.com/notes.php\">Click here to see what's new.</a>", "Wippien update", MB_YESNO);
					m_UpdateHandlerMsgBoxShown = FALSE;

//					m_UpdateHandlerMsgBoxShown = TRUE;
//					answer = MessageBox(NULL, "New version of Wippien is available. Download?\r\n\r\nClick on Help to see what's new.", "Wippien update", MB_YESNO | MB_HELP | MB_ICONQUESTION);
//					m_UpdateHandlerMsgBoxShown = FALSE;
				}

				if (answer == IDYES)
				{
#ifndef _APPUPDLIB
					m_Owner->m_Update->put_Visible(m_Owner->m_Silently?VARIANT_FALSE:VARIANT_TRUE);
					m_Owner->m_Update->Download();
#else
					WODAPPUPDCOMLib::AppUpd_SetVisible(wodAppUpd, m_Owner->m_Silently?VARIANT_FALSE:VARIANT_TRUE);
					WODAPPUPDCOMLib::AppUpd_Download(wodAppUpd);
#endif
				}

			}
			else
			{
				if (!m_Owner->m_SilentCheck)
				{
					if (m_UpdateHandlerMsgBoxShown)
						return;
					m_UpdateHandlerMsgBoxShown = TRUE;
					MessageBox(NULL, "Your version of Wippien is up-to-date.", "Wippien update", MB_OK);
					m_UpdateHandlerMsgBoxShown = FALSE;
				}
			}
		}
		else
			if (!m_Owner->m_SilentCheck)
			{
				if (m_UpdateHandlerMsgBoxShown)
					return;
				m_UpdateHandlerMsgBoxShown = TRUE;
				MessageBox(NULL, "Error connecting to remote server.", "Wippien update", MB_ICONERROR);
				m_UpdateHandlerMsgBoxShown = FALSE;
			}
	}
#ifndef _APPUPDLIB
	void _stdcall StateChange(UpdateStates OldState)
	{

	}

	void _stdcall UpdateDone(long ErrorCode, BSTR ErrorText)
	{
	}

	void _stdcall FileStart(IUpdFile* File)
	{

	}

	void _stdcall FileProgress(IUpdFile* File, long Position, long Total)
	{

	}

	void _stdcall FileDone(IUpdFile* File, long ErrorCode, BSTR ErrorText)
	{

	}
#endif


#ifndef _APPUPDLIB
	void _stdcall DownloadDone(long ErrorCode, BSTR ErrorText)
	{
#else
	void AppUpd_DownloadDone(void *wodAppUpd, long ErrorCode, char *ErrorText)
	{
		VARIANT tag;
		WODAPPUPDCOMLib::AppUpd_GetTag(wodAppUpd, &tag);
		CUpdateHandler *m_Owner = (CUpdateHandler *)tag.plVal;
#endif
		if (!ErrorCode)
		{
			if (m_UpdateHandlerMsgBoxShown)
				return;
			m_UpdateHandlerMsgBoxShown = TRUE;
			int i = 0;
			if (m_Owner->m_SilentCheck)
				i = 6;
			else
				i = ::MessageBox(NULL, "Download successful. Replace now?", "Wippien update", MB_ICONQUESTION | MB_YESNO);
			m_UpdateHandlerMsgBoxShown = FALSE;
			if (i == 6)
			{
				m_Owner->m_Silently = TRUE;
#ifndef _APPUPDLIB
				m_Owner->m_Update->Update();
#else
				WODAPPUPDCOMLib::AppUpd_Update(wodAppUpd);
#endif
			}
		}
		else
		{
			if (!m_Owner->m_SilentCheck)
			{
				CComBSTR2 err = ErrorText;
				MessageBox(NULL, err.ToString(), "Wippien update error!", MB_ICONERROR);
			}

		}
	}

#ifndef _APPUPDLIB
	void _stdcall PrevDetected()
	{
#else
	void AppUpd_PrevDetected(void *wodAppUpd)		
	{
		VARIANT tag;
		WODAPPUPDCOMLib::AppUpd_GetTag(wodAppUpd, &tag);
		CUpdateHandler *m_Owner = (CUpdateHandler *)tag.plVal;
#endif	
		if (m_UpdateHandlerMsgBoxShown)
			return;

		m_UpdateHandlerMsgBoxShown = TRUE;
		int i = 6;
		if (!m_Owner->m_SilentCheck)
			i = ::MessageBox(NULL, "New version found localy. Replace?", "New version", MB_ICONQUESTION | MB_YESNO);
		m_UpdateHandlerMsgBoxShown = FALSE;
		if (i==6)
		{
#ifndef _APPUPDLIB
			m_Owner->m_Update->Update();
#else
			WODAPPUPDCOMLib::AppUpd_Update(wodAppUpd);
#endif
			if (::IsWindow(_MainDlg.m_hWnd))
			{
				::PostMessage(_MainDlg.m_hWnd, WM_COMMAND, ID_EXIT, 0);
			}
		}
		else
#ifndef _APPUPDLIB
			m_Owner->m_Update->Reset();
#else
			WODAPPUPDCOMLib::AppUpd_Reset(wodAppUpd);
#endif
	}    

#ifndef _APPUPDLIB    
    BEGIN_SINK_MAP (wodAppUpdateEvents)
        SINK_ENTRY_INFO (1, DIID__IwodAppUpdateComEvents, 0, CloseApp, &INFO_CloseApp)
		SINK_ENTRY_INFO (1, DIID__IwodAppUpdateComEvents, 1, CheckDone, &INFO_CheckDone)
		SINK_ENTRY_INFO (1, DIID__IwodAppUpdateComEvents, 2, StateChange, &INFO_StateChange)
		SINK_ENTRY_INFO (1, DIID__IwodAppUpdateComEvents, 3, UpdateDone, &INFO_UpdateDone)
		SINK_ENTRY_INFO (1, DIID__IwodAppUpdateComEvents, 4, FileStart, &INFO_FileStart)
		SINK_ENTRY_INFO (1, DIID__IwodAppUpdateComEvents, 5, FileProgress, &INFO_FileProgress)
		SINK_ENTRY_INFO (1, DIID__IwodAppUpdateComEvents, 6, FileDone, &INFO_FileDone)
		SINK_ENTRY_INFO (1, DIID__IwodAppUpdateComEvents, 7, DownloadDone, &INFO_DownloadDone)
		SINK_ENTRY_INFO (1, DIID__IwodAppUpdateComEvents, 8, PrevDetected, &INFO_PrevDetected)
    END_SINK_MAP ()

private:
	CUpdateHandler *m_Owner;
};
#endif


CUpdateHandler::CUpdateHandler()
{
	m_Silently = _Settings.m_CheckUpdateSilently;
	m_SilentCheck = _Settings.m_CheckUpdateSilently;
	m_Dlg = NULL;

	m_Update = NULL;
#ifndef _APPUPDLIB
	m_UpdateEvents = NULL;
#endif
}

CUpdateHandler::~CUpdateHandler()
{
#ifndef _APPUPDLIB
	if (m_UpdateEvents)
		delete m_UpdateEvents;

	if (m_Update)
		m_Update->Release();
#else
	WODAPPUPDCOMLib::_AppUpd_Destroy(m_Update);
#endif
	m_Update = NULL;

	if (m_Dlg)
		delete m_Dlg;
}

void BufferUncompress(z_stream z_str, char *input_buffer, int len, _Buffer * output_buffer)
{
    unsigned char buf[4096];
    int status;

    z_str.next_in = (unsigned char *)input_buffer;
    z_str.avail_in = len;

    for (;;) {
            /* Set up fixed-size output buffer. */
            z_str.next_out = buf;
            z_str.avail_out = sizeof(buf);

            status = inflate(&z_str, Z_PARTIAL_FLUSH);
            switch (status) {
            case Z_OK:
                    output_buffer->Append((char *)buf,sizeof(buf) - z_str.avail_out);
                    break;
            case Z_BUF_ERROR:
                    /*
                     * Comments in zlib.h say that we should keep calling
                     * inflate() until we get an error.  This appears to 
                     * be the error that we get.
                     */
                    return;
            default:
					return;
                    /* NOTREACHED */
            }
    }
}



void CUpdateHandler::InitUpdater(void)
{
	if (!m_Update)
	{
#ifndef _APPUPDLIB
		HRESULT hr = ::CoCreateInstance(CLSID_wodAppUpdateCom, NULL, CLSCTX_ALL, IID_IwodAppUpdateCom, (void **)&m_Update);
//		HRESULT hr = m_Update.CoCreateInstance(CLSID_wodAppUpdateCom);
		
		if (SUCCEEDED(hr))
		{
			CComBSTR dialognoteurl = "http://www.weonlydo.com/index.asp?showform=AppUpdate&from=Wippien";
			CComBSTR updtername="WippienUpdater";

			if (m_UpdateEvents)
				delete m_UpdateEvents;
			m_UpdateEvents = new wodAppUpdateEvents(this);
			m_Update->put_UpdaterName(updtername);
			m_Update->put_DialogNoteURL(dialognoteurl);
			m_Update->put_ReplaceRule(ReplaceIfVersion);

#ifdef WODAPPUPDATE_LICENSE_KEY
			CComBSTR lickey = WODAPPUPDATE_LICENSE_KEY;
			m_Update->put_LicenseKey(lickey);
#endif		
		}
		else
			return;
#else
		memset (&m_AppUpdEvents, 0, sizeof(m_AppUpdEvents));
		m_AppUpdEvents.CheckDone = AppUpd_CheckDone;
		m_AppUpdEvents.CloseApp = AppUpd_CloseApp;
		m_AppUpdEvents.DownloadDone = AppUpd_DownloadDone;
		m_AppUpdEvents.PrevDetected = AppUpd_PrevDetected;

		m_Update = WODAPPUPDCOMLib::_AppUpd_Create(&m_AppUpdEvents);
		VARIANT var;
		var.vt = VT_I4;
		var.lVal = (LONG)this;

		const char *dialognoteurl = "http://www.weonlydo.com/index.asp?showform=AppUpdate&from=Wippien";
		const char *updtername="WippienUpdater";

		WODAPPUPDCOMLib::AppUpd_SetTag(m_Update, var);
		WODAPPUPDCOMLib::AppUpd_SetUpdaterName(m_Update, (char *)updtername);
		WODAPPUPDCOMLib::AppUpd_SetDialogNoteURL(m_Update, (char *)dialognoteurl);
		WODAPPUPDCOMLib::AppUpd_SetReplaceRule(m_Update, (WODAPPUPDCOMLib::ReplaceRulesEnum)/*ReplaceIfVersion*/1);
		WODAPPUPDCOMLib::AppUpd_SetReplaceRule(m_Update, (WODAPPUPDCOMLib::ReplaceRulesEnum)/*ReplaceIfVersion*/1);
		WODAPPUPDCOMLib::AppUpd_SetRequireAdmin(m_Update, (BOOL)TRUE);
#endif
	}
}

void CUpdateHandler::DownloadUpdates(BOOL silently)
{
	if (m_UpdateHandlerMsgBoxShown)
		return;

	InitUpdater();

	// let's see if we have newer versions
	Buffer URLbuff;
	URLbuff.Append("http://wippien.com/Download/update.php?JID=");
	Buffer u1;
	u1.Append(_Settings.m_JID);


#ifndef _APPUPDLIB
	CComBSTR url = URLbuff.Ptr();
	CComVariant a = url;
	m_Update->put_Visible(silently?VARIANT_FALSE:VARIANT_TRUE);
	m_SilentCheck = silently;
	m_Update->put_ReplaceRule((ReplaceRulesEnum)1); //replaceifnewer
	HRESULT hr = m_Update->Check(a);
	if (SUCCEEDED(hr))
	{
	
	}
#else
	WODAPPUPDCOMLib::AppUpd_SetVisible(m_Update, silently?VARIANT_FALSE:VARIANT_TRUE);
	m_SilentCheck = silently;
	WODAPPUPDCOMLib::AppUpd_SetURL(m_Update, URLbuff.Ptr());
	WODAPPUPDCOMLib::AppUpd_SetReplaceRule(m_Update, (WODAPPUPDCOMLib::ReplaceRulesEnum)1);
	WODAPPUPDCOMLib::AppUpd_Check(m_Update);
#endif
	// spawn new thread so this can continue
//	DWORD id = 0;
//	CreateThread(NULL, 0, UpdateHandlerThreadProc, this, 0, &id);
	
//	return;
}
