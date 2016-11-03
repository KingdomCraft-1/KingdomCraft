#ifndef DXSOUNDMANAGER_H
#define DXSOUNDMANAGER_H
#include <windows.h>
#include <dsound.h>
#include <map>

enum SOUND_OPERATION
{
	SOUND_PLAY,
	SOUND_STOP,
	SOUND_LOOP,
	SOUND_RESET,
	SOUND_STOP_RESET
};

class DSoundManager
{
public:
	
	/**
	* ����������
	* @return ����������true
	*/
	bool Init();

	/**
	* �������
	*/
	void Release(){ m_pDSound8->Release(); };

	/**
	* �����̓ǂݍ���
	* @param[in] _key �i�[��̃L�[
	* @param[in] _filename �ǂݍ��ރt�@�C���̖��O
	* @return ����������true
	*/
	bool SoundLoad(int _key, char* _filename);
	
	/**
	* �����̊J��
	* @param[in] _key �J����̃L�[
	*/
	void ReleaseSound(int _key);

	/**
	* �T�E���h�̑���֐�
	* @param[in] _key ���삷��T�E���h�̊i�[��̃L�[
	* @param[in] operation �ǂ̂悤�ȑ�������邩
	*/
	void SoundOperation(int _key, SOUND_OPERATION _operation);

	/**
	* �C���X�^���X�����֐�
	* @param[in] _hWnd �E�B���h�E�n���h���B�R���X�g���N�^���Ŏg��
	*/
	static void Create(HWND _hWnd)
	{
		if (m_pSoundManager == NULL)
		{
			m_pSoundManager = new DSoundManager(_hWnd);
		}
	}
	
	/**
	* @return �C���X�^���X�̔j��
	*/
	static void Destroy()
	{
		if (m_pSoundManager != NULL)
		{
			delete m_pSoundManager;
			m_pSoundManager = NULL;
		}
	}

	/**
	* @return �C���X�^���X���Ԃ�
	*/
	static DSoundManager* GetInstance()
	{
		return m_pSoundManager;
	}

private:
	DSoundManager(HWND _hWnd);
	~DSoundManager();
	HWND								m_hWnd;
	IDirectSound8*						m_pDSound8;
	std::map<int, LPDIRECTSOUNDBUFFER8> m_SoundMap;
	static DSoundManager*				m_pSoundManager;
	bool								OpenWave(TCHAR* _filename, WAVEFORMATEX &_wFmt, char** _pWaveData, DWORD &_waveSize);
};
#endif