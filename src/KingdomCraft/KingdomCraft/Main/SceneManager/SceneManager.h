#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <Windows.h>
enum SceneID;
class Scene;
class DX11Manager;
class SceneManager
{
public:
	SceneManager(HWND _hwnd);
	~SceneManager();
	bool Run();
	void Control();
	void Draw();

private:
	enum SceneState
	{
		/// ����
		SCENE_CREATE,
		/// ������
		SCENE_PROC,
		/// ���
		SCENE_RELEASE
	};

	Scene*		 m_pScene;
	SceneState   m_sceneState;
	SceneID		 m_nextSceneID;
	bool		 m_end; //�Q�[���I���t���O
	/// �E�B���h�E�n���h��
	HWND		 m_hWnd;
};
#endif