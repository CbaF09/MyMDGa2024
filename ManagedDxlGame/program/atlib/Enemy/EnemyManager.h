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
		// 新しいファクトリーサブクラスを設定 ( 次から作るエネミーのファクトリー )
		inline void setCurrentFactory(Shared<Base_EnemyFactory> newFactory) { currentFactory_ = newFactory; }

		// 現在のファクトリーサブクラスの createEnemy を実行し、listに加える
		void generateEnemy();

		// 全エネミーの毎フレーム処理を実行
		void processAllEnemy(float deltaTime);

		// 全エネミーの描画を実行
		void renderAllEnemy(Shared<Atl3DCamera> camera, float deltaTime);

	private:
		Shared<Base_EnemyFactory> currentFactory_ = nullptr;
		std::list<Shared<Base_Enemy>> enemyList_{};

	};

}