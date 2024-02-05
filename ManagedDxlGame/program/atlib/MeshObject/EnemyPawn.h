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

		void adjustChildsMeshes() override;

		// ステート用 enum
		enum class e_EnemyState {
			Wandering,			// 目的のない探索
			ChasePlayer,		// プレイヤーに向かう
			PlayerNeighboring,	// その場で足踏み,プレイヤーの方を向く
			Deading,			// 死亡中 ( 徐々に消えていくなどの処理 )
			Dead,				// 死亡 ( Deading から遷移 )
		};

		// ゲッター
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }
		inline const e_EnemyState& getCurrentState() const{ return currentState_; }
		inline const Shared<EnemyData> getEnemyData() const { return enemyData_; }

		// セッター
		inline void offFlagIsAlreadyTurn() { isAlreadyAction_ = false; isAlreadyMove_ = false; }
		
		// エネミーの毎フレームの行動
		bool enemyUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return true;
		}

		// プレイヤーへの弱参照を設定
		void assignWeakDungeonScene(std::weak_ptr<DungeonScene> player);

	private:
		//----------------------
		// メソッド
		
		// ret ... 移動できる => true , 移動できない => false
		// arg ... 現在位置からの移動先
		bool isCanMovePos(const tnl::Vector2i& moveToPos);
		void setMoveTarget(const tnl::Vector2i& moveToPos);
		// プレイヤーと前後左右で隣接しているか
		bool isNeighborPlayer();


		//-----------------------
		// メンバ変数
		
		// メッシュの生成用
		const tnl::Vector3 ENEMY_SIZE{ 200,200,200 };

		// 移動用
		tnl::Vector3 moveTarget_{ 0,0,0 };
		const float MOVE_TIME = 1.0f; // 移動にかかる時間 ( 値が大きいほど移動に時間がかかる ) 
		const float MOVE_END_BORDER = 0.1f; // 目標地点と現在位置の差がこの値以下であれば、移動は終了したと判定される
		float moveLerpTimeCount_ = 0;

		// ターン制御用
		e_EnemyState currentState_ = e_EnemyState::Wandering;
		bool isAlreadyMove_ = false;
		bool isAlreadyAction_ = false;

		// ダンジョンシーンへの弱参照
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		// データ保持
		Shared<EnemyData> enemyData_ = std::make_shared<EnemyData>();

		//-----------------------
		// シーケンス

		SEQUENCE(EnemyPawn, &EnemyPawn::seqStateTransition);
		bool seqStateTransition(float deltaTime);

		bool seqWandering(float deltaTime);
		bool seqPlayerNeighboring(float deltaTime);
		bool seqDeading(float deltaTime);
		bool seqDead(float deltaTime);

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