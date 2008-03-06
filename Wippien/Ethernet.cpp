// Ethernet.cpp: implementation of the CEthernet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ethernet.h"
#include "Settings.h"
#include "MainDlg.h"
#include <Iphlpapi.h>
#include "updatehandler.h"

unsigned char MAC_BROADCAST[6] = {255,255,255,255,255,255};
extern const char *hex;

extern CAppModule _Module;
extern CSettings _Settings;
extern CMainDlg _MainDlg;
extern CEthernet _Ethernet;
extern CUpdateHandler *_UpdateHandler;

const char * CONFIGURING_ADAPTER_TEXT = "Configuring Wippien network adapter...";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*typedef DWORD (WINAPI *SetAdapterIpAddressx)(char *,DWORD,DWORD,DWORD,DWORD); 
typedef DWORD (WINAPI *DhcpNotifyConfigChangex)(wchar_t*,wchar_t*,BOOL,DWORD,DWORD,DWORD,int); 

SetAdapterIpAddressx SetAdapterIpAddress; 
DhcpNotifyConfigChangex DhcpNotifyConfigChange; 
void ChangeIPAddr(char *id,DWORD ip,DWORD mask, DWORD gway) 
{ 
	SetAdapterIpAddress(id,0,ip,mask,gway); 
	wchar_t uid[256]={0}; 
	MultiByteToWideChar(CP_ACP,0,id,-1,uid,127); 
	DhcpNotifyConfigChange(0,uid,1,0,ip,mask,2); 
} 
*/

CEthernet::CEthernet()
{
	m_Available = FALSE;
	memcpy(&m_MAC, "\0\0\0\0\0\0", sizeof(m_MAC));
	m_AdapterHandle = INVALID_HANDLE_VALUE;
	ReadHandle = INVALID_HANDLE_VALUE;
	WriteHandle = INVALID_HANDLE_VALUE;
	DieHandle = INVALID_HANDLE_VALUE;

	memset(m_EthWriteBuff, 0, sizeof(m_EthWriteBuff));
	m_EthWriteEnd = m_EthWriteStart = 0;

		// init critsections
//	InitializeCriticalSection(&ReadCS);
//	InitializeCriticalSection(&WriteCS);

	hReadThread = INVALID_HANDLE_VALUE;
	hWriteThread = INVALID_HANDLE_VALUE;
	hSetupThread = INVALID_HANDLE_VALUE;
	//hProcThread = INVALID_HANDLE_VALUE;

	m_Alive = FALSE;
//	m_DHCP = NULL;
//	m_DHCPCounter = 0;
//	m_MTU = 1500;

//	m_Started = FALSE;

	m_Guid[0] = m_RegistryKey[0];
	m_FirewallRulesChanged = FALSE;
}

CEthernet::~CEthernet()
{
	// tell everyone to die
	SetEvent(DieHandle);
	
	m_Alive = FALSE;
	while (hReadThread != INVALID_HANDLE_VALUE || hWriteThread != INVALID_HANDLE_VALUE || hSetupThread != INVALID_HANDLE_VALUE)
		Sleep(100);
/*	
	// delete threads
	if (hReadThread != INVALID_HANDLE_VALUE)
		CloseHandle(hReadThread);

	if (hWriteThread != INVALID_HANDLE_VALUE)
		CloseHandle(hWriteThread);

	if (hProcThread != INVALID_HANDLE_VALUE)
		CloseHandle(hProcThread);
*/	
	// close network adapter
	if (m_AdapterHandle != INVALID_HANDLE_VALUE)
		CloseHandle(m_AdapterHandle);
	
	// delete event handles
	if (ReadHandle != INVALID_HANDLE_VALUE)
		CloseHandle(ReadHandle);
	
	if (WriteHandle != INVALID_HANDLE_VALUE)
		CloseHandle(WriteHandle);

	if (DieHandle != INVALID_HANDLE_VALUE)
		CloseHandle(DieHandle);
	
	// delete critical sections
//	DeleteCriticalSection(&ReadCS);
//	DeleteCriticalSection(&WriteCS);
}

void CEthernet::GetMac(MACADDR src, char dst[18])
{
//	static char *hex="0123456789abcdef";
	int i, j;

	dst[17] = 0;

	j = 0;
	for (i=0;i<6;i++)
	{
		if (i)
			dst[j++] = ':';
		dst[j++] = hex[src[i]/16];
		dst[j++] = hex[src[i]%16];
	}
	
}

VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
		if (_Ethernet.m_AdapterHandle == INVALID_HANDLE_VALUE)
			return;

		MessageBeep(-1);
		// otherwise...
///		if (Init())
//			KillTimer(NULL, 

}

BOOL CEthernet::Init(void)
{
	if (!GetAdapterGuid())
		return FALSE;

/*
	HINSTANCE lib=(HINSTANCE)LoadLibrary("iphlpapi.dll"); 
	SetAdapterIpAddress=(SetAdapterIpAddressx)GetProcAddress(lib,"SetAdapterIpAddress"); 
	lib=(HINSTANCE)LoadLibrary("dhcpcsvc.dll"); 
	DhcpNotifyConfigChange=(DhcpNotifyConfigChangex)GetProcAddress(lib,"DhcpNotifyConfigChange"); 

	ChangeIPAddr(m_Guid,inet_addr("192.168.4.10"),inet_addr("255.255.255.0"),inet_addr("192.168.4.10")); 
*/

	char bname[1024];
	sprintf(bname, "%s%s%s", USERMODEDEVICEDIR, m_Guid, WIPPSUFFIX);
	m_AdapterHandle  = CreateFile(bname,
		GENERIC_READ | GENERIC_WRITE,
		0, /* was: FILE_SHARE_READ */
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED,
		0
		);
	
	if (m_AdapterHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	m_Enabled = TRUE;

    unsigned long info[3] = { 0, 0, 0};
	unsigned long len;
    if (DeviceIoControl(m_AdapterHandle, WIPP_IOCTL_GET_VERSION, &info, sizeof(info), &info, sizeof(info), &len, NULL))
	{
		// if wippien version doesn't match, go out
		if (info[0] != WIPP_DRIVER_MAJOR_VERSION || info[1] != WIPP_DRIVER_MINOR_VERSION)
		{
			// close network adapter
			CloseHandle(m_AdapterHandle);
			m_AdapterHandle = INVALID_HANDLE_VALUE;
			return FALSE;
		}

		// get our MAC address
		if (DeviceIoControl(m_AdapterHandle, WIPP_IOCTL_GET_MAC, &m_MAC, sizeof(MACADDR), &m_MAC, sizeof(MACADDR), &len, NULL))
		{

			if (m_RegistryKey[0])
			{
				HKEY key1;
				if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_RegistryKey, 0, KEY_WRITE, &key1) == ERROR_SUCCESS)
				{
					char bf[18];
					memset(bf, 0, sizeof(bf));
					GetMac(m_MAC, bf);
					RegSetValueEx(key1, "MAC", NULL, REG_SZ, (unsigned char *)bf, 18);
					RegCloseKey(key1);
				}
			}

			// get MTU

//			if (DeviceIoControl(m_AdapterHandle, WIPP_IOCTL_GET_MTU, &m_MTU, sizeof(m_MTU), &m_MTU, sizeof(MACADDR), &len, NULL))
//			{
//				// cry...
//			}	

			
/*			// get interface index
			unsigned long index = 0;
			wchar_t wbuf[256];
			_snwprintf(wbuf, sizeof(wbuf), L"%S", bname);
			wbuf[sizeof(wbuf) - 1] = 0;
			if (GetAdapterIndex(wbuf, &index) == NO_ERROR) // todo ne radi
			{
				if (FlushIpNetTable(index) == NO_ERROR)
				{
					MessageBeep(-1);
				}
			}	
*/				
			
			// init events
			ReadHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
			WriteHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
			DieHandle = CreateEvent(NULL, TRUE, FALSE, NULL); // manual reset, so both waits can catch it
			
			
			m_Alive = TRUE;

			// ok, let's create three threads
//			DWORD ReadThreadId; 
			hReadThread = CreateThread(NULL, 0, ReadThreadFunc, this, 0, &ReadThreadId);
			
//			DWORD WriteThreadId; 
			hWriteThread = CreateThread(NULL, 0, WriteThreadFunc, this, 0, &WriteThreadId);
			
//			DWORD ProcThreadId; 
//			hProcThread = CreateThread(NULL, 0, ProcThreadFunc, this, 0, &ProcThreadId);

			// that's it...
			m_Available = TRUE;

			char buff[16384];
			sprintf(buff, "Network adapter opened");
			_MainDlg.ShowStatusText(buff);
			
			return TRUE;
		}

	}

	
	// there was an error
	// close network adapter
	CloseHandle(m_AdapterHandle);
	m_AdapterHandle = INVALID_HANDLE_VALUE;
	return FALSE;
}

