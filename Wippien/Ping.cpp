#include "stdafx.h"
#include <ipexport.h>
#include "User.h"
#include "ping.h"

HINSTANCE m_IcmpInst = NULL;
CRITICAL_SECTION m_IcmpCS;


typedef BOOL (WINAPI *ICMPCLOSEHANDLE)(HANDLE IcmpHandle);
typedef HANDLE (WINAPI *ICMPCREATEFILE)(VOID);
typedef DWORD (WINAPI *ICMPSENDECHO)(HANDLE IcmpHandle,IPAddr DestinationAddress, LPVOID RequestData, WORD RequestSize, PIP_OPTION_INFORMATION RequestOptions, LPVOID ReplyBuffer, DWORD ReplySize, DWORD Timeout);

ICMPCLOSEHANDLE pIcmpCloseHandle = NULL;
ICMPCREATEFILE pIcmpCreateFile = NULL;
ICMPSENDECHO pIcmpSendEcho = NULL;



CDetectMTU::CDetectMTU()
{
	EnterCriticalSection(&m_IcmpCS);
	if (!m_IcmpInst)
	{
		m_IcmpInst = LoadLibrary("icmp.dll");
		if (m_IcmpInst)
		{
			pIcmpCloseHandle = (ICMPCLOSEHANDLE)GetProcAddress(m_IcmpInst, "IcmpCloseHandle");
			pIcmpCreateFile  = (ICMPCREATEFILE) GetProcAddress(m_IcmpInst, "IcmpCreateFile");
			pIcmpSendEcho =	   (ICMPSENDECHO)   GetProcAddress(m_IcmpInst, "IcmpSendEcho");
		}
	}
	LeaveCriticalSection(&m_IcmpCS);

	m_UserWnd = NULL;
	if (pIcmpCreateFile)
	    hIcmp = pIcmpCreateFile();
}

CDetectMTU::~CDetectMTU()
{
	if (pIcmpCloseHandle)
	    pIcmpCloseHandle( hIcmp );
}

int CDetectMTU::Ping(unsigned long target_addr, int icmp_data_size)
{

    struct ip_option_information ip_opts;

    /* Prepare the IP options */
    memset(&ip_opts,0,sizeof(ip_opts));
    ip_opts.Ttl=30;
	ip_opts.Flags = IP_FLAG_DF | IP_OPT_ROUTER_ALERT;


	// ignore icmpbuff data contents 
	char *icmp_data = icmpbuff;

	int status = -1;
	if (pIcmpSendEcho)
		status=pIcmpSendEcho(hIcmp,
                        target_addr,
                        (LPVOID)icmp_data,
                        icmp_data_size,
                        &ip_opts,
                        reply_buffer,
                        sizeof(reply_buffer),
                        3000L); // 5 seconds
	if (!status)
	{
		if (GetLastError() == IP_REQ_TIMED_OUT)
			return 0;
		else
			return -1;
	}
	return status;
}

DWORD WINAPI CalcMTUThreadProc(LPVOID lpParam)
{
	CDetectMTU *mtu = (CDetectMTU *)lpParam;

	int status = 0;
	int min_size = 0;
	int max_size = 2000;
	int retry = 0;

	int packet_size = 0;
	int more = TRUE;
	do
	{
		packet_size = (min_size + max_size)/2;
		//printf("trying %d, retry %d", packet_size, retry);
		status = mtu->Ping(mtu->m_Address, packet_size);
		if (status == 0)
		{
			if (retry == 5)
				status = (-1);
			else
			{
				retry++;
				//printf("... timeout\r\n");
			}
		}

		if (status>0)
		{
			//printf("... ok\r\n");
			if (min_size == packet_size)
				more = FALSE;
			else
			{
				min_size = packet_size;
				retry = 0;
			}
		}

		if (status<0)
		{
			//printf("... err\r\n");
			if (max_size == packet_size)
				more = FALSE;
			else
			{
				max_size = packet_size;
				retry = 0;
			}
		}

		if (more)
			Sleep(100);
	} while (more && IsWindow(mtu->m_UserWnd));

//	if (packet_size)
//		printf("\r\nYou should set MTU to %d\r\n", packet_size+28);

	if (IsWindow(mtu->m_UserWnd))
		::PostMessage(mtu->m_UserWnd, WM_MTUDETECT, packet_size+28, 0);
	delete mtu;
	return 0;
}


int CDetectMTU::CalcMTU(CUser *user)
{
	m_UserWnd = user->m_sockWndHandle;
	m_Address = user->m_HisVirtualIP;

	DWORD id = 0;
	CreateThread(NULL, 0, CalcMTUThreadProc, this, 0, &id);

	return 0;
}
