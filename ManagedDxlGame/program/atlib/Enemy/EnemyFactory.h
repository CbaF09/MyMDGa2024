#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Enemy.h"

namespace atl {

	/// <summary>
	/// 
	/// 全てのエネミーファクトリーの基底となるクラス
	/// 継承先で、各エネミー専用サブファクトリーに具象化される
	/// サブファクトリーは、各エネミーと 1 対 1 の関係
	/// そのモンスター専用のスポーン時処理 ( 音とか？ ) などは、サブファクトリークラスに記述
	/// 
	class Base_EnemyFactory {
	public:
		// 基底クラスなのでデストラクタを virtual
		virtual ~Base_EnemyFactory() {};

		// エネミーを生成する純粋仮想関数。具象クラス ( 各ファクトリーサブクラス ) にて実装される
		virtual Shared<Base_Enemy> createEnemy() = 0;

	};

	/// <summary>
	/// 
	/// スライムのファクトリーサブクラス
	/// 
	class SlimeFactory : public Base_EnemyFactory {
		Shared<Base_Enemy> createEnemy() override;
	};

}