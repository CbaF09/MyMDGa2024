#pragma once
#include "Base_MagicRune.h"

namespace atl {

	// 癒しのルーン
	class HealRune : public Base_MagicRune {
	public:
		HealRune();
		void onNotify(e_EventType eventType,DungeonScene& dungeonScene) override;
	};

	// 炎のルーン
	class FireRune : public Base_MagicRune {
	public:
		FireRune();
		// 処理未記述
		void onNotify(e_EventType eventType, DungeonScene& dungeonScene) override {};
	};

}