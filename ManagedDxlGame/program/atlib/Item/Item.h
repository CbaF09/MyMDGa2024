#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Scenes/DungeonScene.h"
#include "ItemAction.h"

namespace atl {

	// アイテム名とIDを一致させるためのenum
	enum class e_ItemList {
		NONE = 0,
		HealHerb,
		HealPotion,
		ThunderStone,
		ThunderScroll,
		MagicInc,
		HealRune,
		FireRune,
		StoneRune,
		ITEM_MAX,
	};

	/// <summary>
	/// 
	/// アイテムを表すクラス
	/// アイテムのデータを保持
	/// IE_ItemActionポインタを保持 ( ストラテジーパターン )
	/// 
	/// ItemPawn や Inventory から利用される
	/// </summary>
	
	class Item final {
	public:
		// 初期化処理
		// arg ... 生成するアイテム
		void initialize(e_ItemList itemName);

		// ゲッター ( アイテムのイラスト )
		inline const Shared<dxe::Texture> getItemIllust() { return itemIllust_; }
		// ゲッター ( アイテム名 )
		inline const std::string& getItemName() const { return itemName_; }
		// ゲッター ( アイテム説明文 )
		inline const std::string& getItemDescString() const { return descString_; }

		// セッター ( アイテム効果 )
		inline void setItemAction(Shared<IE_ItemAction> itemAction) { itemExecutePerform_ = itemAction; }

		// itemExecutePerform_ の execute が実行される。Inventoryクラス の useItem() から呼び出される
		inline void executeItemPerformAction() { if(itemExecutePerform_) itemExecutePerform_->execute(); }

	private:
		// アイテムID
		e_ItemList itemID_ = e_ItemList::NONE;
		// ゲームに表示されるアイテム名
		std::string itemName_ = "";
		// アイテムの説明文
		std::string descString_ = "";
		// アイテムの見た目用テクスチャ
		Shared<dxe::Texture> itemIllust_ = nullptr;
		// アイテムの効果
		Shared<IE_ItemAction> itemExecutePerform_ = nullptr;
	};


}