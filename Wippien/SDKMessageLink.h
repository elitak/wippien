#ifndef __SDKMESSAGELINK_H
#define __SDKMESSAGELINK_H


#include "SDKMessages.h"


#include <vector>
typedef std::vector<long> voidparray;


class CSDKMessageLink  
{
public:
	CSDKMessageLink();
	virtual ~CSDKMessageLink();


	HWND m_hWnd, m_RemoteWnd;
	void CreateLinkWindow(void);

	voidparray m_Windows;


	long SendBuffer(HWND hwnd, void *Data, int Len);
	long SendBuffer(HWND hwnd, int Event, void *Data, int Len);
	long SendBuffer(HWND hwnd, char *Data);
	long SendBuffer(HWND hwnd, BSTR Data);

	BOOL FireEvent(int Cmd, void *Data, int Len);
	BOOL FireEvent(int Cmd, char *Data);
	BOOL FireEvent(int Cmd, BSTR Data);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	class CJabberEvents;
	class CJabberWiz
	{
	public:

#ifndef _WODXMPPLIB
		CComPtr<WODXMPPCOMLib::IwodXMPPCom> m_Jabb;
#else
		void *m_Jabb;
#endif
		CJabberWiz(CSDKMessageLink *Owner);
		~CJabberWiz();
		void Connect(char *JID, char *pass, char *hostname, int port, BOOL registernew);
		void Disconnect(void);

#ifndef _WODXMPPLIB
		CJabberEvents *m_Events;
#else
		WODXMPPCOMLib::XMPPEventsStruct m_Events;
#endif
		CSDKMessageLink *m_Owner;
	};

	CJabberWiz *Wiz;
};

#endif //__SDKMESSAGELINK_H