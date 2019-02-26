// LookBackMode.cpp : 定义控制台应用程序的入口点。
//

#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#pragma comment(lib,"WINMM.lib")

class MyAudioSink {
public:
	MyAudioSink(FILE* pFile){
		m_pFile = pFile;
	}
	~MyAudioSink() {
		m_mutex.lock();
		m_pFile = nullptr;
		m_mutex.unlock();
	}

	int SetFormat(WAVEFORMATEX *pwfx);
	int CopyData(char *pData, UINT32 numFramesAvailable, BOOL *pbDone);
private:
	FILE* m_pFile;
	std::mutex m_mutex;
};

class CSoundCaptureHandler
{
public:
	CSoundCaptureHandler();
	~CSoundCaptureHandler();
	bool Initialize(MyAudioSink *pMySink, bool bMic);
	bool StartRecord();
	bool StopRecord();
	void ReleaseAll();
	void ThRunRecord();
private:
	BOOL AdjustFormatTo16Bits(WAVEFORMATEX *pwfx);
private:
	MyAudioSink*         m_pMySink;
	bool                 m_bMic;
	BOOL                 m_bDone;

	IMMDeviceEnumerator* pEnumerator;
	IAudioCaptureClient* pCaptureClient;
	IMMDevice*           pDevice;
	IAudioClient*        pAudioClient;
	WAVEFORMATEX*        pwfx;

	HANDLE               hTimerWakeUp;
	REFERENCE_TIME       hnsActualDuration;
	std::mutex           m_mutex;
};

class CSoundRenderHandler
{
public:
	CSoundRenderHandler();
	~CSoundRenderHandler();
	bool Initialize(MyAudioSink *pMySink, bool bMic);
	bool StartRecord();
	bool StopRecord();
	void ReleaseAll();
	void ThRunRecord();
private:
	BOOL AdjustFormatTo16Bits(WAVEFORMATEX *pwfx);
private:
	MyAudioSink*         m_pMySink;
	bool                 m_bMic;
	BOOL                 m_bDone;

	IMMDeviceEnumerator* pEnumerator;
	IAudioCaptureClient* pCaptureClient;
	IMMDevice*           pDevice;
	IAudioClient*        pAudioClient;
	WAVEFORMATEX*        pwfx;

	HANDLE               hTimerWakeUp;
	REFERENCE_TIME       hnsActualDuration;
	std::mutex           m_mutex;
};