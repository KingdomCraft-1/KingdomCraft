#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <Windows.h>
enum SceneID;
class Scene;
class SceneFactory;
class SceneManager
{
public:
	SceneManager(HWND _hwnd);
	~SceneManager();
	bool Run();

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

	void		  Control();
	void		  Draw();
	SceneFactory* m_pSceneFactory;
	Scene*		  m_pScene;
	SceneState    m_sceneState;
	SceneID		  m_nextSceneID;
	bool		  m_end;
	HWND		  m_hWnd;
};
#endif