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

    // �_���W�����V�[��
    // ���� ... �_���W�����T���V�[��
    class DungeonScene final : public Base_Scene, public std::enable_shared_from_this<DungeonScene> {

    public:
        ~DungeonScene();

        // �Q�b�^�[
        // 3D��̃Z����ӂ̑S�����擾�B�F�X�ȃN���X�Ŏ擾����̂� static ��
        inline static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        // ���ݑ��݂��Ă���G�l�~�[�̃��X�g���擾
        inline const std::list<Shared<EnemyPawn>>& getEnemyArray() const { return enemies_; }
        // �v���C���[�|�[�����擾
        inline const Shared<PlayerPawn> getPlayerPawn() const { return player_; }

    private: 
        //----------------------------------------------
        // �ϐ�
        
        // �ėp ----------------------------------------
        // �Z����ӂ̑S�� ( 3D�� ) �F�X�ȏ��Ŏ擾�������̂� static
        static const int32_t CELL_FULL_LENGTH = 1000;
        // ���݂̃^�[��

        // �� �p ---------------------------------------
        Shared<Wall> originWall_ = nullptr; // �N���[�����ɂȂ�ǃ��b�V���ւ̃|�C���^
        std::vector<Shared<Wall>> walls_;   // �ǃ��b�V���Q�̃��X�g

        // �n�� �p -------------------------------------
        Shared<GroundTile> originGroundTile_ = nullptr; // �N���[�����ɂȂ�n�ʃ��b�V���ւ̃|�C���^
        std::vector<Shared<GroundTile>> groundTiles_;   // �n�ʃ��b�V���Q�̃��X�g

        // �K�i�֘A -------------------------------------
        Shared<Stairs> originStairs_ = nullptr; // �K�i�ւ̃|�C���^
        bool isPlayerOnStairs_ = false;

        // �v���C���[�֘A ------------------------------
        Shared<PlayerPawn> player_ = nullptr; // �v���C���[�|�[���ւ̃|�C���^
        bool isOpenMenu_ = false;    // ���j���[���J���Ă��邩

        // �G�l�~�[�֘A --------------------------------
        std::list<Shared<EnemyPawn>> enemies_;  // �t�B�[���h�ɑ��݂���G�l�~�[���X�g

        // �A�C�e���֘A --------------------------------
        std::list<Shared<ItemPawn>> items_; // �t�B�[���h�ɂ���A�C�e�����X�g

        // �K�w�Ǘ��p ----------------------------------
        int32_t currentFloor_ = 1;      // ���݊K�w
        const int32_t MAX_FLOOR = 3;    // �ŏ�K ( ���B������N���A�K )
        const float nextFloorTransitionTime = 0.1f;  // ���K�w�ɐi�ގ��A����ʂ̂܂܂̑ҋ@���鎞��
        bool isNextFloorTransition = false;

        // �^�[������p --------------------------------
        enum class e_turnState {
            KEY_INPUT,
            PLAYER_MOVE,
            PLAYER_ON_STAIRS,
        }currentTurn_ = e_turnState::KEY_INPUT;

        // UI �֘A -------------------------------------
        const tnl::Vector2i HP_BAR_LEFT_UP_POINT{ 5,5 }; // HP�o�[�̘g�̈ʒu
        const tnl::Vector2i HP_BAR_RIGHT_DOWN_POINT{ 355,55 }; // HP�o�[�̘g�̈ʒu
        const tnl::Vector2i HP_BAR_ADJUST_VALUE{ 8,5 }; // HP�o�[�̘g�ƃo�[���̂̊Ԃ̌���
        const int NEXT_FLOOR_TEXT_FONTSIZE = 30;

        // �e�L�X�g���O�֘A ----------------------------
        const tnl::Vector2i TEXT_LOG_POSITION{ 20,550 };   // �e�L�X�g���O��`�悷��ʒu ( ��ԏ�̍s )

        // �I�����E�B���h�E�֘A ------------------------
        Shared<SelectWindow> selectWindow_ = nullptr;

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
        // ���̊K�w�ɑJ�ڒ��ɁA���݂̊K�w��\������
        void drawNextFloorTransition();

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
        // �Q�[���I�[�o�[�̏��� ( �Q�[���I�[�o�[�V�[���ɑJ�� )
        bool seqGameOver(float deltaTime);
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
        void debug_displayMap(float deltaTime);
    };
};