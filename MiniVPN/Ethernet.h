#ifndef _ETHERNET_H
#define _ETHERNET_H

//extern unsigned char MAC_BROADCAST[6];
#include "proto.h"

#define WIPP_DRIVER_MAJOR_VERSION 2
#define WIPP_DRIVER_MINOR_VERSION 4

//======================
// Filesystem prefixes
//======================

#define USERMODEDEVICEDIR "\\\\.\\Global\\"
#define SYSDEVICEDIR      "\\Device\\"
#define USERDEVICEDIR     "\\DosDevices\\Global\\"
#define WIPPSUFFIX        ".wip"
#define WIPP_COMPONENT_ID "wip0204"
#define TAP_COMPONENT_ID "tap0901"
#define TAPSUFFIX         ".tap"

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
#define WIPP_IOCTL_SET_MEDIA_STATUS      WIPP_CONTROL_CODE (18, METHOD_BUFFERED)

#define TAP_CONTROL_CODE(request,method) \
CTL_CODE (FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)

// Present in 8.1

#define TAP_IOCTL_GET_MAC               TAP_CONTROL_CODE (1, METHOD_BUFFERED)
#define TAP_IOCTL_GET_VERSION           TAP_CONTROL_CODE (2, METHOD_BUFFERED)
#define TAP_IOCTL_GET_MTU               TAP_CONTROL_CODE (3, METHOD_BUFFERED)
#define TAP_IOCTL_GET_INFO              TAP_CONTROL_CODE (4, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_POINT_TO_POINT TAP_CONTROL_CODE (5, METHOD_BUFFERED)
#define TAP_IOCTL_SET_MEDIA_STATUS      TAP_CONTROL_CODE (6, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_MASQ      TAP_CONTROL_CODE (7, METHOD_BUFFERED)
#define TAP_IOCTL_GET_LOG_LINE          TAP_CONTROL_CODE (8, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_SET_OPT   TAP_CONTROL_CODE (9, METHOD_BUFFERED)

// Added in 8.2

/* obsoletes TAP_IOCTL_CONFIG_POINT_TO_POINT */
#define TAP_IOCTL_CONFIG_TUN            TAP_CONTROL_CODE (10, METHOD_BUFFERED)


/* Define to the version of this package. */
#define VERSION "1.2.4"

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

	HANDLE m_AdapterHandle;
	BOOL m_Alive, m_Available, m_Enabled;

	HANDLE Handles[3];
	#define DieHandle	Handles[0]
	#define WriteHandle Handles[1]
	#define ReadHandle	Handles[2]


	DWORD ReadThreadId; 
    HANDLE hReadThread;	
	DWORD WriteThreadId; 
    HANDLE hWriteThread;
	HANDLE hSetupThread;
	DWORD SetupThreadId;

	CEthernet();
	virtual ~CEthernet();
	MACADDR m_MAC;

	BOOL Start(unsigned long IP, unsigned long Netmask);
	unsigned long m_MyIP, m_MyNetmask;
	void GetMyIP(void);
	char m_Guid[1024], m_RegistryKey[1024];

	BOOL InitAdapter(void);
	BOOL InitOpenVPNAdapter(void);
	BOOL GetAdapterGuid(char *ID);
	void GetMac(MACADDR src, char dst[18]);
	void InjectPacket(char *packet, int len);
	static unsigned short IPHDRChecksumCalc(unsigned short *buff, int nwords);
	static unsigned short TCPUDPChecksumCalc(unsigned short *addr, int len);


};

#endif
