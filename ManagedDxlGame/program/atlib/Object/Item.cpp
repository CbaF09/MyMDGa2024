#include "Item.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Utilities/AtlRandom.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/EnemyPawn.h"
#include "../Utilities/AtlString.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"
#include "../MagicRuneSystem/MagicRune.h"
#include "PlayerData.h"

namespace atl {

	Item::Item(std::weak_ptr<DungeonScene> dungeonScene) : weakDungeonScene_(dungeonScene) {
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
		case e_itemList::HealRune: healRuneAction(); break;
		case e_itemList::FireRune: fireRuneAction(); break;
		default : break;
		}
	}

	// 回復の草 ... 体力小回復
	void Item::healHerbAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			lock->getPlayerPawn()->getPlayerData()->changeCurrentHP(HERB_HEAL_VALUE);
		}
		addTextItemUse("回復の草を飲んだ");
		addTextItemUse("体力が" + convertFullWidthNumber(HERB_HEAL_VALUE) + "回復した");
	}

	// 回復薬　... 体力中回復
	void Item::healPotionAction(){
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			lock->getPlayerPawn()->getPlayerData()->changeCurrentHP(POTION_HEAL_VALUE);
		}
		addTextItemUse("回復薬を飲んだ");
		addTextItemUse("体力が"+ convertFullWidthNumber(POTION_HEAL_VALUE) + "回復した");

	}

	// プレイヤーと同じエリアの敵にダメージ
	void Item::thunderStoneAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			auto& enemies = lock->getEnemyArray();
			auto& player = lock->getPlayerPawn();
			auto playerArea = DungeonCreater::getDungeonCreater()->getFieldCellID(player->getPlayer2Dpos());
			for (auto& enemy : enemies) {
				auto enemyArea = DungeonCreater::getDungeonCreater()->getFieldCellID(enemy->get2Dpos());
				auto& enemyName = enemy->getEnemyData()->getEnemyName();
				// プレイヤーと同じエリアなら
				if (playerArea == enemyArea) {
					enemy->getEnemyData()->changeCurrentHP(-THUNDER_STONE_DAMAGE_VALUE);
					addTextItemUse(enemyName + "に" + convertFullWidthNumber(THUNDER_STONE_DAMAGE_VALUE) + "のダメージ！");
				}
			}
		}
	}

	// フィールドの敵全員にダメージ
	void Item::thunderScrollAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			auto& enemies = lock->getEnemyArray();
			for (auto& enemy : enemies) {
				enemy->getEnemyData()->changeCurrentHP(-THUNDER_SCROLL_DAMAGE_VALUE);
				auto& enemyName = enemy->getEnemyData()->getEnemyName();
				addTextItemUse(enemyName + "に" + convertFullWidthNumber(THUNDER_SCROLL_DAMAGE_VALUE) + "のダメージ！");
			}
		}
	}

	// 招待状の期限 ( 満腹度 ) を伸ばす
	void Item::magicIncAction() {
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			lock->changeSatiety(MAGIC_INC_HEAL_VALUE);
			addTextItemUse("招待状の期限が伸びた");
		}

	}

	// 癒しのルーン装備
	void Item::healRuneAction() {
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<HealRune>());
		addTextItemUse("癒しのルーンを装備した");
	}

	// 炎のルーン装備
	void Item::fireRuneAction() {
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<FireRune>());
		addTextItemUse("炎のルーンを装備した");
	}
}