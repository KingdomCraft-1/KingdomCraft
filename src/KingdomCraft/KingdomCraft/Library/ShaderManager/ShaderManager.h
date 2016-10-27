#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <d3d11.h>
#include <map>

class ShaderManager
{
public:
	ShaderManager(ID3D11Device* _pDevice);
	~ShaderManager();

	/**
	 * ���_�V�F�[�_�[��ǂݍ���(���_�V�F�[�_�[�̃o�[�W������vs_5_0)
	 * @param[in] _key ���_�V�F�[�_�[���i�[�����ꏊ�̃L�[
	 * @param[in] _fileName �ǂݍ��ރG�t�F�N�g�t�@�C���̃p�X
	 * @param[in] _funcName ���_�V�F�[�_�[�̊֐���
	 * @return �����������̃t���O
	 */
	bool LoadVertexShader(int _key, char* _fileName, char* _funcName);

	/**
	 * �s�N�Z���V�F�[�_�[��ǂݍ���(�s�N�Z���V�F�[�_�[���̃o�[�W������ps_5_0)
	 * @param[in] _key �s�N�Z���V�F�[�_�[���i�[�����ꏊ�̃L�[
	 * @param[in] _fileName �ǂݍ��ރG�t�F�N�g�t�@�C���̃p�X
	 * @param[in] _funcName �s�N�Z���V�F�[�_�[�̊֐���
	 * @return �����������̃t���O
	 */
	bool LoadPixelShader(int _key, char* _fileName, char* _funcName);

	/**
	 * ���_�V�F�[�_�[���擾����
	 * @param[in] _key �擾���������_�V�F�[�_�[�̃L�[
	 * @return ���_�V�F�[�_�[
	 */
	ID3D11VertexShader* GetVertexShader(int _key)
	{
		return m_pVertexShader[_key];
	}

	/**
	 * ���_�V�F�[�_�[�̃R���p�C���f�[�^���擾����
	 * @param[in] _key �擾�������R���p�C���f�[�^�̃L�[
	 * @return ���_�V�F�[�_�[�̃R���p�C���f�[�^
	 */
	ID3DBlob* GetCompiledVertexShader(int _key)
	{
		return m_pCompiledVertexShader[_key];
	}
	
	/**
	 * �s�N�Z���V�F�[�_�[���擾����
	 * @param[in] _key �擾�������s�N�Z���V�F�[�_�[�̃L�[
	 * @return �s�N�Z���V�F�[�_�[
	 */
	ID3D11PixelShader* GetPixelShader(int _key)
	{
		return m_pPixelShader[_key];
	}

	/**
	 * �s�N�Z���V�F�[�_�[�̃R���p�C���f�[�^���擾����
	 * @param[in] _key �擾�������R���p�C���f�[�^�̃L�[
	 * @return �s�N�Z���V�F�[�_�[�̃R���p�C���f�[�^
	 */
	ID3DBlob* GetCompiledPixelShader(int _key)
	{
		return m_pCompiledPixelShader[_key];
	}

	/**
	 * ���_�V�F�[�_�[���������
	 * @param[in] _key ������钸�_�V�F�[�_�[�̃L�[
	 */
	void ReleaseVertexShader(int _key)
	{
		m_pVertexShader[_key]->Release();
		m_pVertexShader.erase(_key);
	}

	/**
	 * ���_�V�F�[�_�[�̃R���p�C���f�[�^���������
	 * @param[in] _key �������R���p�C���f�[�^�̃L�[
	 */
	void ReleaseCompiledVertexShader(int _key)
	{
		m_pCompiledVertexShader[_key]->Release();
		m_pCompiledVertexShader.erase(_key);
	}

	/**
	 * �s�N�Z���V�F�[�_�[���������
	 * @param[in] _key �������s�N�Z���V�F�[�_�[�̃L�[
	 */
	void ReleasePixelShader(int _key)
	{
		m_pPixelShader[_key]->Release();
		m_pPixelShader.erase(_key);
	}

	/**
	 * �s�N�Z���V�F�[�_�[�̃R���p�C���f�[�^���������
	 * @param[in] _key �������R���p�C���f�[�^�̃L�[
	 */
	void ReleaseCompiledPixelShader(int _key)
	{
		m_pCompiledPixelShader[_key]->Release();
		m_pCompiledPixelShader.erase(_key);
	}

private:
	ID3D11Device* m_pDevice;
	std::map<int, ID3D11VertexShader*>	m_pVertexShader;
	std::map<int, ID3D11PixelShader*>	m_pPixelShader;
	std::map<int, ID3DBlob*>			m_pCompiledVertexShader;
	std::map<int, ID3DBlob*>			m_pCompiledPixelShader;

};

#endif