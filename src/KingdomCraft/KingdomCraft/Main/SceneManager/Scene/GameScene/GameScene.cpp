﻿/**
 * @file   GameScene.cpp
 * @brief  GameSceneクラスの実装
 * @author kotani
 */
#include "GameScene.h"
#include "..//Scene.h"

GameScene::GameScene() :
Scene(SceneID::SCENE_GAME)
{
}

GameScene::~GameScene()
{
}

SceneID GameScene::Control()
{
	return m_sceneID;
}

void GameScene::Draw()
{

}