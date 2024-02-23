#include "PlayerPawn.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/EnemyManager/EnemyManager.h"
#include "../Scenes/DungeonScene.h"
#include "../MeshObject/MagicWand.h"
#include "../MeshObject/MenuBook.h"
#include "../MeshObject/ForwardArrow.h"
#include "../Utilities/AtlString.h"
#include "../MagicRuneSystem/MagicRune.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"

namespace atl {

	PlayerPawn::PlayerPawn() {
		// �{�����[������
		ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/SE/DungeonScenePlayerAttack.ogg", 200);
	}

	PlayerPawn::~PlayerPawn() {
		explosion_.reset();
		ResourceManager::getResourceManager()->deleteResource("sound/SE/PlayerFootStep.ogg");
		ResourceManager::getResourceManager()->deleteResource("sound/SE/DungeonScenePlayerAttack.ogg");
	}

	void PlayerPawn::initialize() {
		playerHaveMagicWand_ = std::make_shared<MagicWand>(std::weak_ptr<PlayerPawn>(shared_from_this()));
		playerHaveMenuBook_ = std::make_shared<MenuBook>(std::weak_ptr<PlayerPawn>(shared_from_this()));
		forwardArrow_ = std::make_shared<ForwardArrow>(std::weak_ptr<PlayerPawn>(shared_from_this()));
		player3Dpos_ = playerCamera_->pos_;
	}

	void PlayerPawn::openMenuBook() {
		playerHaveMenuBook_->openMenu();
	}

	void PlayerPawn::closeMenuBook() {
		playerHaveMenuBook_->closeMenu();
	}

