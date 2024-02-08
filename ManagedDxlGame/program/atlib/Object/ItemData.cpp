#include "ItemData.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Utilities/AtlRandom.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/EnemyPawn.h"
#include "../Utilities/AtlString.h"
#include "PlayerData.h"

namespace atl {

	ItemData::ItemData(std::weak_ptr<DungeonScene> dungeonScene) : weakDungeonScene_(dungeonScene) {
		// CSV からロードして、ランダムなアイテムに設定される
		auto csv = tnl::LoadCsv("csv/ItemCSV.csv");

		auto itemID = mtRandomRangeInt(1, static_cast<int>(e_itemList::ITEM_MAX) - 1);

		if (!csv.empty()) {
			// ID を enum に変換してからセット
			itemID_ = static_cast<e_itemList>(csv[itemID][0].getInt());
			// Item名をセット
			itemName_ = csv[itemID][1].getString();
			// Item に対応するテクスチャをセット
			auto& filepath = csv[itemID][2].getString();
			itemIllust_ = dxe::Texture::CreateFromFile(filepath);
			// アイテムの説明文をセット
			descString_ = csv[itemID][3].getString();
		}
	}

	// ログ出力のための関数。いちいちgetTextLogManager書くのがめんどいので。
	void ItemData::addTextItemUse(std::string text) {
		TextLogManager::getTextLogManager()->addTextLog(text);
	}

	void ItemData::executeItemPerformAction() {
		switch (itemID_) {
		case e_itemList::NONE: break;
		case e_itemList::HealHerb: healHerbAction(); break;
		case e_itemList::HealPotion: healPotionAction(); break;
		case e_itemList::ThunderStone: thunderStoneAction(); break;
		case e_itemList::ThunderScroll: thunderScrollAction(); break;
		}
	}

	void ItemData::healHerbAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			lock->getPlayerPawn()->getPlayerData()->changeCurrentHP(HERB_HEAL_VALUE);
		}
		addTextItemUse("回復の草を飲んだ");
		addTextItemUse("体力が" + convertFullWidthNumber(HERB_HEAL_VALUE) + "回復した");
	}

	void ItemData::healPotionAction(){
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			lock->getPlayerPawn()->getPlayerData()->changeCurrentHP(POTION_HEAL_VALUE);
		}
		addTextItemUse("回復薬を飲んだ");
		addTextItemUse("体力が"+ convertFullWidthNumber(POTION_HEAL_VALUE) + "回復した");

	}

	// プレイヤーと同じエリアの敵にダメージ
	void ItemData::thunderStoneAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			auto& enemies = lock->getEnemyArray();
			auto& player = lock->getPlayerPawn();
			auto playerArea = DungeonCreater::getDungeonCreater()->getFieldCellID(player->getPlayer2Dpos());
			for (auto& enemy : enemies) {
				auto enemyArea = DungeonCreater::getDungeonCreater()->getFieldCellID(enemy->get2Dpos());
				// プレイヤーと同じエリアなら
				if (playerArea == enemyArea) {
					auto realyDamaged = enemy->getEnemyData()->damaged(THUNDER_STONE_DAMAGE_VALUE);
					addTextItemUse("敵に" + convertFullWidthNumber(realyDamaged) + "のダメージ！");
				}
			}
		}
	}

	// フィールドの敵全員にダメージ
	void ItemData::thunderScrollAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			auto& enemies = lock->getEnemyArray();
			for (auto& enemy : enemies) {
				auto realyDamaged = enemy->getEnemyData()->damaged(THUNDER_SCROLL_DAMAGE_VALUE);
				addTextItemUse("敵に" + convertFullWidthNumber(realyDamaged) + "のダメージ！");
			}
		}
	}
}