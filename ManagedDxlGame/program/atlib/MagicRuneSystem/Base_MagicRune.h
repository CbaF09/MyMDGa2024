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
	};

}