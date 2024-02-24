#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Item.h"
#include "ItemAction.h"

namespace atl {

	class ItemFactory {
	public:
		// アイテムを指定して生成する
		static Shared<Item> createItem(e_ItemList itemName);

		// アイテムをランダム生成する
		static Shared<Item> createRandomItem();

	private:
		// 指定したアイテムの効果クラス ( 具象クラス ) を生成する
		// createItem から呼び出されるため、private
		static Shared<IE_ItemAction> createItemActionFromItemName(e_ItemList itemName);
	};

}