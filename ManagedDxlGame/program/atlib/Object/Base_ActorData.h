#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	// プレイヤー ・ エネミーに共通するデータと関数を保持
	// EnemyData, PlayerData などに継承
	class Base_ActorData {
	public:
		virtual ~Base_ActorData() {};

		// ゲッター
		inline const int32_t getMaxHP() const { return maxHP_; }
		inline const int32_t getCurrentHP() const { return currentHP_; }
		inline const float getCurrentHPpersent() const { return currentHPpersent; }
		inline const int32_t getattackPower() const { return attackPower_; }

		// セッター
		inline void setMaxHP(int32_t maxHP) { maxHP_ = maxHP; }
		inline void setCurrentHP(int32_t currentHP) { currentHP_ = currentHP; calcAndSetCurrentHPpersent(); }
		inline void setAttackPower(int32_t attackPower) { attackPower_ = attackPower; }

		// 引数の値だけHPを増減する
		void changeCurrentHP(int32_t addsumValue);

		// 攻撃された時に呼び出す。計算式を適用してHPを減らす。
		// 計算式 ... 最大ダメージ = attackPower, 最小ダメージ = attackPower * 7/8 , の間でランダムな値
		// arg ... 攻撃側の攻撃力
		// ret ... 実際に与えたダメージ
		int32_t damaged(int32_t attackedPower);

		// HPがゼロ => true , ゼロではない => false
		bool isZeroHP();

		// 現在HPと最大HPの割合を計算する
		void calcAndSetCurrentHPpersent();

	private:
		int32_t maxHP_ = 0;
		int32_t currentHP_ = 0;
		float currentHPpersent = 0.0f;
		int32_t attackPower_ = 0;
	};

}