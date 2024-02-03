#pragma once
#include "Base_MultiMeshObject.h"
#include "../Object/EnemyData.h"

namespace atl {

	class PlayerPawn;

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... エネミーを生成する位置
		// arg2 ... エネミーの大きさ
		// arg3 ... １セルの全長
		EnemyPawn(const tnl::Vector2i& enemyPos , const tnl::Vector3& enemySize);

		void adjustChildsMeshes() override;

		// ステート用 enum
		enum class e_EnemyMoveState {
			Wandering,	// 目的のない探索
			PlayerNeighboring, // その場で足踏み,プレイヤーの方を向く
		};

		// ゲッター
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }

		// セッター
		inline void setIsAlreadyTurn(bool flag = false) { isAlreadyAction_ = flag; isAlreadyMove_ = flag; }
		
		// エネミーの毎フレームの行動
		bool enemyUpdate(float deltaTime) {
			return seq_.update(deltaTime);
		}

		// プレイヤーへの弱参照を設定
		void assignWeakPlayer(std::weak_ptr<PlayerPawn> player);

	private:
		//----------------------
		// メソッド
		
		// ret ... 移動できる => true , 移動できない => false
		// arg ... 現在位置からの移動先
		bool assignMoveTarget(const tnl::Vector2i& moveToPos);
		// プレイヤーと前後左右で隣接しているか
		bool isNeighborPlayer();


		//-----------------------
		// メンバ変数
		
		bool isAlreadyMove_ = false;
		bool isAlreadyAction_ = false;
		tnl::Vector3 enemySize_{ 0,0,0 };

		// 移動用
		tnl::Vector3 moveTarget_{ 0,0,0 };
		const float MOVE_TIME = 1.0f; // 移動にかかる時間 ( 値が大きいほど移動に時間がかかる ) 
		float moveLerpTimeCount_ = 0;

		// 現在のステート
		e_EnemyMoveState turnState_ = e_EnemyMoveState::Wandering;

		// プレイヤーへの弱参照
		std::weak_ptr<PlayerPawn> weakPlayer;

		// データ保持
		Shared<EnemyData> enemyData_ = std::make_shared<EnemyData>();

		//-----------------------
		// シーケンス

		SEQUENCE(EnemyPawn, &EnemyPawn::seqStateTransition);
		bool seqStateTransition(float deltaTime);

		bool seqWandering(float deltaTime);
		bool seqPlayerNeighboring(float deltaTime);

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