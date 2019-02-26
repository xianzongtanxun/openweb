// LookBackPlayTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define REFTIMES_PER_SEC        10000000
#define REFTIMES_PER_MILLISEC   10000

#define EXIT_ON_ERROR(hres)  \
	if (FAILED(hres)) { goto Exit; }

#define SAFE_RELEASE(punk)  \
	if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator    = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient           = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient     = __uuidof(IAudioRenderClient);

FILE *fp = NULL;

class MyAudioSource {
public:
	int SetFormat(WAVEFORMATEX *pwfx);
	int LoadData(UINT32 numFramesAvailable, char *pData, DWORD *pbDone);
};

int MyAudioSource::SetFormat(WAVEFORMATEX *pwfx)
{
	FILE *fp;
	fopen_s(&fp, "render_format.txt", "w");
	char str[128];
	sprintf_s(str, "wFormatTag \t\tis %x\nnChannels \t\tis %d\nnSamplesPerSec \tis %ld\nnAvgBytesPerSec is %ld\nwBitsPerSample \tis %d",
		pwfx->wFormatTag, pwfx->nChannels, pwfx->nSamplesPerSec, pwfx->nAvgBytesPerSec, pwfx->wBitsPerSample);
	fwrite(str, 1, strlen(str), fp);
	fclose(fp);
	return 0;
}

int MyAudioSource::LoadData(UINT32 numFramesAvailable, char *pData, DWORD *pbDone)
{
	int ret = fread(pData, 1, numFramesAvailable, fp);
	printf("fread returns %d\n", ret);
	if (0 == ret)
		*pbDone = AUDCLNT_BUFFERFLAGS_SILENT;
	return 0;
}

BOOL AdjustFormatTo16Bits(WAVEFORMATEX *pwfx)
{
	BOOL bRet(FALSE);
	if (pwfx->wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
		pwfx->wFormatTag = WAVE_FORMAT_PCM;
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

HRESULT PlayAudioStream(MyAudioSource *pMySource)
{
	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioRenderClient *pRenderClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	BYTE *pData;
	DWORD flags = 0;

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	EXIT_ON_ERROR(hr);

	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	EXIT_ON_ERROR(hr);

	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr);

	AdjustFormatTo16Bits(pwfx);

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0, pwfx, NULL);
	EXIT_ON_ERROR(hr);

	// Tell the audio source which format to use.
	hr = pMySource->SetFormat(pwfx);
	EXIT_ON_ERROR(hr);

	// Get the actual size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr);

	hr = pAudioClient->GetService(IID_IAudioRenderClient, (void**)&pRenderClient);
	EXIT_ON_ERROR(hr);

	// Grab the entire buffer for the initial fill operation.
	hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
	EXIT_ON_ERROR(hr)

		// Load the initial data into the shared buffer.
		hr = pMySource->LoadData(bufferFrameCount * pwfx->nBlockAlign, (char *)pData, &flags);
	EXIT_ON_ERROR(hr);

	hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
	EXIT_ON_ERROR(hr);

	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;

	hr = pAudioClient->Start();  // Start playing.
	EXIT_ON_ERROR(hr);

		// Each loop fills about half of the shared buffer.
		while (flags != AUDCLNT_BUFFERFLAGS_SILENT)
		{
			// Sleep for half the buffer duration.
			Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

			// See how much buffer space is available.
			hr = pAudioClient->GetCurrentPadding(&numFramesPadding);
			EXIT_ON_ERROR(hr);

			numFramesAvailable = bufferFrameCount - numFramesPadding;

			// Grab all the available space in the shared buffer.
			hr = pRenderClient->GetBuffer(numFramesAvailable, &pData);
			EXIT_ON_ERROR(hr);

			// Get next 1/2-second of data from the audio source.
			hr = pMySource->LoadData(numFramesAvailable * pwfx->nBlockAlign, (char *)pData, &flags);
			EXIT_ON_ERROR(hr);

			hr = pRenderClient->ReleaseBuffer(numFramesAvailable, flags);
			EXIT_ON_ERROR(hr);
		}

	// Wait for last data in buffer to play before stopping.
	Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

	hr = pAudioClient->Stop();  // Stop playing.
	EXIT_ON_ERROR(hr);

Exit:
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pAudioClient);
	SAFE_RELEASE(pRenderClient);

	return hr;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//fopen_s(&fp, "foobar.pcm", "rb");
	bool bMic = false;
	if (bMic){
		fopen_s(&fp, "foobarcapture.pcm", "rb");
	}
	else{
		fopen_s(&fp, "foobarrender.pcm", "rb");
	}
	
	MyAudioSource test;
	CoInitialize(NULL);
	PlayAudioStream(&test);
	CoUninitialize();

	fclose(fp);
	return 0;
}

