#ifndef __CEXWINSOCKET
#define __CEXWINSOCKET

#define WM_USER_SOCKET				WM_USER+1
#define WM_SEND_USERS				WM_USER+11
#define WM_SEND_CONNECT_REQUEST		WM_USER+12
#define WM_SEND_HELO				WM_USER+13
#define WM_EXE_CONNECT				WM_USER+14
#define WM_EXE_DISCONNECT			WM_USER+15
#define WM_REFRESH					WM_USER+16
#define WM_TRAYICON					WM_USER+17
#define WM_PURGEINPUTBOX			WM_USER+18
#define WM_NEEDRESTART				WM_USER+19
#define WM_VCARDCHANGED				WM_USER+20
#define WM_MTUDETECT				WM_USER+21
//#define MYDEBUG


#ifdef MYDEBUG
void MyTrace(LPCSTR lpszFormat, ...);
#define _ATLTRACE MyTrace
#else
#define _ATLTRACE ATLTRACE
#endif

#include <vector>
using namespace std;


class CwodWinSocket
{

public:
	CwodWinSocket();
	virtual ~CwodWinSocket();

	HWND m_sockWndHandle;
	SOCKET m_sock;
	WSADATA m_wsadata;
	int m_TimerNo;


private:
	bool m_ClassRegistered;
	void CreateHiddenWindow(void);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	int SetTimer(long nTimeout);
	void SetTimer(long nTimeout, int TimerNo);
	void KillTimer(int TimerNo);

