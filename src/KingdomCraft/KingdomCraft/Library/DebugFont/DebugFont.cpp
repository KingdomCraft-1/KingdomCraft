#include "DebugFont.h"
#include <d3dx11.h>


DebugFont::DebugFont():
m_pDevice(NULL),
m_pDeviceContext(NULL),
m_pResourceView(NULL),
m_pConstantBuffer(NULL),
m_pVertexBuffer(NULL),
m_pVertexShader(NULL),
m_pLayout(NULL),
m_pPixelShader(NULL),
m_pSamplerState(NULL),
m_pBlendState(NULL),
m_pDepthStencilState(NULL)
{
}

DebugFont::~DebugFont()
{
}

bool DebugFont::Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, HWND _hWnd)
{
	m_pDevice = _pDevice;
	m_pDeviceContext = _pDeviceContext;
	m_hWnd = _hWnd;


	if (!InitResourceView())
	{
		MessageBox(m_hWnd, "InitResourceView�����s���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	if (!InitConstantBuffer())
	{
		MessageBox(m_hWnd, "InitConstantBuffer�����s���܂���", "�G���[", MB_ICONSTOP);
		ReleaseResourceView();
		return false;
	}

	if (!InitShader())
	{
		MessageBox(m_hWnd, "InitShader�����s���܂���", "�G���[", MB_ICONSTOP);
		ReleaseConstantBuffer();
		ReleaseResourceView();
		return false;
	}

	if (!InitSamplerState())
	{
		MessageBox(m_hWnd, "InitSamplerState�����s���܂���", "�G���[", MB_ICONSTOP);
		ReleaseShader();
		ReleaseConstantBuffer();
		ReleaseResourceView();
		return false;
	}

	if (!InitBlendState())
	{
		MessageBox(m_hWnd, "InitBlendState�����s���܂���", "�G���[", MB_ICONSTOP);
		ReleaseSamplerState();
		ReleaseShader();
		ReleaseConstantBuffer();
		ReleaseResourceView();
		return false;
	}
	
	if (!InitDepthStencilState())
	{
		MessageBox(m_hWnd, "InitDepthStencilState�����s���܂���", "�G���[", MB_ICONSTOP);
		ReleaseBlendState();
		ReleaseSamplerState();
		ReleaseShader();
		ReleaseConstantBuffer();
		ReleaseResourceView();
		return false;
	}

	OutputDebugString("DebugFont�̏������ɐ������܂���\n");

	return true;
}

void DebugFont::Release()
{
	ReleaseDepthStencilState();
	ReleaseBlendState();
	ReleaseSamplerState();
	ReleaseShader();
	ReleaseConstantBuffer();
	ReleaseResourceView();
}

bool DebugFont::InitFont(float _FontHeight, float _FontWidth, D3DXCOLOR _FontColor)
{
	m_FontWidth = _FontWidth;		// �t�H���g�̕�
	m_FontHeight = _FontHeight;     // �t�H���g�̍���

	RECT WindowRect;
	GetWindowRect(m_hWnd, &WindowRect);
	m_WindowWidth = WindowRect.right - WindowRect.left;
	m_WindowHeight = WindowRect.bottom - WindowRect.top;

	FONT_VERTEX FontVertex[] =
	{
		D3DXVECTOR3(-m_WindowWidth / 2,					-m_WindowHeight / 2, 0),				_FontColor, 0, 0,
		D3DXVECTOR3(-m_WindowWidth / 2 + m_FontWidth,	-m_WindowHeight / 2, 0),				_FontColor, FONT_TU, 0,
		D3DXVECTOR3(-m_WindowWidth / 2,					-m_WindowHeight / 2 + m_FontHeight, 0), _FontColor, 0, 1,
		D3DXVECTOR3(-m_WindowWidth / 2 + m_FontWidth,	-m_WindowHeight / 2 + m_FontHeight, 0), _FontColor, FONT_TU, 1,
	};

	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth = sizeof(FontVertex);         
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;				
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	
	BufferDesc.CPUAccessFlags = 0;                     
	BufferDesc.MiscFlags = 0;							

	D3D11_SUBRESOURCE_DATA ResourceData;
	ZeroMemory(&ResourceData, sizeof(ResourceData));
	ResourceData.pSysMem = (void*)FontVertex;			
	ResourceData.SysMemPitch = 0;
	ResourceData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &ResourceData, &m_pVertexBuffer)))
	{
		MessageBox(m_hWnd, "���_�f�[�^�̍쐬�Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	return true;
}

void DebugFont::DrawFont(D3DXVECTOR2* _pDrawPos, char* _pStr)
{
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->HSSetShader(NULL, NULL, 0);
	m_pDeviceContext->DSSetShader(NULL, NULL, 0);
	m_pDeviceContext->GSSetShader(NULL, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pResourceView);
	m_pDeviceContext->CSSetShader(NULL, NULL, 0);

	m_pDeviceContext->OMSetBlendState(m_pBlendState, 0, 0xffffffff);
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 0);

	UINT stride = sizeof(FONT_VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pDeviceContext->IASetInputLayout(m_pLayout);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CONSTANT_BUFFER* cBuffer;
	D3DXMATRIX matTranslation;
	for (int i = 0; i < (int)strlen(_pStr); i++)
	{
		m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);	
		cBuffer = (CONSTANT_BUFFER*)(mappedResource.pData);

		D3DXMATRIX Proj(
			2.0f / m_WindowWidth, 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / m_WindowHeight, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
		cBuffer->MatWVP = Proj;

		cBuffer->TexelOffset.x = (float)(_pStr[i] - 32) * FONT_TU;
		cBuffer->TexelOffset.y = 0.0f;
		cBuffer->TexelOffset.z = 0.0f;
		cBuffer->TexelOffset.w = 0.0f;

		cBuffer->PosOffset.x = i * m_FontWidth + _pDrawPos->x;
		cBuffer->PosOffset.y = _pDrawPos->y;
		cBuffer->PosOffset.z = 0;
		cBuffer->PosOffset.w = 0;

		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);

		m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

		m_pDeviceContext->Draw(4, 0);
	}
}

