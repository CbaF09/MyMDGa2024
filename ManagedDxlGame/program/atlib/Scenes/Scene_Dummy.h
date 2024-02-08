#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"
#include "../Singletons/ResourceManager.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/EnemyPawn.h"
#include "../MeshObject/PlayerPawn.h"

namespace atl {

    // ダミーシーン
    // 役割   ... デバッグ用
    class Scene_Dummy final :
        public Base_Scene {

    public:

    private:
        Shared<PlayerPawn> player_ = nullptr;
        Shared<EnemyPawn> enemy_ = nullptr;

        Shared<GroundTile> groundOrigin = std::make_shared<GroundTile>(tnl::Vector3{ 1000, 1000, 1000 });
        Shared<GroundTile> ground1_ = std::make_shared<GroundTile>(groundOrigin->getMesh());
        Shared<GroundTile> ground2_ = std::make_shared<GroundTile>(groundOrigin->getMesh());

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            
            auto camera = player_->getPlayerCamera();

            camera->update();

            ground1_->renderObject(camera, deltaTime);
            ground2_->renderObject(camera, deltaTime);
            enemy_->renderObjects(camera, deltaTime);
            enemy_->renderTransparentObject(camera, deltaTime);

            player_->render(deltaTime);

//            DrawDefaultLightGuiController();
//            DrawGridGround(player_->getPlayerCamera(), 50, 20);
            DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);
        }

        // シーケンス
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {

            player_ = std::make_shared<PlayerPawn>();
            player_->initialize();
            player_->playerSpawn2Dpos({ 0,-1 });

            enemy_ = std::make_shared<EnemyPawn>(tnl::Vector2i{ 0,0 });

            ground1_->setMeshPos({ 0,0,0 });
            ground2_->setMeshPos({ 0,0,-1000 });

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {
            
            {// デバッグ用。ESCキーでウィンドウ落とす。
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
                    exit(1);
                }
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
                    ResourceManager::getResourceManager()->playSoundRes("sound/explosion.ogg",DX_PLAYTYPE_BACK);
                }
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

                }
            }

            return true;
        }


    };
};