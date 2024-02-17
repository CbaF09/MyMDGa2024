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
			CELL_TYPE_NONE = 0, // �����Ȃ�
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
			// �ǂ̃G���A�̃}�X�Ȃ̂�
			int32_t regionAreaID_ = 0;
			// ���ɉ������X�|�[�����Ă���}�X��
			bool isAlreadySpawnSomething = false;
			// �v���C���[�ɔ������ꂽ�� ( �~�j�}�b�v�ɕ\������邩 )
			bool isDiscoverByPlayer = false;
		};

		// �_���W�����𐶐��BfieldCells_ �ɕۑ������
		void createDungeon();

		// �Q�b�^�[
		inline const std::vector<std::vector<FieldCell>>& getFieldCells() const { return fieldCells_; }
		inline const tnl::Vector2i& getPlayerSpawnPos() const { return playerSpawnPos_; }
		inline const tnl::Vector2i& getStairsSpawnPos() const { return stairsSpawnPos_; }
		inline const int32_t getEnemySpawnNum() { return ENEMY_SPAWN_NUM; }
		inline const int32_t getItemSpawnNum() { return ITEM_SPAWN_NUM; }
		inline const std::vector<tnl::Vector2i>& getEnemySpawnPos() const { return enemySpawnPosArray_; }
		inline const std::vector<tnl::Vector2i>& getItemSpawnPos() const { return itemSpawnPosArray_; }

		// �Q�b�^�[ ( �����ɓn�������W�̃t�B�[���h�Z����ID���擾 )
		inline const int32_t getFieldCellID(const tnl::Vector2i& pos) const { return fieldCells_[pos.x][pos.y].regionAreaID_; }

		// �Q�b�^�[ ( �����ɓn�������W�̃t�B�[���h�Z�����A�����ς� => true , ��������Ă��Ȃ� => false )
		inline bool isDiscoverFieldCell(const tnl::Vector2i& pos) { return fieldCells_[pos.x][pos.y].isDiscoverByPlayer; }

		// �Q�b�^�[ ( �����ɓn�������W�̃t�B�[���h�Z����cellType���擾���� )
		inline e_FieldCellType getFieldCellType(const tnl::Vector2i& pos) const { return fieldCells_[pos.x][pos.y].cellType_; }

		// �����ɓn�������W�̃t�B�[���h�Z�����A�������ꂽ���肵������ɂ���
		inline void discoverFieldCell(const tnl::Vector2i& pos) { fieldCells_[pos.x][pos.y].isDiscoverByPlayer = true; }

		// fieldCells_ ����A�X�|�[���\��Ԃ̋�Z���𒊏o���ă��X�g�ɂ��A���̒����烉���_���Ɉ�I�сA����XY���W��Ԃ�
		// �I�΂ꂽ XY�n�_�� fieldCell �́A�X�|�[���s��Ԃɐ؂�ւ��
		tnl::Vector2i randomChoiceCanFirstSpawnFieldCellPos();
		// fieldCells_ ����A�X�|�[���\��Ԃ̃Z���𒊏o���ă��X�g�ɂ��A���̒����烉���_���Ɉ�I�сA����XY���W��Ԃ�
		// �G�̃��X�|�[���p
		// arg ... �X�|�[�����珜�O����t�B�[���h�Z����ID
		tnl::Vector2i randomChoiceEnemyRespawnPos(const tnl::Vector2i& playerPos);

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
		// �ʘH���ǂ��������ɐL�т邩��\������ enum
		enum class e_PathDirection {
			PATH_DIR_TOP = 0,
			PATH_DIR_BOTTOM,
			PATH_DIR_LEFT,
			PATH_DIR_RIGHT,
			PATH_DIR_MAX,
		};

		//----------------------- 
		// �ϐ�

		// �t�B�[���h�S�̂̉���
		const int32_t FIELD_WIDTH = 32;
		// �t�B�[���h�S�̂̏c��
		const int32_t FIELD_HEIGHT = 32;
		// ���̍ő吔 , �����̍ő吔�Ɠ������Ȃ�
		const int32_t AREA_MAX = 8;
		// ���̏c���ŏ��T�C�Y
		const int32_t AREA_SIZE_MIN = 8;
		// �����̏c���ŏ��T�C�Y
		const int32_t ROOM_SIZE_MIN = 4;

		// �G�̏����X�|�[����
		const int32_t ENEMY_SPAWN_NUM = 3;
		// �A�C�e���̏����X�|�[����
		const int32_t ITEM_SPAWN_NUM = 5;

		// �v���C���[���X�|�[������ʒu
		tnl::Vector2i playerSpawnPos_{ 0,0 };	
		// �K�i���X�|�[������ʒu
		tnl::Vector2i stairsSpawnPos_{ 0,0 };	
		// �G���X�|�[������ʒu�z��
		std::vector<tnl::Vector2i> enemySpawnPosArray_{};
		// �A�C�e�����X�|�[������ʒu�z��
		std::vector<tnl::Vector2i> itemSpawnPosArray_{};

		// ��敪���p�G���A�z��
		std::vector<Area> areas_;	

		// �񎟌��t�B�[���h�f�[�^
		std::vector<std::vector<FieldCell>> fieldCells_ =
			std::vector<std::vector<FieldCell>>(FIELD_HEIGHT, std::vector<FieldCell>(FIELD_WIDTH)); 

		//----------------------- 
		// ���\�b�h

		// �񎟌��t�B�[���h�f�[�^�̏�����
		void fieldCellsInit();
		// ��敪���p Area �̏�����
		void areaInit();

		// ��敪�� ( �ċA�����ׂ̈Ɉ��������邪�A�Ăяo�����͈��� 0 �ŌĂяo�����O��Ȃ̂Ńf�t�H���g���� )
		void areaSprit(int32_t areaID = 0);

		// ��������
		void roomCreate();
		
		// fieldCells_ �� parentArea ��ݒ�
		void setFieldCellsID();
		
		// �ʘH�ݒ�
		void pathwayCreate();
		// pathwayCreate �̃w���p�[�BpathDir�̕����ɒʘH��L�΂�
		void createPathDirection(const Area& area, e_PathDirection pathDir);

		// �l����ǂň͂܂�Ă���ǂ͏�������
		void eraseNonMeanWall();
		
		// �w���p�[�֐��B�z�񋫊E�`�F�b�N���Ȃ���A�w����W�̃Z�����ǂ��ǂ����𔻒肷��
		// �z��O�͕ǂƂ��Ĉ���
		// ret ... �� => true , �ǂłȂ� = false
		// arg ... �w����W
		bool isWall(int x, int y);

		// �e�I�u�W�F�N�g�̏����X�|�[�����n�_��ݒ肷��֐��Q
		void choiceInitSpawnPos();

	};

};