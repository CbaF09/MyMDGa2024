#include <vector>
#include <random>
#include <algorithm>
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Utilities/AtlRandom.h"
#include "DungeonCreater.h"

namespace atl {


	DungeonCreater* DungeonCreater::getDungeonCreater() {
		static DungeonCreater* p_instance_ = nullptr;

		if (!p_instance_) p_instance_ = new DungeonCreater();
		return p_instance_;
	};

	void DungeonCreater::createDungeon() {
		// ������
		fieldCellsInit();
		areaInit();

		// �_���W��������
		areaSprit();
		roomCreate();
		pathwayCreate();

		// �t�B�[���h�Z����ID��ݒ肷�� ( �ǂ� Area �ɑ����Ă��邩 )
		setFieldCellsID();

		// �l����ǂɈ͂܂�Ă���Z���́ANONE�ɐݒ肷�� ( �l����ǂɈ͂܂�Ă���ӏ��ɂ͉����������Ȃ� )
		eraseNonMeanWall();

		// �e��X�|�[��
		choiceInitSpawnPos();
	}

	//------------------------------
	// 
	//		�������p
	// 
	//------------------------------

	void DungeonCreater::fieldCellsInit() {
		// �S�Ă�ǂɐݒ肷��
		// �������X�|�[�����Ă��邩�t���O���I�t��
		for (auto& row : fieldCells_) {
			for (auto& cell : row) {
				cell.cellType_ = e_FieldCellType::CELL_TYPE_WALL;
				cell.regionAreaID_ = 0;
				cell.isAlreadySpawnSomething = false;
				cell.isDiscoverByPlayer = false;
			}
		}
	}

	void DungeonCreater::areaInit() {
		areas_.clear();
		areas_.resize(1);

		areas_[0].posX_ = 0;
		areas_[0].posY_ = 0;
		areas_[0].areaID_ = 0;

		areas_[0].height_ = FIELD_HEIGHT;
		areas_[0].width_ = FIELD_WIDTH;
	}

	//------------------------------
	// 
	//		�_���W���������p
	// 
	//------------------------------

	void DungeonCreater::areaSprit(int32_t areaID) {
		// �G���A�̐�������ɒB���Ă�����I���
		if (areas_.size() == AREA_MAX) return;

		// �G���A���₷
		areas_.emplace_back();

		int32_t newAreaID = static_cast<int32_t>(areas_.size() - 1);
		int32_t beforeWidth = areas_[areaID].width_;
		int32_t beforeHeight = areas_[areaID].height_;

		// �c�����E�������������_���ɍs��
		if (mtRandom() % 2)
		{ // �c�����ɕ���
			areas_[areaID].width_ /= 2;

			areas_[newAreaID].posX_ = areas_[areaID].posX_ + areas_[areaID].width_;
			areas_[newAreaID].posY_ = areas_[areaID].posY_;
			areas_[newAreaID].width_ = beforeWidth - areas_[areaID].width_;
			areas_[newAreaID].height_ = areas_[areaID].height_;
		}
		else { // �������ɕ���
			areas_[areaID].height_ /= 2;

			areas_[newAreaID].posX_ = areas_[areaID].posX_;
			areas_[newAreaID].posY_ = areas_[areaID].posY_ + areas_[areaID].height_;
			areas_[newAreaID].width_ = areas_[areaID].width_;
			areas_[newAreaID].height_ = beforeHeight - areas_[areaID].height_;
		}

		// --- �����I�������̊m�F�i �ŏ��T�C�Y��菬�����Ȃ��Ă����ꍇ�A�����𖳂��������ɂ��A�������I������ )
		if ((areas_[areaID].width_ < AREA_SIZE_MIN) ||
			(areas_[areaID].height_ < AREA_SIZE_MIN) ||
			(areas_[newAreaID].width_ < AREA_SIZE_MIN) ||
			(areas_[newAreaID].height_ < AREA_SIZE_MIN)) {
			areas_[areaID].width_ = beforeWidth;
			areas_[areaID].height_ = beforeHeight;
			areas_.pop_back();
			return;
		}
		// ---------------

		// �V�����������G���A�ɃG���AID��ݒ�
		areas_[newAreaID].areaID_ = newAreaID;

		// �������������G���A�ɑ΂��ċA���s
		areaSprit(areaID);

		// ������A�������G���A�ɑ΂��ċA���s
		areaSprit(newAreaID);	
	};

