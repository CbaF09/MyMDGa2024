#pragma once
#include "Base_MagicRune.h"

namespace atl {

	/// <summary>
	/// 
	/// Base_MagicRuneを継承した、ルーンの具象クラス ( オブザーバーパターンのサブジェクトの具象クラス )
	/// 
	/// ルーンの増やし方
	/// 1. CSVに、増やしたいルーンの情報を入力
	/// 2. e_RuneIDに、増やしたルーン用の列挙子を追加
	/// 3. 具象クラスを作り、コンストラクタに setRuneData を記述する。
	/// 4. onNotifyに、処理を記述
	/// 
	/// </summary>

	/// <summary>
	/// 癒しのルーン
	/// </summary>
	class HealRune : public Base_MagicRune {
	public:
		HealRune() { setRuneData(e_RuneID::HealRune); }
		void onNotify(e_EventType eventType,DungeonScene& dungeonScene) override;
	};

	/// <summary>
	/// 炎のルーン
	/// </summary>
	class FireRune : public Base_MagicRune {
	public:
		FireRune() { setRuneData(e_RuneID::FireRune); }
		// 処理未記述
		void onNotify(e_EventType eventType, DungeonScene& dungeonScene) override {};
	};

}