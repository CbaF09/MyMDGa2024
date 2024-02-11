#pragma once
#include <vector>
#include <memory>

namespace atl {

	// �_���W���������Ǘ��N���X ( �V���O���g�� )
	// ���� ... �_���W������������ , ���������_���W�����f�[�^�̕ێ�
	class DungeonCreater final {
	//------------------------------------------
	// �V���O���g���݌v
	public:
		static DungeonCreater* getDungeonCreater();
		static void deleteDungeonCreater() { delete getDungeonCreater(); };
	private:
		DungeonCreater() {};
		~DungeonCreater() {};
	//------------------------------------------
	public:
		// �Z���̏�ԗp enum
		enum class e_FieldCellType {
			CELL_TYPE_NONE = 0, // �O��l�A�������p
			CELL_TYPE_ROOM,	// ����
			CELL_TYPE_PATH,	// �ʘH
			CELL_TYPE_WALL,	// ��
		};

		// ����
		class Room {
		public:
			int32_t posX_ = 0, posY_ = 0, width_ = 0, height_ = 0;
		};

		// ��敪���p�̃G���A�N���X
		class Area {
		public:
			int32_t posX_ = 0, posY_ = 0, width_ = 0, height_ = 0;
			int32_t areaID_;

			// �G���A�͈�̕���������
			Room room_;
		};

		// 1�}�X��\������N���X
		class FieldCell {
		public:
			// �Z���̃^�C�v
			e_FieldCellType cellType_ = e_FieldCellType::CELL_TYPE_NONE;
			// ���ɉ������X�|�[�����Ă���}�X��
			bool isAlreadySpawnSomething = false;
			// �ǂ̃G���A�̃}�X�Ȃ̂�
			int32_t regionAreaID_ = 0;
		};

		// �_���W�����𐶐��BfieldCells_ �ɕۑ������
		void createDungeon();

		// �Q�b�^�[�֐�
		inline static const int32_t getFieldWidth() { return FIELD_WIDTH; }
		inline static const int32_t getFieldHeight() { return FIELD_HEIGHT; }
		inline const std::vector<std::vector<FieldCell>>& getFieldCells() const { return fieldCells_; }
		inline const tnl::Vector2i& getPlayerSpawnPos() const { return playerSpawnPos_; }
		inline const tnl::Vector2i& getStairsSpawnPos() const { return stairsSpawnPos_; }
		inline const int32_t getEnemySpawnNum() { return ENEMY_SPAWN_NUM; }
		inline const int32_t getItemSpawnNum() { return ITEM_SPAWN_NUM; }
		inline const std::vector<tnl::Vector2i>& getEnemySpawnPos() const { return enemySpawnPosArray_; }
		inline const std::vector<tnl::Vector2i>& getItemSpawnPos() const { return itemSpawnPosArray_; }

		// �����ɓn�������W�̃t�B�[���h�Z����ID���擾
		inline const int32_t getFieldCellID(tnl::Vector2i pos) const { return fieldCells_[pos.x][pos.y].regionAreaID_; }

		// fieldCells_ ����A�X�|�[���\��Ԃ̋�Z���𒊏o���ă��X�g�ɂ��A���̒����烉���_���Ɉ�I�сA����XY���W��Ԃ�
		// �I�΂ꂽ XY�n�_�� fieldCell �́A�X�|�[���s��Ԃɐ؂�ւ��
		tnl::Vector2i randomChoiceCanSpawnFieldCellPos();
		// fieldCells_ ����A�X�|�[���\��Ԃ̃Z���𒊏o���ă��X�g�ɂ��A���̒����烉���_���Ɉ�I�сA����XY���W��Ԃ�
		// �G�̃��X�|�[���p
		// arg ... �X�|�[�����珜�O����t�B�[���h�Z����ID
		tnl::Vector2i randomChoiceCanSpawnFieldCellPos(const tnl::Vector2i& playerPos);

		// �ړ��悪�A�ړ��\���ǂ����`�F�b�N����
		bool isCanMoveFieldCellPos(const tnl::Vector2i& toMovePos);

		// ------------------------------------------
		//  �f�o�b�O�p
		// ���������G���A�𕶎���ŉ��\��
		// arg ... ���_(����)���I�t�Z�b�g���A�\���ʒu��␳ ( �قڂقڕK�v�Ȃ��̂Ńf�t�H���g���� 0 )
		void debag_DisplayArea(int offsetDisplayPosX = 0, int offsetDisplayPosY = 0) const;
		
