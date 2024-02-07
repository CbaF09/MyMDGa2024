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
		const std::string& getItemDescString() const { return descString_; }

		// �g�p�����ʁBenum�ŊǗ�
		void executeItemPerformAction();

	private:
		enum class e_itemList {
			NONE = 0,
			HealPotion = 1,
			ITEM_MAX,
		};

		e_itemList itemID_ = static_cast<e_itemList>(0);
		std::string itemName_ = "";
		std::string descString_ = "";
		Shared<dxe::Texture> itemIllust_ = nullptr;


	};

}