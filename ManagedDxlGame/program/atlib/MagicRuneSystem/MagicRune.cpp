#include "../../dxlib_ext/dxlib_ext.h"
#include "MagicRune.h"
#include "MagicRuneSystemManager.h"
#include "../Scenes/DungeonScene.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/SceneManager.h"
#include "../MeshObject/PlayerPawn.h"
#include "../Object/PlayerData.h"

namespace atl {

	/// <summary>
	/// ‰Š‚Ìƒ‹[ƒ“
	/// </summary>
	void FireRune::onEquipMomentNotify() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) return;

		auto& playerStats = dungeonScene->getPlayerPawn()->getPlayerData();
		playerStats->setAttackPower(playerStats->getAttackPower() + ATTACK_UP_VALUE);
	}

	void FireRune::onRemoveMomentNotify() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) return;

		auto& playerStats = dungeonScene->getPlayerPawn()->getPlayerData();
		playerStats->setAttackPower(playerStats->getAttackPower() - ATTACK_UP_VALUE);
	}

	/// <summary>
	/// Šâ‚Ìƒ‹[ƒ“
	/// </summary>
	void StoneRune::onEquipMomentNotify() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) return;
		
		auto& playerStats = dungeonScene->getPlayerPawn()->getPlayerData();
		playerStats->setDefencePower(playerStats->getDefencePower() + DEFENCE_UP_VALUE);
	}

	void StoneRune::onRemoveMomentNotify() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) return;

		auto& playerStats = dungeonScene->getPlayerPawn()->getPlayerData();
		playerStats->setDefencePower(playerStats->getDefencePower() - DEFENCE_UP_VALUE);
	}

}