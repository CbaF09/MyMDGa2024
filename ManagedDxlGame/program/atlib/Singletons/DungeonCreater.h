#pragma once
#include <vector>
#include <memory>

namespace atl {

	// ダンジョン生成管理クラス ( シングルトン )
	// 役割 ... ダンジョン自動生成 , 生成したダンジョンデータの保持
	class DungeonCreater final {
	//------------------------------------------
	// シングルトン設計
	public:
		static DungeonCreater* getDungeonCreater();
		static void deleteDungeonCreater();
	private:
		static DungeonCreater* p_instance_;
	//------------------------------------------
	public:
		// セルの状態用 enum
		enum class e_FieldCellType {
			CELL_TYPE_NONE = 0,
			CELL_TYPE_ROOM,
			CELL_TYPE_PATH,
			CELL_TYPE_WALL,
		};

		class Room {
		public:
			int32_t posX_ = 0, posY_ = 0, width_ = 0, height_ = 0;
		};

		class Area {
		public:
			int32_t posX_ = 0, posY_ = 0, width_ = 0, height_ = 0;
			Room room_;
		};

		class FieldCell {
		public:
			e_FieldCellType cellType_ = e_FieldCellType::CELL_TYPE_NONE;
			bool isAlreadySpawnSomething = false;
		};

		// ダンジョンを生成。fieldCells_ に保存される
		void createDungeon();

		// ゲッター関数
		inline static const int32_t getFieldWidth() { return FIELD_WIDTH; }
		inline static const int32_t getFieldHeight() { return FIELD_HEIGHT; }
		inline const std::vector<std::vector<FieldCell>>& getFieldCells() const { return fieldCells_; }
		inline const tnl::Vector2i& getPlayerSpawnPos() const { return playerSpawnPos_; }
		inline const tnl::Vector2i& getStairsSpawnPos() const { return stairsSpawnPos_; }
		inline const int32_t getEnemySpawnNum() { return ENEMY_SPAWN_NUM; }
		inline const int32_t getItemSpawnNum() { return ITEM_SPAWN_NUM; }
		inline const std::vector<tnl::Vector2i>& getEnemySpawnPos() const { return enemySpawnPosArray_; }
		inline const std::vector<tnl::Vector2i>& getItemSpawnPos() const { return itemSpawnPosArray_; }
		inline const FieldCell getFieldCellByV2i(tnl::Vector2i pos) const { return fieldCells_[pos.x][pos.y]; }

		// fieldCells_ から、スポーン可能状態の空セルを抽出してリストにし、その中からランダムに一つ選び、そのXY座標を返す
		// 選ばれた XY地点の fieldCell は、スポーン不可状態に切り替わる
		const tnl::Vector2i& randomChoiceCanSpawnFieldCellPos();

		// 移動先が、移動可能かどうかチェックする
		bool isCanMoveFieldCellPos(const tnl::Vector2i& toMovePos);

		// ------------------------------------------
		//  デバッグ用
		// 分割したエリアを文字列で可視表示
		// arg ... 頂点(左上)をオフセットし、表示位置を補正 ( ほぼほぼ必要ないのでデフォルト引数 0 )
		void debag_DisplayArea(int offsetDisplayPosX = 0, int offsetDisplayPosY = 0) const;
		
		// フィールドを文字列で可視表示
		// arg ... 頂点(左上)をオフセットし、表示位置を補正 ( ほぼほぼ必要ないのでデフォルト引数 0 )
		void debag_DisplayFieldCells(int offsetDisplayPosX = 0, int offsetDisplayPosY = 0) const;

		// エネミー・アイテム・プレイヤー・階段の位置も含め文字列で可視表示
		// arg ... 頂点(左上)をオフセットし、表示位置を補正 ( ほぼほぼ必要ないのでデフォルト引数 0 )
		void debag_DisplayFieldData(int offsetDisplayPosX = 0, int offsetDisplayPosY = 0) const;

		// ログにダンジョンの情報を出力
		void debag_OutputLogGeneratedAreaRoomData() const;

	private:

		// 定数 ,Enum , インナークラス
		static const int32_t FIELD_WIDTH = 32;		// フィールド全体の横幅
		static const int32_t FIELD_HEIGHT = 32;		// フィールド全体の縦幅
		const int32_t AREA_MAX = 8;			// 区域の最大数 , 部屋の最大数と等しくなる
		const int32_t AREA_SIZE_MIN = 8;		// 区域の縦横最小サイズ
		const int32_t ROOM_SIZE_MIN = 4;		// 部屋の縦横最小サイズ

		enum class e_PathDirection {
			PATH_DIR_TOP = 0,
			PATH_DIR_BOTTOM,
			PATH_DIR_LEFT,
			PATH_DIR_RIGHT,
			PATH_DIR_MAX,
		};

		//----------------------- 
		// メンバ変数
		const int32_t ENEMY_SPAWN_NUM = 3; // 敵の数
		const int32_t ITEM_SPAWN_NUM = 5; // アイテムの数

		tnl::Vector2i playerSpawnPos_{ 0,0 };
		tnl::Vector2i stairsSpawnPos_{ 0,0 };
		std::vector<tnl::Vector2i> enemySpawnPosArray_;
		std::vector<tnl::Vector2i> itemSpawnPosArray_;

		std::vector<Area> areas_;

		// 二次元フィールドデータ
		std::vector<std::vector<FieldCell>> fieldCells_ =
			std::vector<std::vector<FieldCell>>(FIELD_HEIGHT, std::vector<FieldCell>(FIELD_WIDTH));

		//----------------------- 
		// メソッド

		void fieldCellsInit();
		void areaInit();

		void areaSprit(int32_t areaID = 0);
		void roomCreate();
		void pathwayCreate();
		void createPathwayToTop(const Area& area);
		void createPathwayToBottom(const Area& area);
		void createPathwayToLeft(const Area& area);
		void createPathwayToRight(const Area& area);
		void choicePlayerSpawnPos();
		void choiceStairsSpawnPos();
		void choiceEnemySpawnPos();
		void choiceItemSpawnPos();
	};

};