#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Item/Item.h"

namespace atl {

	class Inventory final {
	public:
		
		Inventory() {};
		~Inventory();

		// ゲッター ( アイテムのvector配列を取得 )
		const std::vector<Shared<Item>>& getItemList() const { return itemList_; }

		// ゲッター ( インデックスを指定する事で、リストから直接アイテムを取得する )
		const Shared<Item> getItem(int index) const;
		
		// アイテムを配列に追加する。最大数が決まっており、追加できた場合は true ,できなかった場合は false を返す
		bool pushbackItemToInventory(const Shared<Item> newItemData);

		// アイテムの使用効果を処理した後、配列から削除する
		// arg ... 使うアイテムをインデックスで指定
		void useItem(int32_t index);
		
	private:
		// 持てるアイテムの最大数
		const int32_t INVENTORY_MAX = 6;
		
		// インベントリは、アイテムのリストを持つ
		std::vector<Shared<Item>> itemList_{};
	};

}