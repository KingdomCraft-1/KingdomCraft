#ifndef VERTEX2D_H
#define VERTEX2D_H
#include <D3DX11.h>
#include <d3dx10.h>

class Vertex2D
{
public:
	Vertex2D(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, HWND _hwnd);
	~Vertex2D();
	
	/**
	* ������
	* @param[in] _scale �X�P�[���T�C�Y 
	* @param[in] _textureFileName �e�N�X�`���̃t�@�C���p�X
	* @param[in] _pUV �e�N�X�`����4���_����UV�l
	* @return ����������true
	*/
	bool Init(RECT* _scale,char* _textureFileName, D3DXVECTOR2* _pUV);

	/**
	* �`�揈��
	* @param[in] _pDrawPos �`�悷��ʒu
	* @param[in] _angle �`�悷��p�x
	*/
	void Draw(D3DXVECTOR2* _pDrawPos, float _angle);

	/**
	* �J������
	*/
	void Release();
private:
	struct Vertex
	{
		D3DXVECTOR3 pos; //�ʒu
		D3DXVECTOR2 UV; //�e�N�X�`���[���W
	};
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX matWorld;
		D3DXVECTOR4 viewPort;
	};

	bool InitVertexShader();
	void ReleaseVertexShader();
	bool InitPixelShader();
	void ReleasePixelShader();
	bool InitVertexBuffer(RECT* _pScale, D3DXVECTOR2* _pUV);
	bool InitConstantBuffer();
	void ReleaseConstantBuffer();
	bool LoadTexture(char* _textureFileName);
	void ReleaseTexture();
	bool CreateVertexLayout();
	void ReleaseVertexLayout();

	ID3D11Device*			  m_pDevice;
	ID3D11DeviceContext*	  m_pDeviceContext;
	ID3D11PixelShader*		  m_pPixelShader;
	ID3D11VertexShader*		  m_pVertexShader;
	ID3D11Buffer*			  m_pConstantBuffer;
	ID3D11BlendState*		  m_pBlendState;
	ID3D11SamplerState*		  m_pSampler;
	ID3D11ShaderResourceView* m_pTexture;
	HWND					  m_hWnd;

	float					  m_WindowWidth;
	float					  m_WindowHeight;
	//���_���C�A�E�g�̐ݒ�Ɏg��
	ID3DBlob*		   m_pVertexCompiledShader;
	ID3D11InputLayout* m_pVertexLayout;


};
#endif