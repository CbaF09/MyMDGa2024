#pragma once

#include <time.h>
#include <string>
#include <numbers>
#include <functional>

#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"
#include "../atlib/Utilities/AtlRandom.h"
#include "../atlib/Singletons/SceneManager.h"
#include "../atlib/Singletons/FadeInOutManager.h"
#include "../atlib/Singletons/DungeonCreater.h"
#include "../atlib/Singletons/ResourceManager.h"
#include "../atlib/Singletons/TextLogManager.h"
#include "../atlib/Scenes/DungeonScene.h"
#include "../atlib/Scenes/TitleScene.h"
#include "../atlib/Scenes/Scene_Dummy.h"
#include "../atlib/Scenes/GameClearScene.h"
#include "../atlib/Scenes/GameOverScene.h"
#include "gm_main.h"

void gameStart() {
	atl::mtRandomInit();

    // �t�H�O�̃I���I�t�́ASkybox�N���X�ōs���Ă��܂�
	SetFogColor(32, 32, 32);
	SetFogStartEnd(4000.0f, 5000.0f);

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
    
    // �}�E�X�J�[�\�����\����
	SetMouseDispFlag(false);
    // �}�E�X�J�[�\�����E�B���h�E�ɌŒ�
    LockCursorToWindow();

    // atl::SceneManager::getSceneManager(std::make_shared<atl::Scene_Dummy>());

	atl::SceneManager::getSceneManager(std::make_shared<atl::TitleScene>());
}

void gameMain(float delta_time) {
	atl::SceneManager::getSceneManager()->nowSceneUpdate(delta_time);
}

void gameEnd() {
    // �V���O���g���Q�����
    atl::TextLogManager::deleteTextLogManager();
    atl::DungeonCreater::deleteDungeonCreater();
    atl::FadeInOutManager::deleteFadeInOutManager();
    atl::ResourceManager::deleteResourceManager();
    atl::SceneManager::deleteSceneManager();

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

//#pragma once
//
////---------------------------------------------------------------------------------------------------------------
////
////
//// 3D �X�^�[�g�A�b�v�T���v��
////
////
////---------------------------------------------------------------------------------------------------------------
//
//#include <time.h>
//#include <string>
//#include <numbers>
//#include <functional>
//#include "../dxlib_ext/dxlib_ext.h"
//#include "gm_main.h"
//
//Shared<dxe::Camera> camera = nullptr;
//Shared<dxe::Mesh> mesh = nullptr;
//Shared<dxe::Texture> texture = nullptr;
//
//
////------------------------------------------------------------------------------------------------------------
//// �Q�[���N�����ɂP�x�������s����܂�
//void gameStart() {
//    srand(time(0));
//
//    SetFogEnable(true);
//    SetFogColor(32, 32, 32);
//    SetFogStartEnd(500.0f, 1500.0f);
//    ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
//    SetBackgroundColor(32, 32, 32);
//
//
//    camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
//
//    texture = dxe::Texture::CreateFromFile("graphics/test.jpg");
//
//    mesh = dxe::Mesh::CreateSphereMV(100, 20, 20);
//    mesh->setTexture(texture);
//    mesh->setBlendMode(DX_BLENDMODE_ALPHA);
//    mesh->setAlpha(0.5);
//}
//
//
////------------------------------------------------------------------------------------------------------------
//// ���t���[�����s����܂�
//void gameMain(float delta_time) {
//
//    mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(1));
//
//    camera->update();
//
//
//    DrawGridGround(camera, 50, 20);
//
//    mesh->render(camera);
//    mesh->drawGuiMaterialControlloer();
//
//    DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
//
//}
//
////------------------------------------------------------------------------------------------------------------
//// �Q�[���I�����ɂP�x�������s����܂�
//void gameEnd() {
//
//}
//
//
