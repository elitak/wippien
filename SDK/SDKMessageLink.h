#ifndef __SDKMESSAGELINK_H
#define __SDKMESSAGELINK_H


#include "../../Public/Wippien/SDKMessages.h"

#include <vector>
typedef std::vector<long> voidparray;

class _Buffer;
class CWippienSDK;

class CSDKMessageLink  
{
public:
	CSDKMessageLink(CWippienSDK *Owner);
	virtual ~CSDKMessageLink();


	BOOL m_WaitHandle;
	_Buffer *m_WaitBuffer;
	HWND m_hWnd, m_RemoteWnd;
	void CreateLinkWindow(void);

	voidparray m_Windows;
	CWippienSDK *m_Owner;

	HWND FindServerWindow(void);
	BOOL SendCommand(int Command, LPARAM lParam);
	BOOL SendCommandWaitResult(int Command, LPARAM lParam);
	BOOL SendCommand(int Command, char *Data, int Len);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif //__SDKMESSAGELINK_H