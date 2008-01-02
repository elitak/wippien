#ifndef _SIMPLE_HTTP_REQUEST_H
#define _SIMPLE_HTTP_REQUEST_H

#include "CwodWinSocket.h"

class CSimpleHttpRequest
{
public:
	CSimpleHttpRequest(HWND Owner, int TimerID)
	{
		m_Socket = new CSimpleSocketCallback<CSimpleHttpRequest>(this);
		m_Socket->socket(AF_INET, SOCK_STREAM, 0);
		m_Socket->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT);
		m_Owner = Owner;
		m_TimerID = TimerID;
	}
	virtual ~CSimpleHttpRequest()
	{
		delete m_Socket;
	}
	
	HWND m_Owner;
	int m_TimerID;	
	CSimpleSocketCallback<CSimpleHttpRequest> *m_Socket;
	Buffer m_In, m_Out;

	BOOL ParseURL(char *URL, Buffer *Host, Buffer *Uri, int *Port)
	{
		char buff[16384];
		strcpy(buff, URL);
		if (!strncmp(buff, "http://", 7))
		{
			char *host = buff + 7;
			int port = 80;
			
			char *uri = strchr(host, '/');
			if (uri)
				*uri++ = 0;
			else
				uri = "";
			
			char *p = strchr(host, ':');
			if (p)
			{
				*p++ = 0;
				port = atoi(p);
			}
			
			if (Host)
				Host->Append(host);
			if (Uri)
				Uri->Append(uri);
			if (Port)
				*Port = port;
			
			return TRUE;
		}
		
		return FALSE;
	}
	
	void Get(char *URL)
	{
		Buffer h, u;
		int port;
		if (ParseURL(URL, &h, &u, &port))
		{
			Get(h.Ptr(), u.Ptr(), port);
		}
	}
	void Get(char *Host, char *Uri, int Port)
	{
		m_In.Append("GET /");
		m_In.Append(Uri);
		m_In.Append(" HTTP/1.1\r\nHost: ");
		m_In.Append(Host);
		if (Port != 80)
		{
			char buff[1024];
			sprintf(buff, ":%d", Port);
			m_In.Append(buff);
		}
		m_In.Append("\r\nConnection: close\r\n\r\n");
		m_Socket->connect(Host, Port);
	}
	
	void FdAccept(int err, CSimpleSocketCallback<CSimpleHttpRequest> *Socket){};
	void FdClose(int err, CSimpleSocketCallback<CSimpleHttpRequest> *Socket)
	{
		if (!err)
		{
			::PostMessage(m_Owner, WM_TIMER, m_TimerID, NULL);
		}
	}
	void FdConnect(int err, CSimpleSocketCallback<CSimpleHttpRequest> *Socket)
	{
	}
	void FdReceive(int err, CSimpleSocketCallback<CSimpleHttpRequest> *Socket)
	{
		char buff[16384];
		int i = Socket->recv(buff, sizeof(buff), 0);
		if (i>0)
		{
			//		DumpToFileFixed(buff, i);
			m_Out.Append(buff, i);
		}
	}
	void FdSend(int err, CSimpleSocketCallback<CSimpleHttpRequest> *Socket)
	{
		if (!err)
		{
			// let's send all we have
			if (m_In.Len())
			{
				Socket->send(m_In.Ptr(), m_In.Len(), 0);
				m_In.Clear();
			}
		}
	}
	void FdTimer(int timerid, CSimpleSocketCallback<CSimpleHttpRequest> *Socket){};
	
};


#endif