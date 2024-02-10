#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Object/MenuWindow.h"
#include "Base_Scene.h"
#include "../MeshObject/Skybox.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/Wall.h"

namespace atl {

    class Stairs;
    class EnemyPawn;
    class PlayerPawn;
    class ItemPawn;
    class DungeonCreater;
    class Atl3DCamera;
    class SelectWindow;

    // �_���W�����V�[��
    // ���� ... �_���W�����T���V�[��
    class DungeonScene final : public Base_Scene, public std::enable_shared_from_this<DungeonScene> {

    public:
        DungeonScene();
        ~DungeonScene();

        // �Q�b�^�[
        // 3D��̃Z����ӂ̑S�����擾�B�F�X�ȃN���X�Ŏ擾����̂� static ��
        inline static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        // ���ݑ��݂��Ă���G�l�~�[�̃��X�g���擾
        inline const std::list<Shared<EnemyPawn>>& getEnemyArray() const { return enemies_; }
        // �v���C���[�|�[�����擾
        inline const Shared<PlayerPawn> getPlayerPawn() const { return player_; }

        // �Z�b�^�[
        // �����x�̃Z�b�^�[ ( 0 ~ SATIETY_FULL �̊ԂɃN�����v )
        inline void changeSatiety(int32_t changeValue) {
            currentSatiety_ += changeValue;
            if (currentSatiety_ <= 0) { currentSatiety_ = 0; }
            if (currentSatiety_ > SATIETY_FULL) { currentSatiety_ = SATIETY_FULL; }
        }

    private: 
        //----------------------------------------------
        // �ϐ�
        
        // �ėp ----------------------------------------
        // �Z����ӂ̑S�� ( 3D�� ) �F�X�ȏ��Ŏ擾�������̂� static
        static const int32_t CELL_FULL_LENGTH = 1000;
        // ���݂̃^�[��

        // �� �p ---------------------------------------
        Shared<Wall> originWall_ = std::make_shared<Wall>(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH * 2 ,CELL_FULL_LENGTH }); // �N���[�����ɂȂ�ǃ��b�V��
        std::vector<Shared<Wall>> walls_;   // �ǃ��b�V���Q�̃��X�g

        // �n�� �p -------------------------------------
        Shared<GroundTile> originGroundTile_ = std::make_shared<GroundTile>(tnl::Vector3{ CELL_FULL_LENGTH,CELL_FULL_LENGTH,0 }); // �N���[�����ɂȂ�n�ʃ��b�V�� PlaneMV���f���̊֌W�ŁAY�̏���Z�̒l�����Ă܂� 
        std::vector<Shared<GroundTile>> groundTiles_;   // �n�ʃ��b�V���Q�̃��X�g

        // �K�i�֘A -------------------------------------
        Shared<Stairs> originStairs_ = nullptr; // �K�i�ւ̃|�C���^
        bool isPlayerOnStairs_ = false;

        // �v���C���[�֘A ------------------------------
        Shared<PlayerPawn> player_ = nullptr; // �v���C���[�|�[���ւ̃|�C���^
        bool isOpenMenu_ = false;    // ���j���[���J���Ă��邩
        const int32_t EVERY_TURN_HEAL = 1; // �^�[�����Ƃ�HP�񕜗�

        // �G�l�~�[�֘A --------------------------------
        std::list<Shared<EnemyPawn>> enemies_;  // �t�B�[���h�ɑ��݂���G�l�~�[���X�g
        const int32_t RESPORN_TURN_COUNT = 30;   // ���^�[�����ƂɃ��X�|�[�����邩
        int32_t respornTurnTimer_ = 0;           // ���X�|�[���^�[���J�E���^�[

        // �A�C�e���֘A --------------------------------
        std::list<Shared<ItemPawn>> items_; // �t�B�[���h�ɂ���A�C�e�����X�g

        // �K�w�Ǘ��p ----------------------------------
        int32_t currentFloor_ = 1;      // ���݊K�w
        const int32_t MAX_FLOOR = 4;    // �ŏ�K ( ���B������N���A�K )
        const float nextFloorTransitionTime = 2.5f;  // ���K�w�ɐi�ގ��A����ʂ̂܂܂̑ҋ@���鎞��
        bool isNextFloorTransition = false; // ���K�w�ɑJ�ڒ��� ( ����ʂ� )

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
        const tnl::Vector2i HP_STRING_POSITION{ 50,15 }; // HP���l�\���̈ʒu
        const tnl::Vector2i LEVEL_STRING_POSITION{ 60,390 }; // ���x���̕������`�悷��ʒu
        const tnl::Vector2i INSTRUCTION_POSITION{ DXE_WINDOW_WIDTH/2 + 100,600 }; // ��������̉摜��`�悷��ʒu
        const tnl::Vector2i INSTRUCTION_BACK_BOX_SIZE{ 575,160 }; // ��������̉摜��`�悷��ʒu
        const float INSTRUCTION_SIZE = 0.45f; // ��������̉摜��`�悷��ʒu

