// LookBackMode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "sound_handler.h"
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
	if (pData != NULL && m_pFile != nullptr){
		m_mutex.lock();
		fwrite(pData, numFramesAvailable, 1, m_pFile);
		m_mutex.unlock();
	}
	else{
		*pbDone = TRUE;
	}
	return 0;
}

CSoundCaptureHandler::CSoundCaptureHandler()
{
	m_bMic = true;
	m_bDone = FALSE;
	pwfx = nullptr;
}

CSoundCaptureHandler::~CSoundCaptureHandler()
{

}

bool CSoundCaptureHandler::Initialize(MyAudioSink *pMySink, bool bMic)
{
	m_pMySink = pMySink;
	m_bMic = bMic;
	if (hTimerWakeUp) {
		CloseHandle(hTimerWakeUp);
		hTimerWakeUp = nullptr;
	}
	ReleaseAll();
	HRESULT             hr;
	UINT32              bufferFrameCount;
	REFERENCE_TIME      hnsDefaultDevicePeriod(0);

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	if (!pEnumerator){
		return false;
	}
	hr = pEnumerator->GetDefaultAudioEndpoint(bMic ? eCapture : eRender, eConsole, &pDevice);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	if (!pDevice) {
		return false;
	}
	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	if (!pAudioClient) {
		return false;
	}
	hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	hr = pAudioClient->GetMixFormat(&pwfx);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	AdjustFormatTo16Bits(pwfx);
	hTimerWakeUp = CreateWaitableTimer(NULL, FALSE, NULL);
	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, m_bMic ? 0 : AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0, pwfx, NULL);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	hr = pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
	if (FAILED(hr)) {
		ReleaseAll();
	}

	LARGE_INTEGER liFirstFire;
	liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // negative means relative time
	LONG lTimeBetweenFires = (LONG)hnsDefaultDevicePeriod / 2 / (10 * 1000); // convert to milliseconds

	BOOL bOK = SetWaitableTimer(hTimerWakeUp, &liFirstFire, lTimeBetweenFires, NULL, NULL, FALSE);

	// Notify the audio sink which format to use.
	hr = pMySink->SetFormat(pwfx);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;
	return true;
}

bool CSoundCaptureHandler::StartRecord()
{
	HRESULT hr;
	if (!pAudioClient) {
		return false;
	}
	m_mutex.lock();
	m_bDone = FALSE;
	m_mutex.unlock();
	hr = pAudioClient->Start();  // Start recording.
	if (FAILED(hr)) {
		ReleaseAll();
		return false;
	}
	std::thread th(&CSoundCaptureHandler::ThRunRecord, this);
	th.detach();
	return true;
}

void CSoundCaptureHandler::ThRunRecord()
{
	HRESULT             hr;
	UINT32              numFramesAvailable;
	BYTE *              pData;
	DWORD               flags;
	UINT32              packetLength = 0;
	HANDLE waitArray[1] = { /*htemp hEventStop,*/ hTimerWakeUp };
	if (!pCaptureClient) {
		return;
	}
	// Each loop fills about half of the shared buffer.
	while (TRUE)
	{
		if (m_bDone == TRUE) {
			break;
		}
		WaitForMultipleObjects(sizeof(waitArray) / sizeof(waitArray[0]), waitArray, FALSE, INFINITE);
		if (m_bDone == TRUE) {
			break;
		}
		if (!pCaptureClient){
			ReleaseAll();
			break;
		}
		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		if (FAILED(hr)) {
			ReleaseAll();
			break;
		}
		while (packetLength != 0)
		{
			// Get the available data in the shared buffer.
			if (!pCaptureClient){
				ReleaseAll();
				break;
			}
			hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
			if (FAILED(hr)) {
				ReleaseAll();
				break;
			}
			if (!m_pMySink) {
				ReleaseAll();
				break;
			}
			// Copy the available capture data to the audio sink.
			hr = m_pMySink->CopyData((char *)pData, numFramesAvailable * pwfx->nBlockAlign, &m_bDone);
			if (!pData || m_bDone == TRUE) {
				break;
			}
			if (FAILED(hr)) {
				ReleaseAll();
				break;
			}
			if (!pCaptureClient) {
				ReleaseAll();
				break;
			}
			hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
			if (FAILED(hr)) {
				ReleaseAll();
				break;
			}
			if (!pCaptureClient) {
				ReleaseAll();
				break;
			}
			hr = pCaptureClient->GetNextPacketSize(&packetLength);
			if (FAILED(hr)) {
				ReleaseAll();
				break;
			}
		}
	}
}

bool CSoundCaptureHandler::StopRecord()
{
	if (hTimerWakeUp) {
		CloseHandle(hTimerWakeUp);
		hTimerWakeUp = nullptr;
	}
	HRESULT hr;
	if (pAudioClient){
		hr = pAudioClient->Stop();  // Stop recording.
		if (FAILED(hr)) {
			ReleaseAll();
			return false;
		}
	}
	m_mutex.lock();
	m_bDone = TRUE;
	m_mutex.unlock();
	
	ReleaseAll();
	//m_pMySink = nullptr;
	return true;
}

void CSoundCaptureHandler::ReleaseAll()
{
	if (pwfx)
	{
		CoTaskMemFree(pwfx);
		pwfx = nullptr;
	}
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pAudioClient);
	SAFE_RELEASE(pCaptureClient);
}

