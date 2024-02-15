#include "../../dxlib_ext/dxlib_ext.h"
#include "MagicRune.h"
#include "../Scenes/DungeonScene.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	/// <summary>
	/// 癒しのルーン
	/// </summary>
	HealRune::HealRune() {
		setRuneGraph(ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/MagicRuneGreen.png"));
	}

	// ターンスタート時に、追加で体力回復する
	void HealRune::onNotify(e_EventType eventType,DungeonScene& dungeonScene) {
		if (eventType == e_EventType::TurnStart) {
			dungeonScene.turnHealHP();
		}
	}
	
	/// <summary>
	/// 炎のルーン
	/// </summary>
	FireRune::FireRune() {
		setRuneGraph(ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/MagicRuneRed.png"));
	}
}