#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/DungeonCreater.h"
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

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            
        }

        // シーケンス
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {
            DungeonCreater::getDungeonCreater()->createDungeon();

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {
            DungeonCreater::getDungeonCreater()->debag_DisplayFieldData();
            DungeonCreater::getDungeonCreater()->debag_DisplayFieldCells(600);

            if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
                DungeonCreater::getDungeonCreater()->createDungeon();
            }
            if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
                exit(0);
            }

            return true;
        }


    };
};