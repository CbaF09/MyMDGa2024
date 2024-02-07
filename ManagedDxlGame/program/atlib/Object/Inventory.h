#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "ItemData.h"

namespace atl {

	class Inventory final {
	public:
		Inventory() {};

		// �f�o�b�O�p ( �w��̃A�C�e�����ŏ���������Ă��� )
		Inventory(Shared<ItemData> items);

		// �Q�b�^�[
		const std::vector<Shared<ItemData>>& getItemList() const { return itemList_; }
		const Shared<ItemData>& getItemData(int index) const;
		
		// �A�C�e�������X�g�ɒǉ�����B�ő吔�����܂��Ă���A�ǉ��ł����ꍇ�� true ,�ł��Ȃ������ꍇ�� false ��Ԃ�
		bool pushbackItemToInventory(const Shared<ItemData> newItemData);

		void useItem(int32_t index);

	private:
		const int32_t INVENTORY_MAX = 6;
		std::vector<Shared<ItemData>> itemList_;

	};

}