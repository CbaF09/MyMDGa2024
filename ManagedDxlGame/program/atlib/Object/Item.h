#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	/// <summary>
	/// 
	/// アイテムの情報と処理を持つ
	/// ItemPawn や Inventory から利用される
	/// 
	/// </summary>

	class Item final {
	public:
		explicit Item(std::weak_ptr<DungeonScene> dungeonScene);

		// ゲッター ( アイテムのイラスト )
		const Shared<dxe::Texture> getItemIllust() { return itemIllust_; }
		// ゲッター ( アイテム名 )
		const std::string& getItemName() const { return itemName_; }
		// ゲッター ( アイテム説明文 )
		const std::string& getItemDescString() const { return descString_; }

		// 使用時処理を実行する関数。Inventoryクラス の useItem() から呼び出される
		void executeItemPerformAction();

	private:
		//------------------------------------------
		// 変数

		// アイテム名とIDを一致させるためのenum
		enum class e_itemList {
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

		// アイテムID
		e_itemList itemID_ = e_itemList::NONE;
		// ゲームに表示されるアイテム名
		std::string itemName_ = "";
		// アイテムの説明文
		std::string descString_ = "";
		// アイテムの見た目用テクスチャ
		Shared<dxe::Texture> itemIllust_ = nullptr;

		// ダンジョンシーンへの弱参照
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		//------------------------------------------
		// 関数

		// アイテムを使った時にログ出力するヘルパー関数。いちいちgetTextLogManager書くのがめんどいので。
		void addTextItemUse(std::string text);

		// 以下、各アイテムに対応する処理。ダンジョンシーンへの弱参照があるので色々できるはず

		// 回復の草 ... 体力小回復
		const int32_t HERB_HEAL_VALUE = 15;
		void healHerbAction();

		// 回復薬　... 体力中回復
		const int32_t POTION_HEAL_VALUE = 50;
		void healPotionAction();

		// プレイヤーと同じエリアに属している敵全体にダメージを与える
		const int32_t THUNDER_STONE_DAMAGE_VALUE = 20;
		void thunderStoneAction();

		// フィールドの敵全員にダメージを与える
		const int32_t THUNDER_SCROLL_DAMAGE_VALUE = 10;
		void thunderScrollAction();

		// 招待状の状態を回復する ( 満腹度を回復する )
		const int32_t MAGIC_INC_HEAL_VALUE = 1000;
		void magicIncAction();

		// 癒しのルーン装備
		void healRuneAction();

		// 炎のルーン装備
		void fireRuneAction();

		// 岩のルーン装備
		void stoneRuneAction();
	};

}