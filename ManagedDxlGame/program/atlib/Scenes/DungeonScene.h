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
    class SelectWindow;

    // ダンジョンシーン
    // 役割 ... ダンジョン探索シーン
    class DungeonScene final : public Base_Scene, public std::enable_shared_from_this<DungeonScene> {

    public:
        ~DungeonScene();

        // ゲッター
        // 3D上のセル一辺の全長を取得。色々なクラスで取得するので static に
        inline static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        // 現在存在しているエネミーのリストを取得
        inline const std::list<Shared<EnemyPawn>>& getEnemyArray() const { return enemies_; }
        // プレイヤーポーンを取得
        inline const Shared<PlayerPawn> getPlayerPawn() const { return player_; }

    private: 
        //----------------------------------------------
        // 変数
        
        // 汎用 ----------------------------------------
        // セル一辺の全長 ( 3D上 ) 色々な所で取得したいので static
        static const int32_t CELL_FULL_LENGTH = 1000;
        // 現在のターン

        // 壁 用 ---------------------------------------
        Shared<Wall> originWall_ = nullptr; // クローン元になる壁メッシュへのポインタ
        std::vector<Shared<Wall>> walls_;   // 壁メッシュ群のリスト

        // 地面 用 -------------------------------------
        Shared<GroundTile> originGroundTile_ = nullptr; // クローン元になる地面メッシュへのポインタ
        std::vector<Shared<GroundTile>> groundTiles_;   // 地面メッシュ群のリスト

        // 階段関連 -------------------------------------
        Shared<Stairs> originStairs_ = nullptr; // 階段へのポインタ
        bool isPlayerOnStairs_ = false;

        // プレイヤー関連 ------------------------------
        Shared<PlayerPawn> player_ = nullptr; // プレイヤーポーンへのポインタ
        bool isOpenMenu_ = false;    // メニューを開いているか

        // エネミー関連 --------------------------------
        std::list<Shared<EnemyPawn>> enemies_;  // フィールドに存在するエネミーリスト

        // アイテム関連 --------------------------------
        std::list<Shared<ItemPawn>> items_; // フィールドにあるアイテムリスト

        // 階層管理用 ----------------------------------
        int32_t currentFloor_ = 1;      // 現在階層
        const int32_t MAX_FLOOR = 3;    // 最上階 ( 到達したらクリア階 )
        const float nextFloorTransitionTime = 0.1f;  // 次階層に進む時、黒画面のままの待機する時間
        bool isNextFloorTransition = false;

        // ターン制御用 --------------------------------
        enum class e_turnState {
            KEY_INPUT,
            PLAYER_MOVE,
            PLAYER_ON_STAIRS,
        }currentTurn_ = e_turnState::KEY_INPUT;

        // UI 関連 -------------------------------------
        const tnl::Vector2i HP_BAR_LEFT_UP_POINT{ 5,5 }; // HPバーの枠の位置
        const tnl::Vector2i HP_BAR_RIGHT_DOWN_POINT{ 355,55 }; // HPバーの枠の位置
        const tnl::Vector2i HP_BAR_ADJUST_VALUE{ 8,5 }; // HPバーの枠とバー自体の間の隙間
        const int NEXT_FLOOR_TEXT_FONTSIZE = 30;

        // テキストログ関連 ----------------------------
        const tnl::Vector2i TEXT_LOG_POSITION{ 20,550 };   // テキストログを描画する位置 ( 一番上の行 )

        // 選択肢ウィンドウ関連 ------------------------
        Shared<SelectWindow> selectWindow_ = nullptr;

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
        // 次の階層に遷移中に、現在の階層を表示する
        void drawNextFloorTransition();

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
        // ゲームオーバーの処理 ( ゲームオーバーシーンに遷移 )
        bool seqGameOver(float deltaTime);
        // 死んだエネミーの消滅処理
        bool seqDeadEnemyProcess(float deltaTime);
        // 敵とプレイヤーの行動完了フラグをオフにする
        bool seqAllTurnFlagOff(float deltaTime);
        // 次の階層に移動している間
        bool seqToNextFloor(float deltaTime);

        // キー入力待ち
        void processKeyInput(float deltaTime);
        // プレイヤーが移動したターンの処理
        void processPlayerMoveTurn(float deltaTime);
        // プレイヤーが階段に乗った時
        void processPlayerOnStairs(float deltaTime);

        // デバッグ用 ----------------------------------
        void debug_displayDungeonParam(float deltaTime);
        void debug_displayMap(float deltaTime);
    };
};