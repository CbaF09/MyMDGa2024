#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	///
	///  プレイヤー ・ エネミーに共通するデータと関数を保持
	/// 
	/// </summary>

	class Base_ActorData {
	public:
		virtual ~Base_ActorData() {};

		// ゲッター
		inline const int32_t getMaxHP() const { return maxHP_; }
		inline const int32_t getCurrentHP() const { return currentHP_; }
		inline const float getCurrentHPpersent() const { return currentHPpersent_; }
		inline const int32_t getAttackPower() const { return attackPower_; }
		inline const int32_t getDefencePower() const { return defencePower_; }
		inline const int32_t getTrueAttackPower() const { return trueAttackPower_; }
		inline const int32_t getTrueDefencePower() const { return trueDefencePower_; }

		// セッター
		inline void setMaxHP(int32_t maxHP) { maxHP_ = maxHP; calcAndSetCurrentHPpersent(); }
		inline void setCurrentHP(int32_t currentHP) { currentHP_ = currentHP; calcAndSetCurrentHPpersent(); }
		inline void setAttackPower(int32_t attackPower) { attackPower_ = attackPower; }
		inline void setDefencePower(int32_t defencePower) { defencePower_ = defencePower; }
		inline void setTrueAttackPower(int32_t trueAttackPower) { trueAttackPower_ = trueAttackPower; }
		inline void setTrueDefencePower(int32_t trueDefencePower) { trueDefencePower_ = trueDefencePower; }

		// 引数の値だけHPを増減する
		void changeCurrentHP(int32_t addsumValue);

		// 通常攻撃によるダメージが発生した時呼び出す。計算式を適用して防御側のHPを減らす。
		// 呼び出し元インスタンスが必要無い ( 攻撃側インスタンスも防御側インスタンスも引数で渡す為 ) ので static で扱えるように
		// ret ... 実際に与えたダメージ
		// arg1 ... 攻撃した側
		// arg2 ... 攻撃された側
		static int32_t damaged(const Base_ActorData& attackingActor, Base_ActorData& defencingActor);

		// damaged のヘルパー関数
		// 計算式に基づき、実際のダメージを算出する。中で乱数使ってます
		// ret ... 実ダメージ
		// arg1 ... 攻撃した側
		// arg2 ... 攻撃された側
		// 計算式 ... 攻撃した側の攻撃力 = attackPower_ , 防御力 = 攻撃された側のdefencePower_
		// 基本ダメージ ... 攻撃力 * ((15/16)^防御力)
		// 最小ダメージ ... 基本ダメージ * ( 7/8 )
		// 最大ダメージ ... 基本ダメージ * ( 143/128 )
		// 実ダメージ ... 最小ダメージと最大ダメージの間で乱数を得た後、固定値の補正が入る
		static int32_t calcDecideActualyDamage(const Base_ActorData& attackingActor, const Base_ActorData& attakedActor);

		// HPがゼロ => true , ゼロではない => false
		bool isZeroHP();

		// 現在HPと最大HPの割合を計算する
		void calcAndSetCurrentHPpersent();

	private:
		// 最大HP
		int32_t maxHP_ = 0;
		// 現在HP
		int32_t currentHP_ = 0;
		// 現在のHP割合
		float currentHPpersent_ = 0.0f;

		// 攻撃力
		int32_t attackPower_ = 0;
		// 防御力
		int32_t defencePower_ = 0;

		// 固定攻撃ダメージ
		int32_t trueAttackPower_ = 0;
		// 固定防御力
		int32_t trueDefencePower_ = 0;
	};

}