#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <d3d11.h>
#include <map>

class TextureManager
{
public:
	TextureManager(ID3D11Device* _pDevice);
	~TextureManager();

	/**
	 * �e�N�X�`����ǂݍ���
	 * @param[in] _key �ǂݍ��񂾃e�N�X�`���̊i�[��ւ̃L�[
	 * @param[in] _filePath �ǂݍ��ރe�N�X�`��
	 * @return �e�N�X�`���̓ǂݍ��݂ɐ���������true
	 */
	bool LoadTexture(int _key, char* _filePath);

	/**
	 * �i�[���Ă���e�N�X�`�����擾����
	 * @param[in] _key �擾�������e�N�X�`���̃L�[
	 * @return �e�N�X�`���̃��\�[�X�r���[
	 */
	inline ID3D11ShaderResourceView* GetTexture(int _key)
	{ 
		return m_pTextureResourceView[_key];
	}
	
	/**
	 * �i�[���Ă���e�N�X�`�����������
	 * @param[in] _key ����������e�N�X�`���̃L�[
	 */
	inline void ReleaseTexture(int _key)
	{ 
		m_pTextureResourceView[_key]->Release();
		m_pTextureResourceView.erase(_key);
	}

private:
	ID3D11Device*								m_pDevice;
	std::map<int, ID3D11ShaderResourceView*>	m_pTextureResourceView;

};

#endif