// pwfx->nSamplesPerSec = 44100;
BOOL CSoundCaptureHandler::AdjustFormatTo16Bits(WAVEFORMATEX *pwfx)
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

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
CSoundRenderHandler::CSoundRenderHandler()
{
	m_bMic = true;
	m_bDone = FALSE;
	pwfx = nullptr;
}

CSoundRenderHandler::~CSoundRenderHandler()
{

}

bool CSoundRenderHandler::Initialize(MyAudioSink *pMySink, bool bMic)
{
	m_pMySink = pMySink;
	m_bMic = bMic;
	if (hTimerWakeUp) {
		CloseHandle(hTimerWakeUp);
		hTimerWakeUp = nullptr;
	}
	ReleaseAll();
	HRESULT             hr;
	UINT32              bufferFrameCount;
	REFERENCE_TIME      hnsDefaultDevicePeriod(0);

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	if (!pEnumerator) {
		return false;
	}
	hr = pEnumerator->GetDefaultAudioEndpoint(bMic ? eCapture : eRender, eConsole, &pDevice);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	if (!pDevice) {
		return false;
	}
	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	if (!pAudioClient) {
		return false;
	}
	hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	hr = pAudioClient->GetMixFormat(&pwfx);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	AdjustFormatTo16Bits(pwfx);
	hTimerWakeUp = CreateWaitableTimer(NULL, FALSE, NULL);
	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, m_bMic ? 0 : AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0, pwfx, NULL);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	hr = pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
	if (FAILED(hr)) {
		ReleaseAll();
	}

	LARGE_INTEGER liFirstFire;
	liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // negative means relative time
	LONG lTimeBetweenFires = (LONG)hnsDefaultDevicePeriod / 2 / (10 * 1000); // convert to milliseconds

	BOOL bOK = SetWaitableTimer(hTimerWakeUp, &liFirstFire, lTimeBetweenFires, NULL, NULL, FALSE);

	// Notify the audio sink which format to use.
	hr = pMySink->SetFormat(pwfx);
	if (FAILED(hr)) {
		ReleaseAll();
	}
	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;
	return true;
}

bool CSoundRenderHandler::StartRecord()
{
	HRESULT hr;
	if (!pAudioClient) {
		return false;
	}
	m_mutex.lock();
	m_bDone = FALSE;
	m_mutex.unlock();
	hr = pAudioClient->Start();  // Start recording.
	if (FAILED(hr)) {
		ReleaseAll();
		return false;
	}
	std::thread th(&CSoundRenderHandler::ThRunRecord, this);
	th.detach();
	return true;
}

void CSoundRenderHandler::ThRunRecord()
{
	HRESULT             hr;
	UINT32              numFramesAvailable;
	BYTE *              pData;
	DWORD               flags;
	UINT32              packetLength = 0;
	HANDLE waitArray[1] = { /*htemp hEventStop,*/ hTimerWakeUp };
	if (!pCaptureClient) {
		return;
	}
	// Each loop fills about half of the shared buffer.
	while (TRUE)
	{
		if (m_bDone == TRUE) {
			break;
		}
		WaitForMultipleObjects(sizeof(waitArray) / sizeof(waitArray[0]), waitArray, FALSE, INFINITE);
		if (m_bDone == TRUE) {
			break;
		}
		if (!pCaptureClient) {
			ReleaseAll();
			break;
		}
		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		if (FAILED(hr)) {
			ReleaseAll();
			break;
		}
		while (packetLength != 0)
		{
			// Get the available data in the shared buffer.
			if (!pCaptureClient) {
				ReleaseAll();
				break;
			}
			hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
			if (FAILED(hr)) {
				ReleaseAll();
				break;
			}
			// Copy the available capture data to the audio sink.
			if (!m_pMySink){
				ReleaseAll();
				break;
			}
			hr = m_pMySink->CopyData((char *)pData, numFramesAvailable * pwfx->nBlockAlign, &m_bDone);
			if (FAILED(hr)) {
				ReleaseAll();
				break;
			}
			if (!pData || m_bDone == TRUE) {
				break;
			}
			if (!pCaptureClient){
				ReleaseAll();
				break;
			}
			hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
			if (FAILED(hr)) {
				ReleaseAll();
				break;
			}
			if (!pCaptureClient){
				ReleaseAll();
				break;
			}
			hr = pCaptureClient->GetNextPacketSize(&packetLength);
			if (FAILED(hr)) {
				ReleaseAll();
				break;
			}
		}
	}
}

bool CSoundRenderHandler::StopRecord()
{
	if (hTimerWakeUp) {
		CloseHandle(hTimerWakeUp);
		hTimerWakeUp = nullptr;
	}
	HRESULT hr;
	if (pAudioClient) {
		hr = pAudioClient->Stop();  // Stop recording.
		if (FAILED(hr)) {
			ReleaseAll();
			return false;
		}
	}
	m_mutex.lock();
	m_bDone = TRUE;
	m_mutex.unlock();

	ReleaseAll();
	return true;
}

void CSoundRenderHandler::ReleaseAll()
{
	if (pwfx)
	{
		CoTaskMemFree(pwfx);
		pwfx = nullptr;
	}
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pAudioClient);
	SAFE_RELEASE(pCaptureClient);
}

// pwfx->nSamplesPerSec = 44100;
BOOL CSoundRenderHandler::AdjustFormatTo16Bits(WAVEFORMATEX *pwfx)
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