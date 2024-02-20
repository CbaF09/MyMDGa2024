#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Enemy.h"

namespace atl {

	/// <summary>
	/// 
	/// 全てのエネミーファクトリーの基底となるクラス
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
	/// 丸い敵のファクトリーサブクラス
	/// 
	class SlimeFactory : public Base_EnemyFactory {
		Shared<Base_Enemy> createEnemy() override;
	};

}