#include "Inventory.h"
#include "../Singletons/TextLogManager.h"

namespace atl {

	// �w��̃A�C�e�����ŏ���������Ă��� ( �f�o�b�O�p )
	Inventory::Inventory(Shared<ItemData> items) {
		pushbackItemToInventory(items);
	}

	// �͈͊O�A�N�Z�X�`�F�b�N�L�B��������������nullptr��Ԃ�
	const Shared<ItemData>& Inventory::getItemData(int index) const {
		// �͈͊O�A�N�Z�X�`�F�b�N�B��������������nullptr��Ԃ�
		if (index < 0 || index >= static_cast<int>(itemList_.size())) {
			 return nullptr;
		}

		return itemList_[index];
	}

	bool Inventory::pushbackItemToInventory(const Shared<ItemData> newItemData) {
		if (itemList_.size() < INVENTORY_MAX) {
			itemList_.emplace_back(newItemData);
			return true;
		}
		else {
			return false;
		}
	}

	void Inventory::useItem(int32_t index) {
		// �͈͊O�`�F�b�N
		if (index < 0 || index >= itemList_.size()) { return; }

		// �A�C�e���̌��ʎ��s
		itemList_[index]->executeItemPerformAction();

		itemList_.erase(itemList_.begin() + index);
	}

}

