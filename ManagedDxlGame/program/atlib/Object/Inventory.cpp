#include "Inventory.h"
#include "../Singletons/TextLogManager.h"

namespace atl {

	// 指定のアイテムが最初から入っている ( デバッグ用 )
	Inventory::Inventory(Shared<Item> items) {
		pushbackItemToInventory(items);
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
		if (itemList_.size() < INVENTORY_MAX) {
			itemList_.emplace_back(newItemData);
			return true;
		}
		else {
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

