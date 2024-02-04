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
        ~DungeonScene();

        // �^�[������p enum
        enum class e_turnState {
            KEY_INPUT,
            PLAYER_MOVE,
            PLAYER_ON_STAIRS,
        };
        
        // �Q�b�^�[
        inline static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        inline const e_turnState& getCurrentTurn() { return currentTurn_; }
        inline const std::list<Shared<EnemyPawn>>& getEnemyArray() const { return enemies_; }

    private: 
        //----------------------------------------------
        // �ϐ�
        
        // �ėp ----------------------------------------
        // �Z����ӂ̑S�� ( 3D�� )
        static const int32_t CELL_FULL_LENGTH = 1000;
        // ���݂̃^�[��
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
        bool isOpenMenu_ = false;    // ���j���[���J���Ă��邩

        // �G�l�~�[�֘A --------------------------------
        std::list<Shared<EnemyPawn>> enemies_;

        // �A�C�e���֘A --------------------------------
        std::list<Shared<ItemPawn>> items_;

        // UI �֘A -------------------------------------
        const tnl::Vector2i HP_BAR_LEFT_UP_POINT{ 5,5 }; // HP�o�[�̘g�̈ʒu
        const tnl::Vector2i HP_BAR_RIGHT_DOWN_POINT{ 355,55 }; // HP�o�[�̘g�̈ʒu
        const tnl::Vector2i HP_BAR_ADJUST_VALUE{ 8,5 }; // HP�o�[�̘g�ƃo�[���̂̊Ԃ̌���

        //----------------------------------------------
        // ���\�b�h

        void sceneUpdate(float deltaTime) override;

        // 3D�̃����_�[
        void render(float deltaTime , const Shared<Atl3DCamera> camera);
        // 2D�n�̕`�� ( 2D�n���܂Ƃ߂ĕ`�悷�� )
        void draw2D(float deltaTime);
        // 2D UI �̕`��
        void drawUI(float deltaTime);
        // 2D HPbar �̕`��
        void drawHPbar();

        // �_���W�����̏�����
        void initDungeon();

        // �_���W��������
        void generateDungeon();

        // �ǂ𐶐�
        // arg ... 2D���W�ʒu
        void generateWall(int generatePosX, int generatePosZ);
        // �n�ʂ𐶐�
        // arg ... 2D���W�ʒu
        void generateGround(int generatePosX, int generatePosZ);

        // �V�[�P���X
        SEQUENCE(DungeonScene, &DungeonScene::seqInit);
        bool seqInit(float deltaTime);
        // ���݂̃^�[���ɉ��������������s
        bool seqTurnStateProcess(float deltaTime);
        // ���񂾃G�l�~�[�̏��ŏ���
        bool seqDeadEnemyProcess(float deltaTime);
        // �G�ƃv���C���[�̍s�������t���O���I�t�ɂ���
        bool seqAllTurnFlagOff(float deltaTime);
        // ���̊K�w�Ɉړ����Ă����
        bool seqToNextFloor(float deltaTime);

        // �L�[���͑҂�
        void processKeyInput(float deltaTime);
        // �v���C���[���ړ������^�[���̏���
        void processPlayerMoveTurn(float deltaTime);
        // �v���C���[���K�i�ɏ������
        void processPlayerOnStairs(float deltaTime);

        // �f�o�b�O�p ----------------------------------
        void debug_displayDungeonParam(float deltaTime);
    };
};