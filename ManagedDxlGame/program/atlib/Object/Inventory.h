#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "ItemData.h"

namespace atl {

	class Inventory final {
	public:
		Inventory() {};

		// �f�o�b�O�p
		Inventory(Shared<ItemData> items);

		// �Q�b�^�[
		const std::list<Shared<ItemData>>& getItemList() const { return itemList_; }
		
		// �A�C�e�������X�g�ɒǉ�����
		void pushBackItem(const Shared<ItemData> newItemData);

	private:
		std::list<Shared<ItemData>> itemList_;

	};

}