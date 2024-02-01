#include <string>
#include "DungeonScene.h"
#include "../Singletons/DungeonCreater.h"
#include "../MeshObject/Wall.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/EnemyPawn.h"
#include "../MeshObject/PlayerPawn.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	void DungeonScene::render(float deltaTime, const Shared<Atl3DCamera> camera) {

		{//MeshObject 群をレンダリング
			for (const auto& wall : walls_) { wall->renderObject(camera); }
			for (const auto& groundTile : groundTiles_) { groundTile->renderObject(camera); }

			originStairs_->renderObjects(camera);
			for (const auto& enemy : enemies_) { enemy->renderObjects(camera); }
			player_->render(deltaTime);
		}

		debug_displayDungeonParam(camera, deltaTime);
	}

	void DungeonScene::sceneUpdate(float deltaTime) {
		seq_.update(deltaTime);
	}


	//---------------------
	// シーケンス
	//---------------------


	bool DungeonScene::seqInit(float deltaTime) {
		generateDungeon();
		player_->initialize();
		seq_.change(&DungeonScene::seqTurnStateProcess);
		return true;
	}

	bool DungeonScene::seqTurnStateProcess(float deltaTime) {
		switch (currentTurn_) {
		case e_turnState::KEY_INPUT:
			processKeyInput(deltaTime);
			break;
		case e_turnState::PLAYER:
			processPlayerTurn(deltaTime);
			break;
		case e_turnState::ENEMY:
			processEnemyTurn(deltaTime);
			break;
		}

		{// カメラコントロール ( 移動の後にやらないと、なんか変になる )
			player_->getPlayerCamera()->cameraControl(CAMERA_ROT_SPEED);
		}

		{// レンダー ( カメラコントロールの後 )
			render(deltaTime, player_->getPlayerCamera());
		}

		{// デバッグ用操作
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) seq_.change(&DungeonScene::seqInit);
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);
		}

		return true;
	}

	void DungeonScene::processKeyInput(float deltaTime) {
		if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_A, eKeys::KB_S, eKeys::KB_D)) {
			currentTurn_ = e_turnState::PLAYER;
		}
	}

	void DungeonScene::processPlayerTurn(float deltaTime) {
		player_->playerUpdate(deltaTime);
		if (player_->getIsAlreadyAction()) currentTurn_ = e_turnState::ENEMY;
	}

	void DungeonScene::processEnemyTurn(float deltaTime) {
		bool allEnemyAction = true;
		for (auto enemy : enemies_) {
			enemy->enemyUpdate(deltaTime);
			if (!enemy->getIsAlreadyAction()) allEnemyAction = false; // 行動完了になっていないエネミーがいた場合、フラグをオフ
		}
		if (allEnemyAction) { // フラグが立っている ( 全エネミーが行動完了している ) 場合、Turnを切り替え
			currentTurn_ = e_turnState::KEY_INPUT;
		}
	}

	//---------------------
	// ダンジョン生成
	//---------------------
	
	void DungeonScene::initDungeon() {
		walls_.clear();
		groundTiles_.clear();
		enemies_.resize(3);
	}

	void DungeonScene::generateDungeon() {
		initDungeon();

		DungeonCreater::getDungeonCreater()->createDungeon();
		auto& fieldData = DungeonCreater::getDungeonCreater()->getFieldCells();

		for (int x = 0; x < fieldData.size(); ++x) {
			for (int y = 0; y < fieldData[x].size(); ++y) {
				if (fieldData[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_WALL) {
					generateWall(x, y);
				}
				else {
					generateGround(x, y);
				}
			}
		}

		{// 各種スポーン
			auto& playerSpawnPos = DungeonCreater::getDungeonCreater()->getPlayerSpawnPos();
			player_ = std::make_shared<PlayerPawn>();
			player_->playerSpawn2Dpos(playerSpawnPos);

			auto& stairsSpawnPos = DungeonCreater::getDungeonCreater()->getStairsSpawnPos();
			originStairs_ = std::make_shared<Stairs>(stairsSpawnPos, tnl::Vector3{ CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2 });

			auto& enemySpawnPos = DungeonCreater::getDungeonCreater()->getEnemySpawnPos();
			for (int i = 0; i < 3; ++i) {
				enemies_[i] = std::make_shared<EnemyPawn>(enemySpawnPos[i], tnl::Vector3{ 200, 200, 200 });
			}
		}

		// テクスチャによるメモリリークが発生してるっぽいので、実行
		dxe::Texture::DestroyUnReferenceTextures();
	}

	void DungeonScene::generateWall(int generatePosX, int generatePosZ) {
		if (!originWall_) originWall_ = std::make_shared<Wall>(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH ,CELL_FULL_LENGTH });
		Shared<Wall> newWall = std::make_shared<Wall>(originWall_->getMesh());
		newWall->getMesh()->pos_.x = static_cast<float>(generatePosX * CELL_FULL_LENGTH);
		newWall->getMesh()->pos_.y = static_cast<float>(CELL_FULL_LENGTH) / 2;
		newWall->getMesh()->pos_.z = static_cast<float>(generatePosZ * CELL_FULL_LENGTH);
		newWall->setMeshSizeVector3(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH, CELL_FULL_LENGTH });
		walls_.emplace_back(newWall);
	}

	void DungeonScene::generateGround(int generatePosX, int generatePosZ) {
		if (!originGroundTile_) originGroundTile_ = std::make_shared<GroundTile>(tnl::Vector3{ CELL_FULL_LENGTH,CELL_FULL_LENGTH,0 }); // Plane モデルの生成における関係で、Yの所にZの値を入れてます
		Shared<GroundTile> newGroundTile = std::make_shared<GroundTile>(originGroundTile_->getMesh());
		newGroundTile->getMesh()->pos_.x = static_cast<float>(generatePosX * CELL_FULL_LENGTH);
		newGroundTile->getMesh()->pos_.z = static_cast<float>(generatePosZ * CELL_FULL_LENGTH);
		groundTiles_.emplace_back(newGroundTile);
	}


	//------------------------
	// デバッグ用
	//------------------------

	void DungeonScene::debug_displayDungeonParam(const Shared<atl::Atl3DCamera>& camera, float deltaTime) {
		DrawGridGround(player_->getPlayerCamera(), 50, 20);
		DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);

		player_->debug_displayPlayerParam(0, 0);

		DrawStringEx(0, 100, -1, "curentTurn ... [ %d ]", currentTurn_);

		// 階段の位置
		//auto& stairsPos = originStairs_->get2Dpos();
		//DrawStringEx(0, 100, -1, "stairsPos ... [ %d, %d ]", stairsPos.x, stairsPos.y);

		// 敵の位置
		for (int i = 0; i < 3; ++i) {
			DrawStringEx(0, 115 + (15 * i), -1, "Enemy[%d] pos ... [ %d , %d ]", i, enemies_[i]->get2Dpos().x, enemies_[i]->get2Dpos().y);
		}
	}

}