#ifndef INPUTDEVICEMANAGER_H
#define INPUTDEVICEMANAGER_H

#define DIRECTINPUT_VERSION 0x0800

#include "KeyDevice\KeyDevice.h"
#include "MouseDevice\MouseDevice.h"

class InputDeviceManager
{
public:
	~InputDeviceManager();

	/**
	 * InputDeviceManager�̃C���X�^���X�𐶐�����
	 */
	static void Create()
	{
		if (m_pInputDeviceManager == NULL)
		{
			m_pInputDeviceManager = new InputDeviceManager();
		}
	}

	/**
	 * InputDeviceManager�̃C���X�^���X���擾����
	 * @return InputDeviceManager�̃C���X�^���X
	 */
	static InputDeviceManager* GetInstance()
	{
		return m_pInputDeviceManager;
	}

	/**
	 * InputDeviceManager�̃C���X�^���X��j������
	 */
	static void Delete()
	{
		delete m_pInputDeviceManager;
		m_pInputDeviceManager = NULL;
	}

	/**
	 * InputDeviceManager�̏������֐�
	 * InputDeviceManager�𐶐�������ɕK���Ă΂Ȃ��Ƃ����Ȃ�
	 * @param[in] _hWnd ���̓f�o�C�X�ɑΉ�������E�B���h�E�n���h��
	 */
	bool Init(HWND _hWnd);

	/**
	 * InputDeviceManager�̉���֐�
	 * InputDeviceManager��j������O�ɕK���Ă΂Ȃ��Ƃ����Ȃ�
	 */
	void Release();

	/**
	 * KeyDevice�̐����֐�
	 */
	bool CreateKeyDevice();

	/**
	 * MouseDevice�̐����֐�
	 */
	bool CreateMouseDevice();

	/**
	 * �L�[�̏�Ԃ��X�V����
	 */
	void KeyUpdate();

	/**
	 * �}�E�X�̏�Ԃ��X�V����
	 */
	void MouseUpdate();

	/**
	 * �L�[�̏�Ԃ��擾����
	 * @param[in] ��Ԃ��擾�������L�[��DIK
	 * @return �L�[�̏��
	 */
	KEYSTATE GetKeyState(int _DIK);

	/**
	 * �}�E�X�̏�Ԃ��擾����
	 * @return �}�E�X�̏�Ԃ��i�[����Ă���\����
	 */
	MOUSESTATE GetMouseState();

private:
	InputDeviceManager();

	static InputDeviceManager*		m_pInputDeviceManager;

	KeyDevice*		m_pKeyDevice;
	MouseDevice*	m_pMouseDevice;
	LPDIRECTINPUT8	m_pDInput8;
	HWND			m_hWnd;

};

#endif