#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Utilities/Atl3DCamera.h"
#include "../MeshObject/MagicWand.h"
#include "../MeshObject/MagicBook.h"
#include "../MeshObject/ForwardArrow.h"

namespace atl {

	class PlayerPawn final : public std::enable_shared_from_this<PlayerPawn> {
	public:

		// ゲッター
		inline const Shared<Atl3DCamera> getPlayerCamera() const { return playerCamera_; }
		inline const tnl::Vector3& getPlayerPos() const { return player3Dpos_; }
		inline const tnl::Vector2i& getPlayer2Dpos() const { return player2Dpos_; }
		inline bool getIsAlreadyAction() const { return isAlreadyAction; }

		// セッター
		inline void setIsAlreadyAction(bool flag = false) { isAlreadyAction = flag; }
		void setPlayerAndCamera3Dpos(const tnl::Vector3& newPos);

		// 2D座標上の位置で Spawn する
		void playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos);
		
		void debug_displayPlayerParam(int drawPosX = 0, int drawPosY = 0);

		// シーケンス管理実行用
		bool playerUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return false;
		}

		// render系
		void render(float deltaTime) {
			forwardArrow_->renderObject(playerCamera_);
			playerHaveMagicWand_->renderObjects(playerCamera_);
			playerHaveMagicBook_->renderObject(playerCamera_);
		}

		void initialize() {
			playerHaveMagicWand_ = std::make_shared<MagicWand>(std::weak_ptr<PlayerPawn>(shared_from_this()));
			playerHaveMagicBook_ = std::make_shared<MagicBook>(std::weak_ptr<PlayerPawn>(shared_from_this()));
			forwardArrow_ = std::make_shared<ForwardArrow>(std::weak_ptr<PlayerPawn>(shared_from_this()));
			player3Dpos_ = playerCamera_->pos_;
		}

	private:
		// --------------------------------------------------
		enum class e_XZdir {
			NONE,
			Zplus,
			Zminus,
			Xplus,
			Xminus
		};
		
		// --------------------------------------------------
		// メンバー変数

		Shared<Atl3DCamera> playerCamera_ = std::make_shared<Atl3DCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);;
		Shared<MagicWand> playerHaveMagicWand_ = nullptr;
		Shared<MagicBook> playerHaveMagicBook_ = nullptr;
		Shared<ForwardArrow> forwardArrow_ = nullptr;

		tnl::Vector2i player2Dpos_{ 0,0 };
		tnl::Vector3 player3Dpos_{ 0,0,0 };

		// moveLerp用
		const float MOVE_LERP_SPEED = 3.0f;		// 1マス移動にかかる速度 ( 値が大きいほど、時間がかかる )
		const float PLAYER_HEAD_LINE = 500;		// プレイヤーのY高さ（カメラ・目線の高さ）
		float moveLerpTimeCount_ = 0;
		tnl::Vector3 moveTarget_{ 0,0,0 };
		
		// ターン制御用
		bool isAlreadyAction = false;

		// --------------------------------------------------
		// メソッド
		
		// arg ... 現在位置からの移動先
		bool checkIsCanMovePos(const tnl::Vector2i& moveToPos);
		bool moveLerp(float deltaTime);
		e_XZdir checkCurrentFowardDir();
		void calcDirAndMoveSeqChange();

		// --------------------------------------------------
		// シーケンス用
		
		SEQUENCE(PlayerPawn, &PlayerPawn::seqWaitKeyInput);
		
		bool seqWaitKeyInput(float deltaTime);
		bool seqMoveZplus(float deltaTime);
		bool seqMoveZminus(float deltaTime);
		bool seqMoveXplus(float deltaTime);
		bool seqMoveXminus(float deltaTime);
	};

}