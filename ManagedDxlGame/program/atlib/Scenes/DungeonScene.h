#pragma once

#include "../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Singletons/DungeonCreater.h"
#include "../../atlib/Singletons/ResourceManager.h"
#include "../../atlib/MeshObject/Wall.h"
#include "../../atlib/MeshObject/Stairs.h"
#include "../../atlib/MeshObject/EnemyPawn.h"
#include "../../atlib/MeshObject/GroundTile.h"
#include "../../atlib/Object/PlayerPawn.h"
#include "../../atlib/Utilities/Atl3DCamera.h"
#include "../../atlib/Templates/make_shared_withInitFunc.h"
#include "../../atlib/Collision/Collision.h"
#include "Base_Scene.h"

namespace atl {

    // ダンジョンシーン
    // 役割 ... ダンジョン探索シーン
    class DungeonScene final : public Base_Scene {

    public:
        DungeonScene();
        
        // ターン制御用 enum
        enum class e_turnState {
            PLAYER,
            ENEMY,
        };
        
        // ゲッター
        static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        const e_turnState& getCurrentTurn() { return currentTurn_; }

        // セッター
        void setCurrenTurn(e_turnState nextTurn) { currentTurn_ = nextTurn; }

    private: 
        //----------------------
        // 変数
        
        // 汎用 ----------------------------------------
        // セル一辺の全長
        static const int32_t CELL_FULL_LENGTH = 1000;
        e_turnState currentTurn_ = e_turnState::PLAYER;

        // 壁 用 ---------------------------------------
        Shared<Wall> originWall_ = nullptr;
        std::vector<Shared<Wall>> walls_;

        // 地面 用 -------------------------------------
        Shared<GroundTile> originGroundTile_ = nullptr;
        std::vector<Shared<GroundTile>> groundTiles_;

        // 階段 用 -------------------------------------
        Shared<Stairs> originStairs_ = nullptr;

        // プレイヤー関連 ------------------------------
        Shared<PlayerPawn> player_ = nullptr;

        // エネミー関連 --------------------------------
        std::vector<Shared<EnemyPawn>> enemies_;

        //----------------------
        // メソッド

        void sceneUpdate(float deltaTime) override;
        void render(float deltaTime , const Shared<Atl3DCamera> camera);
        void initDungeon();
        void generateDungeon();
        void generateWall(int generatePosX, int generatePosZ);
        void generateGround(int generatePosX, int generatePosZ);

        // シーケンス
        SEQUENCE(DungeonScene, &DungeonScene::seqInit);
        bool seqInit(float deltaTime);
        bool seqProcess(float deltaTime);

        // デバッグ用 ----------------------------------
        void debug_displayDungeonParam(const Shared<atl::Atl3DCamera>& camera, float deltaTime);

    };
};