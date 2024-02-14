#include "../../dxlib_ext/dxlib_ext.h"
#include "MagicRune.h"
#include "../Scenes/DungeonScene.h"

namespace atl {
	
	void HealBuffMagicRune::onNotify(e_EventType eventType,DungeonScene& dungeonScene) {
		if (eventType == e_EventType::TurnStart) {
			dungeonScene.turnHealHP();
		}
	}
}