        const int LEVEL_STRING_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);
        const int NEXT_FLOOR_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);

        // �e�L�X�g���O�֘A ----------------------------
        const tnl::Vector2i TEXT_LOG_POSITION{ 20,550 };   // �e�L�X�g���O��`�悷��ʒu ( ��ԏ�̍s )

        // �I�����E�B���h�E�֘A ------------------------
        Shared<SelectWindow> selectWindow_ = nullptr;

        // ���j���[�֘A --------------------------------
        Shared<MenuWindow> menuWindow_ = nullptr;
        MenuWindow::e_SelectedMenuWindow selectedMenu = MenuWindow::e_SelectedMenuWindow::Item1;

        // �X�J�C�{�b�N�X�p ----------------------------
        Skybox skybox_;

        // �����x ( ���ҏ� ) �֘A ----------------------
        const tnl::Vector2i INVATATION_STRING_POSITION{ 60,480 }; // �u���ҏ�v�ƕ`�悷��ʒu ( ���S���W )
        const tnl::Vector2i INVATATION_POSITION{ 250,500 }; // UI ��`�悷��ʒu ( ���S���W )
        const float INVATATION_SIZE = 0.15f; // ( 1 �ŉ摜���T�C�Y
        const float INVATATION_ANGLE = 90; // ( ��]��,�摜�����c�����Ȃ̂ŉ������ɂ���
        const int32_t SATIETY_FULL = 2550;      // �ő�󕠓x
        int32_t currentSatiety_ = 2550;         // ���݋󕠓x
        const int32_t SATIETY_SUB_VALUE = 3;   // �^�[�����ƂɌ���󕠓x

        //----------------------------------------------
        // ���\�b�h

        // ���t���[���Ăяo�����
        void sceneUpdate(float deltaTime) override;

        // 3D�̃����_�[
        void render(float deltaTime , const Shared<Atl3DCamera> camera);
        // 2D�n�̕`�� ( 2D�n���܂Ƃ߂ĕ`�悷�� )
        void draw2D(float deltaTime);

        // 2D UI �̕`��
        void drawUI(float deltaTime);
        // 2D ��������̕`��
        void drawInstruction();
        // 2D ���݃��x���̕�����`��
        void drawLevel();
        // 2D �����x�̊T�O��`�� ( ���ҏ� �̃C���X�g �j
        void drawInvatation();
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

        // ���j���[���J��
        void openMenu();
        // ���j���[�����
        void closeMenu();
        // �V�[���Ŏg�������f�[�^�̃��H�����[�����܂Ƃ߂Ē���
        void soundVolumeFix();

        // �V�[�P���X�֘A
        SEQUENCE(DungeonScene, &DungeonScene::seqInit);
        
        // �ŏ��Ɉ�񂾂��Ă΂��
        bool seqInit(float deltaTime);

        // ���݂̃^�[���ɉ��������������s
        bool seqTurnStateProcess(float deltaTime);
        // ���j���[�E�B���h�E�J���Ă���Ԃ̏���
        bool seqMenuWindow(float deltaTime);
        // �A�C�e�����g������
        bool seqReallyUseItem(float deltaTime);
        // �Q�[���I�[�o�[�̏��� ( �Q�[���I�[�o�[�V�[���ɑJ�� )
        bool seqGameOver(float deltaTime);
        // �G�ƃv���C���[�̍s�������t���O���I�t�ɂ���
        bool seqAllTurnFlagOff(float deltaTime);
        // �T�����̓G�̃��X�|�[������
        void enemyResporn();
        // ���̊K�w�Ɉړ����Ă����
        bool seqToNextFloor(float deltaTime);
        // ���j���[����^�C�g���ɖ߂��I�������� ( �m�F��� )
        bool seqReallyReturnToTitle(float deltaTime);
        // �^�C�g���ɖ߂�Ԃ̃t�F�[�h�Ȃǂ̏���
        bool seqReturnToTitle(float deltaTime);
        // �A�C�e���̏�ɏ�������̏��� ( �E���A�E���Ȃ��A�Ȃ� )
        bool seqOnItemPosition(float deltaTime);

        // �L�[���͑҂�
        void processKeyInput(float deltaTime);
        // �v���C���[���ړ������^�[���̏���
        void processPlayerMoveTurn(float deltaTime);
        // �v���C���[���K�i�ɏ������
        void processPlayerOnStairs(float deltaTime);

        // �G�l�~�[�̈ړ�����
        void enemyMove(float deltaTime, bool& allEnemyTurned);
        // �G�l�~�[�̍s������
        void enemyAction(float deltaTime, bool& allEnemyTurned);
        // HP ���[���ɂȂ�A���S���o���I������G���폜
        void deadEnemyErase();

        // �f�o�b�O�p ----------------------------------
        bool isDebug = false;

        void debug_displayDungeonParam(float deltaTime);
        void debug_displayMap(float deltaTime);
    };
};