#include "EnemyPawn.h"
#include "../Scenes/DungeonScene.h"
#include "../Singletons/DungeonCreater.h"
#include "../Utilities/AtlRandom.h"
#include "PlayerPawn.h"

namespace atl {

	EnemyPawn::EnemyPawn(const tnl::Vector2i& enemyPos, const tnl::Vector3& enemySize) : enemySize_(enemySize){
		set2Dpos(enemyPos);
		auto cellLength = DungeonScene::getCellLength();
		tnl::Vector3 enemy3Dpos = { static_cast<float>(enemyPos.x * cellLength), 400 ,static_cast<float>(enemyPos.y * cellLength) };

		auto rootMesh = dxe::Mesh::CreateBoxMV(
			enemySize,
			dxe::Texture::CreateFromFile("graphics/box/box_left.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_right.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_up.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_down.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_forward.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_back.bmp")
		);
		rootMesh->pos_ = enemy3Dpos;
		setRootMesh(rootMesh);

		auto dirArrow = dxe::Mesh::CreateBoxMV(
			enemySize / 2,
			dxe::Texture::CreateFromFile("graphics/box/box_left.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_right.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_up.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_down.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_forward.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_back.bmp")
		);
		dirArrow->pos_ = rootMesh->pos_ + tnl::Vector3{ 0, 0, enemySize_.z / 2 };
		addChildMesh(dirArrow);

	}

	void EnemyPawn::adjustChildsMeshes() {
		auto rootMesh = getRootMesh();
		auto& childs = getChildMeshes();

		childs[0]->rot_ = rootMesh->rot_;
		childs[0]->pos_ = rootMesh->pos_ + tnl::Vector3::TransformCoord(tnl::Vector3{ 0,0,enemySize_.z / 2 }, childs[0]->rot_);
	}

	void EnemyPawn::assignWeakPlayer(std::weak_ptr<PlayerPawn> player) {
		weakPlayer = player;
	}

	bool EnemyPawn::isNeighborPlayer() {
		auto& player2Dpos = weakPlayer.lock()->getPlayer2Dpos();
		auto& enemy2Dpos = get2Dpos();

		if (player2Dpos.x == enemy2Dpos.x + 1 && player2Dpos.y == enemy2Dpos.y) return true;
		else if (player2Dpos.x == enemy2Dpos.x - 1 && player2Dpos.y == enemy2Dpos.y) return true;
		else if (player2Dpos.x == enemy2Dpos.x && player2Dpos.y == enemy2Dpos.y + 1) return true;
		else if (player2Dpos.x == enemy2Dpos.x && player2Dpos.y == enemy2Dpos.y - 1) return true;
		else return false;
	}

	bool EnemyPawn::assignMoveTarget(const tnl::Vector2i& moveToPos) {
		auto cellLength = DungeonScene::getCellLength();
		// 移動先が移動可能か確認
		if (DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(get2Dpos() + moveToPos)) {
			auto meshPos = getRootMesh()->pos_;
			moveTarget_ = { meshPos.x + (moveToPos.x * cellLength),meshPos.y,meshPos.z + (moveToPos.y * cellLength) };
			getRootMesh()->rot_ = tnl::Quaternion::LookAt({ 0,0,0 }, { static_cast<float>(moveToPos.x),0,static_cast<float>(moveToPos.y) }, { 0,1,0 });
			add2Dpos(moveToPos);
			return true;
		}
		// 移動不可能な場合、現在位置をmoveTargetに ( 足踏み )
		else {
			moveTarget_ = getRootMesh()->pos_;
			return true;
		}
		return false;
	}

	/// --------------------------
	/// シーケンス
	/// --------------------------

	bool EnemyPawn::seqStateTransition(float deltaTime) {
		if (!isAlreadyAction_ || !isAlreadyMove_) {

			// プレイヤーと隣接していた場合
			if (isNeighborPlayer()) turnState_ = e_EnemyMoveState::PlayerNeighboring;
			// それ以外の場合
			else turnState_ = e_EnemyMoveState::Wandering;

			switch (turnState_) {
			case e_EnemyMoveState::Wandering:			seq_.change(&EnemyPawn::seqWandering); break;
			case e_EnemyMoveState::PlayerNeighboring:	seq_.change(&EnemyPawn::seqPlayerNeighboring); break;
			}
		}
		return true;
	}
	
	// 乱数でランダムに四方移動
	bool EnemyPawn::seqWandering(float deltaTime) {
		int32_t rand = mtRandomRangeInt(0, 3);
		switch (rand) {
		case 0: 
			assignMoveTarget({ 0,1 });
			break;
		case 1:
			assignMoveTarget({ 0,-1 });
			break;
		case 2:
			assignMoveTarget({ 1,0 });
			break;
		case 3:
			assignMoveTarget({ -1,0 });
		}
		seq_.change(&EnemyPawn::seqMoveToTarget);
		return true;
	}

	bool EnemyPawn::seqMoveToTarget(float deltaTime) {
		getRootMesh()->pos_ = tnl::Vector3::DecelLerp(getRootMesh()->pos_, moveTarget_, MOVE_TIME, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		tnl::Vector3 moveVector = moveTarget_ - getRootMesh()->pos_;
		float length = moveVector.length();

		if (length <= 0.01f) {
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
		auto player = weakPlayer.lock();
		if (player->getIsAlreadyTurn()) {
			// プレイヤーの行動が終わってから、プレイヤーの方を向く
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
		if (weakPlayer.lock()->getIsAlreadyTurn()) {

			PlaySoundFile("sound/test_se.wav", 2);
			isAlreadyAction_ = true;
			seq_.change(&EnemyPawn::seqStateTransition);
		}
		return true;
	}


}