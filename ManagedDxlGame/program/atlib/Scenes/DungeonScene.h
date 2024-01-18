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

    // �_���W�����V�[��
    // ���� ... �_���W�����T���V�[��
    class DungeonScene final : public Base_Scene {

    public:
        DungeonScene();

        // �Q�b�^�[
        const float getCellLength() const { return CELL_FULL_LENGTH; }

    private: 
        
        //----------------------
        // �ϐ�
        
        // �ėp ----------------------------------------
        // �Z����ӂ̑S��
        const float CELL_FULL_LENGTH = 1000;
        
        // �� �p ---------------------------------------
        Shared<Wall> originWall_ = nullptr;
        std::vector<Shared<Wall>> walls_;

        // �n�� �p -------------------------------------
        Shared<GroundTile> originGroundTile_ = nullptr;
        std::vector<Shared<GroundTile>> groundTiles_;

        // �K�i �p -------------------------------------
        Shared<Stairs> originStairs_ = nullptr;

        // �v���C���[�֘A ------------------------------
        const float PLAYER_MOVE_SPEED = 50;
        const float PLAYER_CAMERA_ROT_SPEED = 0.3f;
        const float PLAYER_HEAD_LINE = 500; // �v���C���[��Y�����i�J�����E�ڐ��̍����j
        Shared<PlayerPawn> player_ = nullptr;


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