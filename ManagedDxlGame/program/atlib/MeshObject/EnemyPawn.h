#pragma once
#include "Base_MultiMeshObject.h"
#include "../Object/EnemyData.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	/// <summary>
	/// 
	/// 3D上でのエネミー
	/// 見た目と動きを制御
	/// 
	/// </summary>

	class PlayerPawn;

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... エネミーを生成する位置
		EnemyPawn(const tnl::Vector2i& enemyPos);
		~EnemyPawn();

		// ゲッター ( 既に移動を終えたか )
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		// ゲッター ( 既に行動を終えたか )
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }
		// ゲッター ( 死亡演出が終わり、死亡したか )
		inline const bool getIsDead() const{ return isDead_; }
		// ゲッター ( エネミーポーンが持つエネミーデータ )
		inline const Shared<EnemyData> getEnemyData() const { return enemyData_; }

		// 行動フラグをオフに
		inline void offFlagIsAlreadyTurn() { 
			isAlreadyAction_ = false; 
			isAlreadyMove_ = false; 
		}
		
		// エネミーのシーケンスアップデート
		inline bool enemyUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return true;
		}

		// 親メッシュが透明なので、親メッシュの描画と子メッシュの描画を分ける
		// 不透明な物体の描画
		void renderObjects(const Shared<Atl3DCamera> camera, float deltaTime) override;
		// 透明な物体の描画と位置補正
		void renderTransparentObject(const Shared<Atl3DCamera> camera, float deltaTime) override;

		// ダンジョンシーンへの弱参照を設定
		void assignWeakDungeonScene(std::weak_ptr<DungeonScene> dungeonScene);

	private:
		//-----------------------
		// 変数
		
		// メッシュの生成用
		// エネミーの大きさ
		const float ENEMY_SIZE_RADIUS = 200;
		// エネミーのYの高さ
		const float ENEMY_POS_Y = 400;

		// 移動用
		// 移動先
		tnl::Vector3 moveTarget_{ 0,0,0 };
		// 移動にかかる時間 ( 値が大きいほど移動に時間がかかる ) 
		const float MOVE_TIME = 0.3f; 
		// 目標地点と現在位置の差がこの値以下であれば、移動は終了したと判定される
		const float MOVE_END_BORDER = 0.1f; 
		// lerp移動用タイムカウンター
		float moveLerpTimeCount_ = 0; 
		// sin関数で上下にふわふわさせる為のタイムカウンター
		float hoverSinTimer_ = 0; 
		// 上下運動の振幅
		float hoverSinAmplitude_ = 100.0f; 

		// ターン制御用
		// 既に移動を終えたか
		bool isAlreadyMove_ = false;
		// 既に行動を終えたか ( 攻撃など )
		bool isAlreadyAction_ = false;
		// 死亡演出が終わって、死亡したか
		bool isDead_ = false;

		// ダンジョンシーンへの弱参照
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		// エネミーはエネミーデータを一つ保持する
		Shared<EnemyData> enemyData_ = std::make_shared<EnemyData>();

		//----------------------
		// メソッド

		// メッシュ群の位置を調整する。renderObjectsから呼び出される
		void adjustChildsMeshes(float deltaTime) override;

		// 移動できるかどうかの判定
		// ret ... 移動できる => true , 移動できない => false
		// arg ... 現在位置からの移動先
		bool isCanMove(const tnl::Vector2i& moveToPos);

		// 移動先の設定 ( シーケンスのseqMoveToTargetで移動する )
		void setMoveTarget(const tnl::Vector2i& moveToPos);
		
		// プレイヤーと前後左右で隣接しているか
		// ret ... 隣接している => true, 隣接していない => false
		bool isNeighborPlayer();
		
		// プレイヤーと同じエリアにいるか
		// ret ... プレイヤーと同じエリアにいる => true, いない => false
		bool isSameAreaPlayer();

		//-----------------------
		// シーケンス

		SEQUENCE(EnemyPawn, &EnemyPawn::seqStateTransition);
		bool seqStateTransition(float deltaTime);

		// ランダム移動
		bool seqWandering(float deltaTime);
		// プレイヤーに向かって移動
		bool seqChasePlayer(float deltaTime);
		// プレイヤーに隣接している時
		bool seqPlayerNeighboring(float deltaTime);
		// 死亡時の演出
		bool seqDeading(float deltaTime);

		// 移動 ( 隣一マスへ )
		bool seqMoveToTarget(float deltaTime);
		// 移動 ( 足踏み )
		bool seqMoveNone(float deltaTime);

		// 行動 ( 攻撃 )
		bool seqActionAttack(float deltaTime);
		// 行動 ( 無し )
		bool seqActionNone(float deltaTime);

	};

}