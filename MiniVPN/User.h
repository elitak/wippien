#ifndef __USER_H
#define __USER_H

#include "proto.h"
#include "openssl/evp.h"
#include "openssl/pem.h"
#include "openssl/rsa.h"

namespace WODVPN
{
#include "\WeOnlyDo\wodVPN\Code\WIN32LIB\Win32LIB.h"
}

typedef enum WippienState
{
		WipWaitingInitRequest,
		WipWaitingInitResponse,
		WipDisconnected,
		WipConnecting,
		WipNegotiating,
		WipConnected
} WippienState;

class CUser  
{
public:
	CUser();
	virtual ~CUser();

	static long EventStateChange(void *wodVPN, WODVPN::StatesEnum OldState);
	static long EventConnected(void *wodVPN, char * PeerID, char * IP, long Port);
	static long EventDisconnected(void *wodVPN, long ErrorCode, char * ErrorText);
	static long EventSearchDone(void *wodVPN, char * IP, long Port, long ErrorCode, char * ErrorText);
	static long EventIncomingData(void *wodVPN, void *Data, int Len);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL SendNetworkPacket(char *data, int len);
	void Blink(void);

	WODVPN::VPNEventsStruct	m_Events;
	void *m_Handle;
	unsigned long m_RemoteIP;
	MACADDR m_MAC;

	RSA *m_RSA;
	WippienState m_State, m_RemoteState;

	HWND m_hWnd;
	char m_MyKey[16], m_SharedKey[16];
	char m_JID[1024], m_Resource[1024], m_MediatorHost[1024];
	int	 m_HisRandom, m_MyRandom, m_MediatorPort;

	int m_BlinkCounter;

};

#endif