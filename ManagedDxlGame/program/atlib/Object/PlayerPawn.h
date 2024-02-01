#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Utilities/Atl3DCamera.h"
#include "../MeshObject/MagicWand.h"
#include "../MeshObject/MagicBook.h"

namespace atl {

	class PlayerPawn final : public std::enable_shared_from_this<PlayerPawn> {
	public:
		PlayerPawn();

		// ゲッター
		inline const Shared<Atl3DCamera> getPlayerCamera() const { return playerCamera_; }
		inline const tnl::Vector3& getPlayerPos() const { return player3Dpos_; }
		
		void setPlayerAndCamera3Dpos(const tnl::Vector3& newPos);

		// マウスでカメラを操作する
		void cameraControl();

		// 2D座標上の位置で Spawn する
		void playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos);
		
		void debug_displayPlayerParam(int drawPosX = 0, int drawPosY = 0);

		// シーケンス管理実行用
		bool playerUpdate(float deltaTime) {
			bool retV = seq_.update(deltaTime);
			return retV;
		}

		// render系
		void render(float deltaTime) {
			playerHaveMagicWand->renderObjects(playerCamera_);
			playerHaveMagicBook->renderObject(playerCamera_);
		}

		void initialize() {
			playerHaveMagicWand = std::make_shared<MagicWand>(std::weak_ptr<PlayerPawn>(shared_from_this()));
			playerHaveMagicBook = std::make_shared<MagicBook>(std::weak_ptr<PlayerPawn>(shared_from_this()));
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

		const float CAMERA_ROT_SPEED = 0.3f;	// カメラ回転速度
		const float MOVE_LERP_SPEED = 3.0f;		// 1マス移動にかかる速度 ( 値が大きいほど、時間がかかる )
		const float PLAYER_HEAD_LINE = 500;		// プレイヤーのY高さ（カメラ・目線の高さ）
		float moveLerpTimeCount_ = 0;
		tnl::Vector2i player2Dpos_{ 0,0 };
		tnl::Vector3 player3Dpos_{ 0,0,0 };
		tnl::Vector3 moveTarget_{ 0,0,0 };
		
		Shared<Atl3DCamera> playerCamera_ = nullptr;
		Shared<MagicWand> playerHaveMagicWand = nullptr;
		Shared<MagicBook> playerHaveMagicBook = nullptr;

		// --------------------------------------------------
		// メソッド
		
		// arg ... 現在位置からの移動量
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