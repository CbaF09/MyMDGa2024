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

    // ダミーシーン
    // 役割   ... デバッグ用
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

        // シーケンス
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

            {// プレイヤーの移動
                player_->playerUpdate(deltaTime);
            }

            {// カメラコントロール ( 移動の後にやらないと、なんか変になる )
                player_->getPlayerCamera()->cameraControl(0.3f);
            }

            render(deltaTime);

            {// デバッグ用。ESCキーでウィンドウ落とす。
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
                    exit(1);
                }
            }

            return true;
        }


    };
};