// VoiceChat.cpp: implementation of the CVoiceChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VoiceChat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVoiceChat::CVoiceChat()
{
	m_hWavIn = NULL;
	m_hWavOut = NULL;
	m_WaveOutDoSend = 0;
	m_WaveOutBusy = 0;
	m_VadThreshold = 500;
	
	m_WaveInStarted = FALSE;
	m_WaveInDevice = WAVE_MAPPER;
	m_WaveOutStarted = FALSE;
	m_WaveOutDevice = WAVE_MAPPER;

	m_sock = INVALID_SOCKET;
	m_Enabled = FALSE;
	m_LocalEcho = FALSE;

	m_PlaybackActivity = m_RecordingActivity = NULL;

	memset(&m_LocalEchoSock, 0, sizeof(m_LocalEchoSock));
	m_LocalEchoSock.sin_port = 47398;
	m_LocalEchoSock.sin_addr.s_addr = INADDR_LOOPBACK;
	m_LocalEchoSock.sin_family = AF_INET;
}

CVoiceChat::~CVoiceChat()
{
	StopListen();
}

BOOL CVoiceChat::StartListen(void)
{
	if (m_sock != INVALID_SOCKET)
	{
		// start to listen
		m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
		SOCKADDR_IN addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_port = 47398; 
		
		addr.sin_addr.s_addr = INADDR_ANY;
		if (bind(m_sock, (SOCKADDR *)&addr, sizeof(addr)))
		{
			closesocket(m_sock);
			m_sock = INVALID_SOCKET;
			return FALSE;
		}
	}
	return TRUE;
}
void CVoiceChat::StopListen(void)
{
	if (m_sock != INVALID_SOCKET)
		closesocket(m_sock);
	m_sock = INVALID_SOCKET;
}

void CVoiceChat::StopWaveIn(void)
{
	if (m_hWavIn)
	{
		m_WaveInStarted = FALSE;
		
		// in
		int i;
		waveInReset(m_hWavIn);
		for(i=0;i<SNDNBUF;i++) 
			waveInUnprepareHeader(m_hWavIn,&m_WaveHdrIn[i], sizeof(WAVEHDR));
		waveInClose(m_hWavIn); 
		m_hWavIn = NULL;
		
		// destroy speex
		speex_bits_destroy(&m_SpeexBitsOut);
		speex_encoder_destroy(&m_SpeexEncStateOut);
	}
}

BOOL CVoiceChat::StopWaveOut(void)
{
	if (m_hWavOut)
	{
		m_WaveOutStarted = FALSE;
		
		int i;
		//		waveOutReset(m_hWavOut);
		for(i=0;i<SNDNBUF;i++) 
			waveOutUnprepareHeader(m_hWavOut,&m_WaveHdrOut[i], sizeof(WAVEHDR));
		waveOutClose(m_hWavOut); 
		m_hWavOut = NULL;
		
		
		// destroy speex
		speex_bits_destroy(&m_SpeexBitsIn);
		speex_decoder_destroy(&m_SpeexDecStateIn);
	}
	return TRUE;
}

long CVoiceChat::StartWaveIn(void)
{
	if (m_WaveInStarted)
		StopWaveIn();
	
	
	// let's start wavein
	WAVEFORMATEX wfx;
	wfx.cbSize = 0;
	//wfx.nSamplesPerSec = 11025;
	wfx.nSamplesPerSec = 8000;
	wfx.nChannels = 1;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.nChannels * (wfx.wBitsPerSample / 8);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nBlockAlign;
	
	// init speex
	speex_bits_init(&m_SpeexBitsOut);
	m_SpeexEncStateOut = speex_encoder_init(&speex_nb_mode);
		
	int ehc=1;
	speex_encoder_ctl(m_SpeexEncStateOut, SPEEX_SET_SAMPLING_RATE, &wfx.nSamplesPerSec);
	speex_encoder_ctl(m_SpeexEncStateOut, SPEEX_SET_VBR, &ehc);
	speex_encoder_ctl(m_SpeexEncStateOut, SPEEX_SET_VAD, &ehc);
	speex_encoder_ctl(m_SpeexEncStateOut, SPEEX_SET_DTX, &ehc);
	
	
	
	int i;
	memset(m_WaveHdrIn, 0,sizeof(WAVEHDR)*SNDNBUF);
	for(i=0;i<SNDNBUF;i++)
	{	
		m_WaveHdrIn[i].lpData = (char *)m_WaveHdrDataIn[i];
		m_WaveHdrIn[i].dwBufferLength =  SPEEX_FRAME_SIZE*sizeof(short);
	}
	
	if (waveInOpen(&m_hWavIn, m_WaveInDevice, &wfx, (unsigned long)waveInProc, (unsigned long)this, CALLBACK_FUNCTION) == MMSYSERR_NOERROR)
	{
		for (int i=0;i<SNDNBUF;i++)
		{
			if (waveInPrepareHeader(m_hWavIn, &m_WaveHdrIn[i], sizeof(WAVEHDR)) == MMSYSERR_NOERROR)
				waveInAddBuffer(m_hWavIn, &m_WaveHdrIn[i], sizeof(WAVEHDR));
		}
		
		if (waveInStart(m_hWavIn) == MMSYSERR_NOERROR)
		{
		}
	}
	m_WaveInStarted = TRUE;
	
	
	return 0; //S_OK
}

