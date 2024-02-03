#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

    class Stairs;
    class EnemyPawn;
    class PlayerPawn;
    class ItemPawn;
    class Wall;
    class GroundTile;
    class DungeonCreater;
    class Atl3DCamera;

    // ダンジョンシーン
    // 役割 ... ダンジョン探索シーン
    class DungeonScene final : public Base_Scene, public std::enable_shared_from_this<DungeonScene> {

    public:
        // ターン制御用 enum
        enum class e_turnState {
            KEY_INPUT,
            PLAYER_MOVE,
        };
        
        // ゲッター
        inline static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        inline const e_turnState& getCurrentTurn() { return currentTurn_; }
        inline const std::vector<Shared<EnemyPawn>>& getEnemyArray() const { return enemies_; }

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
        bool isOpenMenu = false;    // メニューを開いているか ( 本を開いているか )

        // エネミー関連 --------------------------------
        std::vector<Shared<EnemyPawn>> enemies_;

        // アイテム関連 --------------------------------
        std::vector<Shared<ItemPawn>> items_;

        // UI 関連 -------------------------------------
        const tnl::Vector2i HP_BAR_LEFT_UP_POINT{ 5,5 }; // HPバーの枠の位置
        const tnl::Vector2i HP_BAR_RIGHT_DOWN_POINT{ 355,55 }; // HPバーの枠の位置
        const tnl::Vector2i HP_BAR_ADJUST_VALUE{ 8,5 }; // HPバーの枠とバー自体の間の隙間

        //----------------------
        // メソッド

        void sceneUpdate(float deltaTime) override;

        // 3Dのレンダー
        void render(float deltaTime , const Shared<Atl3DCamera> camera);
        // 2D系の描画
        void draw2D(float deltaTime);
        // UI の描画
        void drawUI(float deltaTime);
        // HPbar
        void drawHPbar();

        // ダンジョンの初期化
        void initDungeon();

        // ダンジョン生成
        void generateDungeon();

        // 壁を生成
        // arg ... 生成する2D座標位置
        void generateWall(int generatePosX, int generatePosZ);
        // 地面を生成
        // arg ... 生成する2D座標位置
        void generateGround(int generatePosX, int generatePosZ);

        // シーケンス
        SEQUENCE(DungeonScene, &DungeonScene::seqInit);
        bool seqInit(float deltaTime);
        bool seqTurnStateProcess(float deltaTime);
        bool seqAllTurnFlagOff(float deltaTime);

        void processKeyInput(float deltaTime);
        void processPlayerMoveTurn(float deltaTime);

        // デバッグ用 ----------------------------------
        void debug_displayDungeonParam(float deltaTime);

    };
};