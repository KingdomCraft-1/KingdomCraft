#include "Vertex2D.h"
#include <Windows.h>
#define VERTEX_NUM 4
Vertex2D::Vertex2D(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, HWND _hwnd) :
m_pDevice(_pDevice),
m_pDeviceContext(_pDeviceContext),
m_hWnd(_hwnd),
m_pVertexShader(NULL),
m_pPixelShader(NULL),
m_pVertexLayout(NULL),
m_pConstantBuffer(NULL),
m_pBlendState(NULL)
{
}

Vertex2D::~Vertex2D()
{
}

void Vertex2D::Release()
{
	ReleaseConstantBuffer();
	ReleaseTexture();
	ReleasePixelShader();
	ReleaseVertexLayout();
	ReleaseVertexShader();
}

bool Vertex2D::Init(RECT* _pScale, char* _textureFileName, D3DXVECTOR2* _pUV)
{
	RECT WindowRect;
	GetWindowRect(m_hWnd, &WindowRect);
	m_WindowWidth = WindowRect.right - WindowRect.left;
	m_WindowHeight = WindowRect.bottom - WindowRect.top;

	if (!InitVertexShader()) return false;

	if (!CreateVertexLayout())
	{
		ReleaseVertexShader();
		return false;
	}

	if (!InitPixelShader())
	{
		ReleaseVertexLayout();
		ReleaseVertexShader();
		return false;
	}

	if (!LoadTexture(_textureFileName))
	{
		ReleasePixelShader();
		ReleaseVertexLayout();
		ReleaseVertexShader();
		return false;
	}

	if (!InitVertexBuffer(_pScale, _pUV))
	{
		ReleaseTexture();
		ReleasePixelShader();
		ReleaseVertexLayout();
		ReleaseVertexShader();
		return false;
	}

	if (!InitConstantBuffer())
	{
		ReleaseTexture();
		ReleasePixelShader();
		ReleaseVertexLayout();
		ReleaseVertexShader();
		return false;
	}

	return true;
}


void Vertex2D::Draw(D3DXVECTOR2* _pDrawPos, float _angle)
{
	D3DXMATRIX matWorld	,matTranslate ,matRotate;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationZ(&matRotate, _angle);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matRotate);
	D3DXMatrixTranslation(&matTranslate, _pDrawPos->x, _pDrawPos->y, 0);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matTranslate);

	//�g�p����V�F�[�_�[�̃Z�b�g
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER	 constantBuffer;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�s���n��
		constantBuffer.matWorld = matWorld;
		D3DXMatrixTranspose(&constantBuffer.matWorld, &constantBuffer.matWorld);
		//�r���[�|�[�g�T�C�Y��n���i�N���C�A���g�̈�̉��Əc�j
		constantBuffer.viewPort.x = m_WindowWidth;
		constantBuffer.viewPort.y = m_WindowHeight;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&constantBuffer), sizeof(constantBuffer));
		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

		//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampler);
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);
	
	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->Draw(VERTEX_NUM, 0);
}


bool Vertex2D::InitVertexBuffer(RECT* _pScale, D3DXVECTOR2* _pUV)
{
	Vertex vertex[] =
	{
		D3DXVECTOR3(-_pScale->left, -_pScale->top, 0.f), D3DXVECTOR2(_pUV[0].x, _pUV[0].y),
		D3DXVECTOR3(+_pScale->right, -_pScale->top, 0.f), D3DXVECTOR2(_pUV[1].x, _pUV[1].y),
		D3DXVECTOR3(-_pScale->left, +_pScale->bottom, 0.f), D3DXVECTOR2(_pUV[2].x, _pUV[2].y),
		D3DXVECTOR3(+_pScale->right, +_pScale->bottom, 0.f), D3DXVECTOR2(_pUV[3].x, _pUV[3].y),
	};

	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.ByteWidth = sizeof(Vertex) * VERTEX_NUM;
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = sizeof(float);

	D3D11_SUBRESOURCE_DATA InitVertexData;
	InitVertexData.pSysMem = vertex;
	ID3D11Buffer* pBuffer;
	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &InitVertexData, &pBuffer)))
	{
		MessageBox(m_hWnd, "�o�[�e�b�N�X�o�b�t�@�[�̐����Ɏ��s���܂���", "Error", MB_ICONSTOP);
		return false;
	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
	pBuffer->Release();
	return true;
}

