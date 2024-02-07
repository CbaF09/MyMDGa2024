#include "Inventory.h"

namespace atl {

	Inventory::Inventory(Shared<ItemData> items) {
		pushBackItem(items);
	}


	void Inventory::pushBackItem(const Shared<ItemData> newItemData) {
		itemList_.emplace_back(newItemData);
	}
}