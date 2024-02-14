#pragma once
#include "Base_ActorData.h"
#include "Inventory.h"
#include "MagicRuneSlot.h"

namespace atl {

	class PlayerData final: public Base_ActorData {
	public:
		PlayerData();

		// ゲッター
		inline const Shared<Inventory> getInventory() { return playerInventory_; }
		inline const int32_t getCurrentLevel() { return currentLevel_; }

		// セッター
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
		int32_t needExp_ = 15;
		const int32_t LEVEL_UP_HP_UP_VALUE = 5;

		// プレイヤーのインベントリ
		Shared<Inventory> playerInventory_ = std::make_shared<Inventory>();
		
		// プレイヤーのマジックルーンスロット
		Shared<MagicRuneSlot> playerMagicRuneSlot = std::make_shared<MagicRuneSlot>();

		// 現在経験値がレベルアップに必要な経験値を超えた時の処理。レベルアップ
		void levelUp();
	};

}