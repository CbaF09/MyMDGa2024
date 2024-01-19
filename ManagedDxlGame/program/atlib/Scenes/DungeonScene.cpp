#include "DungeonScene.h"

namespace atl {

	DungeonScene::DungeonScene() {
		player_ = std::make_shared<PlayerPawn>();
	}

	void DungeonScene::sceneUpdate(float deltaTime) {
		seq_.update(deltaTime);

		render(deltaTime,player_->getPlayerCamera());
	}

	void DungeonScene::render(float deltaTime, const Shared<Atl3DCamera> camera) {
		camera->update();

		{//MeshObject 群をレンダリング
			for (const auto& wall : walls_) { wall->renderObject(camera); }
			for (const auto& groundTile : groundTiles_) { groundTile->renderObject(camera); }
			originStairs_->renderObjects(camera);
		}

		debug_displayDungeonParam(camera, deltaTime);
	}

	void DungeonScene::debug_displayDungeonParam(const Shared<atl::Atl3DCamera>& camera, float deltaTime) {
		DrawGridGround(player_->getPlayerCamera(), 50, 20);
		DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);

		camera->debug_displayCameraParam();
		player_->debug_displayPlayerParam(0, 15);

		// 階段の位置
		auto stairsPos = originStairs_->getRootMesh()->pos_;
		DrawStringEx(0, 30, -1, "stairsPos ... { %.2f, %.2f, %.2f }", stairsPos.x, stairsPos.y, stairsPos.z);
	}

	void DungeonScene::initDungeon() {
		walls_.clear();
		groundTiles_.clear();
		originStairs_ = nullptr;
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
			auto playerSpawnPos = DungeonCreater::getDungeonCreater()->getPlayerSpawnPos();
			player_->setPlayerAndCameraPos({ playerSpawnPos->x * static_cast<float>(CELL_FULL_LENGTH) , PLAYER_HEAD_LINE, playerSpawnPos->y * static_cast<float>(CELL_FULL_LENGTH) });
			auto stairsSpawnPos = DungeonCreater::getDungeonCreater()->getStairsSpawnPos();
			originStairs_ = std::make_shared<Stairs>(tnl::Vector3{ stairsSpawnPos->x * static_cast<float>(CELL_FULL_LENGTH),0,stairsSpawnPos->y * static_cast<float>(CELL_FULL_LENGTH) }, tnl::Vector3{ CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2 });
		}
	}

	void DungeonScene::generateWall(int generatePosX, int generatePosZ) {
		if (!originWall_) originWall_ = std::make_shared<Wall>(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH ,CELL_FULL_LENGTH });
		Shared<Wall> newWall = std::make_shared<Wall>(originWall_->getMesh());
		newWall->getMesh()->pos_.x = generatePosX * CELL_FULL_LENGTH;
		newWall->getMesh()->pos_.y = CELL_FULL_LENGTH / 2;
		newWall->getMesh()->pos_.z = generatePosZ * CELL_FULL_LENGTH;
		newWall->setMeshSizeVector3(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH, CELL_FULL_LENGTH });
		walls_.emplace_back(newWall);
	}

	void DungeonScene::generateGround(int generatePosX, int generatePosZ) {
		if (!originGroundTile_) originGroundTile_ = std::make_shared<GroundTile>(tnl::Vector3{ CELL_FULL_LENGTH,CELL_FULL_LENGTH,0 }); // Plane モデルの生成における関係で、Yの所にZの値を入れてます
		Shared<GroundTile> newGroundTile = std::make_shared<GroundTile>(originGroundTile_->getMesh());
		newGroundTile->getMesh()->pos_.x = generatePosX * CELL_FULL_LENGTH;
		newGroundTile->getMesh()->pos_.z = generatePosZ * CELL_FULL_LENGTH;
		groundTiles_.emplace_back(newGroundTile);
	}

	bool DungeonScene::seqInit(float deltaTime) {
		generateDungeon();
		seq_.change(&DungeonScene::seqProcess);
		return true;
	}

	bool DungeonScene::seqProcess(float deltaTime) {
		{// プレイヤーの移動
			player_->playerUpdate(deltaTime);

			{// プレイヤーと壁の衝突判定

			}
		}

		{// カメラコントロール ( 移動の後にやらないと、なんか変になる )
			player_->getPlayerCamera()->cameraControl(PLAYER_CAMERA_ROT_SPEED);
		}

		{// デバッグ用操作
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) seq_.change(&DungeonScene::seqInit);
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);
		}

		return true;
	}




}