	void DungeonCreater::roomCreate() {
		// ���[���������_���ȑ傫���Ő���
		// �i�������A�G���A�̍��[�E��[�����1�}�X�A�E�[�E���[�����2�}�X�J����悤�ɂ���j
		for (Area& area : areas_) {
			
			area.room_.posX_ = area.posX_ + mtRandomRangeInt(1, ROOM_SIZE_MIN - 2);
			area.room_.posY_ = area.posY_ + mtRandomRangeInt(1, ROOM_SIZE_MIN - 2);
			area.room_.width_ = mtRandomRangeInt(ROOM_SIZE_MIN, area.width_ - 4);
			area.room_.height_ = mtRandomRangeInt(ROOM_SIZE_MIN, area.height_ - 4);

			for (int x = area.room_.posX_; x < area.room_.posX_ + area.room_.width_; ++x) {
				for (int y = area.room_.posY_; y < area.room_.posY_ + area.room_.height_; ++y) {
					fieldCells_[x][y].cellType_ = e_FieldCellType::CELL_TYPE_ROOM;
				}
			}
		}
	}

	// �t�B�[���h�Z��ID���A�������Ă���areaID�ɐݒ肷��
	void DungeonCreater::setFieldCellsID() {
		for (const auto& area : areas_) {
			for (int x = area.posX_; x < area.posX_ + area.width_; ++x) {
				for (int y = area.posY_; y < area.posY_ + area.height_; ++y) {
						fieldCells_[x][y].regionAreaID_ = area.areaID_;
				}
			}
		}
	}



