// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

#pragma once
#include <vector>
#include <Windows.h>

enum SceneID
{
	SCENE_LOGO,
	SCENE_OPENING,
	SCENE_TITLE,
	SCENE_GAME,			// ���߂����I�����ăQ�[���ɓ������Ƃ�
	SCENE_CONTINUE_GAME,	// ���������I�����ăQ�[���ɓ������Ƃ�
	SCENE_RESULT,
	SCENE_ENDING,
	FIN
};
