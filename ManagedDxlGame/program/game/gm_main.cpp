#pragma once

#include <time.h>
#include <string>
#include <numbers>
#include <functional>

#include "gm_main.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"
#include "../atlib/Utilities/AtlRandom.h"
#include "../atlib/Singletons/SceneManager.h"
#include "../atlib/Singletons/FadeInOutManager.h"
#include "../atlib/Singletons/DungeonCreater.h"
#include "../atlib/Singletons/ResourceManager.h"
#include "../atlib/Singletons/TextLogManager.h"
#include "../atlib/Singletons/CsvManager.h"
#include "../atlib/Scenes/DungeonScene.h"
#include "../atlib/Scenes/TitleScene.h"
#include "../atlib/Scenes/Scene_Dummy.h"
#include "../atlib/Scenes/GameClearScene.h"
#include "../atlib/Scenes/GameOverScene.h"

// �ŏ��Ɉ��Ă΂��֐�
void gameStart() {
	atl::mtRandomInit();

    // �t�H�O�̃I���I�t�́ASkysphere�N���X�ōs���Ă��܂�
	SetFogColor(32, 32, 32);
	SetFogStartEnd(4000.0f, 5000.0f);

    // ���ˌ���ݒ�
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));

    // �w�i�F�ݒ�
	SetBackgroundColor(32, 32, 32);
    
    // �}�E�X�J�[�\�����\����
	SetMouseDispFlag(false);
    // �}�E�X�J�[�\�����E�B���h�E�ɌŒ�
    LockCursorToWindow();

    // �f�o�b�O�p�_�~�[�V�[��
    //atl::SceneManager::getSceneManager(std::make_shared<atl::Scene_Dummy>());
    
    // �{�҂̏����V�[���ݒ�
	atl::SceneManager::getSceneManager(std::make_shared<atl::TitleScene>());
}

// �Q�[�����C�����[�v
void gameMain(float delta_time) {
	atl::SceneManager::getSceneManager()->nowSceneUpdate(delta_time);
}

// �I�����Ɉ��Ă΂��֐�
void gameEnd() {
    // �V���O���g���Q�����
    atl::SceneManager::deleteSceneManager();
    atl::DungeonCreater::deleteDungeonCreater();
    atl::TextLogManager::deleteTextLogManager();
    atl::FadeInOutManager::deleteFadeInOutManager();
    atl::MagicRuneSystemManager::deleteMagicRuneSystemManager();
    atl::CsvManager::deleteCsvManager();

    atl::ResourceManager::deleteResourceManager();

    // DXlib �̊֐��Ń��������܂Ƃ߂ĉ��
    InitGraph();
    InitFontToHandle();
    InitSoundMem();
}

// �}�E�X�J�[�\�����E�B���h�E�ɌŒ肳����֐�
void LockCursorToWindow() {
    HWND hwnd = GetMainWindowHandle(); // �E�B���h�E�n���h�����擾
    RECT rect;
    GetClientRect(hwnd, &rect); // �N���C�A���g�̈�̃T�C�Y���擾

    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;

    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;

    MapWindowPoints(hwnd, nullptr, &ul, 1); // �N���C�A���g�̈�̍���̃X�N���[�����W�ւ̕ϊ�
    MapWindowPoints(hwnd, nullptr, &lr, 1); // �N���C�A���g�̈�̉E���̃X�N���[�����W�ւ̕ϊ�

    RECT clipRect;
    clipRect.left = ul.x;
    clipRect.top = ul.y;
    clipRect.right = lr.x;
    clipRect.bottom = lr.y;

    ClipCursor(&clipRect); // �J�[�\���𐧌�
}