#include "PlayerPawn.h"

namespace atl {

	PlayerPawn::PlayerPawn() {
		playerCamera_ = std::make_shared<Atl3DCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	}

	PlayerPawn::PlayerPawn(int x , int z) {
		PlayerPawn();
		playerPos_ = playerCamera_->pos_;
	}

	void PlayerPawn::setPlayerAndCameraPos(const tnl::Vector3& newPos){
		playerPos_ = newPos;
		playerCamera_->pos_ = newPos;
	}

	void PlayerPawn::cameraControl(float controlSpeed) {
		playerCamera_->target_ = playerCamera_->pos_ + playerCamera_->forward();
		playerCamera_->up();

		tnl::Quaternion beforeRot = playerCamera_->getCameraRot();

		tnl::Vector3 mvel = tnl::Input::GetMouseVelocity();

		// cU‚è
		playerCamera_->cameraRotateAxis(playerCamera_->right(), mvel.y * controlSpeed);
		float newAngle = tnl::ToDegree(playerCamera_->forward().angle({ 0,1,0 }));
		if (newAngle < playerCamera_->getMIN_PITCH() || playerCamera_->getMAX_PITCH() < newAngle) {
			playerCamera_->setCameraRot(beforeRot);
		}

		// ‰¡U‚è
		playerCamera_->cameraRotateAxis({ 0, 1, 0 }, mvel.x * controlSpeed);

	}

	void PlayerPawn::moveControl(float moveSpeed) {
		if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
			playerCamera_->pos_.y += moveSpeed;
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
			playerCamera_->pos_.y -= moveSpeed;
		}

		if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			playerCamera_->pos_ += playerCamera_->left() * moveSpeed;
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			playerCamera_->pos_ += playerCamera_->right() * moveSpeed;
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			tnl::Vector3 forward = playerCamera_->forward().xz();
			forward = tnl::Vector3::Normalize(forward);
			playerCamera_->pos_ += forward * moveSpeed;
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			tnl::Vector3 back = playerCamera_->back().xz();
			back = tnl::Vector3::Normalize(back);
			playerCamera_->pos_ += back * moveSpeed;
		}

		playerPos_ = playerCamera_->pos_;
	}

	void PlayerPawn::debug_displayPlayerParam(int drawPosX, int drawPosY) {
		DrawStringEx(drawPosX, drawPosY, -1,
			"playerPos ... [ %.2f , %.2f , %.2f ]", playerPos_.x, playerPos_.y, playerPos_.z);
	}

}