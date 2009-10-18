#include "stdafx.h"
#include "User.h"
#include "JabberLib.h"
#include "Ethernet.h"

BOOL gClassRegistered = FALSE;
extern CJabberLib *_Jabber;
extern CEthernet *_Ethernet;

#ifndef _WIPPIENSERVICE
void SetStatus(char *Text);
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUser::CUser()
{
	memset(&m_Events, 0, sizeof(m_Events));
	m_Events.StateChange = EventStateChange;
	m_Events.Connected = EventConnected;
	m_Events.Disconnected = EventDisconnected;
	m_Events.SearchDone = EventSearchDone;
	m_Events.IncomingData = EventIncomingData;
	
	m_Handle = WODVPN::_VPN_Create(&m_Events);

	m_BlinkCounter = 0;
	m_RemoteIP = 0;
	memset(&m_MAC, 0, sizeof(m_MAC));
	m_JID[0] = m_Resource[0];
	m_RSA = NULL;
	for (int i = 0; i < 16; i++) m_MyKey[i] = rand();

	
	m_State = WipWaitingInitRequest;
	m_RemoteState = WipWaitingInitRequest;


    WNDCLASS wndclass = {0};
	wndclass.lpszClassName = "MiniVPNWindowClass";
	
    if(!gClassRegistered) // thread safety not important
	{
        wndclass.lpfnWndProc = WindowProc;
#ifndef _WIPPIENSERVICE
        wndclass.hInstance   = hMainInstance;
#else
		wndclass.hInstance = _Module.GetModuleInstance();
#endif
		wndclass.cbWndExtra  = 4;
        RegisterClass(&wndclass);
        gClassRegistered = TRUE;
    }
	
#ifndef _WIPPIENSERVICE
	m_hWnd = CreateWindow(wndclass.lpszClassName,"MiniVPNWindow",WS_POPUP,0, 0, 0, 0,NULL,NULL,hMainInstance,NULL);
#else
	m_hWnd = CreateWindow(wndclass.lpszClassName,"MiniVPNWindow",WS_POPUP,0, 0, 0, 0,NULL,NULL,_Module.GetModuleInstance(),NULL);
#endif
	if(m_hWnd) SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

	WODVPN::VPN_SetTag(m_Handle, this);
	m_HisRandom = m_MyRandom = 0;
	while (!m_MyRandom)
	{
		m_MyRandom = (rand()%1024)+1;
	}
	m_MediatorHost[0] = 0;
}

CUser::~CUser()
{
	WODVPN::_VPN_Destroy(m_Handle);
	if (m_RSA)
		RSA_free(m_RSA);
}

LRESULT CALLBACK CUser::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	CUser *user = (CUser*)GetWindowLong(hWnd, GWL_USERDATA);
	if (message == WM_TIMER)
	{
		if (wParam == 1) // timer 1
		{
			KillTimer(hWnd, wParam);
			_Jabber->ExchangeWippienDetails(user, TRUE);
		}
		if (wParam == 2) // blink timer
		{
			user->Blink();
		}
		return TRUE;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


// events
long CUser::EventStateChange(void *wodVPN, WODVPN::StatesEnum OldState)
{
#ifndef _WIPPIENSERVICE
	CUser *me;
	if (!WODVPN::VPN_GetTag(wodVPN, (void **)&me))
	{
		char buff[1024];
		int bflen = sizeof(buff);

		WODVPN::StatesEnum st;
		WODVPN::VPN_GetState(wodVPN, &st);
		WODVPN::VPN_GetStateText(wodVPN, st, buff, &bflen);
		SetStatus(buff);
	}
#endif
	return 0;
}

long CUser::EventConnected(void *wodVPN, char * PeerID, char * IP, long Port)
{
	CUser *me;
	if (!WODVPN::VPN_GetTag(wodVPN, (void **)&me))
	{
		me->m_State = WipConnected;
#ifndef _WIPPIENSERVICE
		char buff[1024];
		sprintf(buff, "CONNECTED to %s", me->m_JID);
		SetStatus(buff);
#endif
		KillTimer(me->m_hWnd, 2);

#ifndef _WIPPIENSERVICE
		LVITEM *li = _Jabber->GetItemByJID(me->m_JID);
		if (li)
		{
			li->mask = LVIF_TEXT;
			li->iSubItem = 1;
			struct  in_addr sa1;
			sa1.S_un.S_addr = me->m_RemoteIP;
			sprintf(buff,"%s **", inet_ntoa(sa1));
			li->pszText = buff;
			li->cchTextMax = strlen(li->pszText);
			SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_SETITEM, 0, (LPARAM)li);
		}	
#endif
	}
	return 0;
}

