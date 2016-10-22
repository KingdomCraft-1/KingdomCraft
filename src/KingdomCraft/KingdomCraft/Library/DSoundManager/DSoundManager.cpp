#include "DSoundManager.h"
#include <mmsystem.h>

DSoundManager* DSoundManager::m_pSoundManager = NULL;

DSoundManager::DSoundManager(HWND _hWnd) :
m_hWnd(_hWnd)
{
}


DSoundManager::~DSoundManager()
{
}

bool DSoundManager::Init()
{
	if (FAILED(DirectSoundCreate8(NULL, &m_pDSound8, NULL)))
	{
		MessageBox(m_hWnd, "サウンドデバイスの生成に失敗しました", "Error", MB_ICONSTOP);
		return false;
	}

	if (FAILED(m_pDSound8->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY)))
	{
		MessageBox(m_hWnd, "協調レベルの設定に失敗しました", "Error", MB_ICONSTOP);
		return false;
	}
	return true;
}

bool DSoundManager::OpenWave(TCHAR* _filename, WAVEFORMATEX &_wFmt, char** _pWaveData, DWORD &_waveSize)
{
	if (_filename == NULL)
	{
		return false;
	}

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	ZeroMemory(&mmioInfo, sizeof(MMIOINFO));
	hMmio = mmioOpen(_filename, &mmioInfo, MMIO_READ);
	if (!hMmio)
	{
		return false;
	}


	// RIFFチャンク検索
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}


	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}


	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&_wFmt, fmsize);
	if (size != fmsize)
	{
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);


	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}

	*_pWaveData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)*_pWaveData, dataChunk.cksize);
	if (size != dataChunk.cksize)
	{
		delete[] * _pWaveData;
		return false;
	}

	_waveSize = size;

	mmioClose(hMmio, 0);

	return true;
}

void DSoundManager::ReleaseSound(int _key)
{
	m_SoundMap[_key]->Release();
	m_SoundMap.erase(_key);
}

void DSoundManager::SoundOperation(int _key, SOUND_OPERATION operation)
{
	switch (operation)
	{
	case SOUND_PLAY:
		m_SoundMap[_key]->Play(0, 0, 0);
		break;
	case SOUND_LOOP:
		m_SoundMap[_key]->Play(0, 0, DSBPLAY_LOOPING);
		break;
	case SOUND_STOP:
		m_SoundMap[_key]->Stop();
		break;
	case SOUND_RESET:
		m_SoundMap[_key]->SetCurrentPosition(0);
		break;
	case SOUND_STOP_RESET:
		m_SoundMap[_key]->Stop();
		m_SoundMap[_key]->SetCurrentPosition(0);
		break;
	}
}

bool DSoundManager::SoundLoad(int _key, char* _filename)
{
	LPDIRECTSOUNDBUFFER8 pDSBuffer = NULL;
	WAVEFORMATEX wFmt;
	char *pWaveData = 0;
	DWORD waveSize = 0;

	if (!OpenWave(_filename, wFmt, &pWaveData, waveSize))
	{
		MessageBox(m_hWnd, "waveファイルが開けませんでした", "Error", MB_ICONSTOP);
		return false;
	}

	DSBUFFERDESC DSBufferDesc;
	DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	DSBufferDesc.dwFlags = 0;
	DSBufferDesc.dwBufferBytes = waveSize;
	DSBufferDesc.dwReserved = 0;
	DSBufferDesc.lpwfxFormat = &wFmt;
	DSBufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer *ptmpBuf = 0;
	m_pDSound8->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pDSBuffer);

	ptmpBuf->Release();
	if (pDSBuffer == NULL)
	{
		m_pDSound8->Release();
		return false;
	}


	// セカンダリバッファにWaveデータ書き込み
	//音声データ
	LPVOID lpvWrite = 0;
	//音声データの大きさ
	DWORD dwLength = 0;
	if (DS_OK == pDSBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER))
	{
		memcpy(lpvWrite, pWaveData, dwLength);
		pDSBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
	}

	delete[] pWaveData;


	m_SoundMap[_key] = pDSBuffer;

	return true;
}