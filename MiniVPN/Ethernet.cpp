#include "StdAfx.h"
#include <WinIoCtl.h>
#include <Iphlpapi.h>
#include "Ethernet.h"
#include "JabberLib.h"
#include "User.h"

unsigned char MAC_BROADCAST[6] = {255,255,255,255,255,255};
char *hex="0123456789abcdef";
extern CEthernet *_Ethernet;
extern USERLIST m_Users;

#ifndef _WIPPIENSERVICE
void SetStatus(char *Text);
#endif

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

	hReadThread = INVALID_HANDLE_VALUE;
	hWriteThread = INVALID_HANDLE_VALUE;
	hSetupThread = INVALID_HANDLE_VALUE;

	m_Alive = FALSE;
	m_Guid[0] = m_RegistryKey[0];

	m_MyIP = 0;
	m_MyNetmask = 0;
	_Ethernet = this;
}

CEthernet::~CEthernet()
{
	// tell everyone to die
	SetEvent(DieHandle);
	
	m_Alive = FALSE;
	while (hReadThread != INVALID_HANDLE_VALUE || hWriteThread != INVALID_HANDLE_VALUE || hSetupThread != INVALID_HANDLE_VALUE)
		Sleep(100);

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
	
}

void CEthernet::GetMac(MACADDR src, char dst[18])
{
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


BOOL CEthernet::InitAdapter(void)
{
	if (!GetAdapterGuid(WIPP_COMPONENT_ID))
		return FALSE;

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

	unsigned long len;
	unsigned long m_AdapterVersion[3];
    if (DeviceIoControl(m_AdapterHandle, WIPP_IOCTL_GET_VERSION, &m_AdapterVersion, sizeof(m_AdapterVersion), &m_AdapterVersion, sizeof(m_AdapterVersion), &len, NULL))
	{
		if (m_AdapterVersion[0] > 2 || m_AdapterVersion[1] >= 4)
		{
			// in 2.4 we have add status
			unsigned long mediaon = TRUE;
			len = sizeof(mediaon);
			if (DeviceIoControl(m_AdapterHandle, WIPP_IOCTL_SET_MEDIA_STATUS, &mediaon, sizeof(mediaon), &mediaon, sizeof(mediaon), &len, NULL))
			{
//				MessageBeep(-1);
			}
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
			return TRUE;
		}

	}

	
	// there was an error
	// close network adapter
	CloseHandle(m_AdapterHandle);
	m_AdapterHandle = INVALID_HANDLE_VALUE;
	return FALSE;
}

BOOL CEthernet::InitOpenVPNAdapter(void)
{
	if (!GetAdapterGuid(TAP_COMPONENT_ID))
		return FALSE;

//	m_AdapterName = TAP_COMPONENT_ID;

	char bname[1024];
	sprintf(bname, "%s%s%s", USERMODEDEVICEDIR, m_Guid, TAPSUFFIX);
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

	unsigned long len;
	unsigned long m_AdapterVersion[3];

    if (DeviceIoControl(m_AdapterHandle, TAP_IOCTL_GET_VERSION, &m_AdapterVersion, sizeof(m_AdapterVersion), &m_AdapterVersion, sizeof(m_AdapterVersion), &len, NULL))
	{
		if (m_AdapterVersion[0] >= 9 || m_AdapterVersion[1] >= 1)
		{
			// in 2.4 we have add status
			unsigned long mediaon = TRUE;
			len = sizeof(mediaon);
			if (DeviceIoControl(m_AdapterHandle, TAP_IOCTL_SET_MEDIA_STATUS, &mediaon, sizeof(mediaon), &mediaon, sizeof(mediaon), &len, NULL))
			{
				//				MessageBeep(-1);
			}
		}

		// get our MAC address
		if (DeviceIoControl(m_AdapterHandle, TAP_IOCTL_GET_MAC, &m_MAC, sizeof(MACADDR), &m_MAC, sizeof(MACADDR), &len, NULL))
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
			return TRUE;
		}

	}

	
	// there was an error
	// close network adapter
	CloseHandle(m_AdapterHandle);
	m_AdapterHandle = INVALID_HANDLE_VALUE;
	return FALSE;
}

/*BOOL CEthernet::Start(unsigned long IP, unsigned long Netmask)
{
	m_MyIP = IP;
	m_MyNetmask = Netmask;
	hSetupThread = CreateThread(NULL, 0, SetupThreadFunc, this, 0, &SetupThreadId);
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
								sa.S_un.S_addr = m_MyIP;
								char *ips = inet_ntoa(sa);
								if (ips)
								{
									if (!strcmp(pNext->IpAddress.String, ips))
										IsValid = TRUE;
								}
								// if yes, is netmask wrong?
								sa.S_un.S_addr = m_MyNetmask;
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
							sa1.S_un.S_addr = m_MyIP;
							sa2.S_un.S_addr = m_MyNetmask;

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
*/

