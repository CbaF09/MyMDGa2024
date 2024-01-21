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

    // �_���W�����V�[��
    // ���� ... �_���W�����T���V�[��
    class DungeonScene final : public Base_Scene {

    public:
        DungeonScene();
        
        // �^�[������p enum
        enum class e_turnState {
            PLAYER,
            ENEMY,
        };
        
        // �Q�b�^�[
        static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        const e_turnState& getCurrentTurn() { return currentTurn_; }

        // �Z�b�^�[
        void setCurrenTurn(e_turnState nextTurn) { currentTurn_ = nextTurn; }

    private: 
        //----------------------
        // �ϐ�
        
        // �ėp ----------------------------------------
        // �Z����ӂ̑S��
        static const int32_t CELL_FULL_LENGTH = 1000;
        e_turnState currentTurn_ = e_turnState::PLAYER;

        // �� �p ---------------------------------------
        Shared<Wall> originWall_ = nullptr;
        std::vector<Shared<Wall>> walls_;

        // �n�� �p -------------------------------------
        Shared<GroundTile> originGroundTile_ = nullptr;
        std::vector<Shared<GroundTile>> groundTiles_;

        // �K�i �p -------------------------------------
        Shared<Stairs> originStairs_ = nullptr;

        // �v���C���[�֘A ------------------------------
        Shared<PlayerPawn> player_ = nullptr;

        // �G�l�~�[�֘A --------------------------------
        std::vector<Shared<EnemyPawn>> enemies_;

        //----------------------
        // ���\�b�h

        void sceneUpdate(float deltaTime) override;
        void render(float deltaTime , const Shared<Atl3DCamera> camera);
        void initDungeon();
        void generateDungeon();
        void generateWall(int generatePosX, int generatePosZ);
        void generateGround(int generatePosX, int generatePosZ);

        // �V�[�P���X
        SEQUENCE(DungeonScene, &DungeonScene::seqInit);
        bool seqInit(float deltaTime);
        bool seqProcess(float deltaTime);

        // �f�o�b�O�p ----------------------------------
        void debug_displayDungeonParam(const Shared<atl::Atl3DCamera>& camera, float deltaTime);

    };
};