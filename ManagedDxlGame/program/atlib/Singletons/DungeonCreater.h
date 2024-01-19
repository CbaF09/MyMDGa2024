#pragma once
#include <vector>
#include <memory>

namespace atl {

	// ダンジョン生成管理クラス ( シングルトン )
	// 役割 ... ダンジョン自動生成 , 生成したダンジョンデータの保持
	class DungeonCreater final {

		class Area;
		class Room;
		class FieldCell;

	public:
		// セルの状態用 enum
		enum class e_FieldCellType {
			CELL_TYPE_NONE = 0,
			CELL_TYPE_WALL,
			CELL_TYPE_MAX,
		};

		// インスタンス生成 , 既に生成されている場合はそのインスタンスを返す
		static DungeonCreater* getDungeonCreater();

		// インスタンスがある場合、消去
		static void deleteDungeonCreater();

		// ダンジョンを生成。fieldCells_ に保存される
		void createDungeon();

		// ゲッター関数
		inline const std::vector<std::vector<FieldCell>>& getFieldCells() const { return fieldCells_; }
		inline Shared<const tnl::Vector2i> getPlayerSpawnPos() const { return playerSpawnPos_; }
		inline Shared<const tnl::Vector2i> getStairsSpawnPos() const { return stairsSpawnPos_; }
		inline static const int32_t getFieldWidth() { return FIELD_WIDTH; }
		inline static const int32_t getFieldHeight() { return FIELD_HEIGHT; }

		// fieldCells_ から、スポーン可能状態の空セルを抽出してリストにし、その中からランダムに一つ選び、そのXY座標を返す
		// 選ばれた XY地点の fieldCell は、スポーン不可状態に切り替わる
		const Shared<tnl::Vector2i> randomChoiceCanSpawnFieldCellPos();

		// --- デバッグ用関数

		// 分割したエリアを文字列で可視表示
		// arg ... 頂点(左上)をオフセットし、表示位置を補正
		void debag_DisplayArea(int offsetDisplayPosX = 0, int offsetDisplayPosY = 0) const;
		
		// フィールドを文字列で可視表示
		// arg ... 頂点(左上)をオフセットし、表示位置を補正
		void debag_DisplayFieldCells(int offsetDisplayPosX = 0, int offsetDisplayPosY = 0) const;

		// ログにダンジョンの情報を出力
		void debag_OutputLogGeneratedData() const;


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

		//----------------------- 
		// メンバ変数

		static DungeonCreater* p_instance_;

		Shared<tnl::Vector2i> playerSpawnPos_ = nullptr;
		Shared<tnl::Vector2i> stairsSpawnPos_ = nullptr;
		Shared<tnl::Vector2i> enemySpawnPos_ = nullptr;

		std::vector<Area> areas_;

		// 二次元フィールドデータ
		std::vector<std::vector<FieldCell>> fieldCells_ =
			std::vector<std::vector<FieldCell>>(FIELD_HEIGHT, std::vector<FieldCell>(FIELD_WIDTH));

		//----------------------- 
		// メソッド

		void fieldCellsInit();
		void areaInit();
		void everySpawnInit();
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
	};

};