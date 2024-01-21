#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... エネミーを生成する位置
		// arg2 ... エネミーの大きさ
		// arg3 ... １セルの全長
		EnemyPawn(const tnl::Vector2i& enemyPos , const tnl::Vector3& enemySize);

		void adjustmentChildMeshes() override;

		// ステートマシン用 enum
		enum class e_EnemyState {
			Wandering,	// 目的のない探索
			Searching,	// 目的のある探索
			Chasing,	// プレイヤーを追跡中
		};

		// ゲッター
		inline const e_EnemyState& getCurrentEnemyState() const { return currentState_; }
		inline const bool getIsAlreadyAction() const { return isAlreadyAction; }

		// セッター
		inline void setEnemySpeed(float enemyMoveSpeed) { enemyMoveSpeed_ = enemyMoveSpeed; }
		
		// エネミーの毎フレームの行動
		bool enemyUpdate(float deltaTime) {
			return seq_.update(deltaTime);
		}

	private:

		//----------------------
		// メソッド
		// arg ... 現在位置からの移動量
		bool checkIsCanMovePos(const tnl::Vector2i& moveToPos);
		bool moveLerp(float deltaTime);

		//-----------------------
		// メンバ変数
		
		bool isAlreadyAction = false;
		tnl::Vector3 enemySize_{ 0,0,0 };

		// 移動用
		tnl::Vector3 moveTarget_{ 0,0,0 };
		const float MOVE_LERP_TIME_ = 0.5f;
		float moveLerpTimeCount_ = 0;
		float enemyMoveSpeed_ = 1.0f;			// 移動速度
		float enemyRotateSpeed_ = 1.0f;			// 回転速度
		float needMoveAmount_ = 0.0f;			// 残り移動量カウンター

		// 現在のステート
		e_EnemyState currentState_ = e_EnemyState::Wandering;

		//-----------------------
		// シーケンス

		SEQUENCE(EnemyPawn, &EnemyPawn::seqCheckCurrentState);
		bool seqCheckCurrentState(float deltaTime);
		bool seqWandering(float deltaTime);
		bool seqSearching(float deltaTime) {/*未実装*/ };
		bool seqChasing(float deltaTime) {/*未実装*/ };

		bool seqMoveZplus(float deltaTime);
		bool seqMoveZminus(float deltaTime);
		bool seqMoveXplus(float deltaTime);
		bool seqMoveXminus(float deltaTime);

	};

}