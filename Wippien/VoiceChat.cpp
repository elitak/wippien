// VoiceChat.cpp: implementation of the CVoiceChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VoiceChat.h"


#define SAMPLES_PER_SEC	8000
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVoiceChat::CVoiceChat()
{
	m_hWavIn = NULL;
	m_hWavOut = NULL;
	m_WaveOutDoSend = 0;
	m_WaveOutBusy = 0;
	m_VadThreshold = 0;
	
	m_WaveInStarted = FALSE;
	m_WaveInDevice = WAVE_MAPPER;
	m_WaveOutStarted = FALSE;
	m_WaveOutDevice = WAVE_MAPPER;

	m_sock = SOCKET_ERROR;
	m_Enabled = FALSE;
	m_LocalEcho = FALSE;

	m_PlaybackActivity = m_RecordingActivity = NULL;

	memset(&m_OutSock, 0, sizeof(m_OutSock));
	m_OutSock.sin_port = 47398;
	m_OutSock.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	m_OutSock.sin_family = AF_INET;



	// init speex
	speex_bits_init(&m_SpeexBitsOut);
	m_SpeexEncStateOut = speex_encoder_init(&speex_nb_mode);
	
	int ehc=1;
	int sps = SAMPLES_PER_SEC;
	speex_encoder_ctl(m_SpeexEncStateOut, SPEEX_SET_SAMPLING_RATE, &sps);
	speex_encoder_ctl(m_SpeexEncStateOut, SPEEX_SET_VBR, &ehc);
	speex_encoder_ctl(m_SpeexEncStateOut, SPEEX_SET_VAD, &ehc);
	speex_encoder_ctl(m_SpeexEncStateOut, SPEEX_SET_DTX, &ehc);

	// init speex
	speex_bits_init(&m_SpeexBitsIn);
	m_SpeexDecStateIn= speex_decoder_init(&speex_nb_mode);	
	
}

CVoiceChat::~CVoiceChat()
{
	StopListen();
	
	// destroy speex
	speex_bits_destroy(&m_SpeexBitsOut);
//	speex_encoder_destroy(&m_SpeexEncStateOut);


	// destroy speex
	speex_bits_destroy(&m_SpeexBitsIn);
//	speex_decoder_destroy(&m_SpeexDecStateIn);
}

void CVoiceChat::FdReceive(int nErrorCode)
{
	char buff[8192];
	SOCKADDR_IN addr = {0};
	int addrlen = sizeof(addr);
	int i = ::recvfrom(m_sock, buff, sizeof(buff), 0, (struct sockaddr *)&addr, &addrlen);
	if (i>0)
	{
		if (m_WaveOutStarted)
		{
			int j;
			for (j=0;j<SNDNBUF;j++)
			{
				if (!m_WaveHdrOut[j].dwUser)
				{
					m_WaveHdrOut[j].dwUser = TRUE;
					float floatBuffer[SPEEX_FRAME_SIZE];
					signed short *databuf = (signed short *)m_WaveHdrOut[j].lpData;
					
					// decompress
					speex_bits_read_from(&m_SpeexBitsIn, buff+1, i-1);
					speex_decode(m_SpeexDecStateIn, &m_SpeexBitsIn, floatBuffer);
					
					int thr = 0;
					for (i = 0; i<SPEEX_FRAME_SIZE;i++)
					{
						if (databuf[i]>m_VadThreshold || databuf[i]<(- m_VadThreshold))
							thr++;
						databuf[i] = (short)floatBuffer[i];
					}
					
					if (m_PlaybackActivity)
						PostMessage(m_PlaybackActivity, PBM_SETPOS, thr/16, 0);
					
					m_WaveOutBusy++;
					//ATLTRACE("Before waveOutPrepareHeader\r\n");
//					if (waveOutPrepareHeader(m_hWavOut, &m_WaveHdrOut[j], sizeof(WAVEHDR)) == MMSYSERR_NOERROR)
						waveOutWrite(m_hWavOut, &m_WaveHdrOut[j], sizeof(WAVEHDR));
					//ATLTRACE("After waveOutPrepareHeader\r\n");
					break;
				}
			}
		}
	}
}

