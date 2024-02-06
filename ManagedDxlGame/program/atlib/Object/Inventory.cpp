#include "Inventory.h"

namespace atl {

	Inventory::Inventory() {
		itemList_.clear();
	}

	void Inventory::pushBackItem(const Shared<ItemData> newItemData) {
		itemList_.emplace_back(newItemData);
	}
}