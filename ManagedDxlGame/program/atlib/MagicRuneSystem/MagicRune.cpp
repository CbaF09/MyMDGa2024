#include "../../dxlib_ext/dxlib_ext.h"
#include "MagicRune.h"
#include "../Scenes/DungeonScene.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	HealBuffMagicRune::HealBuffMagicRune() {
		setRuneGraph(ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/MagicRuneGreen.png"));
	}

	void HealBuffMagicRune::onNotify(e_EventType eventType,DungeonScene& dungeonScene) {
		if (eventType == e_EventType::TurnStart) {
			dungeonScene.turnHealHP();
		}
	}
}