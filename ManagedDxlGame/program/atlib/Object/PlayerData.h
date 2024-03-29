#pragma once
#include "Base_ActorData.h"
#include "Inventory.h"

namespace atl {

	/// <summary>
	/// 
	/// プレイヤーのステータス、レベル、インベントリを管理するクラス
	/// 
	/// </summary>

	class PlayerData final : public Base_ActorData {
	public:
		PlayerData();

		// ゲッター
		inline const Shared<Inventory> getInventory() { return playerInventory_; }
		inline const int32_t getCurrentLevel() { return currentLevel_; }

		// 経験値を得る処理。必要経験値を超えるとレベルアップする ( levelUp 関数が走る )
		void changeCurrentEXP(int32_t getExp);

		// デバッグ用
		// arg ... 表示する左上座標
		void debug_playerDataParam(int x, int y);

	private:
		// 現在のレベル
		int32_t currentLevel_ = 1;
		// 現在蓄積中の経験値
		int32_t currentExp_ = 0;
		// レベルアップに必要な経験値
		// TODO : 将来的に外部データから変化させるつもりなので一旦変数宣言
		int32_t needExp_ = 15;

		// レベルアップした時に上昇するHP
		const int32_t LEVEL_UP_HP_UP_VALUE = 5;

		// プレイヤーのインベントリ
		Shared<Inventory> playerInventory_ = std::make_shared<Inventory>();

		// 現在経験値がレベルアップに必要な経験値を超えた時の処理。レベルアップ
		void levelUp();
	};

}