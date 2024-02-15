#pragma once

namespace atl {

	class DungeonScene;

	// イベント列挙子 MagicRuneが発動するタイミングを表現
	enum class e_EventType {
		Debug,
		TurnStart
	};

	// 名前とCSV上のIDを分かりやすくさせる為の enum
	enum class e_RuneID {
		NONE = 0,
		HealRune = 1,
		FireRune = 2,
	};

	class Base_MagicRune {
	public:
		virtual ~Base_MagicRune() {};
		// イベントタイプに対して発動する処理 ( 純粋仮想関数 )
		virtual void onNotify(e_EventType eventType,DungeonScene& dungeonScene) = 0;

		// ゲッター ( ルーン用2Dアイコン画像を取得 )
		const int getRuneGraph() { return graphHandle_; }
		// ゲッター ( ルーンの名前を取得 )
		const std::string& getRuneName() { return runeName_; }
		// ゲッター ( ルーンの説明文を取得 )
		const std::string& getRuneDesc() { return runeDesc_; }
		

		// ルーンの情報をCSVから読み込んだデータに基づいてまとめて設定
		// arg ... アイテムID ( CSV の A列 に記載 )
		void setRuneData(e_RuneID itemID);

	private:
		// ルーンの名前
		std::string runeName_ = "";
		// ルーンの説明文
		std::string runeDesc_ = "";
		// ルーン用アイコンのグラフィックハンドル
		int graphHandle_ = -1;
	};

}