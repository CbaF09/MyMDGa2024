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
	/// 癒しのルーン
	/// </summary>
	void HealRune::onNotify(e_EventType eventType) {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		// ダンジョンシーンでないなら早期リターン
		if (!dungeonScene) return;

		if (eventType == e_EventType::TurnStart) {
			dungeonScene->turnHealHP();
		}
	}

	/// <summary>
	/// 炎のルーン
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
	/// 岩のルーン
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