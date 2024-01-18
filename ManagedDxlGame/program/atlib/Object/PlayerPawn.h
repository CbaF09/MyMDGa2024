#pragma once
#include "../../atlib/Utilities/Atl3DCamera.h"

namespace atl {

	class PlayerPawn final {
	public:
		PlayerPawn();
		PlayerPawn(int x , int z);

		// ゲッター
		inline const Shared<Atl3DCamera> getPlayerCamera() const { return playerCamera_; }
		inline const tnl::Vector3& getPlayerPos() const { return playerPos_; }
		inline const tnl::Vector3& getPlayerSize() const { return playerSize_; }
		
		void setPlayerAndCameraPos(const tnl::Vector3& newPos);

		// マウスでカメラを操作する
		void cameraControl(float controlSpeed);

		// WASDでカメラを移動し、移動先をplayerPos_と同期する
		void moveControl(float moveSpeed);
		

		void debug_displayPlayerParam(int drawPosX = 0, int drawPosY = 0);

	private:
		tnl::Vector3 playerPos_{ 0,0,0 };
		tnl::Vector3 playerSize_{ 15,15,15 };
		Shared<Atl3DCamera> playerCamera_ = nullptr;
	};

}