BOOL CEthernet::Start(unsigned long IP, unsigned long Netmask)
{
	if ((_Settings.m_MyLastNetwork) != IP || _Settings.m_MyLastNetmask != Netmask)
	{
		char buff[1024];
		if (!CheckIfIPRangeIsFree(htonl(IP), htonl(Netmask), buff))
			MessageBox(NULL, buff, "Warning", MB_OK);

		// let's store these values	
		_Settings.m_MyLastNetwork = IP;
		_Settings.m_MyLastNetmask = Netmask;

		hSetupThread = CreateThread(NULL, 0, SetupThreadFunc, this, 0, &SetupThreadId);
	}
	return TRUE;
}

DWORD WINAPI CEthernet::SetupThreadFunc(LPVOID lpParam) 
{
	CEthernet *eth = (CEthernet *)lpParam;
	DWORD ret = eth->DoRenewRelease(FALSE);

	CloseHandle(eth->hSetupThread);
	eth->hSetupThread = INVALID_HANDLE_VALUE;
	return ret;
}
DWORD CEthernet::DoRenewRelease(BOOL ReleaseOnly) 
{	
	BOOL IsValid = FALSE;
	while (m_Alive && !IsValid)
	{
		// let's renew DHCP
		DWORD len = 0;
		DWORD result = GetAdaptersInfo(NULL, &len);
		if (result == ERROR_BUFFER_OVERFLOW)
		{
			void *newmem = malloc(len);
			IP_ADAPTER_INFO *ad_info = (IP_ADAPTER_INFO *)newmem;
			result = GetAdaptersInfo(ad_info, &len);
			if (result == ERROR_SUCCESS)
			{
				int cnt = len/sizeof(IP_ADAPTER_INFO);
				for (int i=0;i<cnt;i++)
				{
					if (!strcmp(ad_info->AdapterName, m_Guid))
					{

						// is it set to DHCP?
						if (!ad_info->DhcpEnabled)
						{
							// does it contain our IP address?
							IP_ADDR_STRING* pNext = NULL;
							pNext = &(ad_info->IpAddressList);
							while(pNext && !IsValid) 
							{
								// is our IP on the list?
								struct  in_addr sa;
								sa.S_un.S_addr = _Settings.m_MyLastNetwork;
								char *ips = inet_ntoa(sa);
								if (ips)
								{
									if (!strcmp(pNext->IpAddress.String, ips))
										IsValid = TRUE;
								}
								// if yes, is netmask wrong?
								sa.S_un.S_addr = _Settings.m_MyLastNetmask;
								ips = inet_ntoa(sa);
								if (ips)
								{
									if (IsValid && strcmp(pNext->IpMask.String, ips))
										IsValid = FALSE;
								}

								pNext = pNext->Next;
							}

						}

						if (!IsValid)
						{
							char buff[2048];

							struct  in_addr sa1, sa2;
							sa1.S_un.S_addr = _Settings.m_MyLastNetwork;
							sa2.S_un.S_addr = _Settings.m_MyLastNetmask;

//							strcpy(buff, "netsh interface ip set address name=\"Wippien\" static ");
							strcpy(buff, "interface ip set address name=\"Wippien\" static ");
							strcat(buff, inet_ntoa(sa1));
							strcat(buff, " ");
							strcat(buff, inet_ntoa(sa2));	

							ExecNETSH(buff);
						}
						// found
						cnt = 0;
					}
					ad_info++;
				}
			}
			free(newmem);

			if (!IsValid && !ReleaseOnly)
			{
				// wait 5 seconds
				for (int s=0;s<50 && m_Alive;s++)
					Sleep(100);

			}
		}

		if (ReleaseOnly)
			break;
	}
	return 0;
}

