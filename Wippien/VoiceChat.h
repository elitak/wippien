// VoiceChat.h: interface for the CVoiceChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOICECHAT_H__1A5846A8_BA93_4FE5_B08A_3D1166DF939C__INCLUDED_)
#define AFX_VOICECHAT_H__1A5846A8_BA93_4FE5_B08A_3D1166DF939C__INCLUDED_

#include <Mmsystem.h>
#include "../speex-1.0.5/include/speex/speex.h"
#include "CwodWinSocket.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SNDNBUF	20
#define SPEEX_FRAME_SIZE	160

enum VoiceChatPktTypeStruct
{
	VoicePktData,
	VoicePktSetCodec,
	VoicePktOnline,
	VoicePktOffline
};

//#include <vector>
#include "User.h"
class CUser;

class CVoiceChat  : public CwodWinSocket
{
public:
	CVoiceChat();
	virtual ~CVoiceChat();

	BOOL m_Enabled, m_LocalEcho;

	HWAVEIN m_hWavIn;
	HWAVEOUT m_hWavOut;
	WAVEHDR m_WaveHdrIn[SNDNBUF],m_WaveHdrOut[SNDNBUF];
	signed short m_WaveHdrDataIn[SNDNBUF][SPEEX_FRAME_SIZE], m_WaveHdrDataOut[SNDNBUF][SPEEX_FRAME_SIZE];

	int m_WaveOutDoSend, m_WaveOutBusy;
	int m_VadThreshold;

	float m_TempSlot[SPEEX_FRAME_SIZE];
	volatile long m_TempSlotCtr;
	HANDLE m_ThreadHandle, m_PlayHandle;
	BOOL m_Die;

	HWND m_PlaybackActivity, m_RecordingActivity;

	SOCKADDR_IN m_OutSock;

	BOOL m_WaveInStarted;
	int m_WaveInDevice;
	BOOL m_WaveOutStarted;
	int m_WaveOutDevice;


	SpeexBits m_SpeexBitsOut,m_SpeexBitsIn;
	void *m_SpeexEncStateOut, *m_SpeexDecStateIn;

	//std::vector <CUser *> m_Users;

	void FdReceive(int nErrorCode);
	CUser *FindUserByNetaddr(unsigned long addr);
	void NotifyUserStatus(CUser *user, BOOL isonline);


	BOOL StartListen(void);
	void StopListen(void);
	long StartWaveIn(void);
	void StopWaveIn(void);
	long StartWaveOut(void);
	BOOL StopWaveOut(void);
	
	void EnumerateInDevices(HWND hDlg);
	void EnumerateOutDevices(HWND hDlg);

	static void CALLBACK waveInProc(HWAVEIN hwi,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);
	static void CALLBACK waveOutProc(HWAVEOUT hwo,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);

};

#endif // !defined(AFX_VOICECHAT_H__1A5846A8_BA93_4FE5_B08A_3D1166DF939C__INCLUDED_)
