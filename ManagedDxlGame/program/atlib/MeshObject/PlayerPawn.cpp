#include "PlayerPawn.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Scenes/DungeonScene.h"
#include "../MeshObject/MagicWand.h"
#include "../MeshObject/MenuBook.h"
#include "../MeshObject/ForwardArrow.h"
#include "EnemyPawn.h"

namespace atl {

	void PlayerPawn::setPlayerAndCamera3Dpos(const tnl::Vector3& newPos) {
		player3Dpos_ = newPos;
		playerCamera_->pos_ = newPos;
	}

	void PlayerPawn::playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos) {
		player2Dpos_ = spawn2Dpos;

		float cellLength = static_cast<float>(DungeonScene::getCellLength());
		tnl::Vector3 initPos = { spawn2Dpos.x * cellLength, PLAYER_HEAD_LINE, spawn2Dpos.y * cellLength };

		setPlayerAndCamera3Dpos(initPos);
	}

	bool PlayerPawn::isCanMovePos(const tnl::Vector2i& moveToPos) {
		auto newPlayerPos = player2Dpos_ + moveToPos;

		// �_���W�����Z�����ړ��s�� ( �ǂȂ� ) �Ȃ�ړ��s��
		if (!DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(player2Dpos_ + moveToPos)) {
			return false;
		}

		// �G�l�~�[��������ړ��s��
		auto& enemies = weakDungeonScene_.lock()->getEnemyArray();
		for (auto& enemy : enemies) {
			if (newPlayerPos == enemy->get2Dpos()) return false;
		}

		return true;
	}

	void PlayerPawn::setMoveTarget(const tnl::Vector2i& moveToPos) {
		auto cellLength = DungeonScene::getCellLength();
		moveTarget_ = { playerCamera_->pos_.x + (moveToPos.x * cellLength),playerCamera_->pos_.y,playerCamera_->pos_.z + (moveToPos.y * cellLength) };
		player2Dpos_ += moveToPos;
	}
	
	void PlayerPawn::changeMoveDirSeq() {
		bool(PlayerPawn:: * relativeForward)(float) = &PlayerPawn::seqMoveZplus;
		bool(PlayerPawn:: * relativeBack)(float) = &PlayerPawn::seqMoveZminus;
		bool(PlayerPawn:: * relativeRight)(float) = &PlayerPawn::seqMoveXplus;
		bool(PlayerPawn:: * relativeLeft)(float) = &PlayerPawn::seqMoveXminus;

		// ���݂̌����ɉ����� �֐��|�C���^�̒��g��ύX
		switch (playerCamera_->getCurrentForwardDir()) {
		case Atl3DCamera::e_XZdir::Zplus: break; // Z+ �����Ȃ炻�̂܂�
		case Atl3DCamera::e_XZdir::Zminus:
			relativeForward = &PlayerPawn::seqMoveZminus;
			relativeBack = &PlayerPawn::seqMoveZplus;
			relativeRight = &PlayerPawn::seqMoveXminus;
			relativeLeft = &PlayerPawn::seqMoveXplus;
			break;
		case Atl3DCamera::e_XZdir::Xplus:
			relativeForward = &PlayerPawn::seqMoveXplus;
			relativeBack = &PlayerPawn::seqMoveXminus;
			relativeRight = &PlayerPawn::seqMoveZminus;
			relativeLeft = &PlayerPawn::seqMoveZplus;
			break;
		case Atl3DCamera::e_XZdir::Xminus:
			relativeForward = &PlayerPawn::seqMoveXminus;
			relativeBack = &PlayerPawn::seqMoveXplus;
			relativeRight = &PlayerPawn::seqMoveZplus;
			relativeLeft = &PlayerPawn::seqMoveZminus;
			break;
		}

		// WASD �� �֐��|�C���^�̒��g�ɑJ��
		if (tnl::Input::IsKeyDown(eKeys::KB_W)) { seq_.change(relativeForward); }
		else if (tnl::Input::IsKeyDown(eKeys::KB_S)) { seq_.change(relativeBack); }
		else if (tnl::Input::IsKeyDown(eKeys::KB_A)) { seq_.change(relativeLeft); }
		else if (tnl::Input::IsKeyDown(eKeys::KB_D)) { seq_.change(relativeRight); }
	}

	// ---------------------------
	// �V�[�P���X
	// ---------------------------

	bool PlayerPawn::seqWaitKeyInput(float deltaTime) {
		if (!isAlreadyTurn_) {
			if (tnl::Input::IsKeyDown(eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S)) {
				changeMoveDirSeq();
			}
			if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
				seq_.change(&PlayerPawn::actionAttack);
			}
			if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
				playerHaveMenuBook_->isOpenMenu() ? playerHaveMenuBook_->closeMenu() : playerHaveMenuBook_->openMenu();
			}
		}
		return true;
	}

	bool PlayerPawn::seqMoveZplus(float deltaTime) {
		if (seq_.isStart()) {
			if (isCanMovePos({ 0,1 })) {
				setMoveTarget({ 0,1 });
			}
		}
		actionMoveLerp(deltaTime);
		return true;
	}

	bool PlayerPawn::seqMoveZminus(float deltaTime) {
		if (seq_.isStart()) {
			if (isCanMovePos({ 0,-1 })) {
				setMoveTarget({ 0,-1 });
			}
		}
		actionMoveLerp(deltaTime);
		return true;
	}

	bool PlayerPawn::seqMoveXplus(float deltaTime) {
		if (seq_.isStart()) {
			if (isCanMovePos({ 1,0 })) {
				setMoveTarget({ 1,0 });
			}
		}
		actionMoveLerp(deltaTime);
		return true;
	}

	bool PlayerPawn::seqMoveXminus(float deltaTime) {
		if (seq_.isStart()) {
			if (isCanMovePos({ -1,0 })) {
				setMoveTarget({ -1,0 });
			}
		}
		actionMoveLerp(deltaTime);
		return true;
	}
	
	bool PlayerPawn::actionMoveLerp(float deltaTime) {
		playerCamera_->pos_ = tnl::Vector3::DecelLerp(playerCamera_->pos_, moveTarget_, MOVE_TIME, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		tnl::Vector3 moveVector = moveTarget_ - playerCamera_->pos_;
		float length = moveVector.length();

		if (length <= 0.01f) {
			moveLerpTimeCount_ = 0;
			isAlreadyTurn_ = true;
			seq_.change(&PlayerPawn::seqWaitKeyInput);
		}
		player3Dpos_ = playerCamera_->pos_;
		return false;
	}

	bool PlayerPawn::actionAttack(float deltaTime) {
		if (seq_.isStart()) {

			{// �����p�[�e�B�N��
				tnl::Vector3 cameraForward = playerCamera_->forward();
				cameraForward.normalize();

				tnl::Vector3 normals[4] = {
					{1,0,0},
					{-1,0,0},
					{0,0,1},
					{0,0,-1}
				};

				tnl::Vector3 closestNormal = normals[0];
				float minAngle = cameraForward.angle(normals[0]);

				for (int i = 1; i < 4; ++i) {
					float angle = cameraForward.angle(normals[i]);
					if (angle < minAngle) {
						minAngle = angle;
						closestNormal = normals[i];
					}
				}

				auto particlePos = playerCamera_->pos_ + (closestNormal * 500);
				explosion_->setPosition(particlePos);
				explosion_->start();
			}

			PlaySoundFile("sound/explosion.wav", 2);

			TextLogManager::getTextLogManager()->addTextLog("�v���C���[�̍U���I�@��������������");
		}

		// ATTACK_TIME�̊Ԃ����A�V�[�P���X��J�ڂ����Ȃ�
		static float totalDeltaTime = 0.0f;
		totalDeltaTime += deltaTime;
		if (totalDeltaTime > ATTACK_TIME) {
			totalDeltaTime = 0.0f;
			
			// �^�[���I��
			isAlreadyTurn_ = true;
			seq_.change(&PlayerPawn::seqWaitKeyInput);
		}

		return true;
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
		if (playerHaveMagicWand_) {
			DrawStringEx(drawPosX, drawPosY + 60, -1,
				"playerWandPos ... [ %.2f , %.2f, %.2f ]", playerHaveMagicWand_->getRootMesh()->pos_.x, playerHaveMagicWand_->getRootMesh()->pos_.y, playerHaveMagicWand_->getRootMesh()->pos_.z);
		}
		if (playerHaveMenuBook_) {
			DrawStringEx(drawPosX, drawPosY + 75, -1,
				"menuOpen ... [ %d ]", playerHaveMenuBook_->isOpenMenu());
		}
	}

	void PlayerPawn::render(float deltaTime) {
		//forwardArrow_->renderObject(playerCamera_);
		playerHaveMagicWand_->renderObjects(playerCamera_);
		playerHaveMenuBook_->renderObject(playerCamera_, deltaTime);

		dxe::DirectXRenderBegin();
		explosion_->render(playerCamera_);
		dxe::DirectXRenderEnd();
	}

	void PlayerPawn::initialize(std::weak_ptr<DungeonScene> dungeonScene) {
		playerHaveMagicWand_ = std::make_shared<MagicWand>(std::weak_ptr<PlayerPawn>(shared_from_this()));
		playerHaveMenuBook_ = std::make_shared<MenuBook>(std::weak_ptr<PlayerPawn>(shared_from_this()));
		forwardArrow_ = std::make_shared<ForwardArrow>(std::weak_ptr<PlayerPawn>(shared_from_this()));
		player3Dpos_ = playerCamera_->pos_;
		weakDungeonScene_ = dungeonScene;
	}

}