BOOL CheckedIfIPRangeIsFree = FALSE;
BOOL CEthernet::CheckIfIPRangeIsFree(unsigned long IP, unsigned long Netmask, char *buf)
{
	if (CheckedIfIPRangeIsFree)
		return TRUE;
	CheckedIfIPRangeIsFree = TRUE;

	DWORD len = 0;
	DWORD result = GetAdaptersInfo(NULL, &len);
	if (result == ERROR_BUFFER_OVERFLOW)
	{
		void *newmem = malloc(len);
		IP_ADAPTER_INFO *ad_info = (IP_ADAPTER_INFO *)newmem;
		result = GetAdaptersInfo(ad_info, &len);
		if (result == ERROR_SUCCESS)
		{
			int cnt = len/sizeof(IP_ADAPTER_INFO);
			for (int i=0;i<cnt;i++)
			{
//				if (!strcmp(ad_info->AdapterName, m_Guid))
				IP_ADDR_STRING* pNext = NULL;
				pNext = &(ad_info->IpAddressList);
				while(pNext) 
				{

					unsigned long _ip = htonl(inet_addr(pNext->IpAddress.String));
					unsigned long _mask = htonl(inet_addr(pNext->IpMask.String));

					if (_mask)
					{
						if ((_mask & IP) == (_mask & _ip))
						{
							if (strcmp(ad_info->AdapterName, m_Guid))
							{
								in_addr _IP;
								_IP.S_un.S_addr = htonl(IP);
								
								sprintf(buf, "Adapter \"%s\" with address %s conflicts with Wippien's %s. It is possible that Wippien will not be able to send and receive network packets. Please uninstall or disable this adapter.", ad_info->Description, pNext->IpAddress.String, inet_ntoa(_IP));
								free(newmem);
								return FALSE;
							}
						}
					}
					// let's check if IP is in the same range
					pNext = pNext->Next;
				}
					
				ad_info++;
			}
		}
		free(newmem);
	}
		
	return TRUE;
}

BOOL CEthernet::GetAdapterGuid(void)
{
	DWORD status, len;
	HKEY adapter_key;
	
	
	memset(m_Guid, 0, 1024);
	status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, ADAPTER_KEY, 0, KEY_READ, &adapter_key);
	if (status == ERROR_SUCCESS)
	{
		char bf1[1024], bf2[1024], bf3[1024];
		int i = 0;
		do
		{
			len = sizeof(bf1);
			status = RegEnumKeyEx(adapter_key, i++, bf1, &len, NULL, NULL, NULL, NULL);	
			if (status == ERROR_SUCCESS)
			{
				sprintf(bf2, "%s\\%s", ADAPTER_KEY, bf1);
				HKEY key;
				status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, bf2, 0, KEY_READ, &key);
				if (status == ERROR_SUCCESS)
				{
					DWORD type;
					len = sizeof(bf3);
					status = RegQueryValueEx(key, "ComponentId", NULL, &type, (unsigned char *)bf3, &len);
					if (status == ERROR_SUCCESS)
					{
						if (!strcmp(bf3, WIPP_COMPONENT_ID) || !strcmp(bf3, WIPP_COMPONENT_ID_OLDER))
						{
							// this is ours
							len = sizeof(bf3);
							status = RegQueryValueEx(key, "NetCfgInstanceId", NULL, &type, (unsigned char *)bf3, &len);
							if (status == ERROR_SUCCESS)
							{
								if (len>1023)
									len = 1023;
								memcpy(m_Guid, bf3, len);
								strcpy(m_RegistryKey, bf2);

								char bf4[1024];
								sprintf(bf4, "%s\\%s\\Connection", NETWORK_CONNECTIONS_KEY, bf3);
								HKEY key1;
								if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, bf4, 0, KEY_READ, &key1) == ERROR_SUCCESS)
								{
									unsigned char bf5[1024];
									DWORD cb = sizeof(bf5);
									DWORD dwtype = REG_SZ;
									if (RegQueryValueEx(key1, "Name", NULL, &dwtype, bf5, &cb) == ERROR_SUCCESS)
									{
										if (strcmp((char *)bf5, "Wippien"))
										{
											char bf6[2048];

											strcpy(bf6, "interface set interface name=\"");
//											strcpy(bf6, "netsh interface set interface name=\"");
											strcat(bf6, (char *)bf5);
											strcat(bf6, "\" newname=\"Wippien\"");

											ExecNETSH(bf6);
										}
									}

//									RegSetValueEx(key1, "Name", NULL, REG_SZ, (unsigned char *)"Wippien\0", 8);
								}
								RegCloseKey(key1);
								
							}
						}
					}
					RegCloseKey(key);
					status = ERROR_SUCCESS;
					if (m_Guid[0])
					{
						RegCloseKey(adapter_key);
						return TRUE;
					}
				}
			}
		} while (status != ERROR_NO_MORE_ITEMS && !m_Guid[0]);
		RegCloseKey(adapter_key);
	}

	return FALSE;
}

