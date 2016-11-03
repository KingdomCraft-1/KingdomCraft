#ifndef KEYDEVICE_H
#define KEYDEVICE_H

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>	
#include <Windows.h>


enum KEYSTATE
{
	//! �L�[�������ꂽ�u�Ԃ̏��
	KEY_PUSH,

	//! �L�[�������ꂽ�u�Ԃ̏��
	KEY_RELEASE,

	//! �L�[�������ꑱ���Ă�����
	KEY_ON,

	//! �L�[��������Ă��Ȃ����
	KEY_OFF
};

class KeyDevice
{
public:
	KeyDevice();
	~KeyDevice();

	/**
	 * KeyDevice�̏������֐�
	 * @param[in] _pDInput8 DirectInput�I�u�W�F�N�g
	 * @param[in] _hWnd KeyDevice�ɑΉ�������E�B���h�E�n���h��
	 * @return �f�o�C�X�̏������ɐ���������true���Ԃ�
	 */
	bool Init(LPDIRECTINPUT8 _pDInput8, HWND _hWnd);

	/**
	 * KeyDevice�̉���֐�
	 */
	void Release();

	/**
	 * �L�[�̏�Ԃ��X�V����
	 */
	void Update();

	/**
	 * �L�[�̏�Ԃ��擾����
	 * @param[in] ��Ԃ��擾�������L�[��DIK
	 * @return �L�[�̏��
	 */
	KEYSTATE GetKeyState(int _dik);

private:
	LPDIRECTINPUT8			m_pDInput8;
	HWND					m_hWnd;
	LPDIRECTINPUTDEVICE8	m_pDInputDevice8;
	BYTE					m_KeyState[256];
	BYTE					m_OldKeyState[256];

};

#endif