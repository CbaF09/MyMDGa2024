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

    // ダミーシーン
    // 役割   ... デバッグ用
    class Scene_Dummy final :
        public Base_Scene {

    public:

    private:

        Shared<PlayerPawn> player_;
        Shared<EnemyPawn> enemy_;

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            auto camera = player_->getPlayerCamera();

            camera->update();

            enemy_->renderObjects(camera);

            player_->debug_displayPlayerParam(0, 15);

            DrawDefaultLightGuiController();
            DrawGridGround(player_->getPlayerCamera(), 50, 20);
            DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);
        }

        // シーケンス
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {
            player_ = std::make_shared<PlayerPawn>();
            enemy_ = std::make_shared<EnemyPawn>(tnl::Vector3{ 0,50,0 }, tnl::Vector3{ 100,100,100 });

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {

            {// プレイヤーの移動
                player_->playerUpdate(deltaTime);
            }

            {
                enemy_->enemyUpdate(deltaTime);
            }

            {// カメラコントロール ( 移動の後にやらないと、なんか変になる )
                player_->cameraControl(0.3f);
            }

            {// デバッグ用。ESCキーでウィンドウ落とす。
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
                    exit(1);
                }
            }

            return true;
        }


    };
};