DWORD WINAPI CEthernet::WriteThreadFunc(LPVOID lpParam) 
{
	CEthernet *eth = (CEthernet *)lpParam;
	
	typedef std::vector<FirewallStruct *> FIREWALLSTRUCT;
	FIREWALLSTRUCT FirewallRules;

	OVERLAPPED overlap;
	memset(&overlap, 0, sizeof(OVERLAPPED));

	overlap.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hs[2];
	hs[0] = eth->Handles[0];
	hs[1] = overlap.hEvent;

	DWORD dwError;

	int rt = 0;
	while (eth->m_Alive && eth->m_Enabled)
	{

		while (eth->m_Alive && eth->m_Enabled && /*!eth->WriteBuffer.Len()*/ eth->m_EthWriteStart == eth->m_EthWriteEnd)			
		{
			rt = WaitForMultipleObjects(2, eth->Handles, FALSE, 100);
			if (rt == WAIT_OBJECT_0)
			{
				eth->m_Alive = FALSE;
				break;
			}
		}

		if (eth->m_FirewallRulesChanged)
		{
			eth->m_FirewallRulesChanged = FALSE;
			while (FirewallRules.size())
			{
				FirewallStruct *fs = (FirewallStruct *)FirewallRules[0];
				delete fs;
				FirewallRules.erase(FirewallRules.begin());
			}
			for (int i=0;i<_Settings.m_FirewallRules.size();i++)
			{
				FirewallStruct *orgfs = (FirewallStruct *)_Settings.m_FirewallRules[i];
				FirewallStruct *fs = new FirewallStruct;
				fs->Port = htons(orgfs->Port);
				fs->Proto = orgfs->Proto;
				FirewallRules.push_back(fs);
			}
		}
		if (eth->m_Alive && eth->m_Enabled && /*eth->WriteBuffer.Len()*/ eth->m_EthWriteEnd!=eth->m_EthWriteStart)
		{


			char *a = NULL;
//			unsigned int len;
			while (eth->m_EthWriteEnd != eth->m_EthWriteStart)
//			while (eth->WriteBuffer.Len())
			{
				a = NULL;
				EthWriteData *ed = (EthWriteData *)(eth->m_EthWriteBuff + eth->m_EthWriteStart * (sizeof(EthWriteData)+ETH_MAX_PACKET));
//				if (!ed->Occupied)
//					MessageBeep(-1);
				if (ed->Occupied)
				{
					
					a = (char *)ed;
					a += sizeof(EthWriteData);

					if (ed->DataLen && a)
					{

						// should we write?
						ETH_HEADER *ethr = (ETH_HEADER *) a;
						IPHDR *ip = (IPHDR *)(a + sizeof(ETH_HEADER));
						UDPHDR *udp = (UDPHDR *)(a + sizeof(ETH_HEADER) + sizeof(IPHDR));
						//		ICMPHDR *icmp = (ICMPHDR *)(packet + sizeof(ETH_HEADER) + sizeof(IPHDR));
						TCPHDR *tcp = (TCPHDR *)(a + sizeof(ETH_HEADER) + sizeof(IPHDR));
						ARP_PACKET *p = (ARP_PACKET *)a;
						
						BOOL cansend = _Settings.m_FirewallDefaultAllowRule;
						for (int i=0;i<FirewallRules.size();i++)
						{
							FirewallStruct *fs = (FirewallStruct *)FirewallRules[i];
							if (fs->Proto == ip->protocol)
							{
								switch (fs->Proto)
								{
								case IPPROTO_ICMP:
									cansend = !cansend;
									break;
									
								case IPPROTO_UDP:
									if (udp->dest == fs->Port/* || udp->source == fs->Port*/)
										cansend = !cansend;
									break;
									
								case IPPROTO_TCP:
									if (tcp->dest == fs->Port/* || tcp->source == fs->Port*/)
										cansend = !cansend;
									break;
								}
							}
						}
						


						DWORD nwrite = ed->DataLen;
						if (cansend && ed->DataLen>0 && eth->m_AdapterHandle)
						{
							if (!WriteFile(eth->m_AdapterHandle, a, ed->DataLen, &nwrite, &overlap))
							{
								dwError = GetLastError();
								switch (dwError)
								{
									case ERROR_MORE_DATA:
										// loop more...
										break;

									case ERROR_HANDLE_EOF:
									case ERROR_FILE_NOT_FOUND:
										eth->m_Alive = FALSE;
										break;

									case ERROR_IO_PENDING: 
										while(eth->m_Alive && eth->m_Enabled)
										{
	//										Sleep(10);
	//										int ret = 100;
											int ret = WaitForMultipleObjects(2, hs, FALSE, 1000);
											if (ret == (WAIT_OBJECT_0) || !eth->m_Alive)
											{
												eth->m_Alive = FALSE;
												break;
											}
											else
											{
												if (GetOverlappedResult(eth->m_AdapterHandle, &overlap, &nwrite, FALSE))
												{
	//												EnterCriticalSection(&eth->WriteCS);
	//												eth->WriteBuffer.Consume(len + 4);
	//												LeaveCriticalSection(&eth->WriteCS);
													break;
												}
											}
										} 
										break;
								}
	//							ATLTRACE("<<Sent %d bytes to adapter\r\n", nwrite);
							}
							else
	//							ATLTRACE("<<Sent %d bytes to adapter\r\n", nwrite);
							ResetEvent(overlap.hEvent);
	//						ATLTRACE("WriteThread wrote %d bytes\r\n", nwrite);
						}
	//					free(a);
					}
				}
				ed->Occupied = FALSE;
				eth->m_EthWriteStart++;
				if (eth->m_EthWriteStart >= ETH_TOT_PACKETS)
					eth->m_EthWriteStart = 0;
			}
		}
	}
	CloseHandle(overlap.hEvent);
	CloseHandle(eth->hWriteThread);
	_Ethernet.m_Available = FALSE;
	eth->m_AdapterHandle = INVALID_HANDLE_VALUE;
	eth->hWriteThread = INVALID_HANDLE_VALUE;


	// let's see if we can run it up again
	while (eth->m_Alive && !eth->m_Enabled)
	{
		Sleep(500);
		eth->Init();
	}
	return 0;
}
void CEthernet::InjectPacket(char *packet, int len)
{

//	if (len > ETH_MAX_PACKET)
//		MessageBeep(-1);
/*	
	EnterCriticalSection(&WriteCS);
	WriteBuffer.PutString(packet, len);
	LeaveCriticalSection(&WriteCS);
*/

	EthWriteData *ed = (EthWriteData *)(m_EthWriteBuff + m_EthWriteEnd * (sizeof(EthWriteData)+ETH_MAX_PACKET));
	if (ed->Occupied)
		return; // all filled up!

	ed->DataLen = len;
	char *d = (char *)ed;
	d += sizeof(EthWriteData);
	memcpy(d, packet, len);

	ed->Occupied = TRUE;

	m_EthWriteEnd++;
	if (m_EthWriteEnd >= ETH_TOT_PACKETS)
		m_EthWriteEnd = 0;

	SetEvent(WriteHandle);


}


