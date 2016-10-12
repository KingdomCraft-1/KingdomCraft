#include "InputDeviceManager.h"
#include "KeyDevice\KeyDevice.h"
#include "MouseDevice\MouseDevice.h"

InputDeviceManager* InputDeviceManager::m_pInputDeviceManager = NULL;

InputDeviceManager::InputDeviceManager() :
m_pKeyDevice(NULL),
m_pMouseDevice(NULL),
m_pDInput8(NULL)
{
}

InputDeviceManager::~InputDeviceManager()
{
}

bool InputDeviceManager::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;

	if (m_pDInput8 != NULL)
	{
		MessageBox(m_hWnd, "m_pDInput�̒��g�͋�ł͂���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput8, NULL)))
	{
		MessageBox(m_hWnd, "DirectInput8�I�u�W�F�N�g�̐����Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	OutputDebugString("DirectInput8�I�u�W�F�N�g�̐����ɐ���\n");

	return true;
}

void InputDeviceManager::Release()
{
	if (m_pKeyDevice != NULL)
	{
		m_pKeyDevice->Release();
		delete m_pKeyDevice;

		OutputDebugString("KeyDevice�N���X��j�����܂���\n");
	}

	if (m_pMouseDevice != NULL)
	{
		m_pMouseDevice->Release();
		delete m_pMouseDevice;

		OutputDebugString("MouseDevice�N���X��j�����܂���\n");
	}

	m_pDInput8->Release();
}

bool InputDeviceManager::CreateKeyDevice()
{
	if (m_pKeyDevice != NULL)
	{
		MessageBox(m_hWnd, "m_pKeyDevice�̒��g�͋�ł͂���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	m_pKeyDevice = new KeyDevice();

	if (!m_pKeyDevice->Init(m_pDInput8, m_hWnd))
	{
		MessageBox(m_hWnd, "�L�[�f�o�C�X�̏������Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		delete m_pKeyDevice;
		m_pKeyDevice = NULL;
		return false;
	}

	OutputDebugString("KeyDevice�N���X�̐����ɐ���\n");

	return true;
}

bool InputDeviceManager::CreateMouseDevice()
{
	if (m_pMouseDevice != NULL)
	{
		MessageBox(m_hWnd, "m_pMouseDevice�̒��g�͋�ł͂���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	m_pMouseDevice = new MouseDevice();

	if (!m_pMouseDevice->Init(m_pDInput8, m_hWnd))
	{
		MessageBox(m_hWnd, "�}�E�X�f�o�C�X�������Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		delete m_pMouseDevice;
		m_pMouseDevice = NULL;
		return false;
	}

	OutputDebugString("MouseDevice�N���X�̐����ɐ���\n");

	return true;
}

void InputDeviceManager::KeyUpdate()
{
	m_pKeyDevice->Update();
}

void InputDeviceManager::MouseUpdate()
{
	m_pMouseDevice->Update();
}

KEYSTATE InputDeviceManager::GetKeyState(int _DIK)
{
	return m_pKeyDevice->GetKeyState(_DIK);
}

MOUSESTATE InputDeviceManager::GetMouseState()
{
	return m_pMouseDevice->GetMouseState();
}