		// �t�B�[���h�𕶎���ŉ��\��
		// arg ... ���_(����)���I�t�Z�b�g���A�\���ʒu��␳ ( �قڂقڕK�v�Ȃ��̂Ńf�t�H���g���� 0 )
		void debag_DisplayFieldCells(int offsetDisplayPosX = 0, int offsetDisplayPosY = 0) const;

		// �G�l�~�[�E�A�C�e���E�v���C���[�E�K�i�̈ʒu���܂ߕ�����ŉ��\��
		// arg ... ���_(����)���I�t�Z�b�g���A�\���ʒu��␳ ( �قڂقڕK�v�Ȃ��̂Ńf�t�H���g���� 0 )
		void debag_DisplayFieldData(int offsetDisplayPosX = 0, int offsetDisplayPosY = 0) const;

		// ���O�Ƀ_���W�����̏����o��
		void debag_OutputLogGeneratedAreaRoomData() const;

	private:

		// �萔 ,Enum
		static const int32_t FIELD_WIDTH = 32;		// �t�B�[���h�S�̂̉���
		static const int32_t FIELD_HEIGHT = 32;		// �t�B�[���h�S�̂̏c��
		const int32_t AREA_MAX = 8;			// ���̍ő吔 , �����̍ő吔�Ɠ������Ȃ�
		const int32_t AREA_SIZE_MIN = 8;		// ���̏c���ŏ��T�C�Y
		const int32_t ROOM_SIZE_MIN = 4;		// �����̏c���ŏ��T�C�Y

		// �ʘH���ǂ��������ɐL�т邩��\������enum
		enum class e_PathDirection {
			PATH_DIR_TOP = 0,
			PATH_DIR_BOTTOM,
			PATH_DIR_LEFT,
			PATH_DIR_RIGHT,
			PATH_DIR_MAX,
		};

		//----------------------- 
		// �����o�ϐ�
		const int32_t ENEMY_SPAWN_NUM = 1; // �G�̐�
		const int32_t ITEM_SPAWN_NUM = 5; // �A�C�e���̐�

		tnl::Vector2i playerSpawnPos_{ 0,0 };	// �v���C���[���X�|�[������ʒu
		tnl::Vector2i stairsSpawnPos_{ 0,0 };	// �K�i���X�|�[������ʒu
		std::vector<tnl::Vector2i> enemySpawnPosArray_;	// �G���X�|�[������ʒu�z��
		std::vector<tnl::Vector2i> itemSpawnPosArray_;	// �A�C�e�����X�|�[������ʒu�z��

		std::vector<Area> areas_;	// ��敪���p�G���A�z��

		std::vector<std::vector<FieldCell>> fieldCells_ =
			std::vector<std::vector<FieldCell>>(FIELD_HEIGHT, std::vector<FieldCell>(FIELD_WIDTH)); // �񎟌��t�B�[���h�f�[�^

		//----------------------- 
		// ���\�b�h

		// ������
		void fieldCellsInit();
		void areaInit();

		// ��敪�� ( �ċA�����ׂ̈Ɉ��������邪�A�ŏ��ɌĂяo�����͈��� 0 �ŌĂяo�����O��Ȃ̂Ńf�t�H���g���� )
		void areaSprit(int32_t areaID = 0);
		// ��������
		void roomCreate();
		// fieldCells_ �� parentArea ��ݒ�
		void setFieldCellsID();
		// �ʘH�ݒ�
		void pathwayCreate();

		// �ʘH�𐶐�����ׂ̊֐��Q
		// TODO : ���Ԃ����������ɂ܂Ƃ߂Ă݂�̂ɒ���
		void createPathwayToTop(const Area& area);
		void createPathwayToBottom(const Area& area);
		void createPathwayToLeft(const Area& area);
		void createPathwayToRight(const Area& area);

		// �X�|�[�����n�_��ݒ肷��֐��Q
		// TODO : ���Ԃ����������ɂ܂Ƃ߂Ă݂�̂ɒ���
		void choicePlayerSpawnPos();
		void choiceStairsSpawnPos();
		void choiceEnemySpawnPos();
		void choiceItemSpawnPos();
	};

};