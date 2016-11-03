#ifndef SCENEFACTORY_H
#define SCENEFACTORY_H
class Scene;
enum SceneID;
class SceneFactory
{
public:
	SceneFactory(){}
	~SceneFactory(){}

	/**
	* �V�[���N���X�̐���
	* @param _sceneID ��������N���X�̃V�[��ID
	* @return �V�[���N���X�̃C���X�^���X
	*/
	Scene* CreateScene(SceneID _sceneID);
};
#endif