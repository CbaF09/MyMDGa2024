#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// アイテムの効果を表す基底クラス
	/// 継承先にて効果実装
	/// 
	/// </summary>
	class IE_ItemAction {
	public:
		virtual ~IE_ItemAction() {};
		virtual void execute() = 0;
	};

	/// <summary>
	/// 
	/// 回復の草 ... 体力小回復
	/// 
	/// </summary>
	class HealHerbAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// 回復量
		const int32_t HERB_HEAL_VALUE = 15;
	};

	/// <summary>
	/// 
	/// 回復薬　... 体力中回復
	/// 
	/// </summary>
	class HealPotionAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// 回復量
		const int32_t POTION_HEAL_VALUE = 50;
	};

	/// <summary>
	/// 
	/// 雷神の石 ... プレイヤーと同じエリアに属している敵全員にダメージを与える
	/// 
	/// </summary>
	class ThundeStoneAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// 与えるダメージ
		const int32_t THUNDER_STONE_DAMAGE_VALUE = 20;
	};

	/// <summary>
	/// 
	/// 雷神の巻物 ... フィールドの敵全員にダメージを与える
	/// 
	/// </summary>
	class ThundeScrollAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// 与えるダメージ
		const int32_t THUNDER_SCROLL_DAMAGE_VALUE = 10;
	};


	/// <summary>
	/// 
	/// 魔法のインク ... 招待状の状態を回復する ( 満腹度を回復する )
	/// 
	/// </summary>
	class MagicIncAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// 回復量
		const int32_t MAGIC_INC_HEAL_VALUE = 1000;
	};

	/// <summary>
	/// 
	/// 癒しのルーン ... 癒しのルーンを装備する
	/// 
	/// </summary>
	class HealRuneAction : public IE_ItemAction {
	public:
		void execute() override;
	};

	/// <summary>
	/// 
	/// 炎のルーン ... 炎のルーンを装備する
	/// 
	/// </summary>
	class FireRuneAction : public IE_ItemAction {
	public:
		void execute() override;
	};

	/// <summary>
	/// 
	/// 岩のルーン ... 岩のルーンを装備する
	/// 
	/// </summary>
	class StoneRuneAction : public IE_ItemAction {
	public:
		void execute() override;
	};

}