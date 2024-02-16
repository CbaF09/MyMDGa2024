#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"
#include "../Object/MenuWindow.h"
#include "../Object/SelectWindow.h"
#include "../MeshObject/Skybox.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/Wall.h"
#include "../MagicRuneSystem/MagicRuneWindow.h"

namespace atl {

    class Stairs;
    class EnemyPawn;
    class PlayerPawn;
    class ItemPawn;
    class DungeonCreater;
    class Atl3DCamera;

    // ダンジョンシーン
    // 役割 ... ダンジョン探索シーン
    class DungeonScene final : public Base_Scene, public std::enable_shared_from_this<DungeonScene> {

    public:
        DungeonScene();
        ~DungeonScene();

        // ゲッター
        // 3D上のセル一辺の全長を取得。色々なクラスで取得するので static に
        inline static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        // 現在存在しているエネミーのリストを取得
        inline const std::list<Shared<EnemyPawn>>& getEnemyArray() const { return enemies_; }
        // プレイヤーポーンを取得
        inline const Shared<PlayerPawn> getPlayerPawn() const { return player_; }

        // セッター
        // 満腹度のセッター ( 0 ~ SATIETY_FULL の間にクランプ )
        inline void changeSatiety(int32_t changeValue) {
            currentSatiety_ += changeValue;
            if (currentSatiety_ <= 0) { currentSatiety_ = 0; }
            if (currentSatiety_ > SATIETY_FULL) { currentSatiety_ = SATIETY_FULL; }
        }

        // 毎ターンのHP回復
        void turnHealHP();


    private: 
        //----------------------------------------------
        // 変数
        
        // 汎用 ----------------------------------------
        // セル一辺の全長 ( 3D上 ) 色々な所で取得したいので static
        static const int32_t CELL_FULL_LENGTH = 1000;
        // 現在のターン

        // 壁 用 ---------------------------------------
        Shared<Wall> originWall_ = std::make_shared<Wall>(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH * 2 ,CELL_FULL_LENGTH }); // クローン元になる壁メッシュ
        std::vector<Shared<Wall>> walls_{};   // 壁メッシュ群のリスト

        // 地面 用 -------------------------------------
        Shared<GroundTile> originGroundTile_ = std::make_shared<GroundTile>(tnl::Vector3{ CELL_FULL_LENGTH,CELL_FULL_LENGTH,0 }); // クローン元になる地面メッシュ PlaneMVモデルの関係で、Yの所にZの値を入れてます 
        std::vector<Shared<GroundTile>> groundTiles_{};   // 地面メッシュ群のリスト

        // 階段関連 -------------------------------------
        Shared<Stairs> originStairs_ = nullptr; // 階段へのポインタ
        bool isPlayerOnStairs_ = false;

        // プレイヤー関連 ------------------------------
        Shared<PlayerPawn> player_ = nullptr; // プレイヤーポーンへのポインタ
        bool isOpenMenu_ = false;    // メニューを開いているか
        const int32_t EVERY_TURN_HEAL = 1; // ターンごとのHP回復量

        // エネミー関連 --------------------------------
        std::list<Shared<EnemyPawn>> enemies_;  // フィールドに存在するエネミーリスト
        const int32_t RESPORN_TURN_COUNT = 30;   // 何ターンごとにリスポーンするか
        int32_t respornTurnTimer_ = 0;           // リスポーンターンカウンター

        // アイテム関連 --------------------------------
        std::list<Shared<ItemPawn>> items_; // フィールドにあるアイテムリスト

        // 階層管理用 ----------------------------------
        int32_t currentFloor_ = 1;      // 現在階層
        const int32_t MAX_FLOOR = 4;    // 最上階 ( 到達したらクリア階 )
        const float nextFloorTransitionTime = 2.5f;  // 次階層に進む時、黒画面のままの待機する時間
        bool isNextFloorTransition = false; // 次階層に遷移中か ( 黒画面か )

        // UI 関連 -------------------------------------
        const tnl::Vector2i HP_BAR_LEFT_UP_POINT{ 5,5 }; // HPバーの枠の位置
        const tnl::Vector2i HP_BAR_RIGHT_DOWN_POINT{ 355,55 }; // HPバーの枠の位置
        const tnl::Vector2i HP_BAR_ADJUST_VALUE{ 8,5 }; // HPバーの枠とバー自体の間の隙間
        const tnl::Vector2i HP_STRING_POSITION{ 50,15 }; // HP数値表示の位置
        
        const tnl::Vector2i LEVEL_STRING_POSITION{ 60,390 }; // レベルの文字列を描画する位置
        
        const tnl::Vector2i INSTRUCTION_POSITION{ DXE_WINDOW_WIDTH/2 + 100,600 }; // 操作説明の画像を描画する位置
        const tnl::Vector2i INSTRUCTION_BACK_BOX_SIZE{ 575,160 }; // 操作説明の画像を描画する位置
        const float INSTRUCTION_SIZE = 0.45f; // 操作説明の画像を描画する位置
        
        const tnl::Vector2i TEXT_LOG_POSITION{ 20,550 };   // テキストログを描画する位置 ( 一番上の行 )

