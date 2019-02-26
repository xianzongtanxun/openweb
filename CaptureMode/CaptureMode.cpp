// CaptureMode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#pragma comment(lib,"WINMM.lib")

#define REFTIMES_PER_SEC      10000000
#define REFTIMES_PER_MILLISEC 10000

#define EXIT_ON_ERROR(hres) \
			if (FAILED(hres)){ goto Exit;}
#define SAFE_RELEASE(punk) \
			if ((punk) != NULL) \
			{	(punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator    = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient           = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient    = __uuidof(IAudioCaptureClient);

class MyAudioSink {
public:
	MyAudioSink(FILE* pFile) {
		m_pFile = pFile;
	}
	int SetFormat(WAVEFORMATEX *pwfx);
	int CopyData(char *pData, UINT32 numFramesAvailable, BOOL *pbDone);
private:
	FILE* m_pFile;
};

int MyAudioSink::SetFormat(WAVEFORMATEX *pwfx)
{
	printf("wFormatTag is %x\n", pwfx->wFormatTag);
	printf("nChannels is %x\n", pwfx->nChannels);
	printf("nSamplesPerSec is %d\n", pwfx->nSamplesPerSec);
	printf("nAvgBytesPerSec is %d\n", pwfx->nAvgBytesPerSec);
	printf("wBitsPerSample is %d\n", pwfx->wBitsPerSample);
	return 0;
}

int MyAudioSink::CopyData(char *pData, UINT32 numFramesAvailable, BOOL *pbDone)
{
	if (pData != NULL && m_pFile != nullptr)
	{
		fwrite(pData, numFramesAvailable, 1, m_pFile);
	}
	return 0;
}

// pwfx->nSamplesPerSec = 44100;
BOOL AdjustFormatTo16Bits(WAVEFORMATEX *pwfx)
{
	BOOL bRet(FALSE);
	if (pwfx->wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
	{
		pwfx->wFormatTag = WAVE_FORMAT_PCM;
	}
	else if (pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
	{
		PWAVEFORMATEXTENSIBLE pEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pwfx);
		if (IsEqualGUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, pEx->SubFormat))
		{
			pEx->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
			pEx->Samples.wValidBitsPerSample = 16;
		}
	}
	else
		return bRet;
	pwfx->wBitsPerSample = 16;
	pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
	pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;
	bRet = TRUE;
	return bRet;
}

//pwfx->nSamplesPerSec = 44100;
HRESULT RecordAudioStream(MyAudioSink *pMySink)
{
	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	REFERENCE_TIME hnsDefaultDevicePeriod(0);
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient* pAudioClient = NULL;
	IAudioCaptureClient* pCaptureClient = NULL;
	WAVEFORMATEX* pwfx = NULL;
	UINT32 packetLength = 0;
	BOOL bDone = FALSE;
	BYTE* pData;
	DWORD flags;
	HANDLE hTimerWakeUp = nullptr;

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	EXIT_ON_ERROR(hr);

	hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);
	EXIT_ON_ERROR(hr);

	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr);

	AdjustFormatTo16Bits(pwfx);

	hTimerWakeUp = CreateWaitableTimer(NULL, FALSE, NULL);

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 0, 0, pwfx, NULL);
	EXIT_ON_ERROR(hr);

	//Get the size of the allocated buffer
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
	EXIT_ON_ERROR(hr);

	LARGE_INTEGER liFirstFire;
	liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // negative means relative time
	LONG lTimeBetweenFires = (LONG)hnsDefaultDevicePeriod / 2 / (10 * 1000); // convert to milliseconds

	BOOL bOK = SetWaitableTimer(hTimerWakeUp, &liFirstFire, lTimeBetweenFires, NULL, NULL, FALSE);

	//notify the audio sink which format to use
	hr = pMySink->SetFormat(pwfx);
	EXIT_ON_ERROR(hr);

	//Calculate the actual duration of the allocated buffer
	hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;
	
	hr = pAudioClient->Start();//Start recording
	EXIT_ON_ERROR(hr);

	HANDLE waitArray[1] = { /*htemp hEventStop,*/ hTimerWakeUp };
	//Each loop fills about half of the shared buffer
	while (bDone == FALSE)
	{
		//Sleep for half the buffer duration
		//Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);
		WaitForMultipleObjects(sizeof(waitArray) / sizeof(waitArray[0]), waitArray, FALSE, INFINITE);

		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		EXIT_ON_ERROR(hr);

		while (packetLength != 0)
		{
			//Get the available data in the shared buffer
			hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
			EXIT_ON_ERROR(hr);
			//if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
			//{
			//	pData = NULL;//Tell copyData to write silence
			//}
			hr = pMySink->CopyData((char*)pData, numFramesAvailable, &bDone);
			EXIT_ON_ERROR(hr);
			hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
			EXIT_ON_ERROR(hr);
			hr = pCaptureClient->GetNextPacketSize(&packetLength);
			EXIT_ON_ERROR(hr);
		}
	}
	hr = pAudioClient->Stop();//Stop recording
	EXIT_ON_ERROR(hr);

Exit:
	CoTaskMemFree(pwfx);
	if (pEnumerator){
		pEnumerator->Release();
		pEnumerator = NULL;
	}
	if (pDevice) {
		pDevice->Release();
		pDevice = NULL;
	}
	if (pAudioClient) {
		pAudioClient->Release();
		pAudioClient = NULL;
	}
	if (pCaptureClient) {
		pCaptureClient->Release();
		pCaptureClient = NULL;
	}
	return true;
}

int main()
{
	FILE *fp = NULL;
	::CoInitialize(NULL);

	fopen_s(&fp, "foobarcapture.pcm", "wb");
	MyAudioSink test(fp);
	RecordAudioStream(&test);

	::CoUninitialize();

	fclose(fp);
	system("pause");
	return 0;
}

