#include "PlayerPawn.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Scenes/DungeonScene.h"
#include "../MeshObject/MagicWand.h"
#include "../MeshObject/MenuBook.h"
#include "../MeshObject/ForwardArrow.h"
#include "../Utilities/AtlString.h"
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
		moveTarget_ = initPos;

		setPlayerAndCamera3Dpos(initPos);
	}

	bool PlayerPawn::isCanMovePos(const tnl::Vector2i& moveToPos) {
		auto newPlayerPos = player2Dpos_ + moveToPos;

		// ダンジョンセルが移動不可 ( 壁など ) なら移動不可
		if (!DungeonCreater::getDungeonCreater()->isCanMoveFieldCellPos(newPlayerPos)) {
			return false;
		}

		// エネミーがいたら移動不可
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
		// カメラが保持する正面方向を取得
		// 前進 ( W ) => そのまま
		// 後退 ( S ) => 前進を反転
		// 右 ( D ) => 正面方向と Y+軸 で外積を取る
		// 左 ( A ) => 右を反転

		auto forwardNormalDir = playerCamera_->getCurrentForwardDir();
		tnl::Vector2i forwardV2i = { static_cast<int>(forwardNormalDir.x),static_cast<int>(forwardNormalDir.z) };
		tnl::Vector2i backV2i = -forwardV2i;
		auto rightNormalDir = tnl::Vector3::Cross( { 0,1,0 },forwardNormalDir);
		tnl::Vector2i rightV2i = { static_cast<int>(rightNormalDir.x),static_cast<int>(rightNormalDir.z) };
		tnl::Vector2i leftV2i = -rightV2i;

		if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			if (isCanMovePos(forwardV2i)) {	setMoveTarget(forwardV2i); }
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			if (isCanMovePos(backV2i)) { setMoveTarget(backV2i);}
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			if (isCanMovePos(rightV2i)) { setMoveTarget(rightV2i); }
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			if (isCanMovePos(leftV2i)) { setMoveTarget(leftV2i); }
		}

		seq_.change(&PlayerPawn::actionMoveLerp);
	}

	// ---------------------------
	// シーケンス
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
	
	bool PlayerPawn::actionMoveLerp(float deltaTime) {
		if (seq_.isStart()) {
			// moveTarget が設定されていない場合、例外処理
			if (moveTarget_.x == 0 && moveTarget_.y == 0 && moveTarget_.z == 0) {
//				setMoveTarget(getPlayerPos());
			}
		}
		playerCamera_->pos_ = tnl::Vector3::DecelLerp(playerCamera_->pos_, moveTarget_, MOVE_TIME, moveLerpTimeCount_);
		moveLerpTimeCount_ += deltaTime;

		tnl::Vector3 moveVector = moveTarget_ - playerCamera_->pos_;
		float length = moveVector.length();

		if (length <= MOVE_END_BORDER) {
			moveLerpTimeCount_ = 0;
			isAlreadyTurn_ = true;
			seq_.change(&PlayerPawn::seqWaitKeyInput);
		}
		player3Dpos_ = playerCamera_->pos_;
		return false;
	}

	bool PlayerPawn::actionAttack(float deltaTime) {
		// 最初のフレームで本処理、その後は待機時間
		if (seq_.isStart()) {
			auto& enemies = weakDungeonScene_.lock()->getEnemyArray();
			tnl::Vector2i attackPos = player2Dpos_ + playerCamera_->getCurrentForwardDirToV2i();
			
			// 停止時間 => 空振り
			waitTime_ = ATTACK_MISS_TIME;

			for (auto& enemy : enemies) {
				auto& enemyPos = enemy->get2Dpos();
				if (enemyPos.x == attackPos.x && enemyPos.y == attackPos.y) { 
					// 攻撃ヒット処理

					// 停止時間 => 攻撃ヒット時
					waitTime_ = ATTACK_TIME;

					attackHitEffectAndLog(enemy);
				}
			}
		}

		// waitTimeに設定された時間分、停止する
		totalDeltaTimer_ += deltaTime;
		if (totalDeltaTimer_ > waitTime_) {
			totalDeltaTimer_ = 0.0f;
			// ターン終了
			isAlreadyTurn_ = true;
			seq_.change(&PlayerPawn::seqWaitKeyInput);
		}

		return true;
	}

	void PlayerPawn::attackHitEffectAndLog(const Shared<atl::EnemyPawn>& enemy) {
		auto damage = enemy->getEnemyData()->damaged(playerData_->getAttackPower());

		{// 爆発パーティクル
			auto& forwardNormal = playerCamera_->getCurrentForwardDir();
			auto particlePos = playerCamera_->pos_ + (forwardNormal * 500);
			explosion_->setPosition(particlePos);
			explosion_->start();
		}

		{// サウンド
			PlaySoundFile("sound/explosion.wav", 2);
		}

		{// ログ
			TextLogManager::getTextLogManager()->addTextLog("プレイヤーの攻撃！" + convertFullWidthNumber(damage) + "のダメージを与えた");
		}
	}

	// ---------------------------
	// デバッグ用
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