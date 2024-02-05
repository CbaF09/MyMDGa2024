#pragma once

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"
#include "../atlib/Utilities/AtlRandom.h"
#include "../atlib/Singletons/SceneManager.h"
#include "../atlib/Scenes/DungeonScene.h"
#include "../atlib/Scenes/TitleScene.h"
#include "../atlib/Scenes/Scene_Dummy.h"
#include "../atlib/Scenes/GameClearScene.h"
#include "../atlib/Scenes/GameOverScene.h"
#include "gm_main.h"

void gameStart() {
	atl::mtRandomInit();

	//SetFogEnable(true);
	//SetFogColor(32, 32, 32);
	//SetFogStartEnd(500.0f, 5000.0f);
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
	SetMouseDispFlag(false);
    LockCursorToWindow();

	//atl::SceneManager::getSceneManager(std::make_shared<atl::Scene_Dummy>());
	atl::SceneManager::getSceneManager(std::make_shared<atl::TitleScene>());
}

void gameMain(float delta_time) {
	atl::SceneManager::getSceneManager()->nowSceneUpdate(delta_time);
}

void gameEnd() {

}

void LockCursorToWindow() {
    HWND hwnd = GetMainWindowHandle(); // DX���C�u�����ŃE�B���h�E�n���h�����擾
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

    ClipCursor(&clipRect); // �J�[�\�������̗̈�ɐ���
}

////-----------------------------------------------------------------------------------------------------------
////
////
//// �p�[�e�B�N�����p�T���v��
////
//// ���@���̃T���v���̓p�[�e�B�N���G�f�B�^�Ƃ��Ă��@�\���܂�
////
//// ���@[ �d�v ]�@std::shared_ptr �Ŋm�ۂ��� dxe::Particle �͕K�� gameEnd() ��
//// �@�@�^�C�~���O�܂łɎQ�ƃJ�E���^�� 0 �ɂȂ�悤���Z�b�g���Ă������� ( gameEnd �֐����Q�� )
////
//// ���@[ �d�v ]�@���̋@�\�� DxLib �̋@�\�ł͂���܂���
//// �@�@dxe::Particle �� dxe::InstMeshPool �Ȃ� DirectX �𒼐ڐ��䂷��N���X�� render �֐���
////�@�@ dxe::DirectXRenderBegin() dxe::DirectXRenderEnd() �ň͂������ŃR�[�����Ă�������  
////-----------------------------------------------------------------------------------------------------------
////
////#include <time.h>
////#include <string>
////#include <numbers>
////#include <functional>
////#include "../dxlib_ext/dxlib_ext.h"
////#include "gm_main.h"
////
////
////-------------------------------------------------------------------------------------------------------
////
//// ���W�Ǝp���Ő��䂷��J����
////
////class TransformCamera : public dxe::Camera {
////public:
////
////    TransformCamera() {};
////    TransformCamera(int screen_w, int screen_h) : dxe::Camera(screen_w, screen_h) {}
////
////    void update();
////
////
////    inline tnl::Vector3 up() {
////        up_ = tnl::Vector3::TransformCoord({ 0, 1, 0 }, rot_);
////        return up_;
////    }
////    inline tnl::Vector3 down() { return -up(); }
////
////    inline tnl::Vector3 forward() override {
////        target_ = pos_ + tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_);
////        return tnl::Vector3::Normalize(target_ - pos_);
////    }
////    inline tnl::Vector3 back() override { return -forward(); }
////    inline tnl::Vector3 left() override { return tnl::Vector3::Cross(forward(), up()); }
////    inline tnl::Vector3 right() override { return tnl::Vector3::Cross(up(), forward()); }
////
////    tnl::Quaternion rot_;
////
////};
////
////void TransformCamera::update() {
////
////    
////     �p���p�����[�^����^�[�Q�b�g���W�ƃA�b�p�[�x�N�g�����v�Z
////    
////
////    target_ = pos_ + tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_);
////    up_ = tnl::Vector3::TransformCoord({ 0, 1, 0 }, rot_);
////    dxe::Camera::update();
////}
////
////
////
////Shared<TransformCamera> camera = nullptr;
////std::shared_ptr<dxe::Particle> ptcl = nullptr;
////bool is_draw_controller = true;
////
////------------------------------------------------------------------------------------------------------------
//// �Q�[���N�����ɂP�x�������s����܂�
////void gameStart() {
////
////    srand(time(0));
////    SetBackgroundColor(32, 32, 32);
////
////    camera = std::make_shared<TransformCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
////    camera->pos_ = { 0, 100, -250 };
////
////    ptcl = std::make_shared<dxe::Particle>("particle/preset/default.bin");
////
////}
////
////
////------------------------------------------------------------------------------------------------------------
//// ���t���[�����s����܂�
////void gameMain(float delta_time) {
////
////    ----------------------------------------------------------------------------------------------------
////    
////     �J��������
////    
////    tnl::Input::RunIndexKeyDown(
////        [&](uint32_t index) {
////            tnl::Vector3 v[4] = {
////                camera->left(),
////                camera->right(),
////                tnl::Vector3::up,
////                tnl::Vector3::down
////            };
////            camera->pos_ += v[index] * 3.0f;
////
////        }, eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S);
////
////    if (tnl::Input::IsMouseDown(tnl::Input::eMouse::RIGHT)) {
////        tnl::Vector3 mvel = tnl::Input::GetMouseVelocity();
////        camera->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(mvel.x * 0.2f));
////        camera->rot_ *= tnl::Quaternion::RotationAxis(camera->right(), tnl::ToRadian(mvel.y * 0.2f));
////    }
////    camera->pos_ += camera->forward().xz() * tnl::Input::GetMouseWheel() * 0.3f;
////
////
////     GUI �`�� ON / OFF
////    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F3)) is_draw_controller = !is_draw_controller;
////
////     �J�����A�b�v�f�[�g
////    camera->update();
////
////
////
////
////    ----------------------------------------------------------------------------------------------------
////    
////     �`�揈��
////    
////
////    DrawGridGround(camera, 50, 20);
////
////
////    ---------------------------------------------------------------------------
////    
////     �p�[�e�B�N���̕`��J�n
////    
////    dxe::DirectXRenderBegin();
////
////     �p�[�e�B�N�� A ��`��
////    ptcl->render(camera);
////
////     �p�[�e�B�N�� B ��`��
////     �p�[�e�B�N�� C ��...
////
////    ---------------------------------------------------------------------------
////    
////     �p�[�e�B�N���̕`��I��
////    
////    dxe::DirectXRenderEnd();
////
////
////
////
////    if (is_draw_controller) ptcl->drawGuiController({ 0, 0 });
////
////    DrawFpsIndicator({ DXE_WINDOW_WIDTH - 260, 0, 0 }, delta_time);
////
////    DrawString(DXE_WINDOW_WIDTH - 260, 20, "�E�N���b�N : ����", -1);
////    DrawString(DXE_WINDOW_WIDTH - 260, 40, "�z�C�[�� : �O��", -1);
////    DrawString(DXE_WINDOW_WIDTH - 260, 60, "A D : ���E�ړ�", -1);
////    DrawString(DXE_WINDOW_WIDTH - 260, 80, "W S : �㉺�ړ�", -1);
////    DrawString(DXE_WINDOW_WIDTH - 260, 100, "F3 : GUI �`�� ON/OFF", -1);
////
////}
////
////------------------------------------------------------------------------------------------------------------
//// �Q�[���I�����ɂP�x�������s����܂�
////void gameEnd() {
////    ptcl.reset();
////}