void CEthernet::GetMyIP(void) 
{
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

/*					// is it set to DHCP?
					if (!ad_info->DhcpEnabled)
					{
						m_MyIP = 0;
						m_MyNetmask = 0;
						return;
					}
*/
					// does it contain our IP address?
					IP_ADDR_STRING* pNext = NULL;
					pNext = &(ad_info->IpAddressList);
					while(pNext) 
					{
						m_MyIP = inet_addr(pNext->IpAddress.String);
						m_MyNetmask = inet_addr(pNext->IpMask.String);
						pNext = pNext->Next;
					}

					// found
					cnt = 0;
				}
				ad_info++;
			}
		}
		free(newmem);
	}
}

BOOL CEthernet::GetAdapterGuid(char *ID)
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
						if (!strcmp(bf3, ID))
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

/* exec NETSH to set up proper IP
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
*/
								
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

//					EnterCriticalSection(&eth->WriteCS);
//					if (eth->WriteBuffer.Len())
//						a = eth->WriteBuffer.GetString(&len);
//					LeaveCriticalSection(&eth->WriteCS);
					if (ed->DataLen && a)
					{
						DWORD nwrite = ed->DataLen;
						if (ed->DataLen>0 && eth->m_AdapterHandle)
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
	_Ethernet->m_Available = FALSE;
	eth->m_AdapterHandle = INVALID_HANDLE_VALUE;
	eth->hWriteThread = INVALID_HANDLE_VALUE;


	// let's see if we can run it up again
	while (eth->m_Alive && !eth->m_Enabled)
	{
//		Sleep(500);
//		eth->Init();
		::PostQuitMessage(-1);
	}
	return 0;
}
void CEthernet::InjectPacket(char *packet, int len)
{
#if	GETFREQ
	__int64 pNow;
	QueryPerformanceCounter((LARGE_INTEGER *)&pNow);
	ATLTRACE("Before Inject packet at %f, %f\r\n", (float)pNow/(float)pFreq, ((float)(pNow - pPrev))/pFreq);
	pPrev = pNow;
#endif

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


#if	GETFREQ
	QueryPerformanceCounter((LARGE_INTEGER *)&pNow);
	ATLTRACE("After Inject packet at %f, %f\r\n", (float)pNow/(float)pFreq, ((float)(pNow - pPrev))/pFreq);
	pPrev = pNow;
#endif


//	if (!writectr)
//		CreateThread(NULL, 0, WriteThreadFunc, this, 0, &WriteThreadId);

//	DWORD nwrite = len;
//	if (m_AdapterHandle != INVALID_HANDLE_VALUE)
//		WriteFile(m_AdapterHandle, packet, len, &nwrite, /*&overlap*/NULL);

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
#ifndef _WIPPIENSERVICE
									char buff[16384];
									sprintf(buff, "Network adapter disabled!");
									SetStatus(buff);
									MessageBeep(-1);
#endif
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
	_Ethernet->m_Available = FALSE;

    return 0; 
} 
void CEthernet::ProcPacket(char *packet, int len)
{
		ETH_HEADER *ethr = (ETH_HEADER *) packet;
		IPHDR *ip = (IPHDR *)(packet + sizeof(ETH_HEADER));
		UDPHDR *udp = (UDPHDR *)(packet + sizeof(ETH_HEADER) + sizeof(IPHDR));
//		ICMPHDR *icmp = (ICMPHDR *)(packet + sizeof(ETH_HEADER) + sizeof(IPHDR));
//		TCPHDR *tcp = (TCPHDR *)(packet + sizeof(ETH_HEADER) + sizeof(IPHDR));
		ARP_PACKET *p = (ARP_PACKET *)packet;

		{
			BOOL IsARP = FALSE;
			// is it ARP packet?
			if (len == sizeof(ARP_PACKET) && ethr->proto == 1544 && p->m_PROTO_AddressType==8 && p->m_ARP_Operation == 256)
			{
				IsARP = TRUE;
				//-----------------------------------------------
				// Is this the kind of packet we are looking for?
				//-----------------------------------------------
				for (unsigned int i=0;i<m_Users.size();i++)
				{
					CUser *user = (CUser *)m_Users[i];
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
							&& p->m_ARP_IP_Source == (m_MyIP)
							&& p->m_ARP_IP_Destination == user->m_RemoteIP)
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
							arp.m_ARP_IP_Source = user->m_RemoteIP;
							arp.m_ARP_IP_Destination = (m_MyIP);												
							InjectPacket((char *)&arp, sizeof(ARP_PACKET));
						}
					}
				}
			}
			if (!IsARP)
			{
				if (len >= sizeof(ETH_HEADER))
				{
					for (unsigned int i=0;i<m_Users.size();i++)
					{
						CUser *user = (CUser *)m_Users[i];
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

