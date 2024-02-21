#include "Enemy.h"
#include "../Enemy/EnemyManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Utilities/AtlRandom.h"
#include "../Utilities/AtlString.h"
#include "../Object/PlayerData.h"
#include "../MeshObject/PlayerPawn.h"

namespace atl {

	/// <summary>
	/// 
	/// �x�[�X�G�l�~�[
	/// �֐�
	/// 
	/// </summary>

	bool Base_Enemy::isCanMove(const tnl::Vector2i& moveToPos) {
		// �ړ���̍��W
		auto newPos = get2Dpos() + moveToPos;

		// �_���W�����Z�����ړ��s�� ( �ǂȂ� ) �Ȃ�ړ��s��
		if (!DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(newPos)) {
			return false;
		}

		// �ړ���ɁA�ʂ̃G�l�~�[��������ړ��s��
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) {
			if (newPos == enemy->get2Dpos()) { return false; }
		}

		return true;
	}

	void Base_Enemy::setMoveTarget(const tnl::Vector2i& moveToPos) {
		auto cellLength = DungeonScene::getCellLength();
		auto meshPos = getMesh()->pos_;

		// �ړ��\�Ȃ�moveTarget �� �ݒ�
		if (isCanMove(moveToPos)) {

			moveTarget_ = {
				meshPos.x + (moveToPos.x * cellLength),
				meshPos.y,
				meshPos.z + (moveToPos.y * cellLength) };

			getMesh()->rot_ = tnl::Quaternion::LookAt(getMesh()->pos_, moveTarget_, { 0,1,0 });
			add2Dpos(moveToPos);
		}
		// �ړ��s�\�Ȃ� moveTarget �����̏�ɐݒ�
		else {
			moveTarget_ = meshPos;
		}
	}

	bool Base_Enemy::isNeighborPlayer() {
		// �v���C���[�Ɨאڂ��Ă��邩���肷��
		auto& player2Dpos = weakDungeonScene_.lock()->getPlayerPawn()->getPlayer2Dpos();
		auto& enemy2Dpos = get2Dpos();

		// �l�ߖT�Ƀv���C���[�����邩�𔻒�B������ true
		if (player2Dpos.x == enemy2Dpos.x + 1 && player2Dpos.y == enemy2Dpos.y) { return true; }
		else if (player2Dpos.x == enemy2Dpos.x - 1 && player2Dpos.y == enemy2Dpos.y) { return true; }
		else if (player2Dpos.x == enemy2Dpos.x && player2Dpos.y == enemy2Dpos.y + 1) { return true; }
		else if (player2Dpos.x == enemy2Dpos.x && player2Dpos.y == enemy2Dpos.y - 1) { return true; }
		else return false;
	}

	bool Base_Enemy::isSameAreaPlayer() {
		auto& player2Dpos = weakDungeonScene_.lock()->getPlayerPawn()->getPlayer2Dpos();

		auto enemyAreaID = DungeonCreater::getDungeonCreater()->getFieldCellID(get2Dpos());
		auto playerAreaID = DungeonCreater::getDungeonCreater()->getFieldCellID(player2Dpos);

		// �����̃G���AID �ƃv���C���[�̃G���A ID����v�����ꍇ�Atrue
		if (enemyAreaID == playerAreaID) { return true; }

		return false;
	}

	/// <summary>
	/// 
	/// �x�[�X�G�l�~�[
	/// �V�[�P���X�֐�
	/// 
	/// <summary>

	bool Base_Enemy::seqTransition(float deltaTime) {
		// ���Ɉړ��������͍s�����������Ă���ꍇ�A�������^�[�� ( �ҋ@��� )
		if (isAlreadyAction_ || isAlreadyMove_) { return false; }

		// HP���[���ɂȂ��Ă���ꍇ
		if (enemyData_->isZeroHP()) { seq_.change(&Base_Enemy::seqDeading); }

		// �v���C���[�Ɨאڂ��Ă����ꍇ
		else if (isNeighborPlayer()) { seq_.change(&Base_Enemy::seqPlayerNeighboring); }

		// �v���C���[�Ɠ����G���A�ɂ���ꍇ
		else if (isSameAreaPlayer()) { seq_.change(&Base_Enemy::seqChasePlayer); }

		// ����ȊO�̏ꍇ
		else { seq_.change(&Base_Enemy::seqWandering); }

		return true;
	}

	bool Base_Enemy::seqWandering(float deltaTime) {
		// 0 ~ 3 �܂ł̎l�� ( �l���� ) �������_���Ɍ��肵�A������Ɉړ�����
		int32_t rand = mtRandomRangeInt(0, 3);
		switch (rand) {
		case 0:
			setMoveTarget({ 0,1 });
			break;
		case 1:
			setMoveTarget({ 0,-1 });
			break;
		case 2:
			setMoveTarget({ 1,0 });
			break;
		case 3:
			setMoveTarget({ -1,0 });
		}

		seq_.change(&Base_Enemy::seqMoveToTarget);
		return true;
	}

	// �v���C���[�𒼐��Œǂ� 
	bool Base_Enemy::seqChasePlayer(float deltaTime) {
		auto& player2Dpos = weakDungeonScene_.lock()->getPlayerPawn()->getPlayer2Dpos();
		auto& currentPos = get2Dpos();

		// X���W �� Y���W�A�̏��Ńv���C���[��ǂ�������B
		if (player2Dpos.x > currentPos.x && isCanMove({ 1,0 })) {
			setMoveTarget({ 1,0 });
		}
		else if (player2Dpos.x < currentPos.x && isCanMove({ -1, 0 })) {
			setMoveTarget({ -1,0 });
		}
		else if (player2Dpos.y > currentPos.y && isCanMove({ 0,1 })) {
			setMoveTarget({ 0,1 });
		}
		else if (player2Dpos.y < currentPos.y && isCanMove({ 0,-1 })) {
			setMoveTarget({ 0,-1 });
		}

		seq_.change(&Base_Enemy::seqMoveToTarget);
		return true;
	}

	bool Base_Enemy::seqMoveToTarget(float deltaTime) {
		getMesh()->pos_ = tnl::Vector3::DecelLerp(getMesh()->pos_, moveTarget_, MOVE_TIME, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		tnl::Vector3 moveVector = moveTarget_ - getMesh()->pos_;
		float length = moveVector.length();

		// MOVE_END_BORDER ���ړI�n�Ƃ̋������߂��Ȃ��� ( �قڈړ����I������ ) �ꍇ
		if (length <= MOVE_END_BORDER) {
			moveLerpTimeCount_ = 0;
			isAlreadyMove_ = true;

			seq_.change(&Base_Enemy::seqActionNone);
		}

		return true;
	}

	bool Base_Enemy::seqActionNone(float deltaTime) {
		isAlreadyAction_ = true;
		seq_.change(&Base_Enemy::seqTransition);
		return true;
	}

	bool Base_Enemy::seqPlayerNeighboring(float deltaTime) {
		auto player = weakDungeonScene_.lock()->getPlayerPawn();
		if (player->getIsAlreadyTurn()) {
			// �v���C���[�̍s�����I����Ă���A�v���C���[�̕�������
			auto meshPos = getMesh()->pos_;
			getMesh()->rot_ = tnl::Quaternion::LookAt(meshPos, player->getPlayerPos(), { 0,1,0 });
			seq_.change(&Base_Enemy::seqMoveNone);
		}
		return true;
	}

	bool Base_Enemy::seqDeading(float deltaTime) {
		// 0.2�b�ԁA���邭���]
		SEQ_CO_YIELD_RETURN_TIME(0.2f, deltaTime, [&] {
			getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(15));
		});

		// ��񉹂�炵�A���� 1 �b�ԉ�]
		SEQ_CO_YIELD_RETURN_TIME(1.0f, deltaTime, [&] {
			if (SEQ_CO_YIELD_TIME_IS_START) { 
				ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneEnemyDead.ogg", DX_PLAYTYPE_BACK); 
			}

			getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(15));
		});

		// �s���ς݂Ǝ��S�t���O���I����
		isAlreadyAction_ = true;
		isAlreadyMove_ = true;
		isDead_ = true;

		// �e�L�X�g���O�ɏo��
		auto& enemyName = getEnemyData()->getEnemyName();
		TextLogManager::getTextLogManager()->addTextLog(enemyName + "��|�����I");

		SEQ_CO_END
	}

	bool Base_Enemy::seqMoveNone(float deltaTime) {
		isAlreadyMove_ = true;
		seq_.change(&Base_Enemy::seqActionAttack);
		return true;
	}

	bool Base_Enemy::seqActionAttack(float deltaTime) {
		auto player = weakDungeonScene_.lock()->getPlayerPawn();

		// �v���C���[���s���I�����Ă��Ȃ���΁A�������^�[��
		if (!player->getIsAlreadyTurn()) { return true; }

		// �N����̍U���Ȃ̂����e�L�X�g���O�ɏo�͂��A0.5�b�ԑҋ@
		SEQ_CO_YIELD_RETURN_TIME(0.5f, deltaTime, [&] {
			if (SEQ_CO_YIELD_TIME_IS_START) {
				auto& enemyName = getEnemyData()->getEnemyName();
				TextLogManager::getTextLogManager()->addTextLog(enemyName + "�̂��������I");
				ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneEnemyAttack.ogg", DX_PLAYTYPE_BACK);
			}
			});

		// �U�������B�v���C���[�Ƀ_���[�W��^���A���O�ɏo��
		auto damage = Base_ActorData::damaged(*enemyData_, *player->getPlayerData());
		TextLogManager::getTextLogManager()->addTextLog("�v���C���[��" + convertFullWidthNumber(damage) + "�̃_���[�W���󂯂�");
		isAlreadyAction_ = true;
		seq_.change(&Base_Enemy::seqTransition);

		SEQ_CO_END;
	}

	/// <summary>
	/// 
	/// �X���C��
	/// 
	/// <summary>

	void SlimeEnemy::initialize() {
		// ���b�V����ݒ�
		auto mesh = dxe::Mesh::CreateFromFileMV("mesh/mv1/SlimeMesh.mv1", 5);
		setMesh(mesh);

		// �e�N�X�`����ݒ�
		auto texture = dxe::Texture::CreateFromFile("graphics/Texture/EnemyTexture/Albedo.png");
		mesh->setTexture(texture);

		// �}�e���A����ݒ�
		mesh->loadMaterial(getEnemyData()->getEnemyMaterialPath());
	}

}