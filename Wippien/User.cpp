// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Jabber.h"
#include "User.h"
#include "ComBSTR2.h"
#include "Settings.h"
#include "MainDlg.h"
#include "Ethernet.h"
#include "MsgWin.h"
#include "ChatRoom.h"
#include "SettingsDlg.h"
#include "ping.h"
#include "SDKMessageLink.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>


extern CSettings _Settings;
extern CJabber *_Jabber;
extern CMainDlg _MainDlg;
extern CEthernet _Ethernet;
extern CSDKMessageLink *_SDK;

const char *WippienStateString[] = {"WipWaitingInitRequest","WipWaitingInitResponse","WipDisconnected","WipConnecting","WipNegotiating","WipConnected","WipUndefined",
"7","8","9",
"10","11","12","13","14","15","16","17","18","19",
"20","21","22","23","24","25","26","27","28","29",
"30","31","32","33","34","35","36","37","38","39",
"40","41","42","43","44","45","46","47","48","49"

};


#ifdef _WODVPNLIB
long raw_Connected(void *wodVPN, char * PeerID, char * IP, long Port)
{
	void *tag = NULL;
	WODVPNCOMLib::VPN_GetTag(wodVPN, &tag);
	CUser *me = (CUser *)tag;
#else
STDMETHODIMP CUser::raw_Connected(WODVPNCOMLib::IwodVPNCom * Owner, BSTR PeerID, BSTR IP, LONG Port)
{
	CUser *me = (CUser *)this;
#endif
	me->m_RemoteAddr = IP;
	me->m_RemotePort = Port;
	
	if (me->m_WippienState != WipConnected)
	{
		me->KillTimer(2); //blinker
		int oldst = me->m_WippienState;
		me->m_WippienState = WipConnected;
		me->DumpToFile("WippienState set to %s\r\n", WippienStateString[me->m_WippienState]);
		if (_SDK && oldst != me->m_WippienState)
		{
			Buffer b;
			b.PutCString(me->m_JID);
			b.PutInt((int)me->m_WippienState);
			b.PutInt(oldst);
			if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_USERVPNCHANGE, b.Ptr(), b.Len()))
				return E_FAIL;
		}
		me->SetSubtext();
	}	
	return S_OK;
}


