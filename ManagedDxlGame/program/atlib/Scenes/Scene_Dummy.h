#pragma once

#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"
#include "../Singletons/DungeonCreater.h"
#include "../Object/PlayerPawn.h"
#include "../MeshObject/Wall.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/EnemyPawn.h"
#include "../MeshObject/Stairs.h"
#include "../Collision/Collision.h"

namespace atl {

    // �_�~�[�V�[��
    // ����   ... �f�o�b�O�p
    class Scene_Dummy final :
        public Base_Scene {

    public:

    private:

        Shared<PlayerPawn> player_;

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            auto camera = player_->getPlayerCamera();

            camera->update();


            player_->debug_displayPlayerParam(0, 15);

            DrawDefaultLightGuiController();
            DrawGridGround(player_->getPlayerCamera(), 50, 20);
            DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);
        }

        // �V�[�P���X
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {
            player_ = std::make_shared<PlayerPawn>();

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {

            {// �v���C���[�̈ړ�
                player_->playerUpdate(deltaTime);
            }

            {

            }

            {// �J�����R���g���[�� ( �ړ��̌�ɂ��Ȃ��ƁA�Ȃ񂩕ςɂȂ� )
                player_->cameraControl();
            }

            {// �f�o�b�O�p�BESC�L�[�ŃE�B���h�E���Ƃ��B
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
                    exit(1);
                }

                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {

                }
            }

            return true;
        }


    };
};