#ifndef MOUSEDEVICE_H
#define MOUSEDEVICE_H

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>	
#include <Windows.h>

enum MOUSEBUTTONSTATE
{
	//! �}�E�X�̃{�^���������ꂽ�u�Ԃ̏��
	MOUSEBUTTON_PUSH,

	//! �}�E�X�̃{�^���������ꂽ�u�Ԃ̏��
	MOUSEBUTTON_RELEASE,

	//! �}�E�X�̃{�^���������ꑱ���Ă�����
	MOUSEBUTTON_ON,

	//! �}�E�X�̃{�^����������Ă�����
	MOUSEBUTTON_OFF
};

struct MOUSESTATE
{
	LONG    lX;						// �}�E�X��X�����̈ړ���
	LONG    lY;						// �}�E�X��Y�����̈ړ���
	LONG    lZ;						// �}�E�X��Z�����̈ړ���
	MOUSEBUTTONSTATE rgbButtons[4];	// �}�E�X�̃{�^���̏��
	POINT	CursorPos;				// �E�B���h�E���̃J�[�\���ʒu
};

class MouseDevice
{
public:
	MouseDevice();
	~MouseDevice();

	/**
	 * MouseDevice�̏������֐�
	 * @param[in] _pDInput8 DirectInput�I�u�W�F�N�g
	 * @param[in] _hWnd KeyDevice�ɑΉ�������E�B���h�E�n���h��
	 * @return �f�o�C�X�̏������ɐ���������true���Ԃ�
	 */
	bool Init(LPDIRECTINPUT8 _pDInput8, HWND _hWnd);

	/**
	 * MouseDevice�̉���֐�
	 */
	void Release();

	/**
	 * �}�E�X�̏�Ԃ��X�V����
	 */
	void Update();

	/**
	 * �}�E�X�̏�Ԃ��擾����
	 * @return �}�E�X�̏�Ԃ��i�[����Ă���\����
	 */
	MOUSESTATE GetMouseState();

private:
	LPDIRECTINPUT8			m_pDInput8;
	HWND					m_hWnd;
	LPDIRECTINPUTDEVICE8	m_pDInputDevice8;
	DIMOUSESTATE			m_MouseState;
	DIMOUSESTATE			m_OldMouseState;

};

#endif