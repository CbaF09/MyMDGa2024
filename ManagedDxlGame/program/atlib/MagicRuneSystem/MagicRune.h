#pragma once
#include "Base_MagicRune.h"

namespace atl {

	class HealBuffMagicRune : public Base_MagicRune {
	public:
		HealBuffMagicRune();
		void onNotify(e_EventType eventType,DungeonScene& dungeonScene) override;
	};

}