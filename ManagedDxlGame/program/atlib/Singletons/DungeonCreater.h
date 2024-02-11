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
		static void deleteDungeonCreater() { delete getDungeonCreater(); };
	private:
		DungeonCreater() {};
		~DungeonCreater() {};
	//------------------------------------------
	public:
		// セルの状態用 enum
		enum class e_FieldCellType {
			CELL_TYPE_NONE = 0, // 外れ値、初期化用
			CELL_TYPE_ROOM,	// 部屋
			CELL_TYPE_PATH,	// 通路
			CELL_TYPE_WALL,	// 壁
		};

		// 部屋
		class Room {
		public:
			int32_t posX_ = 0, posY_ = 0, width_ = 0, height_ = 0;
		};

		// 区域分割用のエリアクラス
		class Area {
		public:
			int32_t posX_ = 0, posY_ = 0, width_ = 0, height_ = 0;
			int32_t areaID_;

			// エリアは一つの部屋を持つ
			Room room_;
		};

		// 1マスを表現するクラス
		class FieldCell {
		public:
			// セルのタイプ
			e_FieldCellType cellType_ = e_FieldCellType::CELL_TYPE_NONE;
			// 既に何かがスポーンしているマスか
			bool isAlreadySpawnSomething = false;
			// どのエリアのマスなのか
			int32_t regionAreaID_ = 0;
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

		// 引数に渡した座標のフィールドセルのIDを取得
		inline const int32_t getFieldCellID(tnl::Vector2i pos) const { return fieldCells_[pos.x][pos.y].regionAreaID_; }

		// fieldCells_ から、スポーン可能状態の空セルを抽出してリストにし、その中からランダムに一つ選び、そのXY座標を返す
		// 選ばれた XY地点の fieldCell は、スポーン不可状態に切り替わる
		tnl::Vector2i randomChoiceCanSpawnFieldCellPos();
		// fieldCells_ から、スポーン可能状態のセルを抽出してリストにし、その中からランダムに一つ選び、そのXY座標を返す
		// 敵のリスポーン用
		// arg ... スポーンから除外するフィールドセルのID
		tnl::Vector2i randomChoiceCanSpawnFieldCellPos(const tnl::Vector2i& playerPos);

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

		// 定数 ,Enum
		static const int32_t FIELD_WIDTH = 32;		// フィールド全体の横幅
		static const int32_t FIELD_HEIGHT = 32;		// フィールド全体の縦幅
		const int32_t AREA_MAX = 8;			// 区域の最大数 , 部屋の最大数と等しくなる
		const int32_t AREA_SIZE_MIN = 8;		// 区域の縦横最小サイズ
		const int32_t ROOM_SIZE_MIN = 4;		// 部屋の縦横最小サイズ

		// 通路がどっち向きに伸びるかを表現するenum
		enum class e_PathDirection {
			PATH_DIR_TOP = 0,
			PATH_DIR_BOTTOM,
			PATH_DIR_LEFT,
			PATH_DIR_RIGHT,
			PATH_DIR_MAX,
		};

		//----------------------- 
		// メンバ変数
		const int32_t ENEMY_SPAWN_NUM = 1; // 敵の数
		const int32_t ITEM_SPAWN_NUM = 5; // アイテムの数

		tnl::Vector2i playerSpawnPos_{ 0,0 };	// プレイヤーがスポーンする位置
		tnl::Vector2i stairsSpawnPos_{ 0,0 };	// 階段がスポーンする位置
		std::vector<tnl::Vector2i> enemySpawnPosArray_;	// 敵がスポーンする位置配列
		std::vector<tnl::Vector2i> itemSpawnPosArray_;	// アイテムがスポーンする位置配列

		std::vector<Area> areas_;	// 区域分割用エリア配列

		std::vector<std::vector<FieldCell>> fieldCells_ =
			std::vector<std::vector<FieldCell>>(FIELD_HEIGHT, std::vector<FieldCell>(FIELD_WIDTH)); // 二次元フィールドデータ

		//----------------------- 
		// メソッド

		// 初期化
		void fieldCellsInit();
		void areaInit();

		// 区域分割 ( 再帰処理の為に引数があるが、最初に呼び出す時は引数 0 で呼び出される前提なのでデフォルト引数 )
		void areaSprit(int32_t areaID = 0);
		// 部屋生成
		void roomCreate();
		// fieldCells_ の parentArea を設定
		void setFieldCellsID();
		// 通路設定
		void pathwayCreate();

		// 通路を生成する為の関数群
		// TODO : 時間があったら一個にまとめてみるのに挑戦
		void createPathwayToTop(const Area& area);
		void createPathwayToBottom(const Area& area);
		void createPathwayToLeft(const Area& area);
		void createPathwayToRight(const Area& area);

		// スポーン候補地点を設定する関数群
		// TODO : 時間があったら一個にまとめてみるのに挑戦
		void choicePlayerSpawnPos();
		void choiceStairsSpawnPos();
		void choiceEnemySpawnPos();
		void choiceItemSpawnPos();
	};

};