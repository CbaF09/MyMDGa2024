#pragma once

#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"
#include "../Singletons/DungeonCreater.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/Wall.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/EnemyPawn.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/MagicWand.h"
#include "../MeshObject/MagicBook.h"
#include "../MeshObject/ItemPawn.h"
#include "../Collision/Collision.h"

namespace atl {

    // �_�~�[�V�[��
    // ����   ... �f�o�b�O�p
    class Scene_Dummy final :
        public Base_Scene {

    public:

    private:

        Shared<PlayerPawn> player_;
        Shared<ItemPawn> item_;

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            
            player_->render(deltaTime);

            item_->renderObjects(player_->getPlayerCamera());

            DrawDefaultLightGuiController();
            DrawGridGround(player_->getPlayerCamera(), 50, 20);
            DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);
        }

        // �V�[�P���X
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {
            player_ = std::make_shared<PlayerPawn>();
            player_->playerSpawn2Dpos({ 0,1 });
            player_->initialize();

            item_ = std::make_shared<ItemPawn>(tnl::Vector2i{ 0,0 });
            item_->assignWeakPlayer(player_);

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {

            {// �v���C���[�̈ړ�
                player_->playerUpdate(deltaTime);
            }

            {// �J�����R���g���[�� ( �ړ��̌�ɂ��Ȃ��ƁA�Ȃ񂩕ςɂȂ� )
                player_->getPlayerCamera()->cameraControl(0.3f);
            }

            render(deltaTime);

            {// �f�o�b�O�p�BESC�L�[�ŃE�B���h�E���Ƃ��B
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
                    exit(1);
                }
            }

            return true;
        }


    };
};