DWORD WINAPI CEthernet::ReadThreadFunc(LPVOID lpParam) 
{ 
	CEthernet *eth = (CEthernet *)lpParam;
	BOOL success = FALSE;
	DWORD dwError;
	
//	int handle = open("c:\\in", O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
	char buf[16386];
	DWORD nread;
	BOOL mustread = TRUE, ret = FALSE;
	
	OVERLAPPED overlap;
	memset(&overlap, 0, sizeof(OVERLAPPED));

	overlap.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hs[2];
	hs[0] = eth->Handles[0];
	hs[1] = overlap.hEvent;

	success = TRUE;
	do
	{

		if (mustread)
			success = ReadFile(eth->m_AdapterHandle, buf+2, 16384, &nread, &overlap);
		if (success)
		{
//			ATLTRACE(">>Received %d bytes from adapter\r\n", nread);
			eth->ProcPacket(buf+2, nread);

			mustread = TRUE;
		}
		else
		{
			dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_HANDLE_EOF:
				case ERROR_FILE_NOT_FOUND:
					break;

				case ERROR_IO_PENDING: 
//					BOOL dowait = FALSE;
//					ret = 100;
					while(eth->m_Alive && eth->m_Enabled)
					{
//						if (dowait)
//						{
//							Sleep(10);
//							ret = 100;
//						}
						ret = WaitForMultipleObjects(2, hs, FALSE, 1000);
//						dowait = TRUE;
						if (ret == (WAIT_OBJECT_0))
						{
							CloseHandle(overlap.hEvent);
							CloseHandle(eth->hReadThread);
							eth->hReadThread = INVALID_HANDLE_VALUE;
							return 0;
						}
						else
						{
							if (GetOverlappedResult(eth->m_AdapterHandle, &overlap, &nread, FALSE))
							{
								mustread = FALSE;
								success = TRUE;
								break;
							}
							else
							{
								if (GetLastError() == ERROR_OPERATION_ABORTED)
								{
									char buff[16384];
									sprintf(buff, "Network adapter disabled!");
									_MainDlg.ShowStatusText(buff);
									MessageBeep(-1);

									eth->m_Enabled = FALSE;
								}
							}
						}
					} 
					success = TRUE;
					break;
			}
		}
	} while (success && nread && eth->m_Alive && eth->m_Enabled);
