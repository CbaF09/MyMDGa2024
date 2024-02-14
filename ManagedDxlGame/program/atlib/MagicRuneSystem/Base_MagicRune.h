#pragma once

namespace atl {

	class DungeonScene;

	// イベント列挙子 MagicRuneが発動するタイミングを表現
	enum class e_EventType {
		Debug,
		TurnStart
	};

	class Base_MagicRune {
	public:
		virtual ~Base_MagicRune() {};
		// イベントタイプに対して発動する処理
		virtual void onNotify(e_EventType eventType,DungeonScene& dungeonScene) = 0;

		// ゲッター ( ルーン用2Dアイコン画像を取得 )
		const int getRuneGraph() { return graphHandle_; }
		// セッター ( ルーン用2Dアイコン画像をセット )
		void setRuneGraph(int graphHandle) { graphHandle_ = graphHandle; };

	private:
		// ルーン用2Dテクスチャ
		int graphHandle_ = -1;
	};

}