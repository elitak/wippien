#ifndef __PING_H
#define __PING_H

class CUser;

class CDetectMTU
{


	HANDLE hIcmp;
	char icmpbuff[2048];
	unsigned char reply_buffer[10000];


public:
	HWND m_UserWnd;
	int CalcMTU(CUser *user);
	unsigned long m_Address;
	int Ping(unsigned long target_addr, int icmp_data_size);
	CDetectMTU();
	~CDetectMTU();
};




#endif