#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... エネミーを生成する位置
		// arg2 ... エネミーの大きさ
		// arg3 ... １セルの全長
		EnemyPawn(const tnl::Vector3& enemyPos , const tnl::Vector3& enemySize, float CellLength);

		void adjustmentChildMeshes() override;

		// ステートマシン用 enum
		enum class e_EnemyState {
			Wandering,	// 目的のない探索
			Searching,	// 目的のある探索
			Chasing,	// プレイヤーを追跡中
		};

		// ゲッター
		inline const e_EnemyState& getCurrentEnemyState() const { return currentState_; }
		
		// セッター
		inline void setEnemySpeed(float enemyMoveSpeed) { enemyMoveSpeed_ = enemyMoveSpeed; }
		
		// 目標
		void registerTarget(const tnl::Vector3& target);

		// エネミーの毎フレームの行動
		void enemyUpdate(float deltaTime);

	private:

		//----------------------
		// メソッド
		
		inline void setNeedMoveAmount(float needMoveAmount) { needMoveAmount_ = needMoveAmount; }

		//-----------------------
		// メンバ変数

		tnl::Vector3 enemySize_{ 0,0,0 };

		// 移動用
		tnl::Vector3 targetPos_ = { 0,0,0 };		// 目的地
		tnl::Quaternion targetRot_;
		float enemyMoveSpeed_ = 1.0f;			// 移動速度
		float enemyRotateSpeed_ = 1.0f;			// 回転速度
		float oneCellLength_ = 0.0f;			// １マスの大きさ
		float needMoveAmount_ = 0.0f;			// 残り移動量カウンター

		// 現在のステート
		e_EnemyState currentState_ = e_EnemyState::Wandering;

		//-----------------------
		// シーケンス

		SEQUENCE(EnemyPawn, &EnemyPawn::seqCheckCurrentState);
		bool seqInit(float deltaTime) {/*未実装*/ };
		bool seqCheckCurrentState(float deltaTime);
		bool seqWandering(float deltaTime);
		bool seqSearching(float deltaTime) {/*未実装*/ };
		bool seqChasing(float deltaTime) {/*未実装*/ };
		bool seqRotate(float deltaTime);
		bool seqMove(float deltaTime);

	};

}