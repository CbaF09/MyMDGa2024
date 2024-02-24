#include "ItemAction.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/SceneManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/EnemyManager/EnemyManager.h"
#include "../Utilities/AtlString.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"
#include "../MagicRuneSystem/MagicRune.h"
#include "../MeshObject/PlayerPawn.h"
#include "../Object/EnemyData.h"

namespace atl {

	/// <summary>
	/// 
	/// �񕜂̑�
	///	
	/// </summary>
	void HealHerbAction::execute() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) { return; }

		dungeonScene->getPlayerPawn()->getPlayerData()->changeCurrentHP(HERB_HEAL_VALUE);
		TextLogManager::getTextLogManager()->addTextLog("�񕜂̑�������");
		TextLogManager::getTextLogManager()->addTextLog("�̗͂�" + convertFullWidthNumber(HERB_HEAL_VALUE) + "�񕜂���");
	}

	/// <summary>
	/// 
	/// �񕜖�
	/// 
	/// </summary>
	void HealPotionAction::execute() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) { return; }

		dungeonScene->getPlayerPawn()->getPlayerData()->changeCurrentHP(POTION_HEAL_VALUE);
		TextLogManager::getTextLogManager()->addTextLog("�񕜖������");
		TextLogManager::getTextLogManager()->addTextLog("�̗͂�" + convertFullWidthNumber(POTION_HEAL_VALUE) + "�񕜂���");
	}

	/// <summary>
	/// 
	/// ���_�̐�
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
			// �v���C���[�Ɠ����G���A�Ȃ�
			if (playerArea == enemyArea) {
				enemy->getEnemyData()->changeCurrentHP(-THUNDER_STONE_DAMAGE_VALUE);
				TextLogManager::getTextLogManager()->addTextLog(enemyName + "��" + convertFullWidthNumber(THUNDER_STONE_DAMAGE_VALUE) + "�̃_���[�W�I");
			}
		}
	}

	/// <summary>
	/// 
	/// ���_�̊���
	/// 
	/// </summary>
	void ThundeScrollAction::execute() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) { return; }

		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) {
			enemy->getEnemyData()->changeCurrentHP(-THUNDER_SCROLL_DAMAGE_VALUE);
			auto& enemyName = enemy->getEnemyData()->getEnemyName();
			TextLogManager::getTextLogManager()->addTextLog(enemyName + "��" + convertFullWidthNumber(THUNDER_SCROLL_DAMAGE_VALUE) + "�̃_���[�W�I");
		}
	}

	/// <summary>
	/// 
	/// ���@�̃C���N
	/// 
	/// </summary>
	void MagicIncAction::execute() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (!dungeonScene) { return; }

		dungeonScene->changeSatiety(MAGIC_INC_HEAL_VALUE);
		TextLogManager::getTextLogManager()->addTextLog("���ҏ�̊������L�т�");
	}

	/// <summary>
	/// 
	/// ���̃��[��
	/// 
	/// </summary>
	void FireRuneAction::execute() {
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<FireRune>());
		TextLogManager::getTextLogManager()->addTextLog("���̃��[���𑕔�����");
	}

	/// <summary>
	/// 
	/// ��̃��[��
	/// 
	/// </summary>
	void StoneRuneAction::execute() {
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<StoneRune>());
		TextLogManager::getTextLogManager()->addTextLog("��̃��[���𑕔�����");
	}


}