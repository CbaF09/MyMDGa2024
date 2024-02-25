#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/EnemyManager/EnemyManager.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MagicRuneSystem/MagicRune.h"
#include "../MagicRuneSystem/MagicRuneWindow.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"
#include "../Object/MenuWindow.h"
#include "../Object/SelectWindow.h"
#include "../Scenes/GameClearScene.h"
#include "../Scenes/GameOverScene.h"
#include "../Scenes/TitleScene.h"

namespace atl {

    // ダミーシーン
    // 役割   ... デバッグ用
    class Scene_Dummy final :
        public Base_Scene {

    public:

    private:

        Shared<PlayerPawn> player_ = nullptr;
        int soundHandle = -1;
        VECTOR EmitterPos{ 0,0,0 };

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            player_->getPlayerCamera()->update();

            player_->render(deltaTime);
        }

        // シーケンス
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {
            player_ = std::make_shared<PlayerPawn>();
            player_->initialize();

            SetCreate3DSoundFlag(TRUE);
            soundHandle = LoadSoundMem("sound/SE/PlayerFootStep.ogg");
            SetCreate3DSoundFlag(FALSE);

            auto cameraPos = player_->getPlayerCamera()->pos_;
            EmitterPos = { cameraPos.x,cameraPos.y,cameraPos.z };
            Set3DPositionSoundMem(EmitterPos, soundHandle);

            Set3DRadiusSoundMem(500.0f, soundHandle);

            PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP);


            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {
            player_->playerUpdate(deltaTime);
            

            if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
                EmitterPos.z += 5;
            }
            if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
                EmitterPos.z -= 5;
            }

            Set3DPositionSoundMem(EmitterPos, soundHandle);

            return true;
        }


    };
};