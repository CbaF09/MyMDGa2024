#pragma once

namespace atl {


	/// <summary>
	/// 
	/// MagicRune の基底クラス。
	/// オブザーバーパターンのサブジェクトにあたる。
	/// オブザーバーにあたる存在は、MagicRuneSystemManager
	/// 
	/// </summary>
	
	class DungeonScene;

	// イベント列挙子 MagicRuneが発動するタイミングを表現
	enum class e_EventType {
		Debug,			// デバッグ用
		TurnStart,		// ターン開始時
	};

	// 名前とCSV上のIDを分かりやすくさせる為の enum
	enum class e_RuneID {
		NONE,
		FireRune,
		StoneRune,
	};

	class Base_MagicRune {
	public:
		virtual ~Base_MagicRune() {};
		
		// イベントタイプに対して発動する処理 ( 純粋仮想関数 )
		virtual void onNotify(e_EventType eventType) = 0;
		// 装備した時に発動する処理
		virtual void onEquipMomentNotify() {};
		// 装備を外した時に発動する処理
		virtual void onRemoveMomentNotify() {};

		// ゲッター ( ルーン用2Dアイコン画像を取得 )
		const int getRuneGraph() { return graphHandle_; }
		// ゲッター ( ルーンの名前を取得 )
		const std::string& getRuneName() { return runeName_; }
		// ゲッター ( ルーンの説明文を取得 )
		const std::string& getRuneDesc() { return runeDesc_; }

		// ルーンの情報をCSVから読み込んだデータに基づいてまとめて設定
		// arg ... アイテムID ( CSV の A列 に記載 ) を enum で入力
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