	void DungeonCreater::pathwayCreate() {
		// �e�G���A�̉E�[�Ɖ��[�� CELL_TYPE_PATH ��
		for (const Area& area : areas_) {
			// ���[
			for (int x = area.posX_; x < area.posX_ + area.width_; ++x) { 
				fieldCells_[x][area.posY_ + area.height_ - 1].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			// �E�[
			for (int y = area.posY_; y < area.posY_ + area.height_; ++y) { 
				fieldCells_[area.posX_ + area.width_ - 1][y].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
		}
		
		// ���[������㉺���E�ɒʘH��L�΂��B
		for (const Area& area : areas_) {
			// �o����ʘH���̊֐��|�C���^�z�� ( �㉺���E )
			std::vector<std::function<void()>> possiblePathwayFuncs;

			// �����G���A�����[�łȂ��Ȃ�A���Ɍ������ʘH�����֐�������
			if (area.posX_ != 0) {
				possiblePathwayFuncs.emplace_back(
					[this, &area]() { createPathDirection(area, e_PathDirection::PATH_DIR_LEFT); }); }

			// �����G���A����[�łȂ��Ȃ�
			if (area.posY_ != 0) {
				possiblePathwayFuncs.emplace_back(
					[this, &area]() { createPathDirection(area, e_PathDirection::PATH_DIR_TOP); });
			}

			// �����G���A���E�[�łȂ��Ȃ�
			if ((area.posX_ + area.width_) != FIELD_WIDTH) {
				possiblePathwayFuncs.emplace_back(
					[this, &area]() { createPathDirection(area, e_PathDirection::PATH_DIR_RIGHT); });
			}

			// �����G���A�����[�łȂ��Ȃ�
			if ((area.posY_ + area.height_) != FIELD_HEIGHT) {
				possiblePathwayFuncs.emplace_back(
					[this, &area]() { createPathDirection(area, e_PathDirection::PATH_DIR_BOTTOM); });
			}

			// ����ʘH�̖{����2�{�����Ȃ��ꍇ�A��������Ȃ��ƁA���B�s�\�ȕ������o���Ă��܂��\��������̂ŁA��ɏ�������
			if (possiblePathwayFuncs.size() == 2) {
				possiblePathwayFuncs[0]();
				possiblePathwayFuncs[1]();
				continue;
			}

			// �֐��|�C���^�z����V���b�t��
			std::shuffle(possiblePathwayFuncs.begin(), possiblePathwayFuncs.end(), mtRandom);

			// 1 ~ �ő�ō���ʘH�̕����A�ŗ��������A���̐������ʘH����� ( �Œ��{�͒ʘH����� )
			int pathwaysToCreate = mtRandomRangeInt(1, possiblePathwayFuncs.size()); 
			for (int i = 0; i < pathwaysToCreate; ++i) {
				possiblePathwayFuncs[i]();
			}
		}

		// �t�B�[���h�̉��[�ƉE�[��ǂɂ���
		for (int x = 0; x < FIELD_WIDTH; ++x) {
			fieldCells_[x][0].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
			fieldCells_[x][FIELD_HEIGHT - 1].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
		}
		for (int y = 0; y < FIELD_HEIGHT; ++y) {
			fieldCells_[0][y].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
			fieldCells_[FIELD_WIDTH - 1][y].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
		}

		// �Z������4�}�X�̂���3�}�X���ǂȂ�A���̃Z�����ǂɂ���
		for (int x = 1; x < FIELD_WIDTH - 1; ++x) {
			for (int y = 1; y < FIELD_HEIGHT - 1; ++y) {
				if (fieldCells_[x][y].cellType_ == e_FieldCellType::CELL_TYPE_WALL) continue;

				uint16_t wallCount = 0;
				if (isWall(x - 1, y)) ++wallCount;
				if (isWall(x + 1, y)) ++wallCount;
				if (isWall(x, y - 1)) ++wallCount;
				if (isWall(x, y + 1)) ++wallCount;

				if (wallCount == 3) {
					fieldCells_[x][y].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
				}
			}
		}

		// �t����������������Ȃ���ΉE�����E�������ʘH�̍s���~�܂肪�����Ȃ�
		for (int x = FIELD_WIDTH - 2; 1 < x; --x) {
			for (int y = FIELD_HEIGHT - 2; 1 < y; --y) {
				if (fieldCells_[x][y].cellType_ == e_FieldCellType::CELL_TYPE_WALL) continue;
				
				uint16_t wallCount = 0;
				if (isWall(x - 1, y)) ++wallCount;
				if (isWall(x + 1, y)) ++wallCount;
				if (isWall(x, y - 1)) ++wallCount;
				if (isWall(x, y + 1)) ++wallCount;

				if (wallCount == 3) {
					fieldCells_[x][y].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
				}
			}
		}
	}

	void DungeonCreater::createPathDirection(const Area& area, e_PathDirection pathDir) {
		int randomX = 0, randomY = 0;
		
		switch (pathDir) {
		
		// �����
		case e_PathDirection::PATH_DIR_TOP:
			randomX = mtRandomRangeInt(area.room_.posX_ + 1, area.room_.posX_ + area.room_.width_ - 2);
			for (int y = area.posY_; y < area.room_.posY_; ++y) {
				fieldCells_[randomX][y].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			break;

		// ������
		case e_PathDirection::PATH_DIR_BOTTOM:
			randomX = mtRandomRangeInt(area.room_.posX_ + 1, area.room_.posX_ + area.room_.width_ - 2);
			for (int y = area.room_.posY_ + area.room_.height_; y < area.posY_ + area.height_; ++y) {
				fieldCells_[randomX][y].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			break;
		
		// �E����
		case e_PathDirection::PATH_DIR_RIGHT: 
			randomY = mtRandomRangeInt(area.room_.posY_ + 1, area.room_.posY_ + area.room_.height_ - 2);
			for (int x = area.room_.posX_ + area.room_.width_; x < area.posX_ + area.width_; ++x) {
				fieldCells_[x][randomY].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			break;

		// ������
		case e_PathDirection::PATH_DIR_LEFT:
			randomY = mtRandomRangeInt(area.room_.posY_ + 1, area.room_.posY_ + area.room_.height_ - 2);
			for (int x = area.posX_; x < area.room_.posX_; ++x) {
				fieldCells_[x][randomY].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			break;
		}
	}
	
	void DungeonCreater::eraseNonMeanWall() {
		// NONE�ɂȂ�ǂ̃C���f�b�N�X������ꎞ�z��
		std::vector<std::pair<int,int>> eraseCellIndex;

		for (auto x = 0; x < fieldCells_.size(); ++x) {
			for (auto y = 0; y < fieldCells_[x].size(); ++y) {

				// �ǈȊO�̃Z���^�C�v�Ȃ瑁�����^�[��
				if (fieldCells_[x][y].cellType_ != e_FieldCellType::CELL_TYPE_WALL) { continue; }

				// �l�ߖT���ǂȂ�A���g��NONE�ɐݒ肳���
				if (isWall(x + 1, y) && 
					isWall(x - 1, y) && 
					isWall(x, y + 1) && 
					isWall(x, y - 1)) {
					eraseCellIndex.emplace_back(std::make_pair(x,y));
				}
			}
		}

		// �ꎞ�z��̒��g�����ɁANONE�ɐݒ肷��
		for (auto& cellIndex : eraseCellIndex) {
			fieldCells_[cellIndex.first][cellIndex.second].cellType_ = e_FieldCellType::CELL_TYPE_NONE;
		}
	}

	// true => �ǁA fale => �ǂł͂Ȃ�
	bool DungeonCreater::isWall(int x, int y) {
		// ���E�`�F�b�N
		if (x < 0 || x >  fieldCells_.size() - 1 || 
			y < 0 || y > fieldCells_[x].size() - 1) {
			// �z��͈̔͊O�͕ǂƂ��Ĉ���
			return true;
		}

		// �Z���^�C�v���ǂȂ� true
		return fieldCells_[x][y].cellType_ == e_FieldCellType::CELL_TYPE_WALL;
	}

	//------------------------------
	// 
	//	�e��X�|�[���p
	// 
	//------------------------------

	tnl::Vector2i DungeonCreater::randomChoiceCanFirstSpawnFieldCellPos() {
		std::vector<tnl::Vector2i> canSpawnCellsPos{};

		for (int x = 0; x < FIELD_WIDTH; ++x) {
			for (int y = 0; y < FIELD_HEIGHT; ++y) {
				if (fieldCells_[x][y].cellType_ == e_FieldCellType::CELL_TYPE_ROOM && fieldCells_[x][y].isAlreadySpawnSomething == false) {
					canSpawnCellsPos.emplace_back(tnl::Vector2i{ x,y });
				}
			}
		}

		const tnl::Vector2i returnPos = canSpawnCellsPos[static_cast<int>(mtRandomRangeInt(0, canSpawnCellsPos.size() - 1))];
		fieldCells_[returnPos.x][returnPos.y].isAlreadySpawnSomething = true;
		return returnPos;
	}

	tnl::Vector2i DungeonCreater::randomChoiceEnemyRespawnPos(const tnl::Vector2i& playerPos) {
		std::vector <tnl::Vector2i> canSpawnCellsPos{};

		auto playerPosID = getFieldCellID(playerPos);

		for (int x = 0; x < FIELD_WIDTH; ++x) {
			for(int y = 0; y < FIELD_HEIGHT; ++y)
				if (fieldCells_[x][y].cellType_ == e_FieldCellType::CELL_TYPE_ROOM && fieldCells_[x][y].regionAreaID_ != playerPosID) {
					canSpawnCellsPos.emplace_back(tnl::Vector2i{ x,y });
				}
		}

		tnl::Vector2i returnPos = canSpawnCellsPos[static_cast<int>(mtRandomRangeInt(0, canSpawnCellsPos.size() - 1))];
		return returnPos;
	}

	bool DungeonCreater::isCanMoveFieldCellPos(const tnl::Vector2i& toMovePos) {
		if (fieldCells_[toMovePos.x][toMovePos.y].cellType_ == e_FieldCellType::CELL_TYPE_WALL) return false;
		else return true;
	}

	void DungeonCreater::choiceInitSpawnPos() {
		
		// �v���C���[�X�|�[��
		playerSpawnPos_ = randomChoiceCanFirstSpawnFieldCellPos();
		// �K�i�X�|�[��
		stairsSpawnPos_ = randomChoiceCanFirstSpawnFieldCellPos();

		// �G�l�~�[�X�|�[��
		enemySpawnPosArray_.clear();
		for (int i = 0;i < ENEMY_SPAWN_NUM;++i) {
			enemySpawnPosArray_.emplace_back(randomChoiceCanFirstSpawnFieldCellPos());
		}

		// �A�C�e���X�|�[��
		itemSpawnPosArray_.clear();
		for (int i = 0; i < ITEM_SPAWN_NUM;++i) {
			itemSpawnPosArray_.emplace_back(randomChoiceCanFirstSpawnFieldCellPos());
		}
	}

	//------------------------------
	// 
	//	�f�o�b�O�p
	// 
	//------------------------------

	void DungeonCreater::debag_DisplayArea(int offsetDisplayPosX, int offsetDisplayPosY) const {
		for (int i = 0; i < areas_.size(); ++i) {
			for (int x = areas_[i].posX_; x < areas_[i].posX_ + areas_[i].width_; ++x) {
				for (int y = areas_[i].posY_; y < areas_[i].posY_ + areas_[i].height_; ++y) {
					DrawStringEx((x * 15) + offsetDisplayPosX, (y * 15) + offsetDisplayPosY, -1, "%d", i);
				}
			}
		}
	}

	void DungeonCreater::debag_DisplayFieldCells(int offsetDisplayPosX, int offsetDisplayPosY) const {
		for (int x = 0; x < FIELD_WIDTH; ++x) {
			for (int y = 0; y < FIELD_HEIGHT; ++y) {
				DrawStringEx((x * 15) + offsetDisplayPosX, (y * 15) + offsetDisplayPosY, -1, "%d", fieldCells_[x][y].cellType_);
			}
		}
	}

	void DungeonCreater::debag_DisplayFieldData(int offsetDisplayPosX, int offsetDisplayPosY) const {
		for (int x = 0; x < FIELD_WIDTH; ++x) {
			for (int y = 0; y < FIELD_HEIGHT; ++y) {
				if (fieldCells_[x][y].cellType_ == e_FieldCellType::CELL_TYPE_ROOM) {
					DrawStringEx((x * 15) + offsetDisplayPosX, (y * 15) + offsetDisplayPosY, -1, " ");
				}
				if (fieldCells_[x][y].cellType_ == e_FieldCellType::CELL_TYPE_PATH) {
					DrawStringEx((x * 15) + offsetDisplayPosX, (y * 15) + offsetDisplayPosY, -1, "+");
				}
				if (fieldCells_[x][y].cellType_ == e_FieldCellType::CELL_TYPE_WALL) {
					DrawStringEx((x * 15) + offsetDisplayPosX, (y * 15) + offsetDisplayPosY, -1, "*");
				}
			}
		}

		for (const auto& enemyPos : enemySpawnPosArray_) {
			DrawStringEx(enemyPos.x * 15 + offsetDisplayPosX, enemyPos.y * 15 + offsetDisplayPosY,GetColor(255,0,0),"e");
		}

		for (const auto& itemPos : itemSpawnPosArray_) {
			DrawStringEx(itemPos.x * 15 + offsetDisplayPosX, itemPos.y * 15 + offsetDisplayPosY, GetColor(0, 255, 0), "I");
		}

		DrawStringEx(playerSpawnPos_.x * 15 + offsetDisplayPosX, playerSpawnPos_.y * 15 + offsetDisplayPosY, GetColor(0, 0, 255), "P");
	}

	void DungeonCreater::debag_OutputLogGeneratedAreaRoomData() const {
		for (int i = 0; i < areas_.size(); ++i) {
			tnl::DebugTrace("\n -------------- debag_OutputLogGeneratedData �ɂ�� �f�o�b�O���O ------------");
			tnl::DebugTrace("\n area => %d , posX,Y = [%d,%d] , width,height = [%d,%d]", i, areas_[i].posX_, areas_[i].posY_, areas_[i].width_, areas_[i].height_);
			tnl::DebugTrace("\n room => posX,Y = [%d,%d] , width,height = [%d,%d]", areas_[i].room_.posX_, areas_[i].room_.posY_, areas_[i].room_.width_, areas_[i].room_.height_);
		}
	}


}