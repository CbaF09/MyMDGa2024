#include "Inventory.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	Inventory::~Inventory() {
		ResourceManager::getResourceManager()->deleteResource("sound/SE/DungeonSceneItemGet.ogg");
	}

	// �͈͊O�A�N�Z�X�`�F�b�N�L�B��������������nullptr��Ԃ�
	const Shared<Item> Inventory::getItem(int index) const {
		// �͈͊O�A�N�Z�X�`�F�b�N�B��������������nullptr��Ԃ�
		if (index < 0 || index >= static_cast<int>(itemList_.size())) {
			 return nullptr;
		}

		return itemList_[index];
	}

	bool Inventory::pushbackItemToInventory(const Shared<Item> newItemData) {
		if (itemList_.size() < INVENTORY_MAX) {
			itemList_.emplace_back(newItemData);
			ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneItemGet.ogg",DX_PLAYTYPE_BACK);
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

