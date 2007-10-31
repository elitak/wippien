/************************************
  REVISION LOG ENTRY
  Revision By: Ozzie (ozzie@weonlydo.com)
  Revised on 12.11.2001 22:34:42
  Comments: Implementation of CwodWinSocket class
 ************************************/

#include "stdafx.h"
#include "CwodWinSocket.h"

long m_WsockRefCount = 0;
BOOL m_WsockInited = FALSE;


// Function name	: CwodWinSocket::CwodWinSocket
// Description	    : 
// Return type		: 
CwodWinSocket::CwodWinSocket()
{
	m_ClassRegistered = false;
	m_sockWndHandle = NULL;
	m_sock = NULL;
	m_TimerNo = 0;

	InterlockedIncrement(&m_WsockRefCount);

	if (m_WsockRefCount == 1 && !m_WsockInited)
	{
		m_WsockInited = TRUE;
		WORD wVersionRequested = MAKEWORD(2, 0);

		WSAStartup(MAKEWORD(2, 0), &m_wsadata);
	}

	CreateHiddenWindow();
}


// Function name	: CwodWinSocket::~CwodWinSocket
// Description	    : 
// Return type		: 
CwodWinSocket::~CwodWinSocket()
{

	// remove all messages from the queue
	::SetWindowLong(m_sockWndHandle, GWL_USERDATA, 0);
	DestroyWindow(m_sockWndHandle);


	if (m_sock)
		closesocket();

	InterlockedDecrement(&m_WsockRefCount);
}

void FreeWinSock(void)
{
	::WSACleanup();
}


// Function name	: CwodWinSocket::CreateHiddenWindow
// Description	    : 
// Return type		: void 
// Argument         : void
void CwodWinSocket::CreateHiddenWindow(void)
{
    WNDCLASS wndclass;

    if(!m_ClassRegistered) 
	{
        memset(&wndclass, 0, sizeof(wndclass));
        wndclass.lpfnWndProc = WindowProc;
        wndclass.hInstance   = _Module.GetModuleInstance();
        wndclass.lpszClassName = "CwodWinSocketWindowClass";
		wndclass.cbWndExtra  = 4;
        RegisterClass(&wndclass);
        m_ClassRegistered = TRUE;
    }

	m_sockWndHandle = CreateWindow(wndclass.lpszClassName,"CwodWinSocketWindow",WS_POPUP,0, 0, 0, 0,NULL,NULL,_Module.GetModuleInstance(),NULL);
	if(m_sockWndHandle) SetWindowLong(m_sockWndHandle, GWL_USERDATA, (LONG)this);
}



// Function name	: CwodWinSocket::WindowProc
// Description	    : 
// Return type		: LRESULT CALLBACK 
// Argument         : HWND hWnd
// Argument         : UINT message
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
LRESULT CALLBACK CwodWinSocket::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	CwodWinSocket *sock = (CwodWinSocket *)GetWindowLong(hWnd, GWL_USERDATA);

	if (sock)
	{
		if (message == WM_USER_SOCKET)
		{

			int err = WSAGETSELECTERROR(lParam);
			int evt = WSAGETSELECTEVENT(lParam);

			switch (evt)
			{
				case FD_READ: 
					{
//							_ATLTRACE("\r\nFD_READ\r\n");
						sock->FdReceive(err); break;
					}
				case FD_WRITE: sock->FdSend(err); break;
				case FD_CONNECT: sock->FdConnect(err); break;
				case FD_ACCEPT: sock->FdAccept(err); break;
				case FD_CLOSE: sock->FdClose(err); break;
//				case FD_READ: sock->FdReceive(err); break;
			}
			return TRUE;
		}
		else
		if (message == WM_TIMER)
		{
			sock->FdTimer((int)wParam);
			return TRUE;
		}
		else
		if (message == WM_MTUDETECT)
		{
			sock->FdMTU((int)wParam);
			return TRUE;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


// Function name	: CwodWinSocket::FdReceive
// Description	    : 
// Return type		: void 
// Argument         : int nErrorCode
void CwodWinSocket::FdReceive(int nErrorCode)
{

}


// Function name	: CwodWinSocket::FdSend
// Description	    : 
// Return type		: void 
// Argument         : int nErrorCode
void CwodWinSocket::FdSend(int nErrorCode)
{

}


// Function name	: CwodWinSocket::FdAccept
// Description	    : 
// Return type		: void 
// Argument         : int nErrorCode
void CwodWinSocket::FdAccept(int nErrorCode)
{

}


// Function name	: CwodWinSocket::FdConnect
// Description	    : 
// Return type		: void 
// Argument         : int nErrorCode
void CwodWinSocket::FdConnect(int nErrorCode)
{

}

void CwodWinSocket::FdMTU(int MTU)
{

}

// Function name	: CwodWinSocket::FdClose
// Description	    : 
// Return type		: void 
// Argument         : int nErrorCode
void CwodWinSocket::FdClose(int nErrorCode)
{

}


// Function name	: CwodWinSocket::FdTimer
// Description	    : 
// Return type		: void 
// Argument         : int TimerID
void CwodWinSocket::FdTimer(int TimerID)
{

}


// Function name	: CwodWinSocket::SetTimer
// Description	    : 
// Return type		: void 
// Argument         : long nTimeout
// Argument         : int TimerNo
void CwodWinSocket::SetTimer(long nTimeout, int TimerNo)
{
	if (nTimeout)
		::SetTimer(m_sockWndHandle, TimerNo, nTimeout, NULL);
}

// Function name	: CwodWinSocket::SetTimer
// Description	    : 
// Return type		: int 
// Argument         : long nTimeout
int CwodWinSocket::SetTimer(long nTimeout)
{
	SetTimer(nTimeout, m_TimerNo);
	return m_TimerNo++;
}


// Function name	: CwodWinSocket::KillTimer
// Description	    : 
// Return type		: void 
// Argument         : int TimerNo
void CwodWinSocket::KillTimer(int TimerNo)
{
	::KillTimer(m_sockWndHandle, TimerNo);
}

#ifdef MYDEBUG
#include "stdio.h"

void MyTrace(LPCSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	char szBuffer[512];

	nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
	ATLASSERT(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)
	
	FILE *stream = fopen("c:\\debug.txt", "at");
	if (stream)
	{
		fputs(szBuffer, stream);
		fclose(stream);
	}


	OutputDebugStringA(szBuffer);
	va_end(args);
}

#endif