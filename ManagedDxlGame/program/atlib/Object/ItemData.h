#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Scenes/DungeonScene.h"

namespace atl {


	class ItemData final {
	public:
		explicit ItemData(std::weak_ptr<DungeonScene> dungeonScene);

		const Shared<dxe::Texture> getItemIllust() { return itemIllust_; }
		const std::string& getItemName() const { return itemName_; }
		const std::string& getItemDescString() const { return descString_; }

		// 使用時効果。enumで管理
		void executeItemPerformAction();

	private:
		enum class e_itemList {
			NONE = 0,
			HealHerb,
			HealPotion,
			ThunderStone,
			ThunderScroll,
			MagicInc,
			ITEM_MAX,
		};
		e_itemList itemID_ = e_itemList::NONE;
		std::string itemName_ = "";
		std::string descString_ = "";
		Shared<dxe::Texture> itemIllust_ = nullptr;

		// ダンジョンシーンへの弱参照
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		// アイテムを使った時に出力するログ。いちいちgetTextLogManager書くのがめんどいので。
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
	};

}