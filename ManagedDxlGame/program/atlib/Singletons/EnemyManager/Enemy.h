#pragma once
#include "../../../dxlib_ext/dxlib_ext.h"
#include "../../Scenes/DungeonScene.h"
#include "../../Object/EnemyData.h"

namespace atl {

	/// <summary>
	/// 
	/// 全てのエネミーの基底クラス
	/// 
	/// protected で、シーケンサーとシーケンス用関数を継承する。
	/// 継承先でシーケンス用関数をオーバーライドする事で、モンスター固有の動きを作れる
	/// 継承クラスを増やしたら、1 対 1 の関係にあるファクトリークラスを作ること => EnemyFactory 参照
	/// 
	/// <summary>
	/// 

	enum class e_EnemyName {
		NONE = 0,
		BLUE_SLIME = 1,
		GREEN_SLIME = 2,
		RED_SLIME = 3,
		ENEMY_MAX,
	};

	class Base_Enemy : public Base_MeshObject {
	public:
		virtual ~Base_Enemy() {};

		// 初期化
		virtual void initialize() = 0;

		// 計算・処理用関数。主にシーケンスのアップデートを想定
		virtual void process(float deltaTime) { sequenceUpdate(deltaTime); };

		// ゲッター ( 既に移動を終えたか )
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		// ゲッター ( 既に行動を終えたか )
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }
		// ゲッター ( 死亡演出が終わり、死亡したか )
		inline const bool getIsDead() const { return isDead_; }
		// ゲッター ( エネミーポーンが持つエネミーデータ )
		inline const Shared<EnemyData> getEnemyData() const { return enemyData_; }

		// セッター ( エネミーデータ を ID から設定 )
		inline void setEnemyData(int32_t enemyID) { enemyData_ = std::make_shared<EnemyData>(enemyID); }

		// 行動フラグをオフに
		inline void offFlagIsAlreadyTurn() {
			isAlreadyAction_ = false;
			isAlreadyMove_ = false;
		}

		// エネミーデータを元に、メッシュ/テクスチャ/マテリアルをまとめて設定する
		void setEnemyVisual(const Shared<EnemyData> enemyData);

	private:
		// 移動用
		// 移動先
		tnl::Vector3 moveTarget_{ 0,0,0 };
		// 移動にかかる時間 ( 値が大きいほど移動に時間がかかる ) 
		const float MOVE_TIME = 0.3f;
		// 目標地点と現在位置の差がこの値以下であれば、移動は終了したと判定される
		const float MOVE_END_BORDER = 0.1f;
		// lerp移動用タイムカウンター
		float moveLerpTimeCount_ = 0;

		// ターン制御用
		// 既に移動を終えたか
		bool isAlreadyMove_ = false;
		// 既に行動を終えたか ( 攻撃など )
		bool isAlreadyAction_ = false;
		// 死亡演出が終わって、死亡したか
		bool isDead_ = false;

		// エネミーはエネミーデータを一つ保持する
		Shared<EnemyData> enemyData_ = nullptr;

		// シーケンサー
		SEQUENCE(Base_Enemy, &Base_Enemy::seqTransition);

	protected:
		// シーケンス用関数

		// シーケンスアップデート用 ( シーケンス変数を private に隠蔽しているので )
		inline void sequenceUpdate(float deltaTime) { seq_.update(deltaTime); }

		// 移動できるかどうかの判定
		// ret ... 移動できる => true , 移動できない => false
		// arg ... 現在位置からの移動先
		bool isCanMove(const tnl::Vector2i& moveToPos) const;

		// 移動先の設定 ( シーケンスのseqMoveToTargetで移動する )
		void setMoveTarget(const tnl::Vector2i& moveToPos);

		// プレイヤーと前後左右で隣接しているか
		// ret ... 隣接している => true, 隣接していない => false
		bool isNeighborPlayer() const;

		// プレイヤーと同じエリアにいるか
		// ret ... プレイヤーと同じエリアにいる => true, いない => false
		bool isSameAreaPlayer() const;

		// シーケンスの遷移を確認する
		virtual bool seqTransition(float deltaTime);

		// ランダム移動
		virtual bool seqWandering(float deltaTime);
		// プレイヤーに向かって移動
		virtual bool seqChasePlayer(float deltaTime);
		// プレイヤーに隣接している時
		virtual bool seqPlayerNeighboring(float deltaTime);
		// 死亡時の演出
		virtual bool seqDeading(float deltaTime);

		// 移動 ( 隣一マスへ )
		virtual bool seqMoveToTarget(float deltaTime);
		// 移動 ( 足踏み )
		virtual bool seqMoveNone(float deltaTime);

		// 行動 ( 攻撃 )
		virtual bool seqActionAttack(float deltaTime);
		// 行動 ( 無し )
		virtual bool seqActionNone(float deltaTime);


	};

	/// <summary>
	/// 
	/// 青スライム
	/// 
	/// </summary>
	class BlueSlime final : public Base_Enemy {
	public:
		void initialize() override;

		// ふわふわした動きをさせるため、renderObject をオーバーライド
		void renderObject(Shared<Atl3DCamera> camera, float deltaTime) override;

	private:
		// 正弦波でふわふわさせる為のタイマー
		float totalDeltaTimer_ = 0;
		// sin の振れ幅
		const float AMPLITUDE = 64;
		// sin の周波数
		const float FREQUENCY = 2;

		// ふわふわした時に床に突き抜けるのを防ぐ為の計算用 Y座標
		const float INIT_POS_Y = AMPLITUDE;
	};

	/// <summary>
	/// 
	/// 緑スライム
	/// 
	/// </summary>
	class GreenSlime final : public Base_Enemy {
	public:
		void initialize() override;

		// ふわふわした動きをさせるため、renderObject をオーバーライド
		void renderObject(Shared<Atl3DCamera> camera, float deltaTime) override;

	private:
		// 正弦波でふわふわさせる為のタイマー
		float totalDeltaTimer_ = 0;
		// sin の振れ幅
		const float AMPLITUDE = 64;
		// sin の周波数
		const float FREQUENCY = 2.5f;

		// ふわふわした時に床に突き抜けるのを防ぐ為の計算用 Y座標
		const float INIT_POS_Y = AMPLITUDE;
	};


	/// <summary>
	/// 
	/// 赤スライム
	/// 
	/// </summary>
	class RedSlime final : public Base_Enemy {
	public:
		void initialize() override;

		// ふわふわした動きをさせるため、renderObject をオーバーライド
		void renderObject(Shared<Atl3DCamera> camera, float deltaTime) override;

	private:
		// 正弦波でふわふわさせる為のタイマー
		float totalDeltaTimer_ = 0;
		// sin の振れ幅
		const float AMPLITUDE = 64;
		// sin の周波数
		const float FREQUENCY = 3;

		// ふわふわした時に床に突き抜けるのを防ぐ為の計算用 Y座標
		const float INIT_POS_Y = AMPLITUDE;
	};

}