        // フォント-------------------------------------
        const int LEVEL_STRING_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
        const int NEXT_FLOOR_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);

        // ミニマップ関連-------------------------------
        const int32_t MINIMAP_ALPHA = 128;  // ミニマップの透過度
        const tnl::Vector2i MINIMAP_LEFTUP_POSITION{ 600,130 }; // ミニマップの一番左上に位置する座標
        const int32_t MINIMAP_CELL_SIZE = 12;    // ミニマップの1マスの大きさ
        const int32_t MINIMAP_PLAYER_SIZE = MINIMAP_CELL_SIZE / 3;    // ミニマップの1マスの大きさ

        // 選択肢ウィンドウ関連 ------------------------
        SelectWindow selectWindow_;
        bool isSelectWindow_ = false;

        // メニュー関連 --------------------------------
        Shared<MenuWindow> menuWindow_ = nullptr;
        MenuWindow::e_SelectedMenuWindow selectedMenu = MenuWindow::e_SelectedMenuWindow::Item1;

        // ルーンウィンドウ関連
        MagicRuneWindow magicRuneWindow_;

        // スカイボックス用 ----------------------------
        // Skyboxは引数無しコンストラクタ一つのみを持っている
        Skybox skybox_;

        // 満腹度 ( 招待状 ) 関連 ----------------------
        const tnl::Vector2i INVATATION_STRING_POSITION{ 60,480 }; // 「招待状」と描画する位置 ( 中心座標 )
        const tnl::Vector2i INVATATION_POSITION{ 250,500 }; // UI を描画する位置 ( 中心座標 )
        const float INVATATION_SIZE = 0.15f; // ( 1 で画像元サイズ )
        const float INVATATION_ANGLE = 90; // ( 回転量,画像元が縦向きなので横向きにする )
        const int32_t SATIETY_FULL = 2550;      // 最大空腹度
        int32_t currentSatiety_ = 2550;         // 現在空腹度
        const int32_t SATIETY_SUB_VALUE = 3;   // ターンごとに減る空腹度

        //----------------------------------------------
        // メソッド

        // 毎フレーム呼び出される
        void sceneUpdate(float deltaTime) override;

        // 3Dのレンダー
        void render(float deltaTime , const Shared<Atl3DCamera> camera);
        // 2D系の描画 ( 2D系をまとめて描画する )
        void draw2D(float deltaTime);
        // 2D UI の描画
        void drawUI(float deltaTime);
        // 2D 操作説明の描画
        void drawInstruction();
        // 2D 現在レベルの文字列描画
        void drawLevel();
        // 2D 満腹度の概念を描画 ( 招待状 のイラスト ）
        void drawInvatation();
        // 2D HPbar の描画
        void drawHPbar();
        // 2D ミニマップの描画
        void drawMinimap(float deltaTime);
        // 2D ミニマップ描画のヘルパー関数
        // ret ... マップに描画すべき中心位置
        // arg ... 二次元座標
        const tnl::Vector2i calcDrawMinimapPos(int32_t x, int32_t y);
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

        // メニューを開く
        void openMenu();
        // メニューを閉じる
        void closeMenu();
        // 選択肢ウィンドウを開く
        // arg ... 質問文
        void openSelectWindow(const std::string& question);
        // 選択肢ウィンドウを閉じる
        void closeSelectWindow();

        // シーンで使う音源データのヴォリュームをまとめて調整
        void soundVolumeFix();
        
        // 敵のリスポーン処理
        void enemyResporn();
        // アイテムに乗ったら拾う、拾えなかったらアイテムの上に乗る
        void pickUpItem();
        // エネミーの移動処理
        void enemyMove(float deltaTime);
        // エネミーの行動処理
        void enemyAction(float deltaTime);
        // HP がゼロになり、死亡演出が終わった敵を削除
        void deadEnemyErase();
        

        // シーケンス関連
        SEQUENCE(DungeonScene, &DungeonScene::seqInit);
        

        // 最初に一回だけ呼ばれる
        bool seqInit(float deltaTime);

        // 毎ターン初期化処理
        bool seqTurnInit(float deltaTime);
        // ターン開始時処理
        bool seqTurnStart(float deltaTime);

        // キー入力待ち
        bool seqKeyInput(float deltaTime);
        // プレイヤーが移動を選択したターン
        bool seqPlayerMoveTurn(float deltaTime);
        // プレイヤーが攻撃を選択したターン
        bool seqPlayerActionTurn(float deltaTime);

        // ターンエンド処理
        bool seqTurnEnd(float deltaTime);

        // プレイヤーが階段に乗った時
        bool seqOnStairs(float deltaTime);
        // 次の階層に移動している間
        bool seqToNextFloor(float deltaTime);

        // メニューウィンドウ開いている間の処理
        bool seqMenuWindow(float deltaTime);
        // マジックルーンウィンドウに対してアクションしている間の処理
        bool seqEraseMagicRuneWindow(float deltaTime);
        // アイテムを使う処理
        bool seqReallyUseItem(float deltaTime);
        // メニューからタイトルに戻るを選択した時 ( 確認画面 )
        bool seqReallyReturnToTitle(float deltaTime);
        // タイトルに戻る間のフェードなどの処理
        bool seqReturnToTitle(float deltaTime);

        // ゲームオーバーの処理 ( ゲームオーバーシーンに遷移 )
        bool seqGameOver(float deltaTime);

        // デバッグ用 ----------------------------------
        bool isDebug = false;

        void debug_displayDungeonParam(float deltaTime);
        void debug_displayMap(float deltaTime);
    };
};