long CUser::EventDisconnected(void *wodVPN, long ErrorCode, char * ErrorText)
{
	CUser *me;
	if (!WODVPN::VPN_GetTag(wodVPN, (void **)&me))
	{
		me->m_State = WipDisconnected;
#ifndef _WIPPIENSERVICE
		SetStatus("Peer Disconnected");
#endif
		me->m_BlinkCounter = 1;
		me->Blink();
		KillTimer(me->m_hWnd, 2);
	}
	return 0;
}

long CUser::EventSearchDone(void *wodVPN, char * IP, long Port, long ErrorCode, char * ErrorText)
{
	CUser *me;
	if (!WODVPN::VPN_GetTag(wodVPN, (void **)&me))
	{
		if (!ErrorCode)
		{
			WODVPN::VPN_Connect(wodVPN, IP, Port);
		}
		else
		{
			me->m_State = WipDisconnected;
#ifndef _WIPPIENSERVICE
			SetStatus(ErrorText);
#endif
		}
	}
	return 0;
}

long CUser::EventIncomingData(void *wodVPN, void *Data, int Len)
{
	CUser *me;
	if (!WODVPN::VPN_GetTag(wodVPN, (void **)&me))
	{
		_Ethernet->InjectPacket((char *)Data, Len);
	}
	return 0;
}

BOOL CUser::SendNetworkPacket(char *data, int len)
{
	switch (m_State)
	{
		case WipDisconnected:
			// this may arrive in another thread, let's post timer message instead
			SetTimer(m_hWnd, 1, 100, NULL);
			//m_Owner->m_Jabber->ExchangeWippienDetails(m_Contact, TRUE);
			break;
		
		case WipConnecting:
			// do nothing;
			break;
		
		case WipConnected:
			ETH_HEADER *ethr = (ETH_HEADER *) data;
			IPHDR *ip = (IPHDR *)(data + sizeof(ETH_HEADER));
			UDPHDR *udp = (UDPHDR *)(data + sizeof(ETH_HEADER) + sizeof(IPHDR));
			ICMPHDR *icmp = (ICMPHDR *)(data + sizeof(ETH_HEADER) + sizeof(IPHDR));
			TCPHDR *tcp = (TCPHDR *)(data + sizeof(ETH_HEADER) + sizeof(IPHDR));
			ARP_PACKET *p = (ARP_PACKET *)data;
			
			if (ethr->proto == 8)
			{
				if (ip->saddr != _Ethernet->m_MyIP  && ip->saddr)
					return FALSE;
			}
			
			
			// we can safely skip almost whole ethernet header!!
			//			data += sizeof(ETH_HEADER) - sizeof(USHORT);
			//			len -= sizeof(ETH_HEADER) - sizeof(USHORT);
			
			//				int app = 0;	
			
			//				unsigned long *myip = (unsigned long *)&data[-4];
			//				*myip = _Settings.m_MyLastNetwork;
			//				*myip = *myip >> 8;
			//				data[-1] = app;
			//				m_LastSend = 0;
			
	//		EnterCriticalSection(&m_CritCS);
			if (m_Handle)
				WODVPN::VPN_SendData(m_Handle, data, len, FALSE);
	//		LeaveCriticalSection(&m_CritCS);
			break;
	}
	return 0;
}

void CUser::Blink(void)
{
#ifndef _WIPPIENSERVICE
	LVITEM *li = _Jabber->GetItemByJID(m_JID);
	if (li && m_BlinkCounter < 20)
	{
		m_BlinkCounter++;
		li->iSubItem = 1;
		li->mask = LVIF_TEXT;
		if (m_BlinkCounter%2)
		{
			li->cchTextMax = 0;
			li->pszText = "";
			SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_SETITEM, 0, (LPARAM)li);
		}
		else
		{
			struct  in_addr sa1;
			sa1.S_un.S_addr = m_RemoteIP;
			li->pszText = inet_ntoa(sa1);
			li->cchTextMax = strlen(li->pszText);
			SendDlgItemMessage(hMainWnd, IDC_CONTACTLIST, LVM_SETITEM, 0, (LPARAM)li);
		}
	}
	else
#endif
	{
		m_BlinkCounter = 0;
		KillTimer(m_hWnd, 2);
	}

}