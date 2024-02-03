#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class PlayerStatsManager final {
	//------------------------------------------
	// シングルトン設計
	public:
		static PlayerStatsManager* getPlayerStatsManager();
		static void deletePlayerStatsManager();
	private:
		static PlayerStatsManager* p_instance_;
	//------------------------------------------

	public:
		// ゲッター
		inline const int32_t& getPlayerCurrentHP() const { return playerCurrentHP_; }
		inline const float getPlayerCurrentHPpersent() const { return playerCurrentHPpersent; }

		// 引数の値だけHPを増減する
		void changePlayerCurrentHP(int32_t addsumValue);

		// プレイヤーが攻撃された時に呼び出す
		// ① 計算式を適用してからプレイヤーのHPを減らす。
		// ② 受けたダメージをテキストログ先頭に追加する
		// 計算式 ... 最大ダメージ = attackPower, 最小ダメージ = attackPower * 7/8 , の間でランダムな値
		// arg ... 攻撃側の攻撃力
		void damagedPlayer(int32_t attackPower);

		// プレイヤーのHPがゼロ => true , ゼロではない => false
		bool isZeroPlayerHP();
		
		// プレイヤーの現在HPと最大HPの割合を計算する
		float calcAndSetCurrentPlayerHPpersent();

	private:
		int32_t playerCurrentHP_ = 100;
		int32_t playerMaxHP_ = 100;
		float playerCurrentHPpersent = 1.0f;
		
	};
}