BOOL CVoiceChat::StartListen(void)
{
	if (m_sock == SOCKET_ERROR)
	{
		// start to listen
		m_sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
		SOCKADDR_IN addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_port = 47398; 
		
		addr.sin_addr.s_addr = INADDR_ANY;
		if (::bind(m_sock, (SOCKADDR *)&addr, sizeof(addr)))
		{
			::closesocket(m_sock);
			m_sock = SOCKET_ERROR;
			return FALSE;
		}
		AsyncSelect(FD_READ);
	}
	return TRUE;
}
void CVoiceChat::StopListen(void)
{
	if (m_sock != SOCKET_ERROR)
		::closesocket(m_sock);
	m_sock = SOCKET_ERROR;
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
	wfx.nSamplesPerSec = SAMPLES_PER_SEC;
	wfx.nChannels = 1;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.nChannels * (wfx.wBitsPerSample / 8);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nBlockAlign;
		

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
			//ATLTRACE("Before waveInPrepareHeader\r\n");
			if (waveInPrepareHeader(m_hWavIn, &m_WaveHdrIn[i], sizeof(WAVEHDR)) == MMSYSERR_NOERROR)
				waveInAddBuffer(m_hWavIn, &m_WaveHdrIn[i], sizeof(WAVEHDR));
			//ATLTRACE("After waveInPrepareHeader\r\n");
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
	wfx.nSamplesPerSec = SAMPLES_PER_SEC;
	wfx.nChannels = 1;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.nChannels * (wfx.wBitsPerSample / 8);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

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
		for (int i=0;i<SNDNBUF;i++)
		{
			//ATLTRACE("Before waveOutPrepareHeader\r\n");
			waveOutPrepareHeader(m_hWavOut, &m_WaveHdrOut[i], sizeof(WAVEHDR));
			//ATLTRACE("After waveOutPrepareHeader\r\n");
		}
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

				if (me->m_RecordingActivity)
					PostMessage(me->m_RecordingActivity, PBM_SETPOS, thr/16, 0);
				
				//ATLTRACE("Before waveInAddBuffer\r\n");
				waveInAddBuffer(me->m_hWavIn, (LPWAVEHDR)dwParam1, sizeof(WAVEHDR));
				//ATLTRACE("After waveInAddBuffer\r\n");
				if (me->m_WaveOutDoSend || thr)
				{
					if (thr)
						me->m_WaveOutDoSend = 30;
					me->m_WaveOutDoSend--;
					speex_bits_reset(&me->m_SpeexBitsOut);
					speex_encode(me->m_SpeexEncStateOut, floatBuffer, &me->m_SpeexBitsOut);
					int total = speex_bits_write(&me->m_SpeexBitsOut, outbuff+1, sizeof(outbuff)-1);
					
					outbuff[0] = 0; 
					total++;

					for (i=0;i<me->m_Users.size();i++)
					{
						CUser *u = (CUser *)me->m_Users[i];
						if (u->m_WippienState == WipConnected)
						{
							me->m_OutSock.sin_addr.s_addr = u->m_HisVirtualIP;
							::sendto(me->m_sock, outbuff, total, 0, (struct sockaddr *)&me->m_OutSock, sizeof(SOCKADDR_IN));
						}
					}
					if (me->m_LocalEcho)
					{
						me->m_OutSock.sin_addr.s_addr = 0x0100007f; // 127.0.0.1;
						::sendto(me->m_sock, outbuff, total, 0, (struct sockaddr *)&me->m_OutSock, sizeof(SOCKADDR_IN));
					}
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
	//		waveOutUnprepareHeader(me->m_hWavOut,curhdr, sizeof(WAVEHDR));
			curhdr->dwUser = FALSE;
			me->m_WaveOutBusy--;
			break;
	}
}