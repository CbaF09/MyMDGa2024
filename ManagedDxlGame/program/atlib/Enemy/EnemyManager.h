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
		EnemyManager() {
			// 仮でランダムスポーンが動作するように、スポーン候補リストを手入力
			// 将来的には、階層ごとに変化するようにする
			addSpawnableEnemyFactory(std::make_shared<BlueSlimeFactory>());
			addSpawnableEnemyFactory(std::make_shared<GreenSlimeFactory>());
			addSpawnableEnemyFactory(std::make_shared<RedSlimeFactory>());
		};
		~EnemyManager() {};
	//------------------------------------------

	public:
		// ゲッター ( エネミーリスト )
		const std::list<Shared<Base_Enemy>>& getEnemyList() { return enemyList_; }

		// 新しいエネミーサブファクトリークラスを直接設定
		inline void setEnemyFactory(Shared<Base_EnemyFactory> newFactory) { currentFactory_ = newFactory; }

		// ランダムに新しいエネミーサブファクトリークラスを設定
		void setRandomEnemyFactory();

		// ランダムにエネミーサブファクトリーが設定される際の候補を追加
		inline void addSpawnableEnemyFactory(Shared<Base_EnemyFactory> spawnableFactory) { spawnableEnemyFactoryList_.emplace_back(spawnableFactory); }

		// スポーン候補のエネミーサブファクトリーリストをクリアする
		inline void clearSpawnableEnemyFactoryList() { spawnableEnemyFactoryList_.clear(); }

		// 現在のファクトリーサブクラスの createEnemy を実行し、listに加える
		// arg ... スポーンするエネミーの2D座標上の位置
		void spawnEnemy(const tnl::Vector2i& spawn2Dpos);

		// スポーンエネミーをランダムに設定してから、スポーン処理を実行
		void spawnRandomEnemy(const tnl::Vector2i spawn2Dpos);

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

	private:
		// 現在設定されているサブファクトリー
		Shared<Base_EnemyFactory> currentFactory_ = nullptr;

		// 設定候補のサブファクトリーのリスト。addEnemyFactoryによって追加されていく
		std::vector<Shared<Base_EnemyFactory>> spawnableEnemyFactoryList_{};

		// 現在存在しているエネミーリスト
		std::list<Shared<Base_Enemy>> enemyList_{};

	};

}