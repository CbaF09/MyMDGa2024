#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Scenes/DungeonScene.h"
#include "ItemAction.h"

namespace atl {

	/// <summary>
	/// 
	/// アイテムの増やし方
	/// �@ ItemCSVにアイテムの情報を入力
	/// �A e_ItemListを追加 ( アイテム名と関係はないが、可読性の面から気を遣った方が良い )
	/// �B ItemAction .h .cpp に、IE_ItemActionを継承させ、アイテムの処理を実装する
	/// �C ItemFactory .cpp の createItemActionFromItemName 関数の switch-case に、新しいアイテムのe_ItemList列挙子名と新しいItemActionのstd::make_sharedを対応させて書く
	/// 
	/// </summary>

	// アイテム名とIDを一致させるためのenum
	enum class e_ItemList {
		NONE = 0,
		HealHerb,
		HealPotion,
		ThunderStone,
		ThunderScroll,
		MagicInc,
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
		void executeItemPerformAction();

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