bool DebugFont::InitResourceView()
{
	D3DX11_IMAGE_LOAD_INFO LoadInfo;
	ZeroMemory(&LoadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	LoadInfo.Width = D3DX11_DEFAULT;
	LoadInfo.Height = D3DX11_DEFAULT;
	LoadInfo.Depth = D3DX11_DEFAULT;
	LoadInfo.FirstMipLevel = D3DX11_DEFAULT;
	LoadInfo.MipLevels = 1;
	LoadInfo.Usage = D3D11_USAGE_DEFAULT;
	LoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	LoadInfo.CpuAccessFlags = 0;
	LoadInfo.MiscFlags = 0;
	LoadInfo.Format = DXGI_FORMAT_FROM_FILE;
	LoadInfo.Filter = D3DX11_FILTER_POINT;
	LoadInfo.MipFilter = D3DX11_FILTER_POINT;
	LoadInfo.pSrcInfo = NULL;

	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice,
		"Library//DebugFont//Resource//Font.png",
		&LoadInfo,
		NULL,
		&m_pResourceView,
		NULL)))
	{
		MessageBox(m_hWnd, "�e�N�X�`�����[�\�[�X�r���[�̐����Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	OutputDebugString("DebugFont �t�H���g�e�N�X�`����ResourceView�̐����ɐ������܂���\n");

	return true;
}

bool DebugFont::InitConstantBuffer()
{
	D3D11_BUFFER_DESC ConstantBufferDesc;
	ZeroMemory(&ConstantBufferDesc, sizeof(ConstantBufferDesc));
	ConstantBufferDesc.ByteWidth = sizeof(CONSTANT_BUFFER);         // �o�b�t�@�̃T�C�Y
	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;					// ���I���\�[�X���w��(CPU�ɂ�鏑�����݂�GPU�ɂ��ǂݎ�肪�����)
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// �o�b�t�@�̎��(�萔�o�b�t�@���w��)
	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// CPU�̃A�N�Z�X(CPU �ɂ�鏑�����݂�����)
	ConstantBufferDesc.MiscFlags = 0;								// ���̑��̃t���O�͐ݒ肵�Ȃ�

	if (FAILED(m_pDevice->CreateBuffer(&ConstantBufferDesc, NULL, &m_pConstantBuffer)))
	{
		MessageBox(m_hWnd, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	OutputDebugString("DebugFont �萔�o�b�t�@�̐����ɐ������܂���\n");

	return true;
}

bool DebugFont::InitShader()
{
	if (!InitVertexShader())
	{
		MessageBox(m_hWnd, "InitVertexShader�����s���܂���", "�G���[", MB_ICONSTOP);
		return false;
	}

	if (!InitPixelShader())
	{
		MessageBox(m_hWnd, "InitPixelShader�����s���܂���", "�G���[", MB_ICONSTOP);
		ReleaseVertexShader();
		return false;
	}

	OutputDebugString("DebugFont �V�F�[�_�[�̏������ɐ������܂���\n");

	return true;
}

bool DebugFont::InitVertexShader()
{
	ID3DBlob* pCompiledVertexShader = NULL;
	ID3DBlob* pVertexShaderErrors = NULL;
	if (FAILED(D3DX11CompileFromFile(
		"Library//DebugFont//Effect//DebugFont.fx",
		NULL,
		NULL,
		"VS",
		"vs_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION,
		0,
		NULL,
		&pCompiledVertexShader,
		&pVertexShaderErrors,
		NULL)))
	{
		MessageBox(m_hWnd, "VertexShader�̃R���p�C���Ɏ��s", "�G���[", MB_ICONSTOP);
		if (pVertexShaderErrors != NULL) pVertexShaderErrors->Release();
		return false;
	}

	if (pVertexShaderErrors != NULL) pVertexShaderErrors->Release();



	if (FAILED(m_pDevice->CreateVertexShader(
		pCompiledVertexShader->GetBufferPointer(),
		pCompiledVertexShader->GetBufferSize(),
		NULL,
		&m_pVertexShader)))
	{
		MessageBox(m_hWnd, "VertexShader�̍쐬�Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		pCompiledVertexShader->Release();
		return false;
	}



	D3D11_INPUT_ELEMENT_DESC Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (FAILED(m_pDevice->CreateInputLayout(
		Layout,
		sizeof(Layout) / sizeof(Layout[0]),
		pCompiledVertexShader->GetBufferPointer(),
		pCompiledVertexShader->GetBufferSize(),
		&m_pLayout)))
	{
		MessageBox(m_hWnd, "InputLayout�̍쐬�Ɏ��s���܂���", "�G���[", MB_ICONSTOP);
		m_pVertexShader->Release();
		pCompiledVertexShader->Release();
		return false;
	}

	pCompiledVertexShader->Release();

	OutputDebugString("DebugFont �o�[�e�b�N�X�V�F�[�_�[�̏������ɐ������܂���\n");

	return true;
}

bool DebugFont::InitPixelShader()
{
	ID3DBlob* pCompiledPixelShader = NULL;
	ID3DBlob* pPixelShaderErrors = NULL;
	if (FAILED(D3DX11CompileFromFile(
		"Library//DebugFont//Effect//DebugFont.fx",
		NULL,
		NULL,
		"PS",
		"ps_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION,
		0,
		NULL,
		&pCompiledPixelShader,
		&pPixelShaderErrors,
		NULL)))
	{
		MessageBox(m_hWnd, "PixelShader�̃R���p�C���Ɏ��s", "�G���[", MB_ICONSTOP);
		if (pPixelShaderErrors != NULL) pPixelShaderErrors->Release();
		return false;
	}

	if (pPixelShaderErrors != NULL) pPixelShaderErrors->Release();



	if (FAILED(m_pDevice->CreatePixelShader(
		pCompiledPixelShader->GetBufferPointer(),
		pCompiledPixelShader->GetBufferSize(),
		NULL,
		&m_pPixelShader)))
	{
		MessageBox(m_hWnd, "PixelShader�̃R���p�C���Ɏ��s", "�G���[", MB_ICONSTOP);
		pCompiledPixelShader->Release();
		return false;
	}

	OutputDebugString("DebugFont �s�N�Z���V�F�[�_�[�̏������ɐ������܂���\n");

	return true;
}

bool DebugFont::InitSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// �T���v�����O���Ɏg�p����t�B���^�B�����ł͈ٕ����t�B���^�[���g�p����B
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// 0 �` 1 �͈̔͊O�ɂ��� u �e�N�X�`���[���W�̕`����@
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// 0 �` 1 �͈̔͊O�ɂ��� v �e�N�X�`���[���W�̕`����@
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		// 0 �` 1 �͈̔͊O�ɂ��� w �e�N�X�`���[���W�̕`����@
	samplerDesc.MipLODBias = 0;								// �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
	samplerDesc.MaxAnisotropy = 0;							// �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	// ��r�I�v�V�����B
	for (int i = 0; i < 4; i++)
	{
		samplerDesc.BorderColor[i] = 0.0f;					// ���E�F
	}
	samplerDesc.MinLOD = 0;									// �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;					// �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l

	if (FAILED(m_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState)))
	{
		MessageBox(m_hWnd, "�T���v���[�X�e�[�g�̐����Ɏ��s", "�G���[", MB_ICONSTOP);
		return false;
	}

	OutputDebugString("DebugFont �T���v���[�X�e�[�g�̐����ɐ������܂���\n");

	return true;
}