	void PlayerPawn::playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos) {
		player2Dpos_ = spawn2Dpos;

		float cellLength = static_cast<float>(DungeonScene::getCellLength());
		tnl::Vector3 initPos = { spawn2Dpos.x * cellLength, PLAYER_HEAD_LINE, spawn2Dpos.y * cellLength };
		moveTarget_ = initPos;

		player3Dpos_ = initPos;
		playerCamera_->pos_ = initPos;
	}

	bool PlayerPawn::isCanMovePos(const tnl::Vector2i& moveToPos) {
		auto newPlayerPos = player2Dpos_ + moveToPos;

		// �_���W�����Z�����ړ��s�� ( �ǂȂ� ) �Ȃ�ړ��s��
		if (!DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(newPlayerPos)) {
			return false;
		}

		// �G�l�~�[��������ړ��s��
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) {
			if (newPlayerPos == enemy->get2Dpos()) { return false; }
		}

		return true;
	}

	void PlayerPawn::setMoveTarget(const tnl::Vector2i& moveToPos) {
		auto cellLength = DungeonScene::getCellLength();
		moveTarget_ = { playerCamera_->pos_.x + (moveToPos.x * cellLength),playerCamera_->pos_.y,playerCamera_->pos_.z + (moveToPos.y * cellLength) };
		player2Dpos_ += moveToPos;
	}

	// ---------------------------
	// �V�[�P���X
	// ---------------------------

	bool PlayerPawn::seqWait(float deltaTime) {
		// �������Ȃ�
		return true;
	}

	bool PlayerPawn::startMove() {
		// �J�������ێ����鐳�ʕ������擾
		// �O�i ( W ) => ���̂܂�
		// ��� ( S ) => �O�i�𔽓]
		// �E ( D ) => ���ʕ����� Y+�� �ŊO�ς����
		// �� ( A ) => �E�𔽓]

		auto& forwardNormalDir = playerCamera_->getCurrentForwardDir();
		tnl::Vector2i forwardV2i = { static_cast<int>(forwardNormalDir.x),static_cast<int>(forwardNormalDir.z) };
		tnl::Vector2i backV2i = -forwardV2i;
		auto rightNormalDir = tnl::Vector3::Cross({ 0,1,0 }, forwardNormalDir);
		tnl::Vector2i rightV2i = { static_cast<int>(rightNormalDir.x),static_cast<int>(rightNormalDir.z) };
		tnl::Vector2i leftV2i = -rightV2i;

		// �ړ��\���ǂ���
		bool canMove = false;

		if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			canMove = isCanMovePos(forwardV2i);
			if (canMove) { setMoveTarget(forwardV2i); }
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			canMove = isCanMovePos(backV2i);
			if (canMove) { setMoveTarget(backV2i); }
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			canMove = isCanMovePos(rightV2i);
			if (canMove) { setMoveTarget(rightV2i); }
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			canMove = isCanMovePos(leftV2i);
			if (canMove) { setMoveTarget(leftV2i); }
		}

		// �ړ��\�Ȃ�A�V�[�P���X�J�ڂ�����
		if (canMove) {
			seq_.change(&PlayerPawn::seqMoveLerp);
		}

		// �ړ��\���ǂ����̌��ʂ�DungeonScene�ɕԂ�
		return canMove;
	}

	bool PlayerPawn::seqMoveLerp(float deltaTime) {
		if (seq_.isStart()) {
			ResourceManager::getResourceManager()->playSoundRes("sound/SE/PlayerFootStep.ogg", DX_PLAYTYPE_BACK);
		}

		playerCamera_->pos_ = tnl::Vector3::DecelLerp(playerCamera_->pos_, moveTarget_, MOVE_TIME, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		tnl::Vector3 moveVector = moveTarget_ - playerCamera_->pos_;
		float length = moveVector.length();

		if (length <= MOVE_END_BORDER) {
			moveLerpTimeCount_ = 0;
			playerCamera_->pos_ = moveTarget_;
			isAlreadyTurn_ = true;
			seq_.change(&PlayerPawn::seqWait);
		}
		player3Dpos_ = playerCamera_->pos_;
		return false;
	}

	void PlayerPawn::startAttack() {

		// ��~���� => ��U��
		waitTime_ = ATTACK_MISS_TIME;

		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		tnl::Vector2i attackPos = player2Dpos_ + playerCamera_->getCurrentForwardDirToV2i();

		for (auto& enemy : enemyList) {
			auto& enemyPos = enemy->get2Dpos();
			if (enemyPos.x == attackPos.x && enemyPos.y == attackPos.y) {
				// �U���q�b�g����

				// ��~���� => �U���q�b�g��
				waitTime_ = ATTACK_TIME;
				attackHitEffectAndLog(enemy);
			}
		}
		seq_.change(&PlayerPawn::seqActionAttackDelay);
	}

	bool PlayerPawn::seqActionAttackDelay(float deltaTime) {
		// waitTime�ɐݒ肳�ꂽ���ԕ��A��~����
		totalDeltaTimer_ += deltaTime;
		if (totalDeltaTimer_ > waitTime_) {
			totalDeltaTimer_ = 0.0f;
			// �^�[���I��
			isAlreadyTurn_ = true;
			seq_.change(&PlayerPawn::seqWait);
		}

		return true;
	}

	void PlayerPawn::attackHitEffectAndLog(const Shared<atl::Base_Enemy> enemy) {
		auto damage = Base_ActorData::damaged(*playerData_,*enemy->getEnemyData());

		// �����p�[�e�B�N��
		auto& forwardNormal = playerCamera_->getCurrentForwardDir();
		auto particlePos = playerCamera_->pos_ + (forwardNormal * 500);
		explosion_->setPosition(particlePos);
		explosion_->start();

		// �T�E���h
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonScenePlayerAttack.ogg", DX_PLAYTYPE_BACK);

		// ���O
		TextLogManager::getTextLogManager()->addTextLog("�v���C���[�̍U���I" + convertFullWidthNumber(damage) + "�̃_���[�W��^����");
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
			"moveTarget_ ... [ %.2f , %.2f, %.2f ]", moveTarget_.x, moveTarget_.y, moveTarget_.z);
		// ���݂���G���A��ID
		DrawStringEx(drawPosX, drawPosY + 100, -1,
			"cellID ... [ %d ]", DungeonCreater::getDungeonCreater()->getFieldCellID(player2Dpos_));
	}



	void PlayerPawn::render(float deltaTime) {
		forwardArrow_->renderObject(playerCamera_);
		playerHaveMagicWand_->renderObjects(playerCamera_, deltaTime);
		playerHaveMenuBook_->renderObject(playerCamera_, deltaTime);

		dxe::DirectXRenderBegin();
		explosion_->render(playerCamera_);
		dxe::DirectXRenderEnd();
	}
}