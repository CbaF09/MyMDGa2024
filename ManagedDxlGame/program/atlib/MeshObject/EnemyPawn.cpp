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
		
		// 使う音源のボリューム調整
		ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/SE/DungeonSceneEnemyDead.ogg", 180);
	}

	void EnemyPawn::adjustChildsMeshes(float deltaTime) {
		// 正弦波でふわふわ動かす計算
		hoverSinTimer_ += deltaTime;
		float hoverMoveY = sin(hoverSinTimer_) * hoverSinAmplitude_; // sin で 0~1が周期的に返ってくるので、それに振れ幅をかける

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

		// ダンジョンセルが移動不可 ( 壁など ) なら移動不可
		if (!DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(newPos)) {
			return false;
		}

		// エネミーがいたら移動不可
		auto& enemies = weakDungeonScene_.lock()->getEnemyArray();
		for (auto& enemy : enemies) {
			if (newPos == enemy->get2Dpos()) return false;
		}

		return true;
	}

	void EnemyPawn::setMoveTarget(const tnl::Vector2i& moveToPos) {
		auto cellLength = DungeonScene::getCellLength();
		auto meshPos = getRootMesh()->pos_;
		// 移動可能ならmoveTarget を設定
		if (isCanMove(moveToPos)) {
			moveTarget_ = { meshPos.x + (moveToPos.x * cellLength),meshPos.y,meshPos.z + (moveToPos.y * cellLength) };
			getRootMesh()->rot_ = tnl::Quaternion::LookAt(getRootMesh()->pos_, moveTarget_, { 0,1,0 });
			add2Dpos(moveToPos);
		}
		else {
			moveTarget_ = meshPos;
		}
	}

	// プレイヤーと同じエリアにいるか
	bool EnemyPawn::isSameAreaPlayer() {
		auto dungeonCreater = DungeonCreater::getDungeonCreater();
		auto& player2Dpos = weakDungeonScene_.lock()->getPlayerPawn()->getPlayer2Dpos();

		auto enemyAreaID = dungeonCreater->getFieldCellID(get2Dpos());
		auto playerAreaID = dungeonCreater->getFieldCellID(player2Dpos);

		// 自分のエリアID とプレイヤーのエリア IDが一致した場合、true
		if (enemyAreaID == playerAreaID) return true;

		return false;
	}

	/// --------------------------
	/// シーケンス
	/// --------------------------

	bool EnemyPawn::seqStateTransition(float deltaTime) {
		if (!isAlreadyAction_ || !isAlreadyMove_) {

			// HPがゼロになっている場合
			if (enemyData_->isZeroHP()) currentState_ = e_EnemyState::Deading;
			// プレイヤーと隣接していた場合
			else if (isNeighborPlayer()) currentState_ = e_EnemyState::PlayerNeighboring;
			// プレイヤーと同じエリアにいる場合
			else if (isSameAreaPlayer()) currentState_ = e_EnemyState::ChasePlayer;
			// それ以外の場合
			else currentState_ = e_EnemyState::Wandering;

			switch (currentState_) {
			case e_EnemyState::Wandering:			seq_.change(&EnemyPawn::seqWandering); break;
			case e_EnemyState::PlayerNeighboring:	seq_.change(&EnemyPawn::seqPlayerNeighboring); break;
			case e_EnemyState::ChasePlayer:			seq_.change(&EnemyPawn::seqChasePlayer); break;
			case e_EnemyState::Deading:				seq_.change(&EnemyPawn::seqDeading); break;
			case e_EnemyState::Dead:				break; // 死んだ場合、ダンジョンシーン側で削除されるので、こちらでは何も無し
			}
		}
		return true;
	}

	bool EnemyPawn::seqDeading(float deltaTime) {
		// 0.3秒間、くるくる回転
		SEQ_CO_YIELD_RETURN_TIME(0.3f, deltaTime, [&] {
			getRootMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(10));
		})

		// 一回音を鳴らし、もう 2.2 秒間回転
		SEQ_CO_YIELD_RETURN_TIME(2.2f, deltaTime, [&] {
			if (SEQ_CO_YIELD_TIME_IS_START) { ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneEnemyDead.ogg", DX_PLAYTYPE_BACK); }
			getRootMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(10));
			});

		isAlreadyAction_ = true;
		isAlreadyMove_ = true;
		currentState_ = e_EnemyState::Dead;

		auto& enemyName = getEnemyData()->getEnemyName();
		TextLogManager::getTextLogManager()->addTextLog(enemyName + "を倒した！");

		SEQ_CO_END
	}
	
	bool EnemyPawn::seqWandering(float deltaTime) {
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
		seq_.change(&EnemyPawn::seqMoveToTarget);
		return true;
	}

	// プレイヤーを直線で追う 
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
		// 0.5秒間、誰からの攻撃なのかを表示
		SEQ_CO_YIELD_RETURN_TIME(0.5f, deltaTime, [&] {
			auto player = weakDungeonScene_.lock()->getPlayerPawn();
			if (player->getIsAlreadyTurn()) {
				if (SEQ_CO_YIELD_TIME_IS_START) {
					auto& enemyName = getEnemyData()->getEnemyName();
					TextLogManager::getTextLogManager()->addTextLog(enemyName + "のこうげき！");
					ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneEnemyAttack.ogg", DX_PLAYTYPE_BACK);
				}
			}
		});

		// 攻撃処理
		auto player = weakDungeonScene_.lock()->getPlayerPawn();
		if (player->getIsAlreadyTurn()) {
			auto damage = player->getPlayerData()->damaged(enemyData_->getAttackPower());
			TextLogManager::getTextLogManager()->addTextLog("プレイヤーは" + convertFullWidthNumber(damage) + "のダメージを受けた");
			isAlreadyAction_ = true;
			seq_.change(&EnemyPawn::seqStateTransition);
		}

		SEQ_CO_END;
	}


}