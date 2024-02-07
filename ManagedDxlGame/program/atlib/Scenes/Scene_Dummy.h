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
#include "../MeshObject/MenuBook.h"
#include "../MeshObject/ItemPawn.h"
#include "../Object/SelectWindow.h"
#include "../Object/MenuWindow.h"
#include "../Object/ItemData.h"
#include "../Object/Inventory.h"
#include "../Collision/Collision.h"

namespace atl {

    // �_�~�[�V�[��
    // ����   ... �f�o�b�O�p
    class Scene_Dummy final :
        public Base_Scene {

    public:

    private:
        Shared<Inventory> inventory_ = nullptr;
        Shared<MenuWindow> menu_ = nullptr;

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            
//            DrawDefaultLightGuiController();
//            DrawGridGround(player_->getPlayerCamera(), 50, 20);
            DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);
        }

        // �V�[�P���X
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {
            inventory_ = std::make_shared<Inventory>(std::make_shared<ItemData>(1));

            menu_ = std::make_shared<MenuWindow>(inventory_);

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {


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