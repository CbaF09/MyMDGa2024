#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Utilities/Atl3DCamera.h"
#include "../Scenes/DungeonScene.h"
#include "../Object/PlayerData.h"
#include "../Enemy/Enemy.h"
#include "MenuBook.h"

namespace atl {

	/// <summary>
	/// 
	/// 3D上でプレイヤーが操作するプレイヤーポーン
	/// プレイヤーの入力に対して反応するクラス
	/// 
	/// </summary>

	class DungeonScene;
	class MagicWand;
	class ForwardArrow;

	class PlayerPawn final : public std::enable_shared_from_this<PlayerPawn> {
	public:
		PlayerPawn();
		~PlayerPawn();

		// 遅延コンストラクタ。生成後に一回だけ呼ぶ事
		void initialize();

		// ゲッター ( カメラ )
		inline const Shared<Atl3DCamera> getPlayerCamera() const { return playerCamera_; }
		// ゲッター ( プレイヤーポーンが持っているプレイヤーのステータス情報 )
		inline const Shared<PlayerData> getPlayerData() const { return playerData_; }
		// ゲッター ( 既に行動終了したか )
		inline bool getIsAlreadyTurn() const { return isAlreadyTurn_; }
		// ゲッター ( メニューを開いている状態か )
		inline bool getIsMenuOpen() const { return playerHaveMenuBook_->isOpenMenu(); }
		// ゲッター ( 3D上の位置 )
		inline const tnl::Vector3& getPlayerPos() const { return player3Dpos_; }
		// ゲッター ( 2D上の座標 )
		inline const tnl::Vector2i& getPlayer2Dpos() const { return player2Dpos_; }

		// セッター ( 行動したフラグをオフにする )
		inline void offFlagIsAlreadyTurn() { isAlreadyTurn_ = false; }
		// セッター ( 行動したフラグをオンにするにする )
		inline void onFlagIsAlreadyTurn() { isAlreadyTurn_ = true; }

		// メニューを開く ( 本を正面の位置に ) 
		void openMenuBook();
		// メニューを閉じる ( 本を元の位置に ) 
		void closeMenuBook();

		// スポーン処理を行う為の関数
		// arg ... スポーンする2D上の座標
		void playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos);

		// シーケンス実行用
		inline bool playerUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return false;
		}

		// 移動アクションをスタート
		// ret ... 移動先が移動可能 => true , 移動不可能 => false
		bool startMove();
		// 攻撃アクションをスタート
		void startAttack();

		// render系
		void render(float deltaTime);

		// --------------------------------------------------
		// デバッグ用
		// プレイヤーポーンの情報を色々出す
		void debug_displayPlayerParam(int drawPosX = 0, int drawPosY = 0);

	private:
		// --------------------------------------------------
		// メンバー変数
		
		// プレイヤーの2D上の座標
		tnl::Vector2i player2Dpos_{ 0,0 };
		// プレイヤーの3D上の位置
		tnl::Vector3 player3Dpos_{ 0,0,0 };
		// プレイヤーのY高さ（カメラ・目線の高さ, 0 で地面と同じ）
		const float PLAYER_HEAD_LINE = 500;

		// seqMoveLerp用
 		// タイマー
		float moveLerpTimeCount_ = 0;
		// 目標地点
		tnl::Vector3 moveTarget_{ 0,0,0 };
		// 1マス移動にかかる速度 ( 値が大きいほど、時間がかかる )
		const float MOVE_TIME = 0.35f;		
		// 目標地点と現在位置の差がこの値以下であれば、移動は終了したと判定される
		const float MOVE_END_BORDER = 0.1f; 


		// 既に行動したか
		bool isAlreadyTurn_ = false;
		// 停止時間用タイマー
		float totalDeltaTimer_ = 0.0f; 
		// 実際の停止時間設定用
		float waitTime_ = 0.0f; 
		// 攻撃した時の停止時間
		const float ATTACK_TIME = 0.5f;	
		// 攻撃を空振りした時の停止時間
		const float ATTACK_MISS_TIME = 0.05f; 

		// カメラ
		Shared<Atl3DCamera> playerCamera_ = std::make_shared<Atl3DCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
		// 右手に持っている杖
		Shared<MagicWand> playerHaveMagicWand_ = nullptr;
		// 左手の本
		Shared<MenuBook> playerHaveMenuBook_ = nullptr;
		// 向いている方向を分かりやすくする為の枠
		Shared<ForwardArrow> forwardArrow_ = nullptr;
		// 攻撃時のパーティクル ( 爆発 )
		Shared<dxe::Particle> explosion_ = std::make_shared<dxe::Particle>("graphics/particle/explosion.bin");

		// ステータス用
		Shared<PlayerData> playerData_ = std::make_shared<PlayerData>();

		// --------------------------------------------------
		// メソッド

		// 移動できるか判定
		bool isCanMovePos(const tnl::Vector2i& moveToPos);
		// 移動先を指定 ( moveLerp用 ) 
		void setMoveTarget(const tnl::Vector2i& moveToPos);
		// 攻撃命中時の演出
		void attackHitEffectAndLog(const Shared<atl::Base_Enemy> enemy);


		// --------------------------------------------------
		// シーケンス用

		SEQUENCE(PlayerPawn, &PlayerPawn::seqWait);

		// 待機状態
		bool seqWait(float deltaTime);

		// lerpで移動する
		bool seqMoveLerp(float deltaTime);

		// 攻撃した時の待ち時間 ( 空振り時とヒット時で違う )
		bool seqActionAttackDelay(float deltaTime);

	};

}