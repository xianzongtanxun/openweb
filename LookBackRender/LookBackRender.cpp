// LookBackMode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#pragma comment(lib,"WINMM.lib")
#include <iostream>
using namespace std;
// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
	if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
	if ((punk) != NULL)  \
				{ (punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

class MyAudioSink {
public:
	MyAudioSink(FILE* pFile){
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

HRESULT RecordAudioStream(MyAudioSink *pMySink)
{
	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioCaptureClient *pCaptureClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	UINT32 packetLength = 0;
	BOOL bDone = FALSE;
	BYTE *pData;
	DWORD flags;

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL,CLSCTX_ALL, IID_IMMDeviceEnumerator,(void**)&pEnumerator);
	EXIT_ON_ERROR(hr);

	hr = pEnumerator->GetDefaultAudioEndpoint(eCapture/*eRender*/, eConsole, &pDevice);
	EXIT_ON_ERROR(hr);

	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL,NULL, (void**)&pAudioClient);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,	0,hnsRequestedDuration,	0,pwfx,	NULL);
	EXIT_ON_ERROR(hr);

	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetService(IID_IAudioCaptureClient,(void**)&pCaptureClient);
	EXIT_ON_ERROR(hr);

	// Notify the audio sink which format to use.
	hr = pMySink->SetFormat(pwfx);
	EXIT_ON_ERROR(hr);

	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;

	hr = pAudioClient->Start();  // Start recording.
	EXIT_ON_ERROR(hr);

	// Each loop fills about half of the shared buffer.
	while (bDone == FALSE)
	{
		// Sleep for half the buffer duration.
		Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);

		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		EXIT_ON_ERROR(hr);
		while (packetLength != 0)
		{
			// Get the available data in the shared buffer.
			hr = pCaptureClient->GetBuffer(&pData,&numFramesAvailable,&flags, NULL, NULL);
			EXIT_ON_ERROR(hr);

			//if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
			//	pData = NULL;  // Tell CopyData to write silence.
			//}

			// Copy the available capture data to the audio sink.
			hr = pMySink->CopyData((char*)pData, numFramesAvailable, &bDone);
			EXIT_ON_ERROR(hr);

			hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
			EXIT_ON_ERROR(hr);

			hr = pCaptureClient->GetNextPacketSize(&packetLength);
			EXIT_ON_ERROR(hr);
		}
	}
	hr = pAudioClient->Stop();  // Stop recording.
	EXIT_ON_ERROR(hr);

Exit:
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pAudioClient);
	SAFE_RELEASE(pCaptureClient);

	return hr;
}

HRESULT RecordAudioStreamEx(MyAudioSink *pMySink, bool bMic)
{
	HRESULT             hr;
	REFERENCE_TIME      hnsActualDuration;
	UINT32              bufferFrameCount;
	UINT32              numFramesAvailable;
	BYTE *              pData;
	DWORD               flags;
	REFERENCE_TIME      hnsDefaultDevicePeriod(0);

	REFERENCE_TIME          hnsRequestedDuration = REFTIMES_PER_SEC;
	IMMDeviceEnumerator*    pEnumerator = NULL;
	IMMDevice *             pDevice = NULL;
	IAudioClient *          pAudioClient = NULL;
	IAudioCaptureClient *   pCaptureClient = NULL;
	WAVEFORMATEX *          pwfx = NULL;
	UINT32                  packetLength = 0;
	BOOL                    bDone = FALSE;
	HANDLE                  hTimerWakeUp = NULL;

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	EXIT_ON_ERROR(hr);

	hr = pEnumerator->GetDefaultAudioEndpoint(bMic ? eCapture : eRender, eConsole, &pDevice);
	EXIT_ON_ERROR(hr);

	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr);

	AdjustFormatTo16Bits(pwfx);

	hTimerWakeUp = CreateWaitableTimer(NULL, FALSE, NULL);

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, bMic ? 0 : AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0, pwfx, NULL);

	EXIT_ON_ERROR(hr);

	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
	EXIT_ON_ERROR(hr);

	LARGE_INTEGER liFirstFire;
	liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // negative means relative time
	LONG lTimeBetweenFires = (LONG)hnsDefaultDevicePeriod / 2 / (10 * 1000); // convert to milliseconds

	BOOL bOK = SetWaitableTimer(hTimerWakeUp, &liFirstFire, lTimeBetweenFires, NULL, NULL, FALSE);

	// Notify the audio sink which format to use.
	hr = pMySink->SetFormat(pwfx);
	EXIT_ON_ERROR(hr);

	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;

	/*************************************************************/
	hr = pAudioClient->Start();  // Start recording.
	EXIT_ON_ERROR(hr);
	HANDLE waitArray[1] = { /*htemp hEventStop,*/ hTimerWakeUp };

	// Each loop fills about half of the shared buffer.
	while (bDone == FALSE)
	{
		// Sleep for half the buffer duration.
		//Sleep(hnsActualDuration/REFTIMES_PER_MILLISEC/2);
		//WaitForSingleObject(hTimerWakeUp,INFINITE);
		WaitForMultipleObjects(sizeof(waitArray) / sizeof(waitArray[0]), waitArray, FALSE, INFINITE);
		//WaitForMultipleObjects(sizeof(waitArray)/sizeof(waitArray[0]), waitArray, FALSE, INFINITE);

		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		EXIT_ON_ERROR(hr);
		while (packetLength != 0)
		{
			// Get the available data in the shared buffer.
			hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
			EXIT_ON_ERROR(hr);
			//静音状态下设置
			if (flags & AUDCLNT_BUFFERFLAGS_SILENT){
			  pData = NULL;  // Tell CopyData to write silence.
			}
			// Copy the available capture data to the audio sink.
			hr = pMySink->CopyData((char *)pData, numFramesAvailable * pwfx->nBlockAlign, &bDone);
			EXIT_ON_ERROR(hr);

			hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
			EXIT_ON_ERROR(hr);

			hr = pCaptureClient->GetNextPacketSize(&packetLength);
			EXIT_ON_ERROR(hr);
		}
	}
	hr = pAudioClient->Stop();  // Stop recording.
	EXIT_ON_ERROR(hr);

Exit:
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pAudioClient);
	SAFE_RELEASE(pCaptureClient);

	return hr;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool bMic = true; //true：mic  false：speaker
	CoInitialize(NULL);
	FILE *fp = NULL;
	if (bMic){
		fopen_s(&fp, "foobarcapture.pcm", "wb");
		cout << "捕获麦克风音频数据..." << endl;
	}
	else{
		fopen_s(&fp, "foobarrender.pcm", "wb");
		cout << "捕获扬声器音频数据..." << endl;
	}
	MyAudioSink test(fp);
	//RecordAudioStream(&test);
	RecordAudioStreamEx(&test, bMic);
	CoUninitialize();

	fclose(fp);
	system("pause");
	return 0;
}