#ifdef _WODVPNLIB
long raw_Disconnected(void *wodVPN, long ErrorCode, char * ErrorText)
{
	void *tag = NULL;
	WODVPNCOMLib::VPN_GetTag(wodVPN, &tag);
	CUser *me = (CUser *)tag;;
#else
STDMETHODIMP CUser::raw_Disconnected(WODVPNCOMLib::IwodVPNCom * Owner, LONG ErrorCode, BSTR ErrorText)
{
	CUser *me = (CUser *)this;
#endif

	// preserve Wippien state
	me->ReInit(TRUE);
//	me->m_IsAway = FALSE;
	me->SetSubtext();
	me->m_Changed = TRUE;
	_MainDlg.m_UserList.PostMessage(WM_REFRESH, 0, (LPARAM)me);

	return S_OK;
}

#ifdef _WODVPNLIB
long raw_SearchDone(void *wodVPN, char * IP, long Port, long ErrorCode, char * ErrorText)
{
	void *tag = NULL;
	WODVPNCOMLib::VPN_GetTag(wodVPN, &tag);
	CUser *me = (CUser *)tag;;
#else
STDMETHODIMP CUser::raw_SearchDone(WODVPNCOMLib::IwodVPNCom * Owner, BSTR IP, LONG Port, LONG ErrorCode, BSTR ErrorText)
{
	CUser *me = (CUser *)this;
#endif

	if (!ErrorCode)
	{
#ifdef _WODVPNLIB
		WODVPNCOMLib::VPN_Connect(wodVPN, IP, Port);
#else
		m_wodVPN->Connect(IP, Port);
#endif
	}	
	else
	{
		me->m_MyMediatorChoice = (-1); // pick different mediator next time, ok?
		me->m_MyMediatorOffer[0] = 0;
		me->m_WippienState = WipWaitingInitRequest;
		me->m_RemoteWippienState = WipWaitingInitRequest;
		me->DumpToFile("RemoteWippienState set to %s\r\n", WippienStateString[me->m_RemoteWippienState]);
		me->DumpToFile("WippienState set to %s\r\n", WippienStateString[me->m_WippienState]);
	}
	me->SetSubtext();

	return S_OK;
}

#ifdef _WODVPNLIB
long raw_IncomingData(void *wodVPN, void *data, int count)
{
	void *tag = NULL;
	WODVPNCOMLib::VPN_GetTag(wodVPN, &tag);
	CUser *me = (CUser *)tag;;
#else
STDMETHODIMP CUser::raw_IncomingData(WODVPNCOMLib::IwodVPNCom * Owner, VARIANT Data)
{
	CUser *me = (CUser *)this;
#endif

	if (me->m_Block)
		return E_FAIL;
	if (_Settings.IsHiddenContact(me->m_JID))
		return E_FAIL;
	
	if (me->m_WippienState != WipConnected)
	{
		ATLTRACE("Wippien state invalid!!\r\n");
		return E_FAIL;
	}	

#ifndef _WODVPNLIB

	if (!Data.parray)
	{
//		MessageBeep(-1);
		return E_FAIL;
	}

	SAFEARRAY *psa = Data.parray;
	char HUGEP *data;
	unsigned long ubound, lbound;
	SafeArrayGetLBound(psa,1,(long *)&lbound);
	SafeArrayGetUBound(psa,1,(long *)&ubound);
	unsigned long count = ubound-lbound+1;
	SafeArrayAccessData(psa, (void HUGEP**) &data);

#endif
	if (count>0)
	{		
//		ATLTRACE("Got %d bytes from %s\r\n", count, me->m_JID);		
		me->ProcessNetworkData((char *)data, count);
	}
#ifndef _WODVPNLIB
	SafeArrayUnaccessData(psa);
#endif
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUser::CUser()
{
	InitializeCriticalSection(&m_CritCS);
#ifndef _WODVPNLIB
	m_wodVPN.CoCreateInstance(__uuidof(WODVPNCOMLib::wodVPNCom));

#ifdef WODVPN_LICENSE_KEY
	CComBSTR blic(WODVPN_LICENSE_KEY);
	m_wodVPN->put_LicenseKey(blic);
#endif
	m_wodVPN->putref_Notification(this);
	m_dwRef = 0;
#else
	memset(&m_VPNEvents, 0, sizeof(m_VPNEvents));
	m_VPNEvents.Connected = raw_Connected;
	m_VPNEvents.Disconnected = raw_Disconnected;
	m_VPNEvents.SearchDone = raw_SearchDone;
	m_VPNEvents.IncomingData = raw_IncomingData;

	m_wodVPN = WODVPNCOMLib::_VPN_Create(&m_VPNEvents);
	WODVPNCOMLib::VPN_SetTag(m_wodVPN, this);
#endif
	m_BlinkConnectingCounter = 0;
	m_BlinkTimerCounter = 0;
	m_TreeItem = 0;
	m_Changed = TRUE;
	m_ChangeNotify = FALSE;
	m_Online = FALSE;
	m_RSA = NULL;
	m_Image = NULL;
	m_GotVCard = 0;
	time_t now;
	time( &now );
	m_LastOnline = 0;
	m_VoiceChatActive = FALSE;

	memset(&m_ChatWindowRect, 0, sizeof(m_ChatWindowRect));
	memset(m_VisibleName, 0, sizeof(m_VisibleName));
	memset(m_Resource, 0, sizeof(m_Resource));
	memset(m_SubText, 0, sizeof(m_SubText));
	memset(m_IPText, 0, sizeof(m_IPText));
	memset(m_Group, 0, sizeof(m_Group));
	memset(m_JID, 0, sizeof(m_JID));
	memset(m_StatusText, 0, sizeof(m_StatusText));
	memset(m_Email, 0, sizeof(m_Email));
	m_Block = FALSE;
	m_SettingsContactsDlg = NULL;
	m_Hidden = FALSE;

	m_MessageWin = NULL;

//	m_HisDHCPAddress = 0;
//	m_HisDHCPAddressOffset = 0;
	memset(&m_MAC, 0, sizeof(m_MAC));
//	InitializeCriticalSection(&m_OutCS);

	// populate allowed interfaces struct
	m_AllowedRemoteAny = m_AllowedRemoteMediator = TRUE;
	m_HisVirtualIP = 0;

	m_IsUsingWippien = NULL;
	m_MyRandom = m_HisRandom = 0;
	ReInit(TRUE);
	m_MTU = 0;
	m_DetectMTU = NULL;

	m_RemotePort = 0;

	for (int i = 0; i < 16; i++)
		m_MyKey[i] = rand();


	m_ChatRoomPtr = NULL;
	memset(m_MyMediatorOffer, 0, sizeof(m_MyMediatorOffer));
	m_MyMediatorPort = 0;

	CSettings::LinkMediatorStruct *st = _Settings.m_LinkMediators[0];
	if (st)
	{
		strcpy(m_HisMediatorOffer,st->Host);
		m_HisMediatorPort = st->Port;
		
	}
	m_MyMediatorChoice = (-1);
	m_LastSent = m_LastReceive = 0;
	m_TotalReceived = m_TotalSent = 0;
	m_IsAway = FALSE;
	m_IsAlienWippien = FALSE;
	m_LastResource[0] = 0;
	SetDebugLogFile();
	m_DisconnectedTimer = 0;
}

CUser::~CUser()
{
	if (_MainDlg.IsWindow())
	{
		_MainDlg.DisableVoiceChat(this);
		HTREEITEM show_item = _MainDlg.m_UserList.GetFirstVisibleItem();
		if (show_item)
		{
			EnterCriticalSection(&m_UserCS);
			do
			{
				CUser *user = (CUser *)_MainDlg.m_UserList.GetItemData(show_item);
				if (user == this)
				{
					_MainDlg.m_UserList.SendMessage(TVM_DELETEITEM,0,(LPARAM)show_item);
					break;
				} 

				show_item = _MainDlg.m_UserList.GetNextVisibleItem(show_item);
			} while (show_item);
			LeaveCriticalSection(&m_UserCS);
		} 
	}
	ReInit(TRUE);

//	ATLTRACE("Destroying user\r\n");
	
	if (m_RSA)
		RSA_free(m_RSA);
	if (m_Image)
		delete m_Image;

	if (m_MessageWin)
	{
		if (m_MessageWin->m_hWnd)
			::DestroyWindow(m_MessageWin->m_hWnd);
		delete m_MessageWin;
	}

	if (m_SettingsContactsDlg)
	{
		if (m_SettingsContactsDlg->IsWindow())
			m_SettingsContactsDlg->DestroyWindow();
		
		delete m_SettingsContactsDlg;
	}
	while (m_AllowedRemoteIPs.size())
	{
		IPAddressConnectionStruct *ips = (IPAddressConnectionStruct *)m_AllowedRemoteIPs[0];
		delete ips;
		m_AllowedRemoteIPs.erase(m_AllowedRemoteIPs.begin());
	}

	m_dwRef++;
#ifndef _WODVPNLIB
	m_wodVPN->raw_Stop();
	m_wodVPN->putref_Notification(NULL);
	m_wodVPN.Release();
#else
	WODVPNCOMLib::VPN_Stop(m_wodVPN);
	WODVPNCOMLib::_VPN_Destroy(m_wodVPN);
#endif
	DeleteCriticalSection(&m_CritCS);
}

void CUser::ReInit(BOOL WithDirect)
{
	m_VoiceChatActive = FALSE;
	
	m_WaveHdrDataSet = FALSE;
	int i;
	for (i=0;i<SPEEX_FRAME_SIZE;i++)
		m_WaveHdrData[i] = 0;

	if (_MainDlg.IsWindow())
		_MainDlg.DisableVoiceChat(this);
	SetTimer(1000, 8);
	m_MyRandom = 0;
	while (!m_MyRandom)
	{
		m_MyRandom = (rand()%1024)+1;
	}
	if (m_WippienState != WipConnected)
		m_IsUsingWippien = FALSE;

	SetDebugLogFile();
	if (WithDirect)
	{
		int oldst = m_WippienState;
		KillTimer(1);
		KillTimer(2);

		m_MyMediatorOffer[0] = 0;
		m_WippienState = WipWaitingInitRequest;
		m_RemoteWippienState = WipWaitingInitRequest;
		DumpToFile("RemoteWippienState set to %s\r\n", WippienStateString[m_RemoteWippienState]);
		DumpToFile("WippienState set to %s\r\n", WippienStateString[m_WippienState]);
		EnterCriticalSection(&m_CritCS);
#ifndef _WODVPNLIB
		m_wodVPN->raw_Stop();
#else
		WODVPNCOMLib::VPN_Stop(m_wodVPN);
#endif
		LeaveCriticalSection(&m_CritCS);

		memset(&m_RemoteAddr, 0, sizeof(m_RemoteAddr));
	

		if (_SDK && oldst != m_WippienState)
		{
			Buffer b;
			b.PutCString(m_JID);
			b.PutInt((int)m_WippienState);
			b.PutInt(oldst);
			if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_USERVPNCHANGE, b.Ptr(), b.Len()))
				return;
		}
	}
//	m_IsUsingWippien = FALSE;
	m_BlinkConnectingCounter = 0;
	m_BlinkTimerCounter = 0;

	m_MyMediatorChoice = (-1);
	m_MTU = 0;
	m_DetectMTU = NULL;
//	ATLTRACE("2Set %s away to FALSE\r\n", m_JID);
//	m_IsAway = FALSE;
	SetSubtext();
	m_Hidden = FALSE;
}


/*void CUser::SaveRemoteWippienInfo(Buffer *data)
{
	m_RemoteIPList.Clear();
	if (m_RSA)
		RSA_free(m_RSA);
	m_RSA = RSA_new();

	m_RSA->e = BN_new();
	data->GetBignum2(m_RSA->e);
	m_RSA->n = BN_new();
	data->GetBignum2(m_RSA->n);

	m_RemotePort = data->GetInt();

	while (data->Len() >= 4)
	{
		unsigned long y = data->GetInt();
		BOOL found = FALSE;
		for (int x=0;!found && x<m_AllowedRemoteIPs.size();x++)
		{
			IPAddressConnectionStruct *ips = (IPAddressConnectionStruct *)m_AllowedRemoteIPs[x];
			if (ips->Address == y)
				found = TRUE;
		}
		if (!found)
		{
			IPAddressConnectionStruct *ips = new IPAddressConnectionStruct();
			ips->Allowed = m_AllowedRemoteAny;
			ips->Address = y;
			ips->Ignored = TRUE;
			m_AllowedRemoteIPs.push_back(ips);
		}

		m_RemoteIPList.PutInt(y);
	}


	m_Changed = TRUE;
	SetSubtext();
	_MainDlg.m_UserList.PostMessage(WM_REFRESH, NULL, 0);


	// if user looks at contacts setup
	if (m_SettingsContactsDlg)
	{
		CSettingsDlg::CSettingsUser4 *us4 = (CSettingsDlg::CSettingsUser4 *)m_SettingsContactsDlg->m_Dialogs[3];
		us4->InitData();
	}
}*/

BOOL CUser::ExchangeWippienDetails(void)
{
	if (m_WippienState >= WipDisconnected)
		return TRUE;

	SetTimer(100, 3);

	// still connecting...
	return FALSE;
}

void CUser::SendConnectionRequest(BOOL Notify)
{
	if (m_Block)
		return;

	if (!ExchangeWippienDetails())
		return;

	EnterCriticalSection(&m_CritCS);
#ifdef _WODVPNLIB
	WODVPNCOMLib::StatesEnum st;
	WODVPNCOMLib::VPN_GetState(m_wodVPN, &st);

#else
	WODVPNCOMLib::StatesEnum st = m_wodVPN->State;
#endif
	LeaveCriticalSection(&m_CritCS);

	if (st <= (WODVPNCOMLib::StatesEnum)1/*::Listening*/ /*&& _Settings.m_MediatorAddr.Length()*/)
	{
		if (m_WippienState < WipConnecting)
		{
			int oldst = m_WippienState;
			m_WippienState = WipConnecting;
			DumpToFile("WippienState set to %s\r\n", WippienStateString[m_WippienState]);
			if (_SDK)
			{
				Buffer b;
				b.PutCString(m_JID);
				b.PutInt((int)m_WippienState);
				b.PutInt(oldst);
				if (!_SDK->FireEvent(WM_WIPPIEN_EVENT_USERVPNCHANGE, b.Ptr(), b.Len()))
					return;
			}

			m_BlinkConnectingCounter = 0;
			SetTimer(1000, 1);
			SetTimer(100, 2);

			if (Notify)
			{
				SetTimer(200, 9); // notifyconnect
			}


			EnterCriticalSection(&m_CritCS);
#ifdef _WODVPNLIB
			WODVPNCOMLib::VPN_Stop(m_wodVPN);
#else
			m_wodVPN->raw_Stop();
#endif

			CComBSTR myjid, hisjid;
			CComBSTR myid;
			if (m_IsAlienWippien)
			{
				CComBSTR2 j = _Settings.m_JID;
				char *j1 = j.ToString();
				char *j2 = strchr(j1, '@');
				if (j2)
					*j2 = 0;
				j2 = strchr(j1, ' ');
				if (j2)
					*j2 = 0;
				myjid = j1;

				CComBSTR2 l = this->m_VisibleName;
				j1 = l.ToString();
				j2 = strchr(j1, '@');
				if (j2)
					*j2 = 0;
				j2 = strchr(j1, ' ');
				if (j2)
					*j2 = 0;
				hisjid = j1;

			}
			else
			{
				if (m_ChatRoomPtr)
				{
					myjid = m_ChatRoomPtr->m_JID;
					myjid += "/";
					myjid += m_ChatRoomPtr->m_Nick;

					hisjid = m_ChatRoomPtr->m_JID;
					hisjid += "/";
					hisjid += m_Resource;
				}
				else
				{
					char randombuff[128];
					myjid = _Settings.m_JID;	
					//DumpToFile("Preparing VPN, myrandom=%d, hisrandom=%d", m_MyRandom, m_HisRandom);

					if (m_MyRandom && m_HisRandom)
					{
						sprintf(randombuff, "_%s_%u", ((CComBSTR2)_Settings.m_Resource).ToString(), m_MyRandom);
						myjid += randombuff;
					}
					hisjid = m_JID;
					if (m_MyRandom && m_HisRandom)
					{
						sprintf(randombuff, "_%s_%u", ((CComBSTR2)m_Resource).ToString(), m_HisRandom);
						hisjid += randombuff;
					}
				}
			}

			myid += myjid;
			myid += "_";
			myid += hisjid;

			myid.ToLower();
			CComBSTR hisid;

			hisid += hisjid;
			hisid += "_";
			hisid += myjid;
			hisid.ToLower();
	
#ifdef _WODVPNLIB
			CComBSTR2 myid2 = myid;
			WODVPNCOMLib::VPN_SetMyID(m_wodVPN, myid2.ToString());
#else	
			m_wodVPN->put_MyID(/*_Settings.m_JID*/myid);
#endif

			VARIANT varhost, varport;
			varhost.vt = VT_BSTR;
			USES_CONVERSION;
			//varhost.bstrVal = _Settings.m_MediatorAddr;
			CComBSTR mlm;
			mlm = m_HisMediatorOffer;
			varhost.bstrVal = mlm;
			varport.vt = VT_I4;
			varport.lVal = m_HisMediatorPort;
			VARIANT varempty;
			varempty.vt = VT_ERROR;

#ifdef _WODVPNLIB
			WODVPNCOMLib::VPN_SetRetryCount(m_wodVPN, 30);
			WODVPNCOMLib::VPN_SetRetryWait(m_wodVPN, 500);
			WODVPNCOMLib::VPN_SetThreads(m_wodVPN, TRUE);
			WODVPNCOMLib::VPN_SetTimeout(m_wodVPN, 15);
			long port = 0;
			if (_Settings.m_VPNSocketDebugFolder.Length())
			{
				SetDebugLogFile();
				if (m_Log.Length())
					WODVPNCOMLib::VPN_SetDebugFile(m_wodVPN, ((CComBSTR2)m_Log).ToString());
			}

			char intbuff[1024] = {0};
			int ilen = sizeof(intbuff);
			intbuff[0] = ' ';
			ilen--;
			WODVPNCOMLib::VPN_GetBindIP(m_wodVPN, intbuff, &ilen);
			if (ilen)
			{
				char net[1024];
				struct in_addr ina;
				ina.s_addr = _Settings.m_MyLastNetwork;
				strcpy(net, inet_ntoa(ina));
				// does it contain my IP inside
				char *c = strstr(intbuff, net);
				if (c)
				{
					char *d = c + strlen(net);
					if (*d == ' ')
						d++;
					MoveMemory(c, d, strlen(d)+1);
					WODVPNCOMLib::VPN_SetBindIP(m_wodVPN, intbuff);
				}
			}

			ATLTRACE("Using mediator %s for %s/%s\r\n", m_HisMediatorOffer, m_JID, m_Resource);
			WODVPNCOMLib::VPN_Start(m_wodVPN, &port);
			CComBSTR2 hisid2 = hisid;
			WODVPNCOMLib::VPN_Search(m_wodVPN, (WODVPNCOMLib::SearchEnum)0, hisid2.ToString(), varhost, varport, varempty);
#else
			m_wodVPN->RetryCount = 30;
			m_wodVPN->RetryWait = 500;
			m_wodVPN->Threads = TRUE;
			m_wodVPN->Timeout = 8;
			m_wodVPN->Start(0);
			m_wodVPN->raw_Search((WODVPNCOMLib::SearchEnum)0, hisid, varhost, varport, varempty);
#endif				
			LeaveCriticalSection(&m_CritCS);
		}  
		SetSubtext();
	}	

	_MainDlg.m_UserList.Invalidate();
}

void CUser::ProcessNetworkData(char *buffreal, int len)
{
	m_LastReceive = GetTickCount();
	m_TotalReceived += len;
	_Ethernet.m_Activity = TRUE;
	_Ethernet.InjectPacket(buffreal, len);
}

void CUser::FdMTU(int MTU)
{
	ATLTRACE("Got MTU %d for %s\r\n", MTU, m_JID);
	if (MTU)
	{
		if (MTU<100)
			return;

		if (MTU<576)
			MTU = 576;

		m_MTU = MTU;

		int minmtu = m_MTU;
		// let's see if we need to define new mtu in registry
		for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
		{
			CUser *us = (CUser *)_MainDlg.m_UserList.m_Users[i];
			if (us->m_MTU)
			{
				if (us->m_MTU < minmtu)
					minmtu = us->m_MTU;
			}
		}

//		if (minmtu < _Settings.m_FixedMTUNum|| !_Settings.m_FixedMTUNum)
		{
			_Settings.m_FixedMTUNum = minmtu;
		}
	}
	if (m_DetectMTU)
		m_DetectMTU = NULL;
}

void CUser::FdTimer(int TimerID)
{
	if (TimerID == 1)
	{
		if (m_WippienState == WipConnected)
		{
			if (!_Settings.m_FixedMTU && !m_MTU /*&& m_LastSend>3 && m_LastReceive>3*/)
			{
				// let's initialize MTU test
				if (!m_DetectMTU)
				{
					ATLTRACE("Calculating MTU for %s\r\n", m_JID);
					m_DetectMTU = new CDetectMTU();
					m_DetectMTU->CalcMTU(this);
				}
			}
		}
	}
	else
	if (TimerID==2)
	{
		m_BlinkConnectingCounter++;
		if (IsWindow(_MainDlg.m_UserList))
			_MainDlg.m_UserList.Invalidate();
	}
	else
	if (TimerID==3 && _Ethernet.m_Available)
	{
		KillTimer(3);
		if (!m_RSA || m_RemoteWippienState < WipWaitingInitResponse || m_WippienState < WipWaitingInitResponse)
		{
			if (_Settings.m_MyLastNetwork)
			{
				Buffer b;
				b.PutInt(_Settings.m_MyLastNetwork);
				b.Append((char *)_Settings.m_MAC, 6);
				_Settings.KeyToBlob(&b, FALSE);
				b.PutChar((char)m_WippienState);
				// let's add our mediators
				Buffer b1;
				int total = 0;
				for (int i=0;i<255 && i<_Settings.m_LinkMediators.size();i++)
				{
					CSettings::LinkMediatorStruct *st = (CSettings::LinkMediatorStruct *)_Settings.m_LinkMediators[i];
					if (st->Permanent)
					{
						total++;
						b1.PutCString(st->Host);
						b1.PutInt(st->Port);
					}
				}
				if (b1.Len())
				{
					b.PutString(b1.Ptr(), b1.Len());

					if (!*m_MyMediatorOffer)
					{
						// there are mediators to select, pick one
						if (m_MyMediatorChoice<0)
							m_MyMediatorChoice = rand()%total;
						int j = 0;
						for (int i=0;i<255 && i<_Settings.m_LinkMediators.size();i++)
						{
							CSettings::LinkMediatorStruct *st = (CSettings::LinkMediatorStruct *)_Settings.m_LinkMediators[i];
							if (st->Permanent)
							{
								strcpy(m_MyMediatorOffer, st->Host);
								m_MyMediatorPort = st->Port;
								if (j++ == m_MyMediatorChoice)
								{
									ATLTRACE("Offering mediator %s to %s/%s\r\n", st->Host, m_JID, m_Resource);
									break;
								}
							}
						}
					}
				}
				b.PutInt(m_MyRandom); // add random value to append to VPN request

				if (m_IsUsingWippien)
				{
					DumpToFile("Sending WIPPIENINITREQUEST, WippienState %s, RemoteWippienState %s\r\n", WippienStateString[m_WippienState], WippienStateString[m_RemoteWippienState]);
					_Jabber->ExchangeWippienDetails(this, (char *)WIPPIENINITREQUEST, &b);
				}
			}
			return;
		}

		if (m_RemoteWippienState < WipDisconnected || m_WippienState < WipDisconnected)
		{
			if (_Settings.m_MyLastNetwork && m_RSA)
			{
//				m_IsAway = FALSE;
				SetSubtext();
				Buffer b;

				char src[128 - RSA_PKCS1_PADDING_SIZE], dst[128];
				memcpy(src + 24, m_MyKey, 16); // this is stupid, ok?...
				RSA_public_encrypt(128 - RSA_PKCS1_PADDING_SIZE, (unsigned char *)src, (unsigned char *)dst, m_RSA, RSA_PKCS1_PADDING);
				b.Append(dst, 128);
				b.PutChar((char)m_WippienState);
				b.PutCString(m_MyMediatorOffer);
				b.PutInt(m_MyMediatorPort);
				b.PutInt(m_MyMediatorChoice);

				if (m_IsUsingWippien && !m_Block)
				{
					DumpToFile("Sending WIPPIENINITRESPONSE, WippienState %s, RemoteWippienState %s\r\n", WippienStateString[m_WippienState], WippienStateString[m_RemoteWippienState]);
					_Jabber->ExchangeWippienDetails(this , (char *)WIPPIENINITRESPONSE, &b);
				}
			}
			return;
		}

		if (m_RemoteWippienState < WipConnecting || m_WippienState < WipConnecting)
		{
			KillTimer(9);
			NotifyConnect();
			return;
		}

	}
	else
	if (TimerID == 8)
	{
		if (m_IsUsingWippien && !m_Block && m_HisVirtualIP)
		{
			switch (m_WippienState)
			{
				case WipWaitingInitRequest:
				case WipDisconnected:
					m_DisconnectedTimer++;
					if (m_DisconnectedTimer>3)
					{
						m_DisconnectedTimer = 0;
						if (_Settings.m_AutoConnectVPNOnNetwork)
							_MainDlg.m_UserList.ConnectIfPossible(this, TRUE);
					}
					break;

				default:
					m_DisconnectedTimer = 0;

			}
		}
	}	
	else
	if (TimerID == 9)
	{
		KillTimer(9);
		NotifyConnect();
	}	
}

BOOL CUser::IsIPAllowed(unsigned long IP)
{
	BOOL allow = m_AllowedRemoteAny;

	for (int y=0;y<m_AllowedRemoteIPs.size();y++)
	{
		IPAddressConnectionStruct *ips = (IPAddressConnectionStruct *)m_AllowedRemoteIPs[y];
		if (ips->Address == IP && !ips->Ignored)
		{
			allow = ips->Allowed;
			break;
		}
	}

	return allow;
}

void CUser::DumpToFileFixed(char *text)
{
	char buff[16384];
	CComBSTR2 file = m_Log;
	int handle;
	
	if (text && text[0])
	{
		
		handle = open(file.ToString(), O_BINARY | O_WRONLY | O_APPEND | O_CREAT, S_IREAD | S_IWRITE);
		if (handle!=(-1))
		{
			
			struct tm *newtime;
			time_t aclock;
			
			time( &aclock );                 
			newtime = localtime( &aclock );
			
			strcpy(buff, asctime(newtime));
			char *a = buff;
			while (*a)
			{
				if (*a<31)
					*a = 0;
				else
					a++;
			}			
			
			strcat(buff, "-> ");
			write(handle, buff, strlen(buff));						
			write(handle, text, strlen(text));
			close(handle);
		}
	}
}

void CUser::DumpToFile(char *text,...)
{
	char buff[16384];
	va_list marker;
	va_start(marker,text);
	wvsprintf(buff,text,marker);
	va_end(marker);
	DumpToFileFixed(buff);
	
}

void CUser::SetDebugLogFile(void)
{
	if (_Settings.m_VPNSocketDebugFolder.Length())
	{
		CComBSTR2 vs = _Settings.m_VPNSocketDebugFolder;
		char *v2 = vs.ToString();
		int v3 = strlen(v2);
		if (v3)
		{
			if (v2[v3-1] == '\\')
				v2[v3-1] = 0;
		}
		char vsbuff[1024];
		sprintf(vsbuff, "%s\\%s_%s", v2, m_JID, m_Resource);
		v3 = 0;
		while (vsbuff[v3])
		{
			if (vsbuff[v3]=='/')
				vsbuff[v3] = '_';
			v3++;
		}
		m_Log = vsbuff;
	}
	else
		m_Log.Empty();
}

void CUser::SetSubtext(void)
{
	struct in_addr in;
	in.S_un.S_addr  = m_HisVirtualIP;

	BOOL ison = m_Online;
	if (!ison && m_WippienState==WipConnected)
		ison = TRUE;
		
	if (ison /*&& m_HisVirtualIP*/)
	{
		if (strcmp(m_StatusText, m_SubText) && IsMsgWindowOpen())
		{
			if (!strcmp(m_StatusText, _Settings.Translate("Online")) && !strcmp(m_StatusText, _Settings.Translate("Offline")))
			{
				// get our status
				WODXMPPCOMLib::StatusEnum stat = (WODXMPPCOMLib::StatusEnum)0;
#ifndef _WODXMPPLIB					
				if (_Jabber)
					_Jabber->m_Jabb->get_Status(&stat);
#else
				if (_Jabber)					
					WODXMPPCOMLib::XMPP_GetStatus(_Jabber->m_Jabb, &stat);
#endif
						
				if (stat)
				{
					CComBSTR p = _Settings.Translate("Status changed: ");
					p += m_StatusText;
					CComBSTR2 p2 = p;
					PrintMsgWindow(TRUE, p2.ToString(), NULL);
				}
			}
		}
		if (m_Resource[0])
		{
			if (strlen(m_Resource)>7)
			{
				sprintf(m_SubText, "       .. %s", m_StatusText);
				memcpy(m_SubText, m_Resource, 7);
			}
			else
				sprintf(m_SubText, "%s %s", m_Resource, m_StatusText);
		}
		else
			sprintf(m_SubText, "%s", m_StatusText);
		
		m_bstrSubText.FromUTF8String(m_SubText);
	}
	else
	{
		char howlong[1024];
		if (_Settings.m_ShowContactLastOnline)
		{
			if (m_LastOnline)
			{
				time_t now;
				time(&now);
				unsigned long t;
				t = (now - m_LastOnline) / 86400L;//(60 *60 *24);
				if (t>0)
					sprintf(howlong, "%d %s", t, t>1?_Settings.Translate("days"):_Settings.Translate("day"));
				else
				{
					t = (now-m_LastOnline) / 3600L;//60 * 60;
					if (t>0)
						sprintf(howlong, "%d %s", t, t>1?_Settings.Translate("hours"):_Settings.Translate("hour"));
					else
					{
						t = (now-m_LastOnline)/60;
						if (!t)
							t = 1;
						sprintf(howlong, "%d %s", t, t>1?_Settings.Translate("minutes"):_Settings.Translate("minute"));
					}
				}

				sprintf(m_SubText, "%s %s", m_StatusText, howlong);
			}
			else
				strcpy(m_SubText, _Settings.Translate("Last seen: never"));
		}
		else
			strcpy(m_SubText, m_StatusText);


		m_bstrSubText.FromUTF8String(m_SubText);
	}
	if (ison && m_HisVirtualIP)
	{
//		sprintf(m_IPText, " (%s)", inet_ntoa(in));
		sprintf(m_IPText, "(%s)", inet_ntoa(in));
	}
	else
		m_IPText[0] = 0;
	
	if (IsWindow(_MainDlg.m_UserList))
		_MainDlg.m_UserList.Invalidate();
}

BOOL CUser::SendNetworkPacket(char *data, int len)
{
	if (m_Block)
		return FALSE;


	switch (m_WippienState)
	{
		case WipDisconnected:
		case WipWaitingInitRequest:
			if (_Settings.m_AutoConnectVPNOnNetwork)
				_MainDlg.m_UserList.ConnectIfPossible(this, TRUE);
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
					if (ip->saddr != _Settings.m_MyLastNetwork  && ip->saddr)
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

				m_LastSent = GetTickCount();
				m_TotalSent += len;
				_Ethernet.m_Activity = TRUE;
				EnterCriticalSection(&m_CritCS);
#ifdef _WODVPNLIB
				if (m_wodVPN)
					WODVPNCOMLib::VPN_SendData(m_wodVPN, data, len, FALSE);
#else
				SAFEARRAY * psaiNew; 
				SAFEARRAYBOUND aDim[1]; 
				aDim[0].lLbound = 1; 
				aDim[0].cElements = len;

				psaiNew = SafeArrayCreate(VT_UI1, 1, aDim);
				if (psaiNew)
				{
					char HUGEP *arraydata;
					SafeArrayAccessData(psaiNew, (void HUGEP**) &arraydata);
					memcpy(arraydata, data, len);
					SafeArrayUnaccessData(psaiNew);

					VARIANT v;
					v.vt = VT_ARRAY | VT_UI1;
					v.parray = psaiNew;
					m_wodVPN->raw_SendData(v, FALSE); // not reliable
				}
#endif
				LeaveCriticalSection(&m_CritCS);
				break;
	}
	return 0;
}

BOOL CUser::IsMsgWindowOpen(void)
{
	if (m_MessageWin && m_MessageWin->m_hWnd && IsWindow(m_MessageWin->m_hWnd))
		return TRUE;

	return FALSE;
}

void CUser::OpenMsgWindow(BOOL WithFocus)
{
	if (!m_MessageWin)
	{
		m_MessageWin = new CMsgWin(this);
	}
	if (!m_MessageWin->m_hWnd)
	{
		LoadUserImage(&m_MessageWin->m_HumanHead);
		m_MessageWin->Create(NULL);
		m_MessageWin->Show();
	}
	if (WithFocus)
	{
		m_MessageWin->SetTitle();
		SetActiveWindow(m_MessageWin->m_hWnd);
	
		if (m_MessageWin->m_InputBox.IsWindow())
			m_MessageWin->m_InputBox.SetFocus();
	}
}

void CUser::NotifyUserIsTyping(BOOL IsTyping)
{
	if (m_MessageWin && m_MessageWin->IsWindow())
	{
		ShowWindow(GetDlgItem(m_MessageWin->m_hWnd, IDC_ISTYPING),  IsTyping?SW_SHOW:SW_HIDE);
	}
}

void CUser::CloseMsgWindow(void)
{
	if (m_MessageWin)
	{
		if (::IsWindow(m_MessageWin->m_hWnd))
			::DestroyWindow(m_MessageWin->m_hWnd);
		delete m_MessageWin;
		m_MessageWin = NULL;
	}
}

void CUser::PrintMsgWindow(BOOL IsSystem, char *Text, char *Html)
{
	OpenMsgWindow(FALSE);
	if (m_MessageWin)
	{
		strcpy(m_MessageWin->m_JID, m_JID);
		if (m_Resource[0] && !strchr(m_MessageWin->m_JID, '/'))
		{
			strcat(m_MessageWin->m_JID, "/");
			strcat(m_MessageWin->m_JID, m_Resource);
		}
		m_MessageWin->SetTitle();
	}

	if (!IsSystem)
		FlashWindow(m_MessageWin->m_hWnd, TRUE);

	m_MessageWin->Incoming(m_VisibleName, IsSystem, Text, Html);
}

Buffer *CUser::ExpandArgs(char *Text)
{
	Buffer in;
	in.Append(Text);
	in.Append("\0", 1);
	
	Buffer b;
	char *a = NULL;
	char *t = in.Ptr();
	
	a = strstr(t, "{contact_virtual_ip}");
	if (a)
	{
		in_addr ar;
		ar.S_un.S_addr = m_HisVirtualIP;
		b.Append(t, a-t);
		b.Append(inet_ntoa(ar));
		b.Append(a+20);
		b.Append("\0", 1);
		return ExpandArgs(b.Ptr());
	}
	else
	{
		a = strstr(t, "{my_virtual_ip}");
		if (a)
		{
			in_addr ar;
			ar.S_un.S_addr = _Settings.m_MyLastNetwork;
			b.Append(t, a-t);
			b.Append(inet_ntoa(ar));
			b.Append(a+15);
			b.Append("\0", 1);
			return ExpandArgs(b.Ptr());
		}
		else
		{
			a = strstr(t, "{contact_ip}");
			if (a)
			{
				b.Append(t, a-t);
				CComBSTR2 ra = m_RemoteAddr;
				b.Append(ra.ToString());
				b.Append(a+12);
				b.Append("\0", 1);
				return ExpandArgs(b.Ptr());
			}
			else
			{
				b.Append(in.Ptr(), in.Len());
				return ExpandSystemArgs(&b);
			}
		}
	}

}

Buffer *CUser::ExpandSystemArgs(Buffer *in)
{
	Buffer b;
	char *a = NULL;
	char *t = in->Ptr();

	a = strstr(t, "{windows}");
	if (a)
	{
		char buff[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_WINDOWS | CSIDL_FLAG_CREATE, NULL, 0, buff)))
		{				
			b.Append(t, a-t);
			b.Append(buff);
			b.Append(a+9);
			b.Append("\0", 1);
			return ExpandSystemArgs(&b);
		}
	}
	else
	{
		a = strstr(t, "{system32}");
		if (a)
		{
			char buff[MAX_PATH];
			if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM | CSIDL_FLAG_CREATE, NULL, 0, buff)))
			{
				b.Append(t, a-t);
				b.Append(buff);
				b.Append(a+10);
				b.Append("\0", 1);
				return ExpandSystemArgs(&b);
			}
		}
		else
		{
			a = strstr(t, "{program_files}");
			if (a)
			{
				char buff[MAX_PATH];
				if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES | CSIDL_FLAG_CREATE, NULL, 0, buff)))
				{
					b.Append(t, a-t);
					b.Append(buff);
					b.Append(a+15);
					b.Append("\0", 1);
					return ExpandSystemArgs(&b);
				}
			}
			else
			{
				a = strstr(t, "{appdata}");
				if (a)
				{
					char buff[MAX_PATH];
					if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, buff)))
					{
						b.Append(t, a-t);
						b.Append(buff);
						b.Append(a+9);
						b.Append("\0", 1);
						return ExpandSystemArgs(&b);
					}
				}
				else
				{
					a = strstr(t, "{wippien}");
					if (a)
					{
						char buff[MAX_PATH];
						if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, buff)))
						{
							b.Append(t, a-t);
							b.Append(_Settings.m_MyPath);
							b.Append(a+9);
							b.Append("\0", 1);
							return ExpandSystemArgs(&b);
						}
					}
				}
			}
		}
	}

	Buffer *o = new Buffer();
	o->Append(in->Ptr(), in->Len());
	return o;
}

