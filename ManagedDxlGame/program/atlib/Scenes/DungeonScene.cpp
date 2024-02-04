#include <string>
#include "DungeonScene.h"
#include "TitleScene.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/SceneManager.h"
#include "../MeshObject/Wall.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/EnemyPawn.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/ItemPawn.h"
#include "../Utilities/Atl3DCamera.h"


namespace atl {

	DungeonScene::~DungeonScene(){
		{// ダンジョンシーンで使っているリソースを解放

			// 解放するリソースのファイルパスの一時的配列を作成
			std::vector<std::string> tempDeleteRes = {
				"graphics/UI/HPbarBackGround.png",
				"graphics/UI/HPbarRed.bmp",
				"graphics/UI/HPbarGreen.bmp"
			};

			// ファイルパスを一つずつ解放。解放の成功失敗をデバッグログに出力
			for (const auto& res : tempDeleteRes) {
				bool isDelete = ResourceManager::getResourceManager()->deleteResource(res);
				if (!isDelete) { tnl::DebugTrace("\n------------------------------\nDungeonScene::デストラクタ メモリ解放 => 正常に解放されていません"); }
				else { tnl::DebugTrace("\n------------------------------\nDungeonScene::デストラクタ メモリ解放 => 正常"); }
			}
			tnl::DebugTrace("\n------------------------------\n"); // ログが見づらいので最後に改行と切り取り線を入れる
		}

	}

	void DungeonScene::render(float deltaTime, const Shared<Atl3DCamera> camera) {

		{//MeshObject 群をレンダリング
			for (const auto& wall : walls_) { wall->renderObject(camera); }
			for (const auto& groundTile : groundTiles_) { groundTile->renderObject(camera); }

			if (originStairs_) originStairs_->renderObjects(camera);
			for (const auto& enemy : enemies_) { enemy->renderObjects(camera); }
			for (const auto& item : items_) { item->renderObjects(camera); }
			if (player_)player_->render(deltaTime);
		}

	}

	void DungeonScene::draw2D(float deltaTime) {
		debug_displayDungeonParam(deltaTime);
		if (!isOpenMenu_) { TextLogManager::getTextLogManager()->displayTextLog(60, 400, deltaTime); }
		drawUI(deltaTime);
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);
	}

	void DungeonScene::drawUI(float deltaTime) {
		drawHPbar();
	}

	void DungeonScene::drawHPbar() {
		// 背景 ( 枠 ) の描画
		auto HPbarBackGround = ResourceManager::getResourceManager()->getUiRes("graphics/UI/HPbarBackGround.png");
		DrawExtendGraph(HP_BAR_LEFT_UP_POINT.x, HP_BAR_LEFT_UP_POINT.y, HP_BAR_RIGHT_DOWN_POINT.x,HP_BAR_RIGHT_DOWN_POINT.y,HPbarBackGround,true);

		// 赤ゲージの描画
		auto HPbarRed = ResourceManager::getResourceManager()->getUiRes("graphics/UI/HPbarRed.bmp");
		DrawExtendGraph(
			HP_BAR_LEFT_UP_POINT.x + HP_BAR_ADJUST_VALUE.x, 
			HP_BAR_LEFT_UP_POINT.y + HP_BAR_ADJUST_VALUE.y, 
			HP_BAR_RIGHT_DOWN_POINT.x - HP_BAR_ADJUST_VALUE.x, 
			HP_BAR_RIGHT_DOWN_POINT.y - HP_BAR_ADJUST_VALUE.y, HPbarRed, true);

		// 緑ゲージの描画 ( HP がゼロでない場合のみ描画 )
		if (!player_->getPlayerData()->isZeroHP()) {
			auto hpPersent = player_->getPlayerData()->getCurrentHPpersent();
			auto HPbarGreen = ResourceManager::getResourceManager()->getUiRes("graphics/UI/HPbarGreen.bmp");
			DrawExtendGraph(
				HP_BAR_LEFT_UP_POINT.x + HP_BAR_ADJUST_VALUE.x,
				HP_BAR_LEFT_UP_POINT.y + HP_BAR_ADJUST_VALUE.y,
				static_cast<int>((HP_BAR_RIGHT_DOWN_POINT.x * hpPersent)) - HP_BAR_ADJUST_VALUE.x,
				HP_BAR_RIGHT_DOWN_POINT.y - HP_BAR_ADJUST_VALUE.y, HPbarGreen, true);
		}
	}
	
	void DungeonScene::sceneUpdate(float deltaTime) {
		seq_.update(deltaTime);

		// カメラのアップデート
		player_->getPlayerCamera()->update();

		// レンダー ( カメラアップデートの後 )
		render(deltaTime, player_->getPlayerCamera());

		// 2D系の描画
		draw2D(deltaTime);

		{// デバッグ用操作
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) 	generateDungeon();;
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);
		}
	}

	//---------------------
	// シーケンス
	//---------------------

	// 初期設定
	bool DungeonScene::seqInit(float deltaTime) {
		// 最初の1フレームで行う前処理
		if (seq_.isStart()) {
			player_ = std::make_shared<PlayerPawn>();
			player_->initialize(shared_from_this());
			
			auto fadeManager = FadeInOutManager::getFadeInOutManager();
			fadeManager->setFadeAlphaValue(255);
			fadeManager->startFadeIn();

			generateDungeon();;
		}
		else { // その後の処理
			seq_.change(&DungeonScene::seqAllTurnFlagOff);
		}return true;
	}

	// 行動フラグオフ用シーケンス
	bool DungeonScene::seqAllTurnFlagOff(float deltaTime) {
		player_->setIsAlreadyTurn();
		for (auto& enemy : enemies_) { enemy->setIsAlreadyTurn(); }

		currentTurn_ = e_turnState::KEY_INPUT;
		seq_.change(&DungeonScene::seqTurnStateProcess);
		return true;
	}

	// 現在ターンに応じて処理実行
	bool DungeonScene::seqTurnStateProcess(float deltaTime) {
		// ターンの特別遷移がある場合、記述
		if (seq_.isStart()) {
			auto player2Dpos = player_->getPlayer2Dpos();

			{// 階段に乗った場合
				auto stairs2Dpos = originStairs_->get2Dpos();
				if (stairs2Dpos.x == player2Dpos.x && stairs2Dpos.y == player2Dpos.y) {
					currentTurn_ = e_turnState::PLAYER_ON_STAIRS;
				}
			}
		}

		switch (currentTurn_) {
		case e_turnState::KEY_INPUT:
			processKeyInput(deltaTime);
			break;
		case e_turnState::PLAYER_MOVE:
			processPlayerMoveTurn(deltaTime);
			break;
		case e_turnState::PLAYER_ON_STAIRS:
			processPlayerOnStairs(deltaTime);
			break;
		}
		return true;
	}

	void DungeonScene::processKeyInput(float deltaTime) {
		// フェードイン ・ アウト中であれば操作不能
		if (FadeInOutManager::getFadeInOutManager()->isFading()) return;

		if (!isOpenMenu_) {
			if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_A, eKeys::KB_S, eKeys::KB_D)) {
				// プレイヤー側でもキー入力待ちをしていて、プレイヤー操作は 1 フレーム分遅れるので、一回 呼ぶ
				player_->playerUpdate(deltaTime);
				currentTurn_ = e_turnState::PLAYER_MOVE;
			}

			if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
				player_->playerUpdate(deltaTime);
				currentTurn_ = e_turnState::PLAYER_MOVE;
			}
		}

		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			isOpenMenu_ = !isOpenMenu_;
			player_->playerUpdate(deltaTime);
		}
	}

	// プレイヤーの移動入力
	void DungeonScene::processPlayerMoveTurn(float deltaTime) {
		// プレイヤーのターン
		if (!player_->getIsAlreadyTurn()) {
			player_->playerUpdate(deltaTime);
		}

		// エネミーのターン
		bool allEnemyTurned = true;
		
		// 移動処理
		for (auto& enemy : enemies_) {
			if (enemy->getIsAlreadyMove()) { continue; }
			enemy->enemyUpdate(deltaTime);
			allEnemyTurned = false;
		}

		// 行動処理
		for (auto& enemy : enemies_) {
			if (enemy->getIsAlreadyAction()) { continue; }
			enemy->enemyUpdate(deltaTime);
			allEnemyTurned = false;
		}

		// プレイヤー移動完了・エネミー行動完了したら、シーケンス遷移
		if (player_->getIsAlreadyTurn() && allEnemyTurned) seq_.change(&DungeonScene::seqDeadEnemyProcess);
	}

	// 階段に乗った時の処理
	void DungeonScene::processPlayerOnStairs(float deltaTime) {
		seq_.change(&DungeonScene::seqToNextFloor);
	}

	// 次の階層に進む処理
	bool DungeonScene::seqToNextFloor(float deltaTime) {
		// 最初のフレームでフェードアウト開始
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			generateDungeon();
			FadeInOutManager::getFadeInOutManager()->startFadeIn();
			seq_.change(&DungeonScene::seqAllTurnFlagOff);
		}

		return true;
	}

	bool DungeonScene::seqDeadEnemyProcess(float deltaTime) {
		bool allEnemyAlive = true;

		for (auto it = enemies_.begin(); it != enemies_.end();) {
			if ((*it)->getEnemyData()->isZeroHP()) {
				// HP がゼロになっている敵を発見
				allEnemyAlive = false;
				// HP がゼロになっている enemy の update を回す ( enemy の 死亡演出 は、enemyUpdate で管理 )
				(*it)->enemyUpdate(deltaTime);
				// Dead になったエネミーを削除する
				if ((*it)->getCurrentState() == EnemyPawn::e_EnemyState::Dead) {
					it = enemies_.erase(it);
					continue;
				}
			}
			++it;
		}

		if (allEnemyAlive) {
			seq_.change(&DungeonScene::seqAllTurnFlagOff);
		}
		
		return true;
	}

	//---------------------
	// ダンジョン生成
	//---------------------

	void DungeonScene::initDungeon() {
		walls_.clear();
		groundTiles_.clear();
		enemies_.clear();
		items_.clear();
	}

	void DungeonScene::generateDungeon() {
		initDungeon();

		auto dungeonCreater = DungeonCreater::getDungeonCreater();

		dungeonCreater->createDungeon();
		auto& fieldData = dungeonCreater->getFieldCells();

		for (int x = 0; x < fieldData.size(); ++x) {
			for (int y = 0; y < fieldData[x].size(); ++y) {
				// その位置が壁なら壁を生成、壁でないなら地面を生成
				if (fieldData[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_WALL) {
					generateWall(x, y);
				}
				else {
					generateGround(x, y);
				}
			}
		}

		{// 各種スポーン ( DungeonCreaterが作ったスポーン位置を取得し、生成 )
			// プレイヤー
			auto& playerSpawnPos = dungeonCreater->getPlayerSpawnPos();
			player_->playerSpawn2Dpos(playerSpawnPos);

			// 階段
			auto& stairsSpawnPos = dungeonCreater->getStairsSpawnPos();
			// セル全長の半分の大きさで生成
			originStairs_ = std::make_shared<Stairs>(stairsSpawnPos, tnl::Vector3{ CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2 });

			// エネミー
			auto& enemySpawnPos = dungeonCreater->getEnemySpawnPos();
			for (int i = 0; i < dungeonCreater->getEnemySpawnNum(); ++i) {
				// TODO: マジックナンバー。本番用エネミーを作った時に同時になんとかするべし
				auto enemy = std::make_shared<EnemyPawn>(enemySpawnPos[i], tnl::Vector3{ 200, 200, 200 });
				enemy->assignWeakPlayer(player_);
				enemies_.emplace_back(enemy);
			}

			// アイテム
			auto& itemSpawnPos = dungeonCreater->getItemSpawnPos();
			for (int i = 0; i < dungeonCreater->getItemSpawnNum(); ++i) {
				auto item = std::make_shared<ItemPawn>(itemSpawnPos[i]);
				item->assignWeakPlayer(player_);
				items_.emplace_back(item);
			}
		}

		// テクスチャによるメモリリーク対策
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
	
	void DungeonScene::debug_displayDungeonParam(float deltaTime) {
		DrawGridGround(player_->getPlayerCamera(), 50, 20);
		DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);

		player_->debug_displayPlayerParam(600, 0);

		DrawStringEx(0, 75, -1, "curentTurn ... [ %d ]", currentTurn_);

		// 階段の位置
		auto& stairsPos = originStairs_->get2Dpos();
		DrawStringEx(0, 100, -1, "stairsPos ... [ %d, %d ]", stairsPos.x, stairsPos.y);
	}

}