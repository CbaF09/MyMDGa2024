#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "ItemData.h"

namespace atl {

	class Inventory final {
	public:
		Inventory() {};

		// デバッグ用
		Inventory(Shared<ItemData> items);

		// ゲッター
		const std::list<Shared<ItemData>>& getItemList() const { return itemList_; }
		
		// アイテムをリストに追加する
		void pushBackItem(const Shared<ItemData> newItemData);

	private:
		std::list<Shared<ItemData>> itemList_;

	};

}