BOOL CUser::LoadUserImage(CxImage *img)
{
	char buff[MAX_PATH*2];
	strcpy(buff, _Settings.m_UserImagePath);
	strcat(buff, m_JID); 
	strcat(buff, ".png");
	img->Clear();
	if (img->Load(buff, CXIMAGE_FORMAT_PNG))
		return TRUE;
	
	// otherwise get something from the resource
	int hm = (rand() % 61);
	HRSRC h = FindResource(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDB_HUMAN1 + hm), "PNG");
	DWORD rsize= SizeofResource(_Module.GetModuleInstance(),h);
	CxImage cx;
	if (rsize)
	{
		HGLOBAL hMem=::LoadResource(_Module.GetModuleInstance(),h);
		if (hMem)
		{
			char* lpVoid=(char*)LockResource(hMem);
			if (lpVoid)
			{
				CxMemFile fTmp((BYTE*)lpVoid,rsize);
				img->Decode(&fTmp, CXIMAGE_FORMAT_PNG);
				return SaveUserImage(*img);
			}
//			DeleteObject(hMem); // this does not need to be deleted
		}
	}


	return FALSE;
}


BOOL CUser::SaveUserImage(char *data, int len)
{
	char buff[MAX_PATH*2];
	strcpy(buff, _Settings.m_UserImagePath);
	strcat(buff, m_JID); 
	strcat(buff, ".png");

	int handle = open(buff, O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
	if (handle != (-1))
	{
		write(handle, data, len);
		close(handle);
		return TRUE;
	}
	return FALSE;
}

BOOL CUser::SaveUserImage(CxImage &img)
{
	char buff[MAX_PATH*2];
	strcpy(buff, _Settings.m_UserImagePath);
	strcat(buff, m_JID); 
	strcat(buff, ".png");
	return img.Save(buff,CXIMAGE_FORMAT_PNG);
}

void CUser::NotifyDisconnect(void)
{
	Buffer t;
//	Buffer out;
	t.PutChar((char)m_WippienState);
//	_Settings.ToHex(&t, &out);
//	out.Append("\0", 1);
	if (m_IsUsingWippien)
	{
		DumpToFile("Sending WIPPIENDISCONNECT, WippienState %s, RemoteWippienState %s\r\n", WippienStateString[m_WippienState], WippienStateString[m_RemoteWippienState]);
		_Jabber->ExchangeWippienDetails(this, (char *)WIPPIENDISCONNECT, &t);
	}


	BOOL iswip = m_IsUsingWippien;
	ReInit(TRUE);
	m_IsUsingWippien = iswip;
}

void CUser::NotifyConnect(void)
{	
	Buffer t;
//	Buffer out;
	t.PutChar((char)m_WippienState);
//	_Settings.ToHex(&t, &out);
//	out.Append("\0", 1);
	if (m_IsUsingWippien  && !m_Block)
	{
		DumpToFile("Sending WIPPIENCONNECT, WippienState %s, RemoteWippienState %s\r\n", WippienStateString[m_WippienState], WippienStateString[m_RemoteWippienState]);
		_Jabber->ExchangeWippienDetails(this, (char *)WIPPIENCONNECT, &t);
	}

}

void CUser::NotifyBlock(void)
{
	if (m_Block && _Jabber)
	{
		m_WippienState = WipDisconnected;
		DumpToFile("WippienState set to %s\r\n", WippienStateString[m_WippienState]);
		NotifyDisconnect();
	}
}