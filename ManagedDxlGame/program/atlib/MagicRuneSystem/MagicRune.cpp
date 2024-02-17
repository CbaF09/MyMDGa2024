#include "../../dxlib_ext/dxlib_ext.h"
#include "MagicRune.h"
#include "MagicRuneSystemManager.h"
#include "../Scenes/DungeonScene.h"
#include "../Singletons/ResourceManager.h"
#include "../MeshObject/PlayerPawn.h"
#include "../Object/PlayerData.h"

namespace atl {

	/// <summary>
	/// –ü‚µ‚Ìƒ‹[ƒ“
	/// </summary>
	void HealRune::onNotify(e_EventType eventType, DungeonScene& dungeonScene) {
		if (eventType == e_EventType::TurnStart) {
			dungeonScene.turnHealHP();
		}
	}

	/// <summary>
	/// ‰Š‚Ìƒ‹[ƒ“
	/// </summary>
	void FireRune::onEquipMomentNotify(DungeonScene& dungeonScene) {
		auto& playerStats = dungeonScene.getPlayerPawn()->getPlayerData();
		playerStats->setAttackPower(playerStats->getAttackPower() + ATTACK_UP_VALUE);
	}

	void FireRune::onRemoveMomentNotify(DungeonScene& dungeonScene) {
		auto& playerStats = dungeonScene.getPlayerPawn()->getPlayerData();
		playerStats->setAttackPower(playerStats->getAttackPower() - ATTACK_UP_VALUE);
	}

	/// <summary>
	/// Šâ‚Ìƒ‹[ƒ“
	/// </summary>
	void StoneRune::onEquipMomentNotify(DungeonScene& dungeonScene) {
		auto& playerStats = dungeonScene.getPlayerPawn()->getPlayerData();
		playerStats->setDefencePower(playerStats->getDefencePower() + DEFENCE_UP_VALUE);
	}

	void StoneRune::onRemoveMomentNotify(DungeonScene& dungeonScene) {
		auto& playerStats = dungeonScene.getPlayerPawn()->getPlayerData();
		playerStats->setDefencePower(playerStats->getDefencePower() - DEFENCE_UP_VALUE);
	}

}