bool DebugFont::InitBlendState()
{
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState)))
	{
		MessageBox(m_hWnd, "�u�����h�X�e�[�g�̐����Ɏ��s", "�G���[", MB_ICONSTOP);
		return false;
	}

	OutputDebugString("DebugFont �u�����h�X�e�[�g�̐����ɐ������܂���\n");

	return true;
}

bool DebugFont::InitDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC ddsDesc;
	ZeroMemory(&ddsDesc, sizeof(ddsDesc));
	ddsDesc.DepthEnable = FALSE;
	ddsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ddsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ddsDesc.StencilEnable = FALSE;

	if (FAILED(m_pDevice->CreateDepthStencilState(&ddsDesc, &m_pDepthStencilState)))
	{
		OutputDebugString("DebugFont �f�v�X�X�e���V���X�e�[�g�̐����Ɏ��s\n");
		return false;
	}

	OutputDebugString("DebugFont �f�v�X�X�e���V���X�e�[�g�̐����ɐ������܂���\n");

	return true;
}

void DebugFont::ReleaseResourceView()
{
	m_pResourceView->Release();
}

void DebugFont::ReleaseConstantBuffer()
{
	m_pConstantBuffer->Release();
}

void DebugFont::ReleaseShader()
{
	ReleaseVertexShader();
	ReleasePixelShader();
}

void DebugFont::ReleaseVertexShader()
{
	m_pLayout->Release();
	m_pVertexShader->Release();
}

void DebugFont::ReleasePixelShader()
{
	m_pPixelShader->Release();
}

void DebugFont::ReleaseSamplerState()
{
	m_pSamplerState->Release();
}

void DebugFont::ReleaseBlendState()
{
	m_pBlendState->Release();
}

void DebugFont::ReleaseDepthStencilState()
{
	m_pDepthStencilState->Release();
}
