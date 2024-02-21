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
	/// ベースエネミー
	/// 関数
	/// 
	/// </summary>

	bool Base_Enemy::isCanMove(const tnl::Vector2i& moveToPos) {
		// 移動先の座標
		auto newPos = get2Dpos() + moveToPos;

		// ダンジョンセルが移動不可 ( 壁など ) なら移動不可
		if (!DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(newPos)) {
			return false;
		}

		// 移動先に、別のエネミーがいたら移動不可
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) {
			if (newPos == enemy->get2Dpos()) { return false; }
		}

		return true;
	}

	void Base_Enemy::setMoveTarget(const tnl::Vector2i& moveToPos) {
		auto cellLength = DungeonScene::getCellLength();
		auto meshPos = getMesh()->pos_;

		// 移動可能ならmoveTarget を 設定
		if (isCanMove(moveToPos)) {

			moveTarget_ = {
				meshPos.x + (moveToPos.x * cellLength),
				meshPos.y,
				meshPos.z + (moveToPos.y * cellLength) };

			getMesh()->rot_ = tnl::Quaternion::LookAt(getMesh()->pos_, moveTarget_, { 0,1,0 });
			add2Dpos(moveToPos);
		}
		// 移動不能なら moveTarget をその場に設定
		else {
			moveTarget_ = meshPos;
		}
	}

	bool Base_Enemy::isNeighborPlayer() {
		// プレイヤーと隣接しているか判定する
		auto& player2Dpos = weakDungeonScene_.lock()->getPlayerPawn()->getPlayer2Dpos();
		auto& enemy2Dpos = get2Dpos();

		// 四近傍にプレイヤーがいるかを判定。いたら true
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

		// 自分のエリアID とプレイヤーのエリア IDが一致した場合、true
		if (enemyAreaID == playerAreaID) { return true; }

		return false;
	}

	/// <summary>
	/// 
	/// ベースエネミー
	/// シーケンス関数
	/// 
	/// <summary>

	bool Base_Enemy::seqTransition(float deltaTime) {
		// 既に移動もしくは行動が完了している場合、早期リターン ( 待機状態 )
		if (isAlreadyAction_ || isAlreadyMove_) { return false; }

		// HPがゼロになっている場合
		if (enemyData_->isZeroHP()) { seq_.change(&Base_Enemy::seqDeading); }

		// プレイヤーと隣接していた場合
		else if (isNeighborPlayer()) { seq_.change(&Base_Enemy::seqPlayerNeighboring); }

		// プレイヤーと同じエリアにいる場合
		else if (isSameAreaPlayer()) { seq_.change(&Base_Enemy::seqChasePlayer); }

		// それ以外の場合
		else { seq_.change(&Base_Enemy::seqWandering); }

		return true;
	}

	bool Base_Enemy::seqWandering(float deltaTime) {
		// 0 ~ 3 までの四択 ( 四方向 ) をランダムに決定し、そちらに移動する
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

	// プレイヤーを直線で追う 
	bool Base_Enemy::seqChasePlayer(float deltaTime) {
		auto& player2Dpos = weakDungeonScene_.lock()->getPlayerPawn()->getPlayer2Dpos();
		auto& currentPos = get2Dpos();

		// X座標 → Y座標、の順でプレイヤーを追いかける。
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

		// MOVE_END_BORDER より目的地との距離が近くなった ( ほぼ移動が終了した ) 場合
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
			// プレイヤーの行動が終わってから、プレイヤーの方を向く
			auto meshPos = getMesh()->pos_;
			getMesh()->rot_ = tnl::Quaternion::LookAt(meshPos, player->getPlayerPos(), { 0,1,0 });
			seq_.change(&Base_Enemy::seqMoveNone);
		}
		return true;
	}

	bool Base_Enemy::seqDeading(float deltaTime) {
		// 0.2秒間、くるくる回転
		SEQ_CO_YIELD_RETURN_TIME(0.2f, deltaTime, [&] {
			getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(15));
		});

		// 一回音を鳴らし、もう 1 秒間回転
		SEQ_CO_YIELD_RETURN_TIME(1.0f, deltaTime, [&] {
			if (SEQ_CO_YIELD_TIME_IS_START) { 
				ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneEnemyDead.ogg", DX_PLAYTYPE_BACK); 
			}

			getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(15));
		});

		// 行動済みと死亡フラグをオンに
		isAlreadyAction_ = true;
		isAlreadyMove_ = true;
		isDead_ = true;

		// テキストログに出力
		auto& enemyName = getEnemyData()->getEnemyName();
		TextLogManager::getTextLogManager()->addTextLog(enemyName + "を倒した！");

		SEQ_CO_END
	}

	bool Base_Enemy::seqMoveNone(float deltaTime) {
		isAlreadyMove_ = true;
		seq_.change(&Base_Enemy::seqActionAttack);
		return true;
	}

	bool Base_Enemy::seqActionAttack(float deltaTime) {
		auto player = weakDungeonScene_.lock()->getPlayerPawn();

		// プレイヤーが行動終了していなければ、早期リターン
		if (!player->getIsAlreadyTurn()) { return true; }

		// 誰からの攻撃なのかをテキストログに出力し、0.5秒間待機
		SEQ_CO_YIELD_RETURN_TIME(0.5f, deltaTime, [&] {
			if (SEQ_CO_YIELD_TIME_IS_START) {
				auto& enemyName = getEnemyData()->getEnemyName();
				TextLogManager::getTextLogManager()->addTextLog(enemyName + "のこうげき！");
				ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneEnemyAttack.ogg", DX_PLAYTYPE_BACK);
			}
			});

		// 攻撃処理。プレイヤーにダメージを与え、ログに出力
		auto damage = Base_ActorData::damaged(*enemyData_, *player->getPlayerData());
		TextLogManager::getTextLogManager()->addTextLog("プレイヤーは" + convertFullWidthNumber(damage) + "のダメージを受けた");
		isAlreadyAction_ = true;
		seq_.change(&Base_Enemy::seqTransition);

		SEQ_CO_END;
	}

	/// <summary>
	/// 
	/// スライム
	/// 
	/// <summary>

	void SlimeEnemy::initialize() {
		// メッシュを設定
		auto mesh = dxe::Mesh::CreateFromFileMV("mesh/mv1/SlimeMesh.mv1", 5);
		setMesh(mesh);

		// テクスチャを設定
		auto texture = dxe::Texture::CreateFromFile("graphics/Texture/EnemyTexture/Albedo.png");
		mesh->setTexture(texture);

		// マテリアルを設定
		mesh->loadMaterial(getEnemyData()->getEnemyMaterialPath());
	}

}