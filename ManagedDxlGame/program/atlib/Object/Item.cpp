#include "Item.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Utilities/AtlRandom.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/EnemyPawn.h"
#include "../Utilities/AtlString.h"
#include "PlayerData.h"

namespace atl {

	Item::Item(std::weak_ptr<DungeonScene> dungeonScene) : weakDungeonScene_(dungeonScene) {
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

	// ���O�o�͂̂��߂̊֐��B��������getTextLogManager�����̂��߂�ǂ��̂ŁB
	void Item::addTextItemUse(std::string text) {
		TextLogManager::getTextLogManager()->addTextLog(text);
	}

	void Item::executeItemPerformAction() {
		switch (itemID_) {
		case e_itemList::HealHerb: healHerbAction(); break;
		case e_itemList::HealPotion: healPotionAction(); break;
		case e_itemList::ThunderStone: thunderStoneAction(); break;
		case e_itemList::ThunderScroll: thunderScrollAction(); break;
		case e_itemList::MagicInc: magicIncAction(); break;
		default : break;
		}
	}

	// �񕜂̑� ... �̗͏���
	void Item::healHerbAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			lock->getPlayerPawn()->getPlayerData()->changeCurrentHP(HERB_HEAL_VALUE);
		}
		addTextItemUse("�񕜂̑�������");
		addTextItemUse("�̗͂�" + convertFullWidthNumber(HERB_HEAL_VALUE) + "�񕜂���");
	}

	// �񕜖�@... �̗͒���
	void Item::healPotionAction(){
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			lock->getPlayerPawn()->getPlayerData()->changeCurrentHP(POTION_HEAL_VALUE);
		}
		addTextItemUse("�񕜖������");
		addTextItemUse("�̗͂�"+ convertFullWidthNumber(POTION_HEAL_VALUE) + "�񕜂���");

	}

	// �v���C���[�Ɠ����G���A�̓G�Ƀ_���[�W
	void Item::thunderStoneAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			auto& enemies = lock->getEnemyArray();
			auto& player = lock->getPlayerPawn();
			auto playerArea = DungeonCreater::getDungeonCreater()->getFieldCellID(player->getPlayer2Dpos());
			for (auto& enemy : enemies) {
				auto enemyArea = DungeonCreater::getDungeonCreater()->getFieldCellID(enemy->get2Dpos());
				auto enemyName = enemy->getEnemyData()->getEnemyName();
				// �v���C���[�Ɠ����G���A�Ȃ�
				if (playerArea == enemyArea) {
					auto realyDamaged = enemy->getEnemyData()->damaged(THUNDER_STONE_DAMAGE_VALUE);
					addTextItemUse(enemyName + "��" + convertFullWidthNumber(realyDamaged) + "�̃_���[�W�I");
				}
			}
		}
	}

	// �t�B�[���h�̓G�S���Ƀ_���[�W
	void Item::thunderScrollAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			auto& enemies = lock->getEnemyArray();
			for (auto& enemy : enemies) {
				auto realyDamaged = enemy->getEnemyData()->damaged(THUNDER_SCROLL_DAMAGE_VALUE);
				auto enemyName = enemy->getEnemyData()->getEnemyName();
				addTextItemUse(enemyName + "��" + convertFullWidthNumber(realyDamaged) + "�̃_���[�W�I");
			}
		}
	}

	void Item::magicIncAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			lock->changeSatiety(MAGIC_INC_HEAL_VALUE);
			addTextItemUse("���ҏ�̊������L�т�");
		}

	}
}