#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "EnemyFactory.h"

namespace atl {

	class Atl3DCamera;

	class EnemyManager final {
	//------------------------------------------
	// シングルトン設計
	public:
		static EnemyManager* getEnemyManager();
		inline static void deleteEnemyManager() { delete getEnemyManager(); }
	private:
		EnemyManager() {};
		~EnemyManager() {};
	//------------------------------------------

	public:
		// ゲッター ( エネミーリスト )
		const std::list<Shared<Base_Enemy>>& getEnemyList() { return enemyList_; }

		// 新しいファクトリーサブクラスを設定 ( 次から作るエネミーのファクトリー )
		inline void setCurrentFactory(Shared<Base_EnemyFactory> newFactory) { currentFactory_ = newFactory; }

		// 現在のファクトリーサブクラスの createEnemy を実行し、listに加える
		void generateEnemy(const tnl::Vector2i& generate2Dpos);

		// 全エネミーの描画を実行
		void renderAllEnemy(Shared<Atl3DCamera> camera, float deltaTime);

		// 全エネミーの移動を実行
		// ret ... 全エネミーの移動が完了している => true, 完了していない => false
		bool moveAllEnemy(float deltaTime);

		// 全エネミーの行動を実行
		// ret ... 全エネミーの行動が完了している => true, 完了していない => false
		bool actionAllEnemy(float deltaTime);

		// 死亡フラグがオンのエネミーをリストから消去する
		void deadEnemyErase();

		// エネミーリストをクリアする
		inline void clearEnemyList() { enemyList_.clear(); }

		// エネミーのターン完了フラグをオフにする
		void allEnemyOffFlagIsAlreadyTurn();

		// 移動が完了していないエネミーのprocessを回す

	private:
		// 現在設定されているサブファクトリー
		Shared<Base_EnemyFactory> currentFactory_ = nullptr;

		// エネミーリスト。generateEnemy によって勝手に追加されていく
		std::list<Shared<Base_Enemy>> enemyList_{};

	};

}