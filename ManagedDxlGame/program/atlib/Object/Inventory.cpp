#include "Inventory.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"
#include "../atlib/Item/Item.h"

namespace atl {

	Inventory::~Inventory() {
		ResourceManager::getResourceManager()->deleteResource("sound/SE/DungeonSceneItemGet.ogg");
	}

	// 範囲外アクセスチェック有。引っかかったらnullptrを返す
	const Shared<Item> Inventory::getItem(int index) const {
		// 範囲外アクセスチェック。引っかかったらnullptrを返す
		if (index < 0 || index >= static_cast<int>(itemList_.size())) {
			 return nullptr;
		}

		return itemList_[index];
	}

	bool Inventory::pushbackItemToInventory(const Shared<Item> newItemData) {
		// 所持できる最大数を超えていない場合
		if (itemList_.size() < INVENTORY_MAX) {
			itemList_.emplace_back(newItemData);
			ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneItemGet.ogg",DX_PLAYTYPE_BACK);
			return true;
		}
		else {
		// 最大数を超えている場合
			return false;
		}
	}

	void Inventory::useItem(int32_t index) {
		// 範囲外チェック
		if (index < 0 || index >= itemList_.size()) { return; }

		// アイテムの効果実行
		itemList_[index]->executeItemPerformAction();

		itemList_.erase(itemList_.begin() + index);
	}

}

