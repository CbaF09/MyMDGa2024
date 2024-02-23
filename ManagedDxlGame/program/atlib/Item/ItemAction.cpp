#include "ItemAction.h"
#include "../atlib/Singletons/TextLogManager.h"
#include "../atlib/Singletons/SceneManager.h"
#include "../atlib/Singletons/DungeonCreater.h"
#include "../atlib/Singletons/EnemyManager/EnemyManager.h"
#include "../atlib/Utilities/AtlString.h"
#include "../atlib/MagicRuneSystem/MagicRuneSystemManager.h"
#include "../atlib/MagicRuneSystem/MagicRune.h"
#include "../atlib/MeshObject/PlayerPawn.h"

namespace atl {

	/// <summary>
/// 
/// 回復の草
/// 
/// </summary>
	void HealHerbAction::execute() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) { return; }

		dungeonScene->getPlayerPawn()->getPlayerData()->changeCurrentHP(HERB_HEAL_VALUE);
		TextLogManager::getTextLogManager()->addTextLog("回復の草を飲んだ");
		TextLogManager::getTextLogManager()->addTextLog("体力が" + convertFullWidthNumber(HERB_HEAL_VALUE) + "回復した");
	}

	/// <summary>
	/// 
	/// 回復薬
	/// 
	/// </summary>
	void HealPotionAction::execute() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) { return; }

		dungeonScene->getPlayerPawn()->getPlayerData()->changeCurrentHP(POTION_HEAL_VALUE);
		TextLogManager::getTextLogManager()->addTextLog("回復薬を飲んだ");
		TextLogManager::getTextLogManager()->addTextLog("体力が" + convertFullWidthNumber(POTION_HEAL_VALUE) + "回復した");
	}

	/// <summary>
	/// 
	/// 雷神の石
	/// 
	/// </summary>
	void ThundeStoneAction::execute() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) { return; }

		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		auto& player = dungeonScene->getPlayerPawn();
		auto playerArea = DungeonCreater::getDungeonCreater()->getFieldCellID(player->getPlayer2Dpos());
		for (auto& enemy : enemyList) {
			auto enemyArea = DungeonCreater::getDungeonCreater()->getFieldCellID(enemy->get2Dpos());
			auto& enemyName = enemy->getEnemyData()->getEnemyName();
			// プレイヤーと同じエリアなら
			if (playerArea == enemyArea) {
				enemy->getEnemyData()->changeCurrentHP(-THUNDER_STONE_DAMAGE_VALUE);
				TextLogManager::getTextLogManager()->addTextLog(enemyName + "に" + convertFullWidthNumber(THUNDER_STONE_DAMAGE_VALUE) + "のダメージ！");
			}
		}
	}

	/// <summary>
	/// 
	/// 雷神の巻物
	/// 
	/// </summary>
	void ThundeScrollAction::execute() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) { return; }

		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) {
			enemy->getEnemyData()->changeCurrentHP(-THUNDER_SCROLL_DAMAGE_VALUE);
			auto& enemyName = enemy->getEnemyData()->getEnemyName();
			TextLogManager::getTextLogManager()->addTextLog(enemyName + "に" + convertFullWidthNumber(THUNDER_SCROLL_DAMAGE_VALUE) + "のダメージ！");
		}
	}

	/// <summary>
	/// 
	/// 魔法のインク
	/// 
	/// </summary>
	void MagicIncAction::execute() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) { return; }

		dungeonScene->changeSatiety(MAGIC_INC_HEAL_VALUE);
		TextLogManager::getTextLogManager()->addTextLog("招待状の期限が伸びた");
	}

	/// <summary>
	/// 
	/// 癒しのルーン
	/// 
	/// </summary>
	void HealRuneAction::execute() {
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<HealRune>());
		TextLogManager::getTextLogManager()->addTextLog("癒しのルーンを装備した");
	}

	/// <summary>
	/// 
	/// 炎のルーン
	/// 
	/// </summary>
	void FireRuneAction::execute() {
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<FireRune>());
		TextLogManager::getTextLogManager()->addTextLog("炎のルーンを装備した");
	}

	/// <summary>
	/// 
	/// 岩のルーン
	/// 
	/// </summary>
	void StoneRuneAction::execute() {
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<StoneRune>());
		TextLogManager::getTextLogManager()->addTextLog("岩のルーンを装備した");
	}


}