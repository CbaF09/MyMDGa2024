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

    // ダミーシーン
    // 役割   ... デバッグ用
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

            auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
            for (auto& enemy : enemyList) {
                enemy->getMesh()->drawGuiMaterialControlloer();
                enemy->getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1));
            }

            EnemyManager::getEnemyManager()->renderAllEnemy(camera, deltaTime);

            DrawGridGround(camera, 50, 20);
        }

        // シーケンス
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {
            player_ = std::make_shared<PlayerPawn>();
            player_->initialize();
            player_->playerSpawn2Dpos({ 0,0 });

            EnemyManager::getEnemyManager()->setCurrentFactory(std::make_shared<BlueSlimeFactory>());
            EnemyManager::getEnemyManager()->spawnEnemy({0,1});
            
            SetMouseDispFlag(true);

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {

            player_->playerUpdate(deltaTime);

            return true;
        }


    };
};