	virtual void FdReceive(int nErrorCode);
	virtual void FdSend(int nErrorCode);
	virtual void FdAccept(int nErrorCode);
	virtual void FdConnect(int nErrorCode);
	virtual void FdClose(int nErrorCode);
	virtual void FdTimer(int TimerID);
	virtual void FdMTU(int MTU);
// pass through
	SOCKET PASCAL FAR accept (OUT struct sockaddr FAR *addr,IN OUT int FAR *addrlen)
	{
		return ::accept(m_sock, addr, addrlen);
	}
	int PASCAL FAR bind (IN const struct sockaddr FAR *addr,IN int namelen)
	{
		return ::bind(m_sock, addr, namelen);
	}
	int PASCAL FAR closesocket(void)
	{
		return ::closesocket(m_sock);
	}
	int PASCAL FAR connect (IN const struct sockaddr FAR *name,IN int namelen)
	{
		return ::connect(m_sock, name, namelen);
	}
	int PASCAL FAR connect (char *hostname, int port)
	{
		SOCKADDR_IN sockAddr;
		memset(&sockAddr,0,sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = inet_addr(hostname);
		// was translation ok?
		if (sockAddr.sin_addr.s_addr == INADDR_NONE)
		{
			struct hostent FAR * phost;
			phost = gethostbyname(hostname);
			if(!phost)
			{
				SetLastError(WSAHOST_NOT_FOUND);
				return INVALID_SOCKET;
			}

			sockAddr.sin_addr.s_addr = inet_addr(inet_ntoa((struct in_addr)*(struct in_addr *)phost->h_addr));
			// is it bad again?
			if (sockAddr.sin_addr.s_addr == INADDR_NONE)
			{
				SetLastError(WSAHOST_NOT_FOUND);
				return INVALID_SOCKET;
			}
		}
		sockAddr.sin_port = htons((u_short)port);
		return ::connect(m_sock, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	}
	int PASCAL FAR ioctl (IN long cmd,IN OUT u_long FAR *argp)
	{
		return ::ioctlsocket(m_sock, cmd, argp);
	}
	int PASCAL FAR getpeername (OUT struct sockaddr FAR *name,IN OUT int FAR * namelen)
	{
		return ::getpeername(m_sock, name, namelen);
	}
	int PASCAL FAR getsockname (OUT struct sockaddr FAR *name,IN OUT int FAR * namelen)
	{
		return ::getsockname(m_sock, name, namelen);
	}
	int PASCAL FAR getsockopt (IN int level,IN int optname,OUT char FAR * optval,IN OUT int FAR *optlen)
	{
		return ::getsockopt(m_sock, level, optname, optval, optlen);
	}
	u_long PASCAL FAR htonl ( IN u_long hostlong)
	{
		return ::htonl(hostlong);
	}
	u_short PASCAL FAR htons (IN u_short hostshort)
	{
		return ::htons(hostshort);
	}
	unsigned long PASCAL FAR inet_addr (IN const char FAR * cp)
	{
		return ::inet_addr(cp);
	}
	char FAR * PASCAL FAR inet_ntoa (IN struct in_addr in)
	{
		return ::inet_ntoa(in);
	}
	int PASCAL FAR listen (IN int backlog)
	{
		return ::listen(m_sock, backlog);
	}
	u_long PASCAL FAR ntohl (IN u_long netlong)
	{
		return ::ntohl(netlong);
	}
	u_short PASCAL FAR ntohs (IN u_short netshort)
	{
		return ::ntohs(netshort);
	}
	int PASCAL FAR recv (OUT char FAR * buf,IN int len,IN int flags)
	{
		return ::recv(m_sock, buf, len, flags);
	}
	int PASCAL FAR recvfrom (OUT char FAR * buf,IN int len,IN int flags,OUT struct sockaddr FAR *from,IN OUT int FAR * fromlen)
	{
		return ::recvfrom(m_sock, buf, len, flags, from, fromlen);
	}
	int PASCAL FAR select (IN int nfds,IN OUT fd_set FAR *readfds,IN OUT fd_set FAR *writefds,IN OUT fd_set FAR *exceptfds,IN const struct timeval FAR *timeout)
	{
		return ::select(nfds, readfds, writefds, exceptfds, timeout);
	}
	int PASCAL FAR send (IN const char FAR * buf,IN int len,IN int flags)
	{
		return ::send(m_sock, buf, len, flags);
	}
	int PASCAL FAR sendto (IN const char FAR * buf,IN int len,IN int flags,IN const struct sockaddr FAR *to,IN int tolen)
	{
		return ::sendto(m_sock, buf, len, flags, to, tolen);
	}
	int PASCAL FAR setsockopt (IN int level,IN int optname,IN const char FAR * optval,IN int optlen)
	{
		return ::setsockopt(m_sock, level, optname, optval, optlen);
	}
	int PASCAL FAR shutdown (IN int how)
	{
		return ::shutdown (m_sock, how);
	}
	SOCKET PASCAL FAR socket (IN int af,IN int type,IN int protocol)
	{
		m_sock = ::socket(af, type, protocol);
		return m_sock;
	}
	struct hostent FAR * PASCAL FAR gethostbyaddr(IN const char FAR * addr,IN int len,IN int type)
	{
		return ::gethostbyaddr(addr, len, type);
	}

	struct hostent FAR * PASCAL FAR gethostbyname(IN const char FAR * name)
	{
		return ::gethostbyname(name);
	}

	int PASCAL FAR gethostname (OUT char FAR * name,IN int namelen)
	{
		return ::gethostname(name, namelen);
	}
	struct servent FAR * PASCAL FAR getservbyport(IN int port,IN const char FAR * proto)
	{
		return ::getservbyport( port, proto);
	}
	struct servent FAR * PASCAL FAR getservbyname(IN const char FAR * name,IN const char FAR * proto)
	{
		return ::getservbyname(name, proto);
	}
	struct protoent FAR * PASCAL FAR getprotobynumber(IN int proto)
	{
		return ::getprotobynumber(proto);
	}
	struct protoent FAR * PASCAL FAR getprotobyname(IN const char FAR * name)
	{
		return ::getprotobyname(name);
	}
	void PASCAL FAR SetLastError(IN int iError)
	{
		::WSASetLastError(iError);
	}
	virtual int PASCAL FAR GetLastError(void)
	{
		return ::WSAGetLastError();
	}
	BOOL PASCAL FAR IsBlocking(void)
	{
		return ::WSAIsBlocking();
	}
	int PASCAL FAR UnhookBlockingHook(void)
	{
		return ::WSAUnhookBlockingHook();
	}
	FARPROC PASCAL FAR SetBlockingHook(IN FARPROC lpBlockFunc)
	{
		return ::WSASetBlockingHook(lpBlockFunc);
	}
	int PASCAL FAR CancelBlockingCall(void)
	{
		return ::WSACancelBlockingCall();
	}
/*	HANDLE PASCAL FAR WSAAsyncGetServByName(IN HWND hWnd,
											IN u_int wMsg,
											IN const char FAR * name,
											IN const char FAR * proto,
											OUT char FAR * buf,
											IN int buflen);

	HANDLE PASCAL FAR WSAAsyncGetServByPort(
											IN HWND hWnd,
											IN u_int wMsg,
											IN int port,
											IN const char FAR * proto,
											OUT char FAR * buf,
											IN int buflen);

	HANDLE PASCAL FAR WSAAsyncGetProtoByName(
											 IN HWND hWnd,
											 IN u_int wMsg,
											 IN const char FAR * name,
											 OUT char FAR * buf,
											 IN int buflen);

	HANDLE PASCAL FAR WSAAsyncGetProtoByNumber(
											   IN HWND hWnd,
											   IN u_int wMsg,
											   IN int number,
											   OUT char FAR * buf,
											   IN int buflen);

	HANDLE PASCAL FAR WSAAsyncGetHostByName(
											IN HWND hWnd,
											IN u_int wMsg,
											IN const char FAR * name,
											OUT char FAR * buf,
											IN int buflen);

	HANDLE PASCAL FAR WSAAsyncGetHostByAddr(
											IN HWND hWnd,
											IN u_int wMsg,
											IN const char FAR * addr,
											IN int len,
											IN int type,
											OUT char FAR * buf,
											IN int buflen);
*/
	int PASCAL FAR CancelAsyncRequest(IN HANDLE hAsyncTaskHandle)
	{
		return ::WSACancelAsyncRequest(hAsyncTaskHandle);
	}
	int PASCAL FAR AsyncSelect(IN long lEvent)
	{
		return ::WSAAsyncSelect(m_sock, m_sockWndHandle, WM_USER_SOCKET, lEvent);
	}
	int PASCAL FAR WSARecvEx (OUT char FAR * buf,IN int len,IN OUT int FAR *flags)
	{
		return ::WSARecvEx(m_sock, buf, len, flags);
	}

};

template <class T> class CSimpleSocketCallback : public CwodWinSocket
{
	
public:
	CSimpleSocketCallback( T* ctrl) : CwodWinSocket()
	{
		m_Ctrl = ctrl;
	}

	T *m_Ctrl;
private:


	virtual void FdAccept(int err){m_Ctrl->FdAccept(err, this);}
	virtual void FdClose(int err){m_Ctrl->FdClose(err, this);}
	virtual void FdConnect(int err){m_Ctrl->FdConnect(err, this);}
	virtual void FdReceive(int err){m_Ctrl->FdReceive(err, this);}
	virtual void FdTimer(int timerid){m_Ctrl->FdTimer(timerid, this);}
public:
	virtual void FdSend(int err){m_Ctrl->FdSend(err, this);}
};

#endif