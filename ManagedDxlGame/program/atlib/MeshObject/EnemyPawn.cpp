#include "EnemyPawn.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Utilities/AtlRandom.h"
#include "../Utilities/AtlString.h"
#include "PlayerPawn.h"

namespace atl {

	EnemyPawn::~EnemyPawn() {
		ResourceManager::getResourceManager()->deleteResource("sound/SE/DungeonSceneEnemyDead.ogg");
		ResourceManager::getResourceManager()->deleteResource("sound/SE/DungeonSceneEnemyAttack.ogg");
	}

	EnemyPawn::EnemyPawn(const tnl::Vector2i& enemyPos) {
		set2Dpos(enemyPos);
		auto cellLength = DungeonScene::getCellLength();
		tnl::Vector3 enemy3Dpos = { static_cast<float>(enemyPos.x * cellLength), ENEMY_POS_Y ,static_cast<float>(enemyPos.y * cellLength) };

		auto texture = enemyData_->getEnemyTexture();

		auto rootMesh = dxe::Mesh::CreateSphereMV(ENEMY_SIZE_RADIUS,30,30);
		rootMesh->setTexture(texture);
		rootMesh->pos_ = enemy3Dpos;
		rootMesh->setAlpha(0.5f);
		rootMesh->setBlendMode(DX_BLENDMODE_ALPHA);
		setRootMesh(rootMesh);

		auto rightHone = dxe::Mesh::CreateConeMV(ENEMY_SIZE_RADIUS / 3.0f,ENEMY_SIZE_RADIUS/2.0f);
		rightHone->setTexture(texture);
		addChildMesh(rightHone);

		auto leftHone = dxe::Mesh::CreateConeMV(ENEMY_SIZE_RADIUS / 3.0f, ENEMY_SIZE_RADIUS / 2.0f);
		leftHone->setTexture(texture);
		addChildMesh(leftHone);

		auto core = dxe::Mesh::CreateCubeMV(ENEMY_SIZE_RADIUS / 3);
		core->rot_ *= tnl::Quaternion::RotationAxis({ 1,1,1 }, tnl::ToRadian(90));
		addChildMesh(core);
		
		// �g�������̃{�����[������
		ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/SE/DungeonSceneEnemyDead.ogg", 180);
	}

