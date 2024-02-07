#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class ItemData final {
	public:
		ItemData();
		// �f�o�b�O�p �A�C�e��ID����A�C�e���𐶐�����
		explicit ItemData(int32_t itemID);

		const Shared<dxe::Texture> getItemIllust() { return itemIllust_; }
		const std::string& getItemName() const { return itemName_; }

	private:
		int32_t itemID_ = 0;
		std::string itemName_ = "";
		Shared<dxe::Texture> itemIllust_ = nullptr;

		enum class e_itemList {
			NONE = 0,
			HealPotion = 1,
			ITEM_MAX,
		};
	};

}