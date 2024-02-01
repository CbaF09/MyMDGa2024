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

	void EnemyPawn::adjustmentChildMeshes() {
		auto rootMesh = getRootMesh();
		auto& childs = getChildMeshes();

		childs[0]->rot_ = rootMesh->rot_;
		childs[0]->pos_ = rootMesh->pos_ + tnl::Vector3::TransformCoord(tnl::Vector3{ 0,0,enemySize_.z / 2 }, childs[0]->rot_);
	}

	const tnl::Vector2i& EnemyPawn::searchPlayerPos(Shared<PlayerPawn> player) {
		return player->getPlayer2Dpos();
	}

	bool EnemyPawn::isNeighborPlayer(Shared<PlayerPawn> player) {
		auto& player2Dpos = searchPlayerPos(player);
		auto& enemy2Dpos = get2Dpos();

		if (player2Dpos.x == enemy2Dpos.x + 1 && player2Dpos.y == enemy2Dpos.y) return true;
		else if (player2Dpos.x == enemy2Dpos.x - 1 && player2Dpos.y == enemy2Dpos.y) return true;
		else if (player2Dpos.x == enemy2Dpos.x && player2Dpos.y == enemy2Dpos.y + 1) return true;
		else if (player2Dpos.x == enemy2Dpos.x && player2Dpos.y == enemy2Dpos.y - 1) return true;
		else return false;
	}

	bool EnemyPawn::isCanMovePos(const tnl::Vector2i& moveToPos) {
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



	/// --------------------------
	/// シーケンス
	/// --------------------------

	bool EnemyPawn::seqCheckCurrentState(float deltaTime) {
		switch (currentState_) {
		case e_EnemyState::Wandering: seq_.change(&EnemyPawn::seqWandering); break;
		case e_EnemyState::PlayerNeighboring: seq_.change(&EnemyPawn::seqPlayerNeighboring); break;
		}
		return true;
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
		return true;
	}



	bool EnemyPawn::seqMoveZplus(float deltaTime) {
		if (seq_.isStart()) {
			isCanMovePos({ 0,1 });
		}
		moveLerp(deltaTime);
		return true;
	}

	bool EnemyPawn::seqMoveZminus(float deltaTime) {
		if (seq_.isStart()) {
			isCanMovePos({ 0,-1 });
		}
		moveLerp(deltaTime);

		return true;
	}

	bool EnemyPawn::seqMoveXplus(float deltaTime) {
		if (seq_.isStart()) {
			isCanMovePos({ 1,0 });
		}
		moveLerp(deltaTime);
		return true;
	}

	bool EnemyPawn::seqMoveXminus(float deltaTime) {
		if (seq_.isStart()) {
			isCanMovePos({ -1,0 });
		}
		moveLerp(deltaTime);
		return true;
	}

	void EnemyPawn::moveLerp(float deltaTime) {
		getRootMesh()->pos_ = tnl::Vector3::DecelLerp(getRootMesh()->pos_, moveTarget_, MOVE_LERP_TIME_, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		tnl::Vector3 moveVector = moveTarget_ - getRootMesh()->pos_;
		float length = moveVector.length();

		if (length <= 0.01f) {
			moveLerpTimeCount_ = 0;
			isAlreadyAction = true;
			seq_.change(&EnemyPawn::seqCheckCurrentState);
		}
	}

	bool EnemyPawn::seqPlayerNeighboring(float deltaTime) {
		// デバッグ用 仮でSEが鳴る
		PlaySoundFile("sound/test_se.wav", 2);
		seq_.change(&EnemyPawn::seqCheckCurrentState);
		isAlreadyAction = true;
		return true;
	}

}