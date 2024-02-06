#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class ItemData final {
	public:
		ItemData();

		const Shared<dxe::Texture> getItemIllust() { return itemIllust_; }
		const std::string& getItemName() const { return itemName_; }

	private:
		int32_t itemID_ = 0;
		std::string itemName_ = "";
		Shared<dxe::Texture> itemIllust_ = nullptr;

		enum class e_itemList {
			HealPotion = 0,

		};
	};

}