	void EnemyPawn::adjustChildsMeshes(float deltaTime) {
		// �����g�łӂ�ӂ퓮�����v�Z
		hoverSinTimer_ += deltaTime;
		float hoverMoveY = sin(hoverSinTimer_) * hoverSinAmplitude_; // sin �� 0~1�������I�ɕԂ��Ă���̂ŁA����ɐU�ꕝ��������

		auto rootMesh = getRootMesh();
		rootMesh->pos_.y = ENEMY_POS_Y + hoverMoveY;
		auto& childs = getChildMeshes();
		// rightHone
		childs[0]->rot_ = tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(-45)) * rootMesh->rot_;
		childs[0]->pos_ = rootMesh->pos_ + tnl::Vector3::TransformCoord({ ENEMY_SIZE_RADIUS / 1.2f,ENEMY_SIZE_RADIUS / 1.2f,0 }, { rootMesh->rot_ });
		// leftHone
		childs[1]->rot_ = tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(45)) * rootMesh->rot_;
		childs[1]->pos_ = rootMesh->pos_ + tnl::Vector3::TransformCoord({ -ENEMY_SIZE_RADIUS / 1.2f,ENEMY_SIZE_RADIUS / 1.2f,0 }, { rootMesh->rot_ });
		// core
		childs[2]->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(-1.5f));
		childs[2]->pos_ = rootMesh->pos_;

	}

	void EnemyPawn::renderObjects(const Shared<Atl3DCamera> camera, float deltaTime) {
		adjustChildsMeshes(deltaTime);
		auto& childs = getChildMeshes();
		for (const auto& child : childs) {
			child->render(camera);
		}
	}

	void EnemyPawn::renderTransparentObject(const Shared<Atl3DCamera> camera, float deltaTime) {
		getRootMesh()->render(camera);
	}

	void EnemyPawn::assignWeakDungeonScene(std::weak_ptr<DungeonScene> dungeonScene) {
		weakDungeonScene_ = dungeonScene;
	}

	bool EnemyPawn::isNeighborPlayer() {
		auto& player2Dpos = weakDungeonScene_.lock()->getPlayerPawn()->getPlayer2Dpos();
		auto& enemy2Dpos = get2Dpos();

		if (player2Dpos.x == enemy2Dpos.x + 1 && player2Dpos.y == enemy2Dpos.y) return true;
		else if (player2Dpos.x == enemy2Dpos.x - 1 && player2Dpos.y == enemy2Dpos.y) return true;
		else if (player2Dpos.x == enemy2Dpos.x && player2Dpos.y == enemy2Dpos.y + 1) return true;
		else if (player2Dpos.x == enemy2Dpos.x && player2Dpos.y == enemy2Dpos.y - 1) return true;
		else return false;
	}

	bool EnemyPawn::isCanMove(const tnl::Vector2i& moveToPos) {
		auto newPos = get2Dpos() + moveToPos;

		// �_���W�����Z�����ړ��s�� ( �ǂȂ� ) �Ȃ�ړ��s��
		if (!DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(newPos)) {
			return false;
		}

		// �G�l�~�[��������ړ��s��
		auto& enemies = weakDungeonScene_.lock()->getEnemyArray();
		for (auto& enemy : enemies) {
			if (newPos == enemy->get2Dpos()) return false;
		}

		return true;
	}

	void EnemyPawn::setMoveTarget(const tnl::Vector2i& moveToPos) {
		auto cellLength = DungeonScene::getCellLength();
		auto meshPos = getRootMesh()->pos_;
		// �ړ��\�Ȃ�moveTarget ��ݒ�
		if (isCanMove(moveToPos)) {
			moveTarget_ = { meshPos.x + (moveToPos.x * cellLength),meshPos.y,meshPos.z + (moveToPos.y * cellLength) };
			getRootMesh()->rot_ = tnl::Quaternion::LookAt(getRootMesh()->pos_, moveTarget_, { 0,1,0 });
			add2Dpos(moveToPos);
		}
		else {
			moveTarget_ = meshPos;
		}
	}

	// �v���C���[�Ɠ����G���A�ɂ��邩
	bool EnemyPawn::isSameAreaPlayer() {
		auto dungeonCreater = DungeonCreater::getDungeonCreater();
		auto& player2Dpos = weakDungeonScene_.lock()->getPlayerPawn()->getPlayer2Dpos();

		auto enemyAreaID = dungeonCreater->getFieldCellID(get2Dpos());
		auto playerAreaID = dungeonCreater->getFieldCellID(player2Dpos);

		// �����̃G���AID �ƃv���C���[�̃G���A ID����v�����ꍇ�Atrue
		if (enemyAreaID == playerAreaID) return true;

		return false;
	}

	/// --------------------------
	/// �V�[�P���X
	/// --------------------------

	bool EnemyPawn::seqStateTransition(float deltaTime) {
		if (!isAlreadyAction_ || !isAlreadyMove_) {

			// HP���[���ɂȂ��Ă���ꍇ
			if (enemyData_->isZeroHP()) currentState_ = e_EnemyState::Deading;
			// �v���C���[�Ɨאڂ��Ă����ꍇ
			else if (isNeighborPlayer()) currentState_ = e_EnemyState::PlayerNeighboring;
			// �v���C���[�Ɠ����G���A�ɂ���ꍇ
			else if (isSameAreaPlayer()) currentState_ = e_EnemyState::ChasePlayer;
			// ����ȊO�̏ꍇ
			else currentState_ = e_EnemyState::Wandering;

			switch (currentState_) {
			case e_EnemyState::Wandering:			seq_.change(&EnemyPawn::seqWandering); break;
			case e_EnemyState::PlayerNeighboring:	seq_.change(&EnemyPawn::seqPlayerNeighboring); break;
			case e_EnemyState::ChasePlayer:			seq_.change(&EnemyPawn::seqChasePlayer); break;
			case e_EnemyState::Deading:				seq_.change(&EnemyPawn::seqDeading); break;
			case e_EnemyState::Dead:				break; // ���񂾏ꍇ�A�_���W�����V�[�����ō폜�����̂ŁA������ł͉�������
			}
		}
		return true;
	}

	bool EnemyPawn::seqDeading(float deltaTime) {
		// 0.3�b�ԁA���邭���]
		SEQ_CO_YIELD_RETURN_TIME(0.3f, deltaTime, [&] {
			getRootMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(10));
		})

		// ��񉹂�炵�A���� 2.2 �b�ԉ�]
		SEQ_CO_YIELD_RETURN_TIME(2.2f, deltaTime, [&] {
			if (SEQ_CO_YIELD_TIME_IS_START) { ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneEnemyDead.ogg", DX_PLAYTYPE_BACK); }
			getRootMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(10));
			});

		isAlreadyAction_ = true;
		isAlreadyMove_ = true;
		currentState_ = e_EnemyState::Dead;

		auto& enemyName = getEnemyData()->getEnemyName();
		TextLogManager::getTextLogManager()->addTextLog(enemyName + "��|�����I");

		SEQ_CO_END
	}
	
	bool EnemyPawn::seqWandering(float deltaTime) {
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
		seq_.change(&EnemyPawn::seqMoveToTarget);
		return true;
	}

	// �v���C���[�𒼐��Œǂ� 
	bool EnemyPawn::seqChasePlayer(float deltaTime) {
		auto& player2Dpos = weakDungeonScene_.lock()->getPlayerPawn()->getPlayer2Dpos();
		auto& currentPos = get2Dpos();

		if (player2Dpos.x > currentPos.x && isCanMove({1,0})) {
			setMoveTarget({ 1,0 });
		}
		else if (player2Dpos.x < currentPos.x && isCanMove({ -1, 0 })) {
			setMoveTarget({ -1,0 });
		}
		else if (player2Dpos.y > currentPos.y && isCanMove({0,1})) {
			setMoveTarget({ 0,1 });
		}
		else if (player2Dpos.y < currentPos.y && isCanMove({0,-1})) {
			setMoveTarget({ 0,-1 });
		}
		else {
			setMoveTarget({ 0,0 });
		}
		
		seq_.change(&EnemyPawn::seqMoveToTarget);
		return true;
	}

	bool EnemyPawn::seqMoveToTarget(float deltaTime) {
		getRootMesh()->pos_ = tnl::Vector3::DecelLerp(getRootMesh()->pos_, moveTarget_, MOVE_TIME, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		tnl::Vector3 moveVector = moveTarget_ - getRootMesh()->pos_;
		float length = moveVector.length();

		if (length <= MOVE_END_BORDER) {
			moveLerpTimeCount_ = 0;
			isAlreadyMove_ = true;
			seq_.change(&EnemyPawn::seqActionNone);
		}

		return true;
	}

	bool EnemyPawn::seqActionNone(float deltaTime) {
		isAlreadyAction_ = true;
		seq_.change(&EnemyPawn::seqStateTransition);
		return true;
	}

	bool EnemyPawn::seqPlayerNeighboring(float deltaTime) {
		auto player = weakDungeonScene_.lock()->getPlayerPawn();
		if (player->getIsAlreadyTurn()) {
			// �v���C���[�̍s�����I����Ă���A�v���C���[�̕�������
			auto meshPos = getRootMesh()->pos_;
			getRootMesh()->rot_ = tnl::Quaternion::LookAt(meshPos, player->getPlayerPos(), { 0,1,0 });
			seq_.change(&EnemyPawn::seqMoveNone);
		}
		return true;
	}

	bool EnemyPawn::seqMoveNone(float deltaTime) {
		isAlreadyMove_ = true;
		seq_.change(&EnemyPawn::seqActionAttack);
		return true;
	}

	bool EnemyPawn::seqActionAttack(float deltaTime) {
		// 0.5�b�ԁA�N����̍U���Ȃ̂���\��
		SEQ_CO_YIELD_RETURN_TIME(0.5f, deltaTime, [&] {
			auto player = weakDungeonScene_.lock()->getPlayerPawn();
			if (player->getIsAlreadyTurn()) {
				if (SEQ_CO_YIELD_TIME_IS_START) {
					auto& enemyName = getEnemyData()->getEnemyName();
					TextLogManager::getTextLogManager()->addTextLog(enemyName + "�̂��������I");
					ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneEnemyAttack.ogg", DX_PLAYTYPE_BACK);
				}
			}
		});

		// �U������
		auto player = weakDungeonScene_.lock()->getPlayerPawn();
		if (player->getIsAlreadyTurn()) {
			auto damage = player->getPlayerData()->damaged(enemyData_->getAttackPower());
			TextLogManager::getTextLogManager()->addTextLog("�v���C���[��" + convertFullWidthNumber(damage) + "�̃_���[�W���󂯂�");
			isAlreadyAction_ = true;
			seq_.change(&EnemyPawn::seqStateTransition);
		}

		SEQ_CO_END;
	}


}