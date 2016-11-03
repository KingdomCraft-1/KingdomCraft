#include "MouseDevice.h"


MouseDevice::MouseDevice() :
m_pDInput8(NULL)
{
	for (int i = 0; i < 4; i++)
	{
		m_MouseState.rgbButtons[i] = MOUSEBUTTON_OFF;
		m_OldMouseState.rgbButtons[i] = MOUSEBUTTON_OFF;
	}
}

MouseDevice::~MouseDevice()
{
}

bool MouseDevice::Init(LPDIRECTINPUT8 _pDInput8, HWND _hWnd)
{
	m_pDInput8 = _pDInput8;
	m_hWnd = _hWnd;

	if (m_pDInput8 == NULL)
	{
		MessageBox(m_hWnd, "DirectInput8�I�u�W�F�N�g���L���ł͂���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	if (FAILED(m_pDInput8->CreateDevice(GUID_SysMouse, &m_pDInputDevice8, NULL))) 
	{
		MessageBox(m_hWnd, "DirectInput8�}�E�X�f�o�C�X�̐����Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	if (FAILED(m_pDInputDevice8->SetDataFormat(&c_dfDIMouse))) 
	{
		MessageBox(m_hWnd, "DirectInput8�}�E�X�f�o�C�X�̃f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		m_pDInputDevice8->Release();
		return false;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = 1000;
	if (FAILED(m_pDInputDevice8->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph))) 
	{
		MessageBox(m_hWnd, "DirectInput8�}�E�X�f�o�C�X�̃o�b�t�@�ݒ�Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		m_pDInputDevice8->Release();
		return false;
	}

	if (FAILED(m_pDInputDevice8->SetCooperativeLevel(m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) 
	{
		MessageBox(m_hWnd, "DirectInput8�}�E�X�f�o�C�X�̋������x���̐ݒ�Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		m_pDInputDevice8->Release();
		return false;
	}

	m_pDInputDevice8->Acquire();

	OutputDebugString("DirectInput��MouseDevice�̏������ɐ�������\n");

	return true;
}

void MouseDevice::Release()
{
	m_pDInputDevice8->Release();

	OutputDebugString("DirectInput��MouseDevice��������܂���\n");
}

void MouseDevice::Update()
{
	HRESULT hr = m_pDInputDevice8->Acquire();
	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		m_pDInputDevice8->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
	}
}

MOUSESTATE MouseDevice::GetMouseState()
{
	MOUSESTATE MouseState;
	MouseState.lX = m_MouseState.lX;
	MouseState.lY = m_MouseState.lY;
	MouseState.lZ = m_MouseState.lZ;
	GetCursorPos(&MouseState.CursorPos);
	ScreenToClient(m_hWnd, &MouseState.CursorPos);

	for (int i = 0; i < 4; i++)
	{
		if (m_MouseState.rgbButtons[i])
		{
			if (m_OldMouseState.rgbButtons[i] == MOUSEBUTTON_OFF)
			{
				MouseState.rgbButtons[i] = MOUSEBUTTON_PUSH;
			}
			else
			{
				MouseState.rgbButtons[i] = MOUSEBUTTON_ON;
			}
			m_OldMouseState.rgbButtons[i] = MOUSEBUTTON_ON;
		}
		else
		{
			if (m_OldMouseState.rgbButtons[i] == MOUSEBUTTON_ON)
			{
				MouseState.rgbButtons[i] = MOUSEBUTTON_RELEASE;
			}
			else
			{
				MouseState.rgbButtons[i] = MOUSEBUTTON_OFF;
			}
			m_OldMouseState.rgbButtons[i] = MOUSEBUTTON_OFF;
		}
	}


	return MouseState;
}
