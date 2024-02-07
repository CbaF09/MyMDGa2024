#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "ItemData.h"

namespace atl {

	class Inventory final {
	public:
		Inventory() {};

		// デバッグ用 ( 指定のアイテムが最初から入っている )
		Inventory(Shared<ItemData> items);

		// ゲッター
		const std::vector<Shared<ItemData>>& getItemList() const { return itemList_; }
		const Shared<ItemData>& getItemData(int index) const;
		
		// アイテムをリストに追加する。最大数が決まっており、追加できた場合は true ,できなかった場合は false を返す
		bool pushbackItemToInventory(const Shared<ItemData> newItemData);

		void useItem(int32_t index);

	private:
		const int32_t INVENTORY_MAX = 6;
		std::vector<Shared<ItemData>> itemList_;

	};

}