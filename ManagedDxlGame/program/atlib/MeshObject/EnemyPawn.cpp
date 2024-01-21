#include "EnemyPawn.h"
#include "../Scenes/DungeonScene.h"
#include "../Utilities/AtlRandom.h"

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

	void EnemyPawn::adjustmentChildMeshes() {
		auto rootMesh = getRootMesh();
		auto& childs = getChildMeshes();

		childs[0]->rot_ = rootMesh->rot_;
		childs[0]->pos_ = rootMesh->pos_ + tnl::Vector3::TransformCoord(tnl::Vector3{ 0,0,enemySize_.z / 2 }, childs[0]->rot_);
	}

	bool EnemyPawn::checkIsCanMovePos(const tnl::Vector2i& moveToPos) {
		auto cellLength = DungeonScene::getCellLength();
		if (DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(get2Dpos() + moveToPos)) {
			auto meshPos = getRootMesh()->pos_;
			moveTarget_ = { meshPos.x + (moveToPos.x * cellLength),meshPos.y,meshPos.z + (moveToPos.y * cellLength) };
			getRootMesh()->rot_ = tnl::Quaternion::LookAt({ 0,0,0 }, { static_cast<float>(moveToPos.x),0,static_cast<float>(moveToPos.y) }, { 0,1,0 });
			add2Dpos(moveToPos);
			return true;
		}
		else {
			moveTarget_ = getRootMesh()->pos_;
			return true;
		}
		return false;
	}

	bool EnemyPawn::moveLerp(float deltaTime) {
		getRootMesh()->pos_ = tnl::Vector3::DecelLerp(getRootMesh()->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&EnemyPawn::seqCheckCurrentState);
			isAlreadyAction = true;
			return true;
		}
		return false;
	}

	/// --------------------------
	/// シーケンス
	/// --------------------------

	bool EnemyPawn::seqCheckCurrentState(float deltaTime) {
		isAlreadyAction = false;
		switch (currentState_) {
		case e_EnemyState::Wandering:	seq_.change(&EnemyPawn::seqWandering); break;
		case e_EnemyState::Searching:; break;
		case e_EnemyState::Chasing:; break;
		}
		return false;
	}

	bool EnemyPawn::seqWandering(float deltaTime) {
		// 現状 : 乱数でランダムに四方移動
		int32_t rand = mtRandomRangeInt(0, 3);
		switch (rand) {
		case 0:
			seq_.change(&EnemyPawn::seqMoveZplus);
			break;
		case 1:
			seq_.change(&EnemyPawn::seqMoveZminus);
			break;
		case 2:
			seq_.change(&EnemyPawn::seqMoveXplus);
			break;
		case 3:
			seq_.change(&EnemyPawn::seqMoveXminus);
		}
		return false;
	}

	bool EnemyPawn::seqMoveZplus(float deltaTime) {
		if (seq_.isStart()) {
			checkIsCanMovePos({ 0,1 });
		}
		return moveLerp(deltaTime);

	}

	bool EnemyPawn::seqMoveZminus(float deltaTime) {
		if (seq_.isStart()) {
			checkIsCanMovePos({ 0,-1 });
		}
		return moveLerp(deltaTime);
	}

	bool EnemyPawn::seqMoveXplus(float deltaTime) {
		if (seq_.isStart()) {
			checkIsCanMovePos({ 1,0 });
		}
		return moveLerp(deltaTime);
	}

	bool EnemyPawn::seqMoveXminus(float deltaTime) {
		if (seq_.isStart()) {
			checkIsCanMovePos({ -1,0 });
		}
		return moveLerp(deltaTime);
	}

}