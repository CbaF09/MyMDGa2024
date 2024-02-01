#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

    class Stairs;
    class EnemyPawn;
    class Wall;
    class GroundTile;
    class PlayerPawn;
    class DungeonCreater;
    class Atl3DCamera;

    // ダンジョンシーン
    // 役割 ... ダンジョン探索シーン
    class DungeonScene final : public Base_Scene {

    public:
        // ターン制御用 enum
        enum class e_turnState {
            KEY_INPUT,
            PLAYER_MOVE,
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
        e_turnState currentTurn_ = e_turnState::KEY_INPUT;

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
        const float CAMERA_ROT_SPEED = 0.3f;	// カメラ回転速度

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
        bool seqTurnStateProcess(float deltaTime);
        bool seqAllActionFlagOff(float deltaTime);

        void processKeyInput(float deltaTime);
        void processPlayerMoveTurn(float deltaTime);

        // デバッグ用 ----------------------------------
        void debug_displayDungeonParam(const Shared<atl::Atl3DCamera>& camera, float deltaTime);

    };
};