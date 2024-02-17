#include "Item.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/SceneManager.h"
#include "../Utilities/AtlRandom.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/EnemyPawn.h"
#include "../Utilities/AtlString.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"
#include "../MagicRuneSystem/MagicRune.h"
#include "PlayerData.h"

namespace atl {

	Item::Item() {
		// CSV ���烍�[�h���āA�����_���ȃA�C�e���ɐݒ肳���
		auto csv = tnl::LoadCsv("csv/ItemCSV.csv");

		auto itemID = mtRandomRangeInt(1, static_cast<int>(e_itemList::ITEM_MAX) - 1);

		if (!csv.empty()) {
			// ID �� enum �ɕϊ����Ă���Z�b�g
			itemID_ = static_cast<e_itemList>(csv[itemID][0].getInt());
			// Item�����Z�b�g
			itemName_ = csv[itemID][1].getString();
			// Item �ɑΉ�����e�N�X�`�����Z�b�g
			auto& filepath = csv[itemID][2].getString();
			itemIllust_ = dxe::Texture::CreateFromFile(filepath);
			// �A�C�e���̐��������Z�b�g
			descString_ = csv[itemID][3].getString();
		}
	}

	// ���O�o�͂̂��߂̃w���p�[�֐��B��������getTextLogManager�����̂��߂�ǂ��̂ŁB
	void Item::addTextItemUse(std::string text) {
		TextLogManager::getTextLogManager()->addTextLog(text);
	}

	void Item::executeItemPerformAction() {
		// itemID �ɉ����āA���ʂ𔭓�
		switch (itemID_) {
		case e_itemList::HealHerb: healHerbAction(); break;
		case e_itemList::HealPotion: healPotionAction(); break;
		case e_itemList::ThunderStone: thunderStoneAction(); break;
		case e_itemList::ThunderScroll: thunderScrollAction(); break;
		case e_itemList::MagicInc: magicIncAction(); break;
		case e_itemList::HealRune: healRuneAction(); break;
		case e_itemList::FireRune: fireRuneAction(); break;
		case e_itemList::StoneRune: stoneRuneAction(); break;
		default: break;
		}
	}

	// �񕜂̑� ... �̗͏���
	void Item::healHerbAction() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (dungeonScene) {
			dungeonScene->getPlayerPawn()->getPlayerData()->changeCurrentHP(HERB_HEAL_VALUE);
			addTextItemUse("�񕜂̑�������");
			addTextItemUse("�̗͂�" + convertFullWidthNumber(HERB_HEAL_VALUE) + "�񕜂���");
		}
	}

	// �񕜖�@... �̗͒���
	void Item::healPotionAction() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (dungeonScene) {
			dungeonScene->getPlayerPawn()->getPlayerData()->changeCurrentHP(POTION_HEAL_VALUE);
			addTextItemUse("�񕜖������");
			addTextItemUse("�̗͂�" + convertFullWidthNumber(POTION_HEAL_VALUE) + "�񕜂���");
		}
	}

	// �v���C���[�Ɠ����G���A�̓G�Ƀ_���[�W
	void Item::thunderStoneAction() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (dungeonScene) {
			auto& enemies = dungeonScene->getEnemyArray();
			auto& player = dungeonScene->getPlayerPawn();
			auto playerArea = DungeonCreater::getDungeonCreater()->getFieldCellID(player->getPlayer2Dpos());
			for (auto& enemy : enemies) {
				auto enemyArea = DungeonCreater::getDungeonCreater()->getFieldCellID(enemy->get2Dpos());
				auto& enemyName = enemy->getEnemyData()->getEnemyName();
				// �v���C���[�Ɠ����G���A�Ȃ�
				if (playerArea == enemyArea) {
					enemy->getEnemyData()->changeCurrentHP(-THUNDER_STONE_DAMAGE_VALUE);
					addTextItemUse(enemyName + "��" + convertFullWidthNumber(THUNDER_STONE_DAMAGE_VALUE) + "�̃_���[�W�I");
				}
			}
		}
	}

	// �t�B�[���h�̓G�S���Ƀ_���[�W
	void Item::thunderScrollAction() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (dungeonScene) {
			auto& enemies = dungeonScene->getEnemyArray();
			for (auto& enemy : enemies) {
				enemy->getEnemyData()->changeCurrentHP(-THUNDER_SCROLL_DAMAGE_VALUE);
				auto& enemyName = enemy->getEnemyData()->getEnemyName();
				addTextItemUse(enemyName + "��" + convertFullWidthNumber(THUNDER_SCROLL_DAMAGE_VALUE) + "�̃_���[�W�I");
			}
		}
	}

	// ���ҏ�̊��� ( �����x ) ��L�΂�
	void Item::magicIncAction() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (dungeonScene) {
			dungeonScene->changeSatiety(MAGIC_INC_HEAL_VALUE);
			addTextItemUse("���ҏ�̊������L�т�");
		}

	}

	// �����̃��[������
	void Item::healRuneAction() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (dungeonScene) {
			MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<HealRune>(), *dungeonScene);
			addTextItemUse("�����̃��[���𑕔�����");
		}
	}

	// ���̃��[������
	void Item::fireRuneAction() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (dungeonScene) {
			MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<FireRune>(), *dungeonScene);
			addTextItemUse("���̃��[���𑕔�����");
		}
	}

	// ��̃��[������
	void Item::stoneRuneAction() {
		auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
		if (dungeonScene) {
			MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<StoneRune>(), *dungeonScene);
			addTextItemUse("��̃��[���𑕔�����");
		}
	}
}