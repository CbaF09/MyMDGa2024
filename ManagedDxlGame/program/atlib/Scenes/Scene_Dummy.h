#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/EnemyFactory.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MagicRuneSystem/MagicRune.h"
#include "../MagicRuneSystem/MagicRuneWindow.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"

namespace atl {

    // �_�~�[�V�[��
    // ����   ... �f�o�b�O�p
    class Scene_Dummy final :
        public Base_Scene {

    public:

    private:

        Shared<PlayerPawn> player_ = nullptr;

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            auto camera = player_->getPlayerCamera();
            camera->update();

            player_->render(deltaTime);

            EnemyManager::getEnemyManager()->renderAllEnemy(camera, deltaTime);

            DrawGridGround(camera, 50, 20);
        }

        // �V�[�P���X
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {
            player_ = std::make_shared<PlayerPawn>();
            player_->initialize();
            player_->playerSpawn2Dpos({ 0,1 });

            EnemyManager::getEnemyManager()->setCurrentFactory(std::make_shared<SlimeFactory>());
            EnemyManager::getEnemyManager()->generateEnemy();

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {

            player_->playerUpdate(deltaTime);

            EnemyManager::getEnemyManager()->processAllEnemy(deltaTime);

            return true;
        }


    };
};