#ifndef DEBUGFONT_H
#define DEBUGFONT_H
#include <d3d11.h>
#include <d3dx10.h>

#define FONT_TU 1.0f / 95

class DebugFont
{
public:
	DebugFont();
	~DebugFont();

	/**
	 * DebugFont�N���X�̏������֐�
	 * @param[in] _pDevice DirectX11�̃f�o�C�X
	 * @param[in] _pDeviceContext DirectX11�̃f�o�C�X�R���e�L�X�g
	 * @param[in] _hWnd DebugFont�N���X���Ή�����Window�̃n���h��
	 * @return �������ɐ���������true���Ԃ�
	 */
	bool Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, HWND _hWnd);

	/**
	 * DebugFont�N���X�̉���֐�
	 */
	void Release();

	/**
	 * �t�H���g�̏�����
	 * @param[in] _FontHeight �t�H���g�̍���
	 * @param[in] _FontWidth �t�H���g�̕�
	 * @param[in] _FontColor �t�H���g�̐F
	 */
	bool InitFont(float _FontHeight, float _FontWidth, D3DXCOLOR _FontColor);

	/**
	 * �t�H���g�`��֐�
	 * @param[in] _DrawPos �t�H���g��\������ʒu
	 * @param[in] _pStr �\�����镶����
	 */
	void DrawFont(D3DXVECTOR2* _pDrawPos, char* _pStr);

private:
	struct FONT_VERTEX
	{
		D3DXVECTOR3 Pos;
		D3DXCOLOR	Color;
		float		tu;
		float		tv;
	};

	struct CONSTANT_BUFFER
	{
		D3DXMATRIX MatWVP;
		D3DXVECTOR4 TexelOffset;
		D3DXVECTOR4 PosOffset;
	};

	bool InitResourceView();
	bool InitConstantBuffer();
	bool InitShader();
	bool InitVertexShader();
	bool InitPixelShader();
	bool InitSamplerState();
	bool InitBlendState();
	bool InitDepthStencilState();

	void ReleaseResourceView();
	void ReleaseConstantBuffer();
	void ReleaseShader();
	void ReleaseVertexShader();
	void ReleasePixelShader();
	void ReleaseSamplerState();
	void ReleaseBlendState();
	void ReleaseDepthStencilState();


	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pDeviceContext;
	ID3D11ShaderResourceView*	m_pResourceView;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11InputLayout*			m_pLayout;
	ID3D11PixelShader*			m_pPixelShader;
	ID3D11SamplerState*			m_pSamplerState;
	ID3D11BlendState*			m_pBlendState;
	ID3D11DepthStencilState*	m_pDepthStencilState;
	HWND						m_hWnd;
	float						m_FontHeight;
	float						m_FontWidth;
	float						m_WindowWidth;
	float						m_WindowHeight;

};

#endif