#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Utilities/Atl3DCamera.h"

namespace atl {

	class PlayerPawn final {
	public:
		PlayerPawn();

		// ゲッター
		inline const Shared<Atl3DCamera> getPlayerCamera() const { return playerCamera_; }
		inline const tnl::Vector3& getPlayerPos() const { return playerPos_; }
		
		void setPlayerAndCameraPos(const tnl::Vector3& newPos);

		// マウスでカメラを操作する
		void cameraControl(float controlSpeed);
		
		void debug_displayPlayerParam(int drawPosX = 0, int drawPosY = 0);

		void playerUpdate(float deltaTime) {
			seq_.update(deltaTime);
		}

	private:
		// --------------------------------------------------
		// メンバー変数

		const float CAMERA_ROT_SPEED_ = 0.3f;
		const float MOVE_LERP_TIME_ = 0.5f;
		float moveLerpTimeCount_ = 0;
		tnl::Vector3 playerPos_{ 0,0,0 };
		tnl::Vector3 moveTarget_{ 0,0,0 };
		Shared<Atl3DCamera> playerCamera_ = nullptr;

		// --------------------------------------------------
		// シーケンス用
		
		SEQUENCE(PlayerPawn, &PlayerPawn::seqIdle);

		const tnl::Vector3& calcMoveDir();

		bool seqIdle(float deltaTime);
		bool seqCalcMoveDir(float deltaTime);
		bool seqMoveZplus(float deltaTime);
		bool seqMoveZminus(float deltaTime);
		bool seqMoveXplus(float deltaTime);
		bool seqMoveXminus(float deltaTime);
	};

}