long CVoiceChat::StartWaveOut(void)
{
	if (m_hWavOut)
		if (!StopWaveOut())
			return 0;
		
	// let's start waveout
	WAVEFORMATEX wfx;
	wfx.cbSize = 0;
	//wfx.nSamplesPerSec = 11025;
	wfx.nSamplesPerSec = 8000;
	wfx.nChannels = 1;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.nChannels * (wfx.wBitsPerSample / 8);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	// init speex
	speex_bits_init(&m_SpeexBitsIn);
	m_SpeexDecStateIn= speex_decoder_init(&speex_nb_mode);	
		
	int i;
	memset(m_WaveHdrOut, 0,sizeof(WAVEHDR)*SNDNBUF);
	for(i=0;i<SNDNBUF;i++)
	{	
		m_WaveHdrOut[i].lpData = (char *)m_WaveHdrDataOut[i];
		m_WaveHdrOut[i].dwBufferLength =  SPEEX_FRAME_SIZE*sizeof(short);
	}
	if (waveOutOpen(&m_hWavOut, m_WaveOutDevice, &wfx, (unsigned long)waveOutProc, (unsigned long)this, CALLBACK_FUNCTION) == MMSYSERR_NOERROR)
	{
		// all ok
	}
	m_WaveOutStarted = TRUE;
	
	return 0; //S_OK
}

void CVoiceChat::EnumerateInDevices(HWND hCombo)
{
	//	SendDlgItemMessage(hDlg, IDC_INPUTDEVICE, CB_ADDSTRING, 0, 0);
//	SendMessage(hCombo, CB_ADDSTRING, NULL, (LPARAM)"Default input device");
	
	int tot = waveInGetNumDevs();
	for (int i=0;i<tot;i++)
	{
		WAVEINCAPS	waveCaps = {0};
		if(waveInGetDevCaps(i,&waveCaps,sizeof(waveCaps)) == MMSYSERR_NOERROR)
		{
			SendMessage(hCombo, CB_ADDSTRING, NULL, (LPARAM)waveCaps.szPname);
		}
	}
	
//	SendMessage(hCombo, CB_SETCURSEL, 0, 0);
}

void CVoiceChat::EnumerateOutDevices(HWND hCombo)
{
//	SendMessage(hCombo, CB_ADDSTRING, NULL, (LPARAM)"Default output device");
	
	int tot = waveOutGetNumDevs();
	for (int i=0;i<tot;i++)
	{
		WAVEOUTCAPS	waveCaps = {0};
		if(waveOutGetDevCaps(i,&waveCaps,sizeof(waveCaps)) == MMSYSERR_NOERROR)
		{
			SendMessage(hCombo, CB_ADDSTRING, NULL, (LPARAM)waveCaps.szPname);
		}
	}
	
//	SendMessage(hCombo, CB_SETCURSEL, 0, 0);
//	DWORD vol = 0;
//	waveOutGetVolume((HWAVEOUT)m_WaveOutDevice, &vol);
//	SendDlgItemMessage(hDlg, IDC_OUTPUTDEVICE, TBM_SETPOS, 0, HIWORD(vol)/4096);
}

void CVoiceChat::waveInProc(HWAVEIN hwi,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2)
{
	CVoiceChat *me = (CVoiceChat *)dwInstance;

	WAVEHDR* curhdr = (WAVEHDR*)dwParam1;
	switch (uMsg)
	{
		case WIM_CLOSE:
			break;
		
		case WIM_OPEN:
			break;
		
		case WIM_DATA:
			if (me->m_WaveInStarted)
			{		
				float floatBuffer[SPEEX_FRAME_SIZE];
				char outbuff[8192];
				signed short *databuf = (signed short *)curhdr->lpData;
				size_t i;
				
				int thr = 0;
				for (i = 0; i<SPEEX_FRAME_SIZE;i++)
				{
					if (databuf[i]>me->m_VadThreshold || databuf[i]<(- me->m_VadThreshold))
						thr++;
					floatBuffer[i] = databuf[i];
				}

				if (me->m_PlaybackActivity)
					SendMessage(me->m_PlaybackActivity, PBM_SETPOS, thr/16, 0);
				
				waveInAddBuffer(me->m_hWavIn, (LPWAVEHDR)dwParam1, sizeof(WAVEHDR));
				if (me->m_WaveOutDoSend || thr)
				{
					if (thr)
						me->m_WaveOutDoSend = 30;
					me->m_WaveOutDoSend--;
					speex_bits_reset(&me->m_SpeexBitsOut);
					speex_encode(me->m_SpeexEncStateOut, floatBuffer, &me->m_SpeexBitsOut);
					int total = speex_bits_write(&me->m_SpeexBitsOut, outbuff, sizeof(outbuff));
					
					
					if (me->m_LocalEcho)
						sendto(me->m_sock, outbuff, total, 0, (struct sockaddr *)&me->m_LocalEchoSock, sizeof(SOCKADDR_IN));
				}
			}
			break;
	}	
}

void CVoiceChat::waveOutProc(HWAVEOUT hwo,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2)
{
	CVoiceChat *me = (CVoiceChat *)dwInstance;

	WAVEHDR* curhdr = (WAVEHDR*)dwParam1;
	switch (uMsg)
	{
		case WOM_CLOSE:
			break;
		
		case WOM_OPEN:
			break;
		
		case WOM_DONE:
			waveOutUnprepareHeader(me->m_hWavOut,curhdr, sizeof(WAVEHDR));
			curhdr->dwUser = FALSE;
			me->m_WaveOutBusy--;
			break;
	}
}