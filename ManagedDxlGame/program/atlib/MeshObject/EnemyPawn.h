#pragma once
#include "Base_MultiMeshObject.h"
#include "../Object/EnemyData.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	class PlayerPawn;

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... エネミーを生成する位置
		EnemyPawn(const tnl::Vector2i& enemyPos);
		~EnemyPawn();

		// ゲッター
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }
		inline const bool getIsDead() const{ return isDead_; }
		inline const Shared<EnemyData> getEnemyData() const { return enemyData_; }

		// セッター
		inline void offFlagIsAlreadyTurn() { isAlreadyAction_ = false; isAlreadyMove_ = false; }
		
		// エネミーの毎フレームの行動
		bool enemyUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return true;
		}

		// 不透明な物体の描画
		void renderObjects(const Shared<Atl3DCamera> camera, float deltaTime) override;
		// 透明な物体の描画と位置補正
		void renderTransparentObject(const Shared<Atl3DCamera> camera, float deltaTime) override;

		// プレイヤーへの弱参照を設定
		void assignWeakDungeonScene(std::weak_ptr<DungeonScene> player);

	private:
		//-----------------------
		// 変数
		
		// メッシュの生成用
		// エネミーの大きさ
		const float ENEMY_SIZE_RADIUS = 200;
		// エネミーのYの高さ
		const float ENEMY_POS_Y = 400; // 

		// 移動用
		tnl::Vector3 moveTarget_{ 0,0,0 };
		const float MOVE_TIME = 0.3f; // 移動にかかる時間 ( 値が大きいほど移動に時間がかかる ) 
		const float MOVE_END_BORDER = 0.1f; // 目標地点と現在位置の差がこの値以下であれば、移動は終了したと判定される
		float moveLerpTimeCount_ = 0; // lerp移動用タイムカウンター
		float hoverSinTimer_ = 0; // sin関数で上下にふわふわさせる為のタイムカウンター
		float hoverSinAmplitude_ = 100.0f; // 上下運動の振幅

		// ターン制御用
		bool isAlreadyMove_ = false;
		bool isAlreadyAction_ = false;
		bool isDead_ = false;

		// ダンジョンシーンへの弱参照
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		// データ保持
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
		bool isNeighborPlayer();
		// プレイヤーと同じエリアにいるか
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