// Ethernet.h: interface for the CEthernet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETHERNET_H__5CA29249_27DC_495E_A711_04CC4C683E4C__INCLUDED_)
#define AFX_ETHERNET_H__5CA29249_27DC_495E_A711_04CC4C683E4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WIPPIENDRV

#include "proto.h"
#include "Buffer.h"
#include "Settings.h"

#endif

#define WIPP_DRIVER_MAJOR_VERSION 2
#define WIPP_DRIVER_MINOR_VERSION 2

//======================
// Filesystem prefixes
//======================

#define USERMODEDEVICEDIR "\\\\.\\Global\\"
#define SYSDEVICEDIR      "\\Device\\"
#define USERDEVICEDIR     "\\DosDevices\\Global\\"
#define WIPPSUFFIX        ".wip"
#define WIPP_COMPONENT_ID "wip0203"
#define WIPP_COMPONENT_ID_OLDER "wip0202"

//=================
// Registry keys
//=================

#define ADAPTER_KEY "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}"
#define NETWORK_CONNECTIONS_KEY "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}"



//=============
// TAP IOCTLs
//=============

#define WIPP_CONTROL_CODE(request,method) CTL_CODE (FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)

#define WIPP_IOCTL_GET_MAC               WIPP_CONTROL_CODE (10, METHOD_BUFFERED)
#define WIPP_IOCTL_SET_MAC               WIPP_CONTROL_CODE (11, METHOD_BUFFERED)
#define WIPP_IOCTL_GET_VERSION           WIPP_CONTROL_CODE (13, METHOD_BUFFERED)
#define WIPP_IOCTL_GET_MTU               WIPP_CONTROL_CODE (15, METHOD_BUFFERED)
#define WIPP_IOCTL_GET_INFO              WIPP_CONTROL_CODE (16, METHOD_BUFFERED)
#define WIPP_IOCTL_GET_LOG_LINE          WIPP_CONTROL_CODE (17, METHOD_BUFFERED)

/* Define to the version of this package. */
#define VERSION "1.2.2"

#ifndef WIPPIENDRV
#define ETH_MAX_PACKET	1520
#define ETH_TOT_PACKETS	50
#pragma pack(push, 1)
typedef struct EthWriteData
{
	BOOL Occupied; //1 means should be processed, 0 means it's already processed
	int DataLen;
} EthWriteData;
#pragma pack(pop,1)

class CEthernet  
{
public:

	char m_EthWriteBuff[(ETH_MAX_PACKET+sizeof(EthWriteData)) * ETH_TOT_PACKETS];
	unsigned int m_EthWriteStart, m_EthWriteEnd;

	static DWORD WINAPI ReadThreadFunc(LPVOID lpParam);
	static DWORD WINAPI WriteThreadFunc(LPVOID lpParam);
	static DWORD WINAPI SetupThreadFunc(LPVOID lpParam);
	void ProcPacket(char *Packet, int len);
	DWORD DoRenewRelease(BOOL ReleaseOnly);

//	CRITICAL_SECTION WriteCS, ReadCS;
	HANDLE m_AdapterHandle;
	BOOL m_Alive, m_Available, m_Enabled;
	BOOL m_FirewallRulesChanged;

	HANDLE Handles[3];
	#define DieHandle	Handles[0]
	#define WriteHandle Handles[1]
	#define ReadHandle	Handles[2]


	DWORD ReadThreadId; 
    HANDLE hReadThread;	
	DWORD WriteThreadId; 
    HANDLE hWriteThread;
//	DWORD ProcThreadId; 
//  HANDLE hProcThread;
	HANDLE hSetupThread;
	DWORD SetupThreadId;

	Buffer ReadBuffer/*, WriteBuffer*/;


	CEthernet();
	virtual ~CEthernet();
	MACADDR m_MAC;

	BOOL Init(void);
	BOOL Start(unsigned long IP, unsigned long Netmask);
	BOOL CheckIfIPRangeIsFree(unsigned long IP, unsigned long Netmask, char *buff);
	char m_Guid[1024], m_RegistryKey[1024];

	BOOL GetAdapterGuid(void);
	void GetMac(MACADDR src, char dst[18]);
	void InjectPacket(char *packet, int len);
	BOOL ExecNETSH(char *text);
	static unsigned short IPHDRChecksumCalc(unsigned short *buff, int nwords);
	static unsigned short TCPUDPChecksumCalc(unsigned short *addr, int len);


};

#endif

#endif // !defined(AFX_ETHERNET_H__5CA29249_27DC_495E_A711_04CC4C683E4C__INCLUDED_)
