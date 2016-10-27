#include "DSoundManager.h"
#include <mmsystem.h>

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxguid.lib")
DSoundManager* DSoundManager::m_pSoundManager = NULL;

DSoundManager::DSoundManager(HWND _hWnd)
{
	if (FAILED(DirectSoundCreate8(NULL, &m_pDSound8, NULL)))
	{
		MessageBox(0, "�T�E���h�f�o�C�X�̐����Ɏ��s���܂���", "DSoundManager.cpp", MB_OK);
	}

	if (FAILED(m_pDSound8->SetCooperativeLevel(_hWnd, DSSCL_PRIORITY)))
	{
		MessageBox(0, "�������x���̐ݒ�Ɏ��s���܂���", "DSoundManager.cpp", MB_OK);
	}
}


DSoundManager::~DSoundManager()
{
	m_pDSound8->Release();
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


	// RIFF�`�����N����
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}


	// �t�H�[�}�b�g�`�����N����
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


	// �f�[�^�`�����N����
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

void DSoundManager::ReleaseSound(int _Key)
{
	m_SoundMap[_Key]->Release();
	m_SoundMap.erase(_Key);
}

void DSoundManager::SoundOperation(int _Key, SOUND_OPERATION operation)
{
	switch (operation)
	{
	case SOUND_PLAY:
		m_SoundMap[_Key]->Play(0, 0, 0);
		break;
	case SOUND_LOOP:
		m_SoundMap[_Key]->Play(0, 0, DSBPLAY_LOOPING);
		break;
	case SOUND_STOP:
		m_SoundMap[_Key]->Stop();
		break;
	case SOUND_RESET:
		m_SoundMap[_Key]->SetCurrentPosition(0);
		break;
	case SOUND_STOP_RESET:
		m_SoundMap[_Key]->Stop();
		m_SoundMap[_Key]->SetCurrentPosition(0);
		break;
	}
}

int DSoundManager::SoundLoad(int _Key, char* _filename)
{
	LPDIRECTSOUNDBUFFER8 pDSBuffer = NULL;
	WAVEFORMATEX wFmt;
	char *pWaveData = 0;
	DWORD waveSize = 0;

	if (!OpenWave((_filename), wFmt, &pWaveData, waveSize))
	{
		return 0;
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
		return 0;
	}


	// �Z�J���_���o�b�t�@��Wave�f�[�^��������
	//�����f�[�^
	LPVOID lpvWrite = 0;
	//�����f�[�^�̑傫��
	DWORD dwLength = 0;
	if (DS_OK == pDSBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER))
	{
		memcpy(lpvWrite, pWaveData, dwLength);
		pDSBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
	}

	delete[] pWaveData; // �����͂�������Ȃ�


	m_SoundMap[_Key] = pDSBuffer;

	return 0;
}