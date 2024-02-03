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

    // �_���W�����V�[��
    // ���� ... �_���W�����T���V�[��
    class DungeonScene final : public Base_Scene, public std::enable_shared_from_this<DungeonScene> {

    public:
        // �^�[������p enum
        enum class e_turnState {
            KEY_INPUT,
            PLAYER_MOVE,
        };
        
        // �Q�b�^�[
        inline static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        inline const e_turnState& getCurrentTurn() { return currentTurn_; }
        inline const std::vector<Shared<EnemyPawn>>& getEnemyArray() const { return enemies_; }

        // �Z�b�^�[
        void setCurrenTurn(e_turnState nextTurn) { currentTurn_ = nextTurn; }

    private: 
        //----------------------
        // �ϐ�
        
        // �ėp ----------------------------------------
        // �Z����ӂ̑S��
        static const int32_t CELL_FULL_LENGTH = 1000;
        e_turnState currentTurn_ = e_turnState::KEY_INPUT;

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
        const float CAMERA_ROT_SPEED = 0.3f;	// �J������]���x
        bool isOpenMenu = false;    // ���j���[���J���Ă��邩 ( �{���J���Ă��邩 )

        // �G�l�~�[�֘A --------------------------------
        std::vector<Shared<EnemyPawn>> enemies_;

        // �A�C�e���֘A --------------------------------
        std::vector<Shared<ItemPawn>> items_;

        // UI �֘A -------------------------------------
        const tnl::Vector2i HP_BAR_LEFT_UP_POINT{ 5,5 }; // HP�o�[�̘g�̈ʒu
        const tnl::Vector2i HP_BAR_RIGHT_DOWN_POINT{ 355,55 }; // HP�o�[�̘g�̈ʒu
        const tnl::Vector2i HP_BAR_ADJUST_VALUE{ 8,5 }; // HP�o�[�̘g�ƃo�[���̂̊Ԃ̌���

        //----------------------
        // ���\�b�h

        void sceneUpdate(float deltaTime) override;

        // 3D�̃����_�[
        void render(float deltaTime , const Shared<Atl3DCamera> camera);
        // 2D�n�̕`��
        void draw2D(float deltaTime);
        // UI �̕`��
        void drawUI(float deltaTime);
        // HPbar
        void drawHPbar();

        // �_���W�����̏�����
        void initDungeon();

        // �_���W��������
        void generateDungeon();

        // �ǂ𐶐�
        // arg ... ��������2D���W�ʒu
        void generateWall(int generatePosX, int generatePosZ);
        // �n�ʂ𐶐�
        // arg ... ��������2D���W�ʒu
        void generateGround(int generatePosX, int generatePosZ);

        // �V�[�P���X
        SEQUENCE(DungeonScene, &DungeonScene::seqInit);
        bool seqInit(float deltaTime);
        bool seqTurnStateProcess(float deltaTime);
        bool seqAllTurnFlagOff(float deltaTime);

        void processKeyInput(float deltaTime);
        void processPlayerMoveTurn(float deltaTime);

        // �f�o�b�O�p ----------------------------------
        void debug_displayDungeonParam(float deltaTime);

    };
};