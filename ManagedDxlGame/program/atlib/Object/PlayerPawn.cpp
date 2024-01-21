#include "PlayerPawn.h"
#include "../Singletons/DungeonCreater.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	PlayerPawn::PlayerPawn() {
		playerCamera_ = std::make_shared<Atl3DCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
		player3Dpos_ = playerCamera_->pos_;
	}

	void PlayerPawn::setPlayerAndCamera3Dpos(const tnl::Vector3& newPos) {
		player3Dpos_ = newPos;
		playerCamera_->pos_ = newPos;
	}

	void PlayerPawn::cameraControl() {
		playerCamera_->target_ = playerCamera_->pos_ + playerCamera_->forward();
		playerCamera_->up();

		tnl::Quaternion beforeRot = playerCamera_->getCameraRot();

		tnl::Vector3 mvel = tnl::Input::GetMouseVelocity();

		// �c�U��
		playerCamera_->cameraRotateAxis(playerCamera_->right(), mvel.y * CAMERA_ROT_SPEED_);
		float newAngle = tnl::ToDegree(playerCamera_->forward().angle({ 0,1,0 }));
		if (newAngle < playerCamera_->getMIN_PITCH() || playerCamera_->getMAX_PITCH() < newAngle) {
			playerCamera_->setCameraRot(beforeRot);
		}

		// ���U��
		playerCamera_->cameraRotateAxis({ 0, 1, 0 }, mvel.x * CAMERA_ROT_SPEED_);
	}

	void PlayerPawn::playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos) {
		player2Dpos_ = spawn2Dpos;

		float cellLength = DungeonScene::getCellLength();
		tnl::Vector3 initPos = { spawn2Dpos.x * cellLength, PLAYER_HEAD_LINE, spawn2Dpos.y * cellLength };

		setPlayerAndCamera3Dpos(initPos);
	}

	bool PlayerPawn::checkIsCanMovePos(const tnl::Vector2i& moveToPos) {
		auto cellLength = DungeonScene::getCellLength();
		if (DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(player2Dpos_ + moveToPos)) {
			moveTarget_ = { playerCamera_->pos_.x + (moveToPos.x * cellLength),playerCamera_->pos_.y,playerCamera_->pos_.z + (moveToPos.y * cellLength) };
			player2Dpos_ += moveToPos;
			return true;
		}
		else {
			moveTarget_ = player3Dpos_;
			return true;
		}
		return false;
	}

	bool PlayerPawn::moveLerp(float deltaTime) {
		playerCamera_->pos_ = tnl::Vector3::DecelLerp(playerCamera_->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&PlayerPawn::seqIdle);
			return true;
		}
		player3Dpos_ = playerCamera_->pos_;
		return false;
	}

	PlayerPawn::e_XZdir PlayerPawn::checkCurrentFowardDir() {
		tnl::Vector3 cameraForward = playerCamera_->forward().xz();
		cameraForward.normalize();

		float angleToZplus = cameraForward.angle({ 0, 0, 1 });
		float angleToZminus = cameraForward.angle({ 0,0,-1 });
		float angleToXplus = cameraForward.angle({ 1, 0, 0 });
		float angleToXminus = cameraForward.angle({ -1,0,0 });

		float minAngle = (std::min)({ angleToZplus, angleToZminus, angleToXplus, angleToXminus });

		e_XZdir retV;
		if (minAngle == angleToZplus) {
			retV = e_XZdir::Zplus;
		}
		else if (minAngle == angleToZminus) {
			retV = e_XZdir::Zminus;
		}
		else if (minAngle == angleToXplus) {
			retV = e_XZdir::Xplus;
		}
		else if (minAngle == angleToXminus) {
			retV = e_XZdir::Xminus;
		}
		else {
			retV = e_XZdir::NONE;
		}
		return retV;
	}	
	
	void PlayerPawn::calcDirAndMoveSeqChange() {
		bool(PlayerPawn:: * relativeForward)(float) = &PlayerPawn::seqMoveZplus;
		bool(PlayerPawn:: * relativeBack)(float) = &PlayerPawn::seqMoveZminus;
		bool(PlayerPawn:: * relativeRight)(float) = &PlayerPawn::seqMoveXplus;
		bool(PlayerPawn:: * relativeLeft)(float) = &PlayerPawn::seqMoveXminus;

		// ���݂̌����ɉ����� �֐��|�C���^�̒��g��ύX
		switch (checkCurrentFowardDir()) {
		case e_XZdir::Zplus: break; // Z+ �����Ȃ炻�̂܂�
		case e_XZdir::Zminus:
			relativeForward = &PlayerPawn::seqMoveZminus;
			relativeBack = &PlayerPawn::seqMoveZplus;
			relativeRight = &PlayerPawn::seqMoveXminus;
			relativeLeft = &PlayerPawn::seqMoveXplus;
			break;
		case e_XZdir::Xplus:
			relativeForward = &PlayerPawn::seqMoveXplus;
			relativeBack = &PlayerPawn::seqMoveXminus;
			relativeRight = &PlayerPawn::seqMoveZminus;
			relativeLeft = &PlayerPawn::seqMoveZplus;
			break;
		case e_XZdir::Xminus:
			relativeForward = &PlayerPawn::seqMoveXminus;
			relativeBack = &PlayerPawn::seqMoveXplus;
			relativeRight = &PlayerPawn::seqMoveZplus;
			relativeLeft = &PlayerPawn::seqMoveZminus;
			break;
		}

		// WASD �� �֐��|�C���^�̒��g�J��
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
	}


	// ---------------------------
	// �V�[�P���X
	// ---------------------------

	bool PlayerPawn::seqIdle(float deltaTime) {
		{// �ړ�����
			if (tnl::Input::IsKeyDown(eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S)) {
				calcDirAndMoveSeqChange();
				return false;
			}
		}
		return false;
	}

	bool PlayerPawn::seqMoveZplus(float deltaTime) {
		if (seq_.isStart()) {
			checkIsCanMovePos({ 0,1 });
		}
		
		return moveLerp(deltaTime);
	}

	bool PlayerPawn::seqMoveZminus(float deltaTime) {
		if (seq_.isStart()) {
			checkIsCanMovePos({ 0,-1 });
		}

		return moveLerp(deltaTime);
	}

	bool PlayerPawn::seqMoveXplus(float deltaTime) {
		if (seq_.isStart()) {
			checkIsCanMovePos({ 1,0 });
		}

		return moveLerp(deltaTime);
	}

	bool PlayerPawn::seqMoveXminus(float deltaTime) {
		if (seq_.isStart()) {
			checkIsCanMovePos({ -1,0 });
		}
		
		return moveLerp(deltaTime);
	}

	// ---------------------------
	// �f�o�b�O�p
	// ---------------------------

	void PlayerPawn::debug_displayPlayerParam(int drawPosX, int drawPosY) {
			DrawStringEx(drawPosX, drawPosY, -1,
				"player3Dpos ... [ %.2f , %.2f , %.2f ]", player3Dpos_.x, player3Dpos_.y, player3Dpos_.z);
			DrawStringEx(drawPosX, drawPosY + 15, -1,
				"player2Dpos ... [ %d , %d ]", player2Dpos_.x, player2Dpos_.y);
			DrawStringEx(drawPosX, drawPosY + 30, -1,
				"playerCameraPos ... [ %.2f , %.2f , %.2f ]", playerCamera_->pos_.x, playerCamera_->pos_.y, playerCamera_->pos_.z);
			DrawStringEx(drawPosX, drawPosY + 45, -1,
				"playerCameraAngle ... [ %.2f , %.2f, %.2f ]", tnl::ToDegree(playerCamera_->forward().angle({ 1,0,0 })), tnl::ToDegree(playerCamera_->forward().angle({ 0,1,0 })), tnl::ToDegree(playerCamera_->forward().angle({ 0,0,1 })));
	}

}