//	close(handle);
	CloseHandle(overlap.hEvent);
	CloseHandle(eth->hReadThread);
	eth->hReadThread = INVALID_HANDLE_VALUE;
	_Ethernet.m_Available = FALSE;

    return 0; 
} 
void CEthernet::ProcPacket(char *packet, int len)
{

		ETH_HEADER *ethr = (ETH_HEADER *) packet;
		IPHDR *ip = (IPHDR *)(packet + sizeof(ETH_HEADER));
		UDPHDR *udp = (UDPHDR *)(packet + sizeof(ETH_HEADER) + sizeof(IPHDR));
//		ICMPHDR *icmp = (ICMPHDR *)(packet + sizeof(ETH_HEADER) + sizeof(IPHDR));
		TCPHDR *tcp = (TCPHDR *)(packet + sizeof(ETH_HEADER) + sizeof(IPHDR));
		ARP_PACKET *p = (ARP_PACKET *)packet;

		// let's see if this is DHCP request
		BOOL IsARP = FALSE;
		// is it ARP packet?
		if (len == sizeof(ARP_PACKET) && ethr->proto == 1544 && p->m_PROTO_AddressType==8 && p->m_ARP_Operation == 256)
		{
			IsARP = TRUE;
			//-----------------------------------------------
			// Is this the kind of packet we are looking for?
			//-----------------------------------------------

			for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
			{
				CUser *user = _MainDlg.m_UserList.m_Users[i];
//					if (user->m_WippienState == WipConnected)
				{
					if (p->m_Proto == htons(ETH_P_ARP)
						&& MAC_EQUAL(p->m_MAC_Source, m_MAC)
						&& MAC_EQUAL(p->m_ARP_MAC_Source, m_MAC)
						&& MAC_EQUAL(p->m_MAC_Destination, MAC_BROADCAST)
						&& p->m_ARP_Operation == htons(ARP_REQUEST)
						&& p->m_MAC_AddressType == htons(MAC_ADDR_TYPE)
						&& p->m_MAC_AddressSize == sizeof(MACADDR)
						&& p->m_PROTO_AddressType == htons(ETH_P_IP)
						&& p->m_PROTO_AddressSize == sizeof(IPADDR)
						&& p->m_ARP_IP_Source == (_Settings.m_MyLastNetwork)
						&& p->m_ARP_IP_Destination == user->m_HisVirtualIP)
					{
						ARP_PACKET arp;
						//----------------------------------------------
						// Initialize ARP reply fields
						//----------------------------------------------
						arp.m_Proto = htons(ETH_P_ARP);
						arp.m_MAC_AddressType = htons(MAC_ADDR_TYPE);
						arp.m_PROTO_AddressType = htons(ETH_P_IP);
						arp.m_MAC_AddressSize = sizeof(MACADDR);
						arp.m_PROTO_AddressSize = sizeof(IPADDR);
						arp.m_ARP_Operation = htons(ARP_REPLY);
						
						//----------------------------------------------
						// ARP addresses
						//----------------------------------------------      
						COPY_MAC(arp.m_MAC_Source, user->m_MAC);
						COPY_MAC(arp.m_MAC_Destination, m_MAC);
						COPY_MAC(arp.m_ARP_MAC_Source, user->m_MAC);
						COPY_MAC(arp.m_ARP_MAC_Destination, m_MAC);
						arp.m_ARP_IP_Source = user->m_HisVirtualIP;
						arp.m_ARP_IP_Destination = (_Settings.m_MyLastNetwork);												
						InjectPacket((char *)&arp, sizeof(ARP_PACKET));
					}
				}
			}
		}

		if (!IsARP)
		{
			if (len >= sizeof(ETH_HEADER))
			{
				for (int i=0;i<_MainDlg.m_UserList.m_Users.size();i++)
				{
					CUser *user = _MainDlg.m_UserList.m_Users[i];
//					if (user->m_WippienState == WipConnected)
					{
						if (!memcmp(user->m_MAC, ethr->dest, sizeof(MACADDR)) || !memcmp(ethr->dest, MAC_BROADCAST, sizeof(MACADDR)))
						{
							user->SendNetworkPacket(packet, len);
						}
					}
				}			
			}
		}
}


