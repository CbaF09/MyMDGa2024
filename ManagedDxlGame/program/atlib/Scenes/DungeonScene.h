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

    /// <summary>
    /// 
    /// �_���W�����V�[�� ( �Q�[���{�� )
    /// �Q�[���̖w�ǂ��߂�V�[��
    /// 
    /// </summary>

    class Stairs;
    class EnemyPawn;
    class PlayerPawn;
    class ItemPawn;
    class DungeonCreater;
    class Atl3DCamera;

    class DungeonScene final : public Base_Scene, public std::enable_shared_from_this<DungeonScene> {

    public:
        DungeonScene();
        ~DungeonScene();

        // �Q�b�^�[ ( 3D��̃Z����ӂ̑S�����擾�B�F�X�ȃN���X�Ŏ擾����̂� static �� )
        inline static const int32_t getCellLength() { return CELL_FULL_LENGTH; }
        // �Q�b�^�[ ( ���ݑ��݂��Ă���G�l�~�[�̃��X�g���擾 )
        inline const std::list<Shared<EnemyPawn>>& getEnemyArray() const { return enemies_; }
        // �Q�b�^�[ ( �v���C���[�|�[�����擾 )
        inline const Shared<PlayerPawn> getPlayerPawn() const { return player_; }

        // �����x��ύX���� ( 0 ~ SATIETY_FULL �̊ԂɃN�����v )
        inline void changeSatiety(int32_t changeValue) {
            currentSatiety_ += changeValue;
            if (currentSatiety_ <= 0) { currentSatiety_ = 0; }
            if (currentSatiety_ > SATIETY_FULL) { currentSatiety_ = SATIETY_FULL; }
        }

        // ���^�[����HP��
        void turnHealHP();


    private: 
        //----------------------------------------------
        // �ϐ�
        
        // �ėp ----------------------------------------
        // �Z����ӂ̑S�� ( 3D�� ) �F�X�ȏ��Ŏ擾�������̂� static
        static const int32_t CELL_FULL_LENGTH = 1000;

        // �� �p ---------------------------------------
        // �N���[�����ɂȂ�ǃ��b�V��
        Shared<Wall> originWall_ = std::make_shared<Wall>(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH * 2 ,CELL_FULL_LENGTH }); 
        // �ǃ��b�V���Q�̔z��
        std::vector<Shared<Wall>> walls_{};   

        // �n�� �p -------------------------------------
        // �N���[�����ɂȂ�n�ʃ��b�V�� PlaneMV���f���̊֌W�ŁAY�̏���Z�̒l�����Ă܂�
        Shared<GroundTile> originGroundTile_ = std::make_shared<GroundTile>(tnl::Vector3{ CELL_FULL_LENGTH,CELL_FULL_LENGTH,0 });  
        // �n�ʃ��b�V���Q�̔z��
        std::vector<Shared<GroundTile>> groundTiles_{};   

        // �K�i�֘A -------------------------------------
        // �K�i�ւ̃|�C���^
        Shared<Stairs> stairs_ = nullptr; 
        // �K�i�Ƀv���C���[������Ă��邩�t���O
        bool isPlayerOnStairs_ = false;

        // �v���C���[�֘A ------------------------------
        Shared<PlayerPawn> player_ = nullptr; 
        // ���j���[���J���Ă��邩
        bool isOpenMenu_ = false;    
        // �^�[�����Ƃ�HP�񕜗�
        const int32_t EVERY_TURN_HEAL = 1; 

        // �G�l�~�[�֘A --------------------------------
        std::list<Shared<EnemyPawn>> enemies_; 
        // ���^�[�����ƂɃ��X�|�[�����邩
        const int32_t RESPORN_TURN_COUNT = 30;  
        // ���X�|�[���J�E���^�[
        int32_t respornTurnTimer_ = 0;           

        // �A�C�e���֘A --------------------------------
        std::list<Shared<ItemPawn>> items_;

        // �K�w�Ǘ��p ----------------------------------
        // ���݊K�w
        int32_t currentFloor_ = 1;
        // �ŏ�K ( ���B������N���A�K )
        const int32_t MAX_FLOOR = 4;
        // ���K�w�ɐi�ގ��A����ʂ̂܂܂̑ҋ@���鎞��
        const float nextFloorTransitionTime = 2.5f;
        // ���K�w�ɑJ�ڒ���
        bool isNextFloorTransition = false;

        // UI �֘A -------------------------------------
        
        // HP�o�[�̘g�̈ʒu ( ���㒸�_ )
        const tnl::Vector2i HP_BAR_LEFT_UP_POINT{ 5,5 }; 
        // HP�o�[�̘g�̈ʒu ( �E�����_ )
        const tnl::Vector2i HP_BAR_RIGHT_DOWN_POINT{ 355,55 };
        // HP�o�[�̘g�ƃo�[���̂̊Ԃ̌���
        const tnl::Vector2i HP_BAR_ADJUST_VALUE{ 8,5 }; 
        // HP���l�\���̈ʒu
        const tnl::Vector2i HP_STRING_POSITION{ 50,15 }; 

        // ���x���̕������`�悷��ʒu
        const tnl::Vector2i LEVEL_STRING_POSITION{ 60,390 }; 
        
        // ��������̉摜��`�悷��ʒu
        const tnl::Vector2i INSTRUCTION_POSITION{ DXE_WINDOW_WIDTH/2 + 100,600 }; 
        // ��������̉摜�̑傫��
        const float INSTRUCTION_SIZE = 0.45f;
        // ��������̉摜�̔w�i�ɕ`�悷��I�[�o�[���C�̑傫��
        const tnl::Vector2i INSTRUCTION_BACK_BOX_SIZE{ 575,160 }; 

        // �e�L�X�g���O��`�悷��ʒu ( ��ԏ�̍s )
        const tnl::Vector2i TEXT_LOG_POSITION{ 20,550 };  

        // �~�j�}�b�v�̈�ԍ���Ɉʒu������W
        const tnl::Vector2i MINIMAP_LEFTUP_POSITION{ 600,130 }; 
        // �~�j�}�b�v��1�}�X�̑傫��
        const int32_t MINIMAP_CELL_SIZE = 12;    
        // �~�j�}�b�v�̃v���C���[�̑傫��
        const int32_t MINIMAP_PLAYER_SIZE = MINIMAP_CELL_SIZE / 3;
        // �~�j�}�b�v�̓��ߓx
        const int32_t MINIMAP_ALPHA = 128;

        // �I�����E�B���h�E�֘A ------------------------
        SelectWindow selectWindow_;
        // �Z���N�g�E�B���h�E���J���Ă��邩
        bool isSelectWindow_ = false;

        // ���j���[�֘A --------------------------------
        Shared<MenuWindow> menuWindow_ = nullptr;
        // ���ݑI�𒆂̑I����
        MenuWindow::e_SelectedMenuWindow selectedMenu_ = MenuWindow::e_SelectedMenuWindow::Item1;

        // ���[���E�B���h�E�֘A ------------------------
        MagicRuneWindow magicRuneWindow_;

        // �X�J�C�X�t�B�A�p ----------------------------
        Skysphere skysphere_;

        // �����x ( ���ҏ� ) �֘A ----------------------
        // UI ��`�悷��ʒu ( ���S���W )
        const tnl::Vector2i INVATATION_POSITION{ 250,500 }; 
        // �u���ҏ�v�Ƃ���������`�悷��ʒu ( ���S���W )
        const tnl::Vector2i INVATATION_STRING_POSITION{ 60,480 };
        // �摜�T�C�Y ( 1 �ŉ摜���T�C�Y )
        const float INVATATION_SIZE = 0.15f; 
        // ��]��,�摜�����c�����Ȃ̂ŉ������ɂ���
        const float INVATATION_ANGLE = 90; 
        // �ő喞���x
        const int32_t SATIETY_FULL = 2550;      
        // ���ݖ����x
        int32_t currentSatiety_ = 2550;         
        // �^�[�����ƂɌ���󕠓x
        const int32_t SATIETY_SUB_VALUE = 3;   

        // �t�H���g-------------------------------------
        const int LEVEL_STRING_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
        const int NEXT_FLOOR_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);

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
        // 2D �~�j�}�b�v�̕`��
        void drawMinimap(float deltaTime);
        // 2D �~�j�}�b�v�`��̃w���p�[�֐�
        // ret ... �}�b�v�ɕ`�悷�ׂ����S�ʒu
        // arg ... �񎟌����W
        const tnl::Vector2i calcDrawMinimapPos(int32_t x, int32_t y);
        // ���̊K�w�ɑJ�ڒ��ɁA���݂̊K�w��\������
        void drawNextFloorTransition();

        // ���j���[���J��
        void openMenu();
        // ���j���[�����
        void closeMenu();
        // �I�����E�B���h�E���J��
        // arg ... ���╶
        void openSelectWindow(const std::string& question);
        // �I�����E�B���h�E�����
        void closeSelectWindow();

        // �~�j�}�b�v�̏����X�V ( �l�ߖT���J���B���[�����Ȃ炻�̃��[���S�̂��ċA�����ŊJ�� )
        void minimapUpdate(const tnl::Vector2i& openCellPos);

        // �V�[���Ŏg�������f�[�^�̃��H�����[�����܂Ƃ߂Ē���
        void soundVolumeFix();
        
        // �G�̃��X�|�[������
        void enemyResporn();
        // �A�C�e���ɏ������E���A�E���Ȃ�������A�C�e���̏�ɏ��
        void pickUpItem();
        // �G�l�~�[�̈ړ�����
        void enemyMove(float deltaTime);
        // �G�l�~�[�̍s������
        void enemyAction(float deltaTime);
        // HP ���[���ɂȂ�A���S���o���I������G���폜
        void deadEnemyErase();

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

        //----------------------------------------------
        // �V�[�P���X
        SEQUENCE(DungeonScene, &DungeonScene::seqInit);

        // �ŏ��Ɉ�񂾂��Ă΂��
        bool seqInit(float deltaTime);

        // ���^�[������������
        bool seqTurnInit(float deltaTime);
        // �^�[���J�n������
        bool seqTurnStart(float deltaTime);


        // �L�[���͑҂�
        bool seqKeyInput(float deltaTime);
        // �v���C���[���ړ���I�������^�[��
        bool seqPlayerMoveTurn(float deltaTime);
        // �v���C���[���U����I�������^�[��
        bool seqPlayerActionTurn(float deltaTime);

        // �^�[���G���h����
        bool seqTurnEnd(float deltaTime);

        // �v���C���[���K�i�ɏ������
        bool seqOnStairs(float deltaTime);
        // ���̊K�w�Ɉړ����Ă����
        bool seqToNextFloor(float deltaTime);

        // ���j���[�E�B���h�E�J���Ă���Ԃ̏���
        bool seqMenuWindow(float deltaTime);
        // �}�W�b�N���[���E�B���h�E�ɑ΂��ăA�N�V�������Ă���Ԃ̏���
        bool seqEraseMagicRuneWindow(float deltaTime);
        // �A�C�e�����g������
        bool seqReallyUseItem(float deltaTime);
        // ���j���[����^�C�g���ɖ߂��I�������� ( �m�F��� )
        bool seqReallyReturnToTitle(float deltaTime);
        // �^�C�g���ɖ߂�Ԃ̃t�F�[�h�Ȃǂ̏���
        bool seqReturnToTitle(float deltaTime);

        // �Q�[���I�[�o�[�̏��� ( �Q�[���I�[�o�[�V�[���ɑJ�� )
        bool seqGameOver(float deltaTime);

        // �f�o�b�O�p ----------------------------------
        bool isDebug = false;

        void debug_displayDungeonParam(float deltaTime);
        void debug_displayMap(float deltaTime);
    };
};