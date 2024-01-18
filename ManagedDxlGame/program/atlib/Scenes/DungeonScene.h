#pragma once

#include "../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Singletons/DungeonCreater.h"
#include "../../atlib/MeshObject/Wall.h"
#include "../../atlib/MeshObject/Stairs.h"
#include "../../atlib/MeshObject/GroundTile.h"
#include "../../atlib/Object/PlayerPawn.h"
#include "../../atlib/Utilities/Atl3DCamera.h"
#include "../../atlib/Collision/Collision.h"
#include "Base_Scene.h"

namespace atl {

    // ダンジョンシーン
    // 役割 ... ダンジョン探索シーン
    class DungeonScene final : public Base_Scene {

    public:
        DungeonScene();

        // ゲッター
        const float getCellLength() const { return CELL_FULL_LENGTH; }

    private: 
        
        //----------------------
        // 変数
        
        // 汎用 ----------------------------------------
        // セル一辺の全長
        const float CELL_FULL_LENGTH = 1000;
        
        // 壁 用 ---------------------------------------
        Shared<Wall> originWall_ = nullptr;
        std::vector<Shared<Wall>> walls_;

        // 地面 用 -------------------------------------
        Shared<GroundTile> originGroundTile_ = nullptr;
        std::vector<Shared<GroundTile>> groundTiles_;

        // 階段 用 -------------------------------------
        Shared<Stairs> originStairs_ = nullptr;

        // プレイヤー関連 ------------------------------
        const float PLAYER_MOVE_SPEED = 50;
        const float PLAYER_CAMERA_ROT_SPEED = 0.3f;
        const float PLAYER_HEAD_LINE = 500; // プレイヤーのY高さ（カメラ・目線の高さ）
        Shared<PlayerPawn> player_ = nullptr;


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