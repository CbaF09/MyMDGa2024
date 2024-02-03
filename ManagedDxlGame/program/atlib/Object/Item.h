#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class Item {
	public:
		Item();

		const Shared<dxe::Texture> getItemIllust() { return itemIllust_; }

	private:
		int32_t itemID_ = 0;
		std::string itemName_ = "";
		Shared<dxe::Texture> itemIllust_ = nullptr;
	};

}