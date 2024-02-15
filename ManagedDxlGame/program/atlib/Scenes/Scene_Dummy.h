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


            player_->render(deltaTime);

            magicRuneWindow_.draw();


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

            MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<HealRune>());
            MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<FireRune>());
            MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<HealRune>());

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {
            
            if (magicRuneWindow_.IsOpenMagicRuneWindow()) {
                magicRuneWindow_.process();
            }

            if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
                magicRuneWindow_.switchOpenMagicRuneWindow();
            }

            return true;
        }


    };
};