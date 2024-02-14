#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"
#include "../Singletons/ResourceManager.h"
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
        MagicRuneWindow magicRuneWindow_;

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            
            auto camera = player_->getPlayerCamera();

            camera->update();

            magicRuneWindow_.draw();

            player_->render(deltaTime);


//            DrawDefaultLightGuiController();
            DrawGridGround(player_->getPlayerCamera(), 50, 20);
            DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);
        }

        // シーケンス
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {

            player_ = std::make_shared<PlayerPawn>();
            player_->initialize();
            player_->playerSpawn2Dpos({ 0,0 });

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {
            
            if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
                MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<HealBuffMagicRune>());
            }

            if (tnl::Input::IsKeyDownTrigger(eKeys::KB_2)) {
                MagicRuneSystemManager::getMagicRuneSystemManager()->removeRune(2);
            }

            {// デバッグ用。ESCキーでウィンドウ落とす。
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