#include <cmath>
#include "Base_ActorData.h"
#include "../Utilities/AtlRandom.h"

namespace atl {

	void Base_ActorData::changeCurrentHP(int32_t addsubValue) {
		currentHP_ += addsubValue;
		// HPがゼロを下回ったら、ゼロにクランプ
		if (currentHP_ < 0) { currentHP_ = 0; }
		// HP が maxHP を超えたら、maxHP にクランプ
		if (currentHP_ > maxHP_) { currentHP_ = maxHP_; }

		// 現在HPの割合を計算
		calcAndSetCurrentHPpersent();
	}

	int32_t Base_ActorData::damaged(const Base_ActorData& attackingActor, Base_ActorData& defencingActor) {
		int32_t actualyDamage = calcDecideActualyDamage(attackingActor, defencingActor);

		defencingActor.changeCurrentHP(-actualyDamage);

		return actualyDamage;
	}

	// damaged のヘルパー関数。
	// 計算式に基づき、実際のダメージを算出する。中で乱数使ってます
	// ret ... 実ダメージ
	// arg1 ... 攻撃した側
	// arg2 ... 攻撃された側
	// 計算式 ... 攻撃した側の攻撃力 = attackPower_ , 防御力 = 攻撃された側のdefencePower_
	// 基本ダメージ ... 攻撃力 * ((15/16)^防御力)
	// 最小ダメージ ... 基本ダメージ * ( 7/8 )
	// 最大ダメージ ... 基本ダメージ * ( 143/128 )
	// 実ダメージ ... 最小ダメージと最大ダメージの間で乱数を得た後、固定値の補正が入る
	int32_t Base_ActorData::calcDecideActualyDamage(const Base_ActorData& attackingActor, const Base_ActorData& defencingActor) {
		// 計算は float 型で行う
		float attackPower = static_cast<float>(attackingActor.getAttackPower());
		float defencePower = static_cast<float>(defencingActor.getDefencePower());

		// 基本ダメージ
		float baseAttack = attackPower * pow((15.0f / 16.0f), defencePower);
		// 最小ダメージ
		float smallerDamage = baseAttack * (7.0f / 8.0f);
		// 最大ダメージ
		float biggerDamage = baseAttack * (143.0f / 128.0f);

		// 乱数で値を取る
		float randomDamage = mtRandomRangeFloat(smallerDamage, biggerDamage);

		// 固定値によるダメージの増減計算
		float trueAttack = static_cast<float>(attackingActor.getTrueAttackPower());
		float trueDefence = static_cast<float>(defencingActor.getTrueDefencePower());

		// 固定ダメージ値の補正
		float fixedDamage = randomDamage + trueAttack - trueDefence;

		// int32_t型に変換
		int32_t retV = static_cast<int32_t>(fixedDamage);

		return retV;
	}

	void Base_ActorData::calcAndSetCurrentHPpersent() {
	
		if (maxHP_ > 0) { // ゼロ除算回避、maxHPがゼロより大きい場合に限り計算
			currentHPpersent_ =
				static_cast<float>(currentHP_) / static_cast<float>(maxHP_);
		}

		assert(maxHP_ > 0 && "最大HPがゼロ以下になっています。calcAndSetCurrentHPpersent()");

	}

}