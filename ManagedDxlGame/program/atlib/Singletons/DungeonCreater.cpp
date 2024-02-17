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
		// 初期化
		fieldCellsInit();
		areaInit();

		// ダンジョン生成
		areaSprit();
		roomCreate();
		pathwayCreate();

		// フィールドセルにIDを設定する ( どの Area に属しているか )
		setFieldCellsID();

		// 四方を壁に囲まれているセルは、NONEに設定する ( 四方を壁に囲まれている箇所には何も生成しない )
		eraseNonMeanWall();

		// 各種スポーン
		choiceInitSpawnPos();
	}

	//------------------------------
	// 
	//		初期化用
	// 
	//------------------------------

	void DungeonCreater::fieldCellsInit() {
		// 全てを壁に設定する
		// 何かがスポーンしているかフラグをオフに
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
	//		ダンジョン生成用
	// 
	//------------------------------

	void DungeonCreater::areaSprit(int32_t areaID) {
		// エリアの生成上限に達していたら終わり
		if (areas_.size() == AREA_MAX) return;

		// エリア増やす
		areas_.emplace_back();

		int32_t newAreaID = static_cast<int32_t>(areas_.size() - 1);
		int32_t beforeWidth = areas_[areaID].width_;
		int32_t beforeHeight = areas_[areaID].height_;

		// 縦分割・横分割をランダムに行う
		if (mtRandom() % 2)
		{ // 縦方向に分割
			areas_[areaID].width_ /= 2;

			areas_[newAreaID].posX_ = areas_[areaID].posX_ + areas_[areaID].width_;
			areas_[newAreaID].posY_ = areas_[areaID].posY_;
			areas_[newAreaID].width_ = beforeWidth - areas_[areaID].width_;
			areas_[newAreaID].height_ = areas_[areaID].height_;
		}
		else { // 横方向に分割
			areas_[areaID].height_ /= 2;

			areas_[newAreaID].posX_ = areas_[areaID].posX_;
			areas_[newAreaID].posY_ = areas_[areaID].posY_ + areas_[areaID].height_;
			areas_[newAreaID].width_ = areas_[areaID].width_;
			areas_[newAreaID].height_ = beforeHeight - areas_[areaID].height_;
		}

		// --- 分割終了条件の確認（ 最小サイズより小さくなっていた場合、分割を無かった事にし、分割を終了する )
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

		// 新しく増えたエリアにエリアIDを設定
		areas_[newAreaID].areaID_ = newAreaID;

		// 分割元だったエリアに対し再帰実行
		areaSprit(areaID);

		// 分割後、増えたエリアに対し再帰実行
		areaSprit(newAreaID);	
	};

	void DungeonCreater::roomCreate() {
		// ルームをランダムな大きさで生成
		// （ただし、エリアの左端・上端からは1マス、右端・下端からは2マス開けるようにする）
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

	// フィールドセルIDを、所属しているareaIDに設定する
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
		// 各エリアの右端と下端を CELL_TYPE_PATH に
		for (const Area& area : areas_) {
			// 下端
			for (int x = area.posX_; x < area.posX_ + area.width_; ++x) { 
				fieldCells_[x][area.posY_ + area.height_ - 1].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			// 右端
			for (int y = area.posY_; y < area.posY_ + area.height_; ++y) { 
				fieldCells_[area.posX_ + area.width_ - 1][y].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
		}
		
		// ルームから上下左右に通路を伸ばす。
		for (const Area& area : areas_) {
			// 出来る通路候補の関数ポインタ配列 ( 上下左右 )
			std::vector<std::function<void()>> possiblePathwayFuncs;

			// もしエリアが左端でないなら、左に向かう通路を作る関数を候補に
			if (area.posX_ != 0) {
				possiblePathwayFuncs.emplace_back(
					[this, &area]() { createPathDirection(area, e_PathDirection::PATH_DIR_LEFT); }); }

			// もしエリアが上端でないなら
			if (area.posY_ != 0) {
				possiblePathwayFuncs.emplace_back(
					[this, &area]() { createPathDirection(area, e_PathDirection::PATH_DIR_TOP); });
			}

			// もしエリアが右端でないなら
			if ((area.posX_ + area.width_) != FIELD_WIDTH) {
				possiblePathwayFuncs.emplace_back(
					[this, &area]() { createPathDirection(area, e_PathDirection::PATH_DIR_RIGHT); });
			}

			// もしエリアが下端でないなら
			if ((area.posY_ + area.height_) != FIELD_HEIGHT) {
				possiblePathwayFuncs.emplace_back(
					[this, &area]() { createPathDirection(area, e_PathDirection::PATH_DIR_BOTTOM); });
			}

			// 作れる通路の本数が2本しかない場合、それを作らないと、到達不能な部屋が出来てしまう可能性があるので、先に処理する
			if (possiblePathwayFuncs.size() == 2) {
				possiblePathwayFuncs[0]();
				possiblePathwayFuncs[1]();
				continue;
			}

			// 関数ポインタ配列をシャッフル
			std::shuffle(possiblePathwayFuncs.begin(), possiblePathwayFuncs.end(), mtRandom);

			// 1 ~ 最大で作れる通路の方向、で乱数を取り、その数だけ通路を作る ( 最低一本は通路を作る )
			int pathwaysToCreate = mtRandomRangeInt(1, possiblePathwayFuncs.size()); 
			for (int i = 0; i < pathwaysToCreate; ++i) {
				possiblePathwayFuncs[i]();
			}
		}

		// フィールドの下端と右端を壁にする
		for (int x = 0; x < FIELD_WIDTH; ++x) {
			fieldCells_[x][0].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
			fieldCells_[x][FIELD_HEIGHT - 1].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
		}
		for (int y = 0; y < FIELD_HEIGHT; ++y) {
			fieldCells_[0][y].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
			fieldCells_[FIELD_WIDTH - 1][y].cellType_ = e_FieldCellType::CELL_TYPE_WALL;
		}

		// セル周囲4マスのうち3マスが壁なら、そのセルも壁にする
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

		// 逆向きからも走査しなければ右向き・下向き通路の行き止まりが消えない
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
		
		// 上向き
		case e_PathDirection::PATH_DIR_TOP:
			randomX = mtRandomRangeInt(area.room_.posX_ + 1, area.room_.posX_ + area.room_.width_ - 2);
			for (int y = area.posY_; y < area.room_.posY_; ++y) {
				fieldCells_[randomX][y].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			break;

		// 下向き
		case e_PathDirection::PATH_DIR_BOTTOM:
			randomX = mtRandomRangeInt(area.room_.posX_ + 1, area.room_.posX_ + area.room_.width_ - 2);
			for (int y = area.room_.posY_ + area.room_.height_; y < area.posY_ + area.height_; ++y) {
				fieldCells_[randomX][y].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			break;
		
		// 右向き
		case e_PathDirection::PATH_DIR_RIGHT: 
			randomY = mtRandomRangeInt(area.room_.posY_ + 1, area.room_.posY_ + area.room_.height_ - 2);
			for (int x = area.room_.posX_ + area.room_.width_; x < area.posX_ + area.width_; ++x) {
				fieldCells_[x][randomY].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			break;

		// 下向き
		case e_PathDirection::PATH_DIR_LEFT:
			randomY = mtRandomRangeInt(area.room_.posY_ + 1, area.room_.posY_ + area.room_.height_ - 2);
			for (int x = area.posX_; x < area.room_.posX_; ++x) {
				fieldCells_[x][randomY].cellType_ = e_FieldCellType::CELL_TYPE_PATH;
			}
			break;
		}
	}
	
	void DungeonCreater::eraseNonMeanWall() {
		// NONEになる壁のインデックスが入る一時配列
		std::vector<std::pair<int,int>> eraseCellIndex;

		for (auto x = 0; x < fieldCells_.size(); ++x) {
			for (auto y = 0; y < fieldCells_[x].size(); ++y) {

				// 壁以外のセルタイプなら早期リターン
				if (fieldCells_[x][y].cellType_ != e_FieldCellType::CELL_TYPE_WALL) { continue; }

				// 四近傍が壁なら、自身はNONEに設定される
				if (isWall(x + 1, y) && 
					isWall(x - 1, y) && 
					isWall(x, y + 1) && 
					isWall(x, y - 1)) {
					eraseCellIndex.emplace_back(std::make_pair(x,y));
				}
			}
		}

		// 一時配列の中身を元に、NONEに設定する
		for (auto& cellIndex : eraseCellIndex) {
			fieldCells_[cellIndex.first][cellIndex.second].cellType_ = e_FieldCellType::CELL_TYPE_NONE;
		}
	}

	// true => 壁、 fale => 壁ではない
	bool DungeonCreater::isWall(int x, int y) {
		// 境界チェック
		if (x < 0 || x >  fieldCells_.size() - 1 || 
			y < 0 || y > fieldCells_[x].size() - 1) {
			// 配列の範囲外は壁として扱う
			return true;
		}

		// セルタイプが壁なら true
		return fieldCells_[x][y].cellType_ == e_FieldCellType::CELL_TYPE_WALL;
	}

	//------------------------------
	// 
	//	各種スポーン用
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
		
		// プレイヤースポーン
		playerSpawnPos_ = randomChoiceCanFirstSpawnFieldCellPos();
		// 階段スポーン
		stairsSpawnPos_ = randomChoiceCanFirstSpawnFieldCellPos();

		// エネミースポーン
		enemySpawnPosArray_.clear();
		for (int i = 0;i < ENEMY_SPAWN_NUM;++i) {
			enemySpawnPosArray_.emplace_back(randomChoiceCanFirstSpawnFieldCellPos());
		}

		// アイテムスポーン
		itemSpawnPosArray_.clear();
		for (int i = 0; i < ITEM_SPAWN_NUM;++i) {
			itemSpawnPosArray_.emplace_back(randomChoiceCanFirstSpawnFieldCellPos());
		}
	}

	//------------------------------
	// 
	//	デバッグ用
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
			tnl::DebugTrace("\n -------------- debag_OutputLogGeneratedData による デバッグログ ------------");
			tnl::DebugTrace("\n area => %d , posX,Y = [%d,%d] , width,height = [%d,%d]", i, areas_[i].posX_, areas_[i].posY_, areas_[i].width_, areas_[i].height_);
			tnl::DebugTrace("\n room => posX,Y = [%d,%d] , width,height = [%d,%d]", areas_[i].room_.posX_, areas_[i].room_.posY_, areas_[i].room_.width_, areas_[i].room_.height_);
		}
	}


}