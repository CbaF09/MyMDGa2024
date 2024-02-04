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
        inline const std::list<Shared<EnemyPawn>>& getEnemyArray() const { return enemies_; }

    private: 
        //----------------------------------------------
        // 変数
        
        // 汎用 ----------------------------------------
        // セル一辺の全長 ( 3D上 )
        static const int32_t CELL_FULL_LENGTH = 1000;
        // 現在のターン
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
        bool isOpenMenu_ = false;    // メニューを開いているか

        // エネミー関連 --------------------------------
        std::list<Shared<EnemyPawn>> enemies_;

        // アイテム関連 --------------------------------
        std::list<Shared<ItemPawn>> items_;

        // UI 関連 -------------------------------------
        const tnl::Vector2i HP_BAR_LEFT_UP_POINT{ 5,5 }; // HPバーの枠の位置
        const tnl::Vector2i HP_BAR_RIGHT_DOWN_POINT{ 355,55 }; // HPバーの枠の位置
        const tnl::Vector2i HP_BAR_ADJUST_VALUE{ 8,5 }; // HPバーの枠とバー自体の間の隙間

        // フェードイン ・ アウト関連 ------------------
        int32_t fadeAlphaValue_ = 255; // 0 ... 透明、 255 ... 真っ黒
        const int32_t fadeSpeed_ = 3;
        enum class e_FadeState{
            FADE_NONE, // フェード中でない
            FADE_IN, // フェードイン中 ( 透明に向かっている )
            FADE_OUT // フェードアウト中 ( 真っ黒に向かっている )
        }
        currentFadeState_ = e_FadeState::FADE_NONE;

        //----------------------------------------------
        // メソッド

        void sceneUpdate(float deltaTime) override;

        // 3Dのレンダー
        void render(float deltaTime , const Shared<Atl3DCamera> camera);
        // 2D系の描画 ( 2D系をまとめて描画する )
        void draw2D(float deltaTime);
        // 2D UI の描画
        void drawUI(float deltaTime);
        // 2D HPbar の描画
        void drawHPbar();
        // フェードイン ・ アウト用黒色短径の透明度を徐々に変化
        void fadeBlackRect();
        // フェードイン ・ アウト用の黒色短形の描画
        void drawFadeBlackRect(float deltaTime);

        // ダンジョンの初期化
        void initDungeon();

        // ダンジョン生成
        void generateDungeon();

        // 壁を生成
        // arg ... 2D座標位置
        void generateWall(int generatePosX, int generatePosZ);
        // 地面を生成
        // arg ... 2D座標位置
        void generateGround(int generatePosX, int generatePosZ);

        // シーケンス
        SEQUENCE(DungeonScene, &DungeonScene::seqInit);
        bool seqInit(float deltaTime);
        // 現在のターンに応じた処理を実行
        bool seqTurnStateProcess(float deltaTime);
        // 死んだエネミーの消滅処理
        bool seqDeadEnemyProcess(float deltaTime);
        // 敵とプレイヤーの行動完了フラグをオフにする
        bool seqAllTurnFlagOff(float deltaTime);

        void processKeyInput(float deltaTime);
        void processPlayerMoveTurn(float deltaTime);

        // デバッグ用 ----------------------------------
        void debug_displayDungeonParam(float deltaTime);

    };
};