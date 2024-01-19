#include "PlayerPawn.h"
#include "../Singletons/DungeonCreater.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	PlayerPawn::PlayerPawn() {
		playerCamera_ = std::make_shared<Atl3DCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
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

	// ---------------------------
	// ƒV[ƒPƒ“ƒX
	// ---------------------------

	const tnl::Vector3& PlayerPawn::calcMoveDir() {
		tnl::Vector3 returnVec{ 0,0,0 };
		return tnl::Vector3();
	}

	bool PlayerPawn::seqIdle(float deltaTime) {

		{// ˆÚ“®“ü—Í
			if (tnl::Input::IsKeyDown(eKeys::KB_A,eKeys::KB_D,eKeys::KB_W,eKeys::KB_S)) {
				seq_.change(&PlayerPawn::seqCalcMoveDir);
			}
		}
		return true;
	}

	bool PlayerPawn::seqCalcMoveDir(float deltaTime) {
		tnl::Vector3 cameraForward = playerCamera_->forward().xz();
		cameraForward.normalize();

		float angleToZplus = cameraForward.angle({ 0, 0, 1 });
		float angleToZminus = cameraForward.angle({ 0,0,-1 });
		float angleToXplus = cameraForward.angle({ 1, 0, 0 });
		float angleToXminus = cameraForward.angle({ -1,0,0 });

		float minAngle = (std::min)({ angleToZplus, angleToZminus, angleToXplus, angleToXminus });

		bool(PlayerPawn::*relativeForward)(float) = &PlayerPawn::seqMoveZplus;
		bool(PlayerPawn::*relativeBack)(float) = &PlayerPawn::seqMoveZminus;
		bool(PlayerPawn::*relativeRight)(float) = &PlayerPawn::seqMoveXplus;
		bool(PlayerPawn::*relativeLeft)(float) = &PlayerPawn::seqMoveXminus;

		if (minAngle == angleToZplus) {
		}
		else if (minAngle == angleToZminus) {
			relativeForward = &PlayerPawn::seqMoveZminus;
			relativeBack = &PlayerPawn::seqMoveZplus;
			relativeRight = &PlayerPawn::seqMoveXminus;
			relativeLeft = &PlayerPawn::seqMoveXplus;
		}
		else if (minAngle == angleToXplus) {
			relativeForward = &PlayerPawn::seqMoveXplus;
			relativeBack = &PlayerPawn::seqMoveXminus;
			relativeRight = &PlayerPawn::seqMoveZminus;
			relativeLeft = &PlayerPawn::seqMoveZplus;
		}
		else if (minAngle == angleToXminus) {
			relativeForward = &PlayerPawn::seqMoveXminus;
			relativeBack = &PlayerPawn::seqMoveXplus;
			relativeRight = &PlayerPawn::seqMoveZplus;
			relativeLeft = &PlayerPawn::seqMoveZminus;
		}

		if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			seq_.change(relativeForward);
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			seq_.change(relativeBack);
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			seq_.change(relativeLeft);
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			seq_.change(relativeRight);
		}

		return true;
	}

	bool PlayerPawn::seqMoveZplus(float deltaTime) {
		if (seq_.isStart()) {
			moveTarget_ = { playerCamera_->pos_.x,playerCamera_->pos_.y,playerCamera_->pos_.z + DungeonScene::getCellLength() };
		}
		playerCamera_->pos_ = tnl::Vector3::DecelLerp(playerCamera_->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&PlayerPawn::seqIdle);
		}
		playerPos_ = playerCamera_->pos_;

		return true;
	}

	bool PlayerPawn::seqMoveZminus(float deltaTime) {
		if (seq_.isStart()) {
			moveTarget_ = { playerCamera_->pos_.x, playerCamera_->pos_.y, playerCamera_->pos_.z - DungeonScene::getCellLength() };
		}
		playerCamera_->pos_ = tnl::Vector3::DecelLerp(playerCamera_->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&PlayerPawn::seqIdle);
		}
		playerPos_ = playerCamera_->pos_;

		return true;
	}

	bool PlayerPawn::seqMoveXplus(float deltaTime) {
		if (seq_.isStart()) {
			moveTarget_ = { playerCamera_->pos_.x + DungeonScene::getCellLength(), playerCamera_->pos_.y, playerCamera_->pos_.z };
		}
		playerCamera_->pos_ = tnl::Vector3::DecelLerp(playerCamera_->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&PlayerPawn::seqIdle);
		}
		playerPos_ = playerCamera_->pos_;

		return true;
	}

	bool PlayerPawn::seqMoveXminus(float deltaTime) {
		if (seq_.isStart()) {
			moveTarget_ = { playerCamera_->pos_.x - DungeonScene::getCellLength(), playerCamera_->pos_.y, playerCamera_->pos_.z };
		}
		playerCamera_->pos_ = tnl::Vector3::DecelLerp(playerCamera_->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&PlayerPawn::seqIdle);
		}
		playerPos_ = playerCamera_->pos_;

		return true;
	}

	void PlayerPawn::debug_displayPlayerParam(int drawPosX, int drawPosY) {
		DrawStringEx(drawPosX, drawPosY, -1,
			"playerPos ... [ %.2f , %.2f , %.2f ]", playerPos_.x, playerPos_.y, playerPos_.z);
		DrawStringEx(drawPosX, drawPosY + 15, -1,
			"playerCameraPos ... [ %.2f , %.2f , %.2f ]", playerCamera_->pos_.x, playerCamera_->pos_.y, playerCamera_->pos_.z);
		DrawStringEx(drawPosX, drawPosY + 32, -1,
			"playerCameraAngle ... [ %.2f , %.2f, %.2f ]", tnl::ToDegree(playerCamera_->forward().angle({ 1,0,0 })), tnl::ToDegree(playerCamera_->forward().angle({ 0,1,0 })),tnl::ToDegree(playerCamera_->forward().angle({0,0,1})));
	}

}