unsigned short CEthernet::IPHDRChecksumCalc(unsigned short *addr, int len)
{
	unsigned long sum = 0; 
	for (sum = 0; len > 0; len--) 
		sum += *addr++; 
	sum = (sum >> 16) + (sum & 0xffff); 
	sum +=(sum >> 16); 
	return (unsigned short)~sum; 
}

unsigned short CEthernet::TCPUDPChecksumCalc(unsigned short *addr, int len)
{
	register int sum = 0;
	u_short answer = 0;
	register u_short *w = addr;
	register int nleft = len;
	/*
	* * Our algorithm is simple, using a 32 bit accumulator(sum), we add
	* * sequential 16 bit words to it, and at the end, fold back all the
	* * carry bits from the top 16 bits into the lower 16 bits.
	* */
	while (nleft > 1) 
	{
		sum += *w++;
		nleft -= 2;
	}
	
	/* mop up an odd byte, if necessary */
	if (nleft == 1)
	{
		* (u_char *)(&answer) = * (u_char *)w;
		sum += answer;
	}
	
	/* add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
	sum +=(sum >> 16); /* add carry */
	answer = ~sum; /* truncate to 16 bits */
	return (answer);
}

BOOL EXEmessageboxshown = FALSE;
BOOL CEthernet::ExecNETSH(char *text)
{
	// let's see if there's already our netsh called
	if (!FindWindow(NULL, CONFIGURING_ADAPTER_TEXT))
	{
		// am I administrator?
		if (_UpdateHandler->m_Update)
		{
			VARIANT_BOOL isadmin = VARIANT_TRUE;
#ifndef _APPUPDLIB
			if (SUCCEEDED(_UpdateHandler->m_Update->get_UserIsAdmin(&isadmin)))
#else
				WODAPPUPDCOMLib::AppUpd_GetUserIsAdmin(_UpdateHandler->m_Update, (BOOL *)&isadmin);
#endif
			{
				if (!isadmin)
				{
					if(!EXEmessageboxshown)
					{
						EXEmessageboxshown = TRUE;
						MessageBox(NULL, "Wippien must set up adapter IP address using external tool - and requires administrative privileges. Please run Wippien at least once as administrator in order to obtain enough permissions.", "Admin privilege required!", MB_OK);
					}
/*
					// let's run main app
					SHELLEXECUTEINFO shExecInfo = {0};

					shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

					shExecInfo.fMask = NULL;
					shExecInfo.hwnd = NULL;
					shExecInfo.lpVerb = "runas";
					shExecInfo.lpFile = "c:\\windows\\system32\\netsh.exe";
					shExecInfo.lpParameters = text;
					shExecInfo.lpDirectory = NULL;
					shExecInfo.nShow = SW_HIDE;
					shExecInfo.hInstApp = NULL;

					if (ShellExecuteEx(&shExecInfo))
					{
						return TRUE;
					}
*/
				}
			}
		}
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		Buffer b;
		b.Append("netsh ");
		b.Append(text);
		b.Append("\0",1);
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		si.lpTitle = (char *)CONFIGURING_ADAPTER_TEXT;
		ZeroMemory( &pi, sizeof(pi) );
	

		return CreateProcess(NULL, b.Ptr(),NULL,NULL,FALSE,0,NULL,NULL,&si, &pi);						
	}

	return FALSE;
}