bool Vertex2D::InitVertexShader()
{
	ID3DBlob *pErrors = NULL;

	// ���_�V�F�[�_�[�̓ǂݍ��݂ƃ��C�A�E�g�쐬
	if (FAILED(D3DX11CompileFromFile(
		"Library//Vertex2D//Vertex2D.hlsl",
		NULL,
		NULL,
		"VS",
		"vs_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION,
		0,
		NULL,
		&m_pVertexCompiledShader,
		&pErrors,
		NULL)))
	{
		MessageBox(0, "VertexShader�̃R���p�C���Ɏ��s", "Error", MB_ICONSTOP);
		pErrors->Release();
		return false;
	}
	m_pDevice->CreateVertexShader(m_pVertexCompiledShader->GetBufferPointer(), m_pVertexCompiledShader->GetBufferSize(), NULL, &m_pVertexShader);
	return true;
}

void Vertex2D::ReleaseVertexShader()
{
	if (m_pVertexShader != NULL)
	{
		m_pVertexShader = NULL;
		m_pVertexShader->Release();
	}
	if (m_pVertexCompiledShader != NULL)
	{
		m_pVertexCompiledShader = NULL;
		m_pVertexCompiledShader->Release();
	}
}

bool Vertex2D::InitPixelShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	m_pPixelShader = NULL;
	if (FAILED(D3DX11CompileFromFile(
		"Library//Vertex2D//Vertex2D.hlsl",
		NULL,
		NULL,
		"PS",
		"ps_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION,
		0,
		NULL,
		&pCompiledShader,
		&pErrors,
		NULL)))
	{
		MessageBox(m_hWnd, "PixelShader�̃R���p�C���Ɏ��s", "Error", MB_ICONSTOP);
		pErrors->Release();
		return false;
	}
	m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader);
	pCompiledShader->Release();
	return true;
}

void Vertex2D::ReleasePixelShader()
{
	if (m_pPixelShader != NULL)
	{
		m_pPixelShader = NULL;
		m_pPixelShader->Release();
	}
}

bool Vertex2D::CreateVertexLayout()
{
	//���_���C�A�E�g��`
	D3D11_INPUT_ELEMENT_DESC InElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(
		InElementDesc,
		sizeof(InElementDesc) / sizeof(InElementDesc[0]),
		m_pVertexCompiledShader->GetBufferPointer(),
		m_pVertexCompiledShader->GetBufferSize(),
		&m_pVertexLayout)))
	{
		MessageBox(m_hWnd, "PixelShader�̃R���p�C���Ɏ��s", "Error", MB_ICONSTOP);
		return false;
	}

	return true;
}

void Vertex2D::ReleaseVertexLayout()
{
	if (m_pVertexLayout != NULL)
	{
		m_pVertexLayout->Release();
		m_pVertexLayout = NULL;
	}
}

bool Vertex2D::LoadTexture(char* _textureFileName)
{
	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));
	BlendDesc.IndependentBlendEnable = false;
	BlendDesc.AlphaToCoverageEnable = false;
	BlendDesc.RenderTarget[0].BlendEnable = true;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState)))
	{
		MessageBox(m_hWnd, "�A���t�@�u�����h�X�e�[�g�쐬�Ɏ��s���܂���", "Error", MB_ICONSTOP);
		return false;
	}

	UINT mask = 0xffffffff;
	m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, mask);

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if (FAILED(m_pDevice->CreateSamplerState(&SamDesc, &m_pSampler)))
	{
		MessageBox(m_hWnd, "�T���v���[�X�e�[�g�̐����Ɏ��s���܂���", "Error", MB_ICONSTOP);
		return false;
	}

	if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, _textureFileName, NULL, NULL, &m_pTexture, NULL)))
	{
		MessageBox(m_hWnd, "�e�N�X�`���[��ǂݍ��߂܂���", "Error", MB_ICONSTOP);
		return false;
	}
	return true;
}

void Vertex2D::ReleaseTexture()
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (m_pSampler != NULL)
	{
		m_pSampler->Release();
		m_pSampler = NULL;
	}
	if (m_pBlendState != NULL)
	{
		m_pBlendState = NULL;
		m_pBlendState->Release();
	}
}

bool Vertex2D::InitConstantBuffer()
{
	D3D11_BUFFER_DESC ConstantBufferDesc;
	ZeroMemory(&ConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBufferDesc.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);
	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstantBufferDesc.MiscFlags = 0;
	ConstantBufferDesc.StructureByteStride = 0;
	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&ConstantBufferDesc, NULL, &m_pConstantBuffer)))
	{
		MessageBox(m_hWnd, "�R���X�^���g�o�b�t�@�[�̐����Ɏ��s���܂���", "Error", MB_ICONSTOP);
		return false;
	}
	return true;
}

void Vertex2D::ReleaseConstantBuffer()
{
	if (m_pConstantBuffer != NULL)
	{
		m_pConstantBuffer->Release();
		m_pConstantBuffer = NULL;
	}
}