#include "EnemyPawn.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	EnemyPawn::EnemyPawn(const tnl::Vector3& enemyPos, const tnl::Vector3& enemySize) : enemySize_(enemySize){
		auto rootMesh = dxe::Mesh::CreateBoxMV(
			enemySize,
			dxe::Texture::CreateFromFile("graphics/box/box_left.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_right.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_up.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_down.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_forward.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_back.bmp")
		);
		rootMesh->pos_ = enemyPos;
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

	void EnemyPawn::enemyUpdate(float deltaTime) {
		seq_.update(deltaTime);
	}

	/// --------------------------
	/// シーケンス
	/// --------------------------

	bool EnemyPawn::seqCheckCurrentState(float deltaTime) {
		switch (currentState_) {
		case e_EnemyState::Wandering:	seq_.change(&EnemyPawn::seqWandering); break;
		case e_EnemyState::Searching:; break;
		case e_EnemyState::Chasing:; break;
			return true;
		}
	}

	bool EnemyPawn::seqWandering(float deltaTime) {

		{// デバッグ用インプット
			if (tnl::Input::IsKeyDown(eKeys::KB_NUMPAD8)) {
				seq_.change(&EnemyPawn::seqMoveZplus);
			}
			if (tnl::Input::IsKeyDown(eKeys::KB_NUMPAD2)) {
				seq_.change(&EnemyPawn::seqMoveZminus);
			}
			if (tnl::Input::IsKeyDown(eKeys::KB_NUMPAD6)) {
				seq_.change(&EnemyPawn::seqMoveXplus);
			}
			if (tnl::Input::IsKeyDown(eKeys::KB_NUMPAD4)) {
				seq_.change(&EnemyPawn::seqMoveXminus);
			}
		}

		return true;
	}

	bool EnemyPawn::seqMoveZplus(float deltaTime) {
		if (seq_.isStart()) {
			moveTarget_ = { getRootMesh()->pos_.x,getRootMesh()->pos_.y,getRootMesh()->pos_.z + DungeonScene::getCellLength()};
			getRootMesh()->rot_ = tnl::Quaternion::LookAt({0,0,0}, {0,0,1}, {0,1,0});
		}
		getRootMesh()->pos_ = tnl::Vector3::DecelLerp(getRootMesh()->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&EnemyPawn::seqCheckCurrentState);
		}
		return true;
	}

	bool EnemyPawn::seqMoveZminus(float deltaTime) {
		if (seq_.isStart()) {
			moveTarget_ = { getRootMesh()->pos_.x,getRootMesh()->pos_.y,getRootMesh()->pos_.z - DungeonScene::getCellLength() };
			getRootMesh()->rot_ = tnl::Quaternion::LookAt({0,0,0}, {0,0,-1}, {0,1,0});
		}
		getRootMesh()->pos_ = tnl::Vector3::DecelLerp(getRootMesh()->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&EnemyPawn::seqCheckCurrentState);
		}
		return true;
	}

	bool EnemyPawn::seqMoveXplus(float deltaTime) {
		if (seq_.isStart()) {
			moveTarget_ = { getRootMesh()->pos_.x + DungeonScene::getCellLength(),getRootMesh()->pos_.y,getRootMesh()->pos_.z  };
			getRootMesh()->rot_ = tnl::Quaternion::LookAt({0,0,0}, {1,0,0}, {0,1,0});
		}
		getRootMesh()->pos_ = tnl::Vector3::DecelLerp(getRootMesh()->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&EnemyPawn::seqCheckCurrentState);
		}
		return true;
	}

	bool EnemyPawn::seqMoveXminus(float deltaTime) {
		if (seq_.isStart()) {
			moveTarget_ = { getRootMesh()->pos_.x - DungeonScene::getCellLength(),getRootMesh()->pos_.y,getRootMesh()->pos_.z };
			getRootMesh()->rot_ = tnl::Quaternion::LookAt({0,0,0}, {-1,0,0}, {0,1,0});
		}
		getRootMesh()->pos_ = tnl::Vector3::DecelLerp(getRootMesh()->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		if (moveLerpTimeCount_ >= MOVE_LERP_TIME_) {
			moveLerpTimeCount_ = 0;
			seq_.change(&EnemyPawn::seqCheckCurrentState);
		}
		return true;
	}

}