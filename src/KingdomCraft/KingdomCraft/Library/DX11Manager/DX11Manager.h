#ifndef DX11MANAGER_H
#define DX11MANAGER_H

#include <Windows.h>
#include <d3d11.h>
#include <d3dx11.h>	

class DX11Manager
{
public:
	~DX11Manager();

	/**
	 * �C���X�^���X�𐶐�
	 */
	static void Create()
	{
		if (m_pDX11Manager == NULL)
		{
			m_pDX11Manager = new DX11Manager();
		}
	}

	/**
	 * �C���X�^���X���擾����
	 * @return �C���X�^���X���Ԃ�
	 */
	static DX11Manager* GetInstance()
	{
		return m_pDX11Manager;
	}

	/**
	 * DX11Manager�C���X�^���X��j������
	 */
	static void Delete()
	{
		delete m_pDX11Manager;
		m_pDX11Manager = NULL;
	}

	/**
	 * DX11Manager�̏������֐�
	 * @param[in] _hWnd DX11Manager���Ή�����E�B���h�E�̃n���h��
	 * @return �������ɐ���������true
	 */
	bool Init(HWND _hWnd);

	/**
	 * DX11Manager�̉���֐�
	 */
	void Release();

	/**
	 * �`��O�������s���֐�
	 */
	void BeginScene();

	/**
	 * �`��㏈�����s���֐�
	 */	
	void EndScene();
	
	/**
	 * DirectX11�f�o�C�X���擾����֐�
	 * @return DirectX11�f�o�C�X
	 */
	ID3D11Device* GetDevice();

	/**
	 * DirectX11�f�o�C�X�R���e�L�X�g���擾����֐�
	 * @return DirectX11�f�o�C�X�R���e�L�X�g
	 */
	ID3D11DeviceContext* GetDeviceContext();

private:
	DX11Manager();
	bool InitDevice();
	bool InitDisplay();
	void ReleaseDevice();
	void ReleaseDisplay();

	static DX11Manager*		m_pDX11Manager;

	ID3D11Device*			m_pDevice;			
	ID3D11DeviceContext*	m_pDeviceContext;

	IDXGIDevice1*			m_pDXGI;			
	IDXGIAdapter*			m_pAdapter;			
	IDXGIFactory*			m_pDXGIFactory;

	IDXGISwapChain*			m_pDXGISwapChain;
	DXGI_SWAP_CHAIN_DESC	m_DXGISwapChainDesc;

	ID3D11Texture2D*		m_pBackBuffer;
	ID3D11RenderTargetView* m_pRenderTargetView;

	ID3D11Texture2D*		m_pDepthStencilBuffer;
	D3D11_TEXTURE2D_DESC	m_DepthDesc;
	ID3D11DepthStencilView* m_pDepthStencilView;

	D3D11_VIEWPORT			m_ViewPort;

	ID3D11RasterizerState*  m_pRasterizerState;
	D3D11_RASTERIZER_DESC	m_RasterizerDesc;

	HWND					m_hWnd;
	RECT					m_WindowRect;

};

#endif