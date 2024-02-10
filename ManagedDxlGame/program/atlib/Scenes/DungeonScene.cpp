#include <string>
#include <algorithm>
#include "DungeonScene.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/SceneManager.h"
#include "../MeshObject/Wall.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/EnemyPawn.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/ItemPawn.h"
#include "../Object/SelectWindow.h"
#include "../Object/MenuWindow.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {
	DungeonScene::DungeonScene() {
		walls_.clear();
		groundTiles_.clear();
		enemies_.clear();
		items_.clear();
	}

	DungeonScene::~DungeonScene() {
		{// ダンジョンシーンで使っているリソースを解放

			// 解放するリソースのファイルパスの一時的配列を作成
			std::vector<std::string> tempDeleteRes = {
				"graphics/UI/HPbarBackGround.png",
				"graphics/UI/HPbarRed.bmp",
				"graphics/UI/HPbarGreen.bmp",
				"graphics/UI/Instruction.png"
				"sound/BGM/DungeonSceneBGM.ogg",
				"sound/SE/DungeonSceneCloseMenu.ogg",
				"sound/SE/DungeonSceneOpenMenu.ogg",
			};

			// リソース解放
			for (const auto& res : tempDeleteRes) {
				ResourceManager::getResourceManager()->deleteResource(res);
			}

			// フォントデータの解放
			DeleteFontToHandle(NEXT_FLOOR_FONT);
			DeleteFontToHandle(LEVEL_STRING_FONT);

			{// ダンジョンクリエイターのシングルトンを解放
				DungeonCreater::getDungeonCreater()->deleteDungeonCreater();
			}

			{// テキストログのシングルトンを解放
				TextLogManager::getTextLogManager()->deleteTextLogManager();
			}
		}
	}

	void DungeonScene::render(float deltaTime, const Shared<Atl3DCamera> camera) {
		// 不透明なもの描画
		SetWriteZBuffer3D(TRUE);
		for (const auto& ground : groundTiles_) { ground->renderObject(camera); }
		for (const auto& wall : walls_) { wall->renderObject(camera); }
		for (const auto& enemy : enemies_) { enemy->renderObjects(camera, deltaTime); }
		if (originStairs_) originStairs_->renderObjects(camera,deltaTime);
		for (const auto& item : items_) { item->renderObjects(camera,deltaTime); }
		if (player_)player_->render(deltaTime);

		// 透明なもの描画
		SetWriteZBuffer3D(FALSE);
		for (const auto& enemy : enemies_) { enemy->renderTransparentObject(camera, deltaTime); }

		SetWriteZBuffer3D(TRUE);
	}

	void DungeonScene::draw2D(float deltaTime) {

		drawUI(deltaTime);

		// セレクトウィンドウがある時は、描画する
		if (selectWindow_) selectWindow_->draw(deltaTime);

		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);

		// 現在階層の描画は真っ黒な画面の上にやりたいので、この位置
		if (isNextFloorTransition) { drawNextFloorTransition(); }

		// デバッグモード中に描画するマップやその他情報
		if (isDebug) {
			debug_displayMap(deltaTime);
			debug_displayDungeonParam(deltaTime);
			player_->getPlayerData()->debug_playerDataParam(600, 500);
		}
	}

	// 次の階層を描画する
	void DungeonScene::drawNextFloorTransition() {
		// 現在階層が、最上階 ( クリア階 ) でない場合の描画
		if (currentFloor_ != MAX_FLOOR) {
			DrawStringToHandleEx(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, -1, NEXT_FLOOR_FONT, "ワイズマンの修練塔 [%d / %d]", currentFloor_, MAX_FLOOR);
		}
		else { // クリア階の場合の描画
			DrawStringToHandleEx(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, -1, NEXT_FLOOR_FONT, "ワイズマンの修練塔 [ 最上階 ]",  currentFloor_);
		}
	}

	void DungeonScene::drawUI(float deltaTime) {
		// HP バー 表示
		drawHPbar();
		
		//
		drawInstruction();

		// メニューを開いている時はログ表示無し,レベル表示無し,満腹度表示無し
		if (!menuWindow_) { 
			TextLogManager::getTextLogManager()->displayTextLog(TEXT_LOG_POSITION.x, TEXT_LOG_POSITION.y, deltaTime); 
			drawLevel();
			drawInvatation();
		}

		// メニューを開いている間の描画
		if (menuWindow_) { menuWindow_->draw(deltaTime); }
	}

	void DungeonScene::drawInstruction() {
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 125);
		DrawBoxEx({static_cast<float>(INSTRUCTION_POSITION.x),static_cast<float>(INSTRUCTION_POSITION.y),0}, static_cast<float>(INSTRUCTION_BACK_BOX_SIZE.x), static_cast<float>(INSTRUCTION_BACK_BOX_SIZE.y),true,GetColor(0,0,255));
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		DrawRotaGraph(INSTRUCTION_POSITION.x, INSTRUCTION_POSITION.y, INSTRUCTION_SIZE, 0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/Instruction.png"), true);
	}

	void DungeonScene::drawLevel() {
		auto playerData = player_->getPlayerData();
		DrawStringToHandleEx(static_cast<float>(LEVEL_STRING_POSITION.x), static_cast<float>(LEVEL_STRING_POSITION.y), -1, LEVEL_STRING_FONT,"レベル ... [ %d ]", playerData->getCurrentLevel());
	}

	void DungeonScene::drawInvatation() {
		// 招待状 UI を描画
		auto invatation = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/Invatation.png");
		// 空腹度は、最大2550なので、現在空腹度の10分の1を輝度に
		SetDrawBright(currentSatiety_/10, currentSatiety_/10, currentSatiety_/10);
		DrawRotaGraph(INVATATION_POSITION.x, INVATATION_POSITION.y, INVATATION_SIZE, tnl::ToRadian(INVATATION_ANGLE), invatation, true);
		// 描画輝度を元に戻す
		SetDrawBright(255,255,255);

		// 文字列「招待状」を描画。レベルを表示しているフォントを流用。
		DrawStringToHandleEx(static_cast<float>(INVATATION_STRING_POSITION.x), static_cast<float>(INVATATION_STRING_POSITION.y), -1, LEVEL_STRING_FONT, "招待状");
	}

	void DungeonScene::drawHPbar() {
		// 背景 ( 枠 ) の描画
		auto HPbarBackGround = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/HPbarBackGround.png");
		DrawExtendGraph(HP_BAR_LEFT_UP_POINT.x, HP_BAR_LEFT_UP_POINT.y, HP_BAR_RIGHT_DOWN_POINT.x, HP_BAR_RIGHT_DOWN_POINT.y, HPbarBackGround, true);

		// 赤ゲージの描画
		auto HPbarRed = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/HPbarRed.bmp");
		DrawExtendGraph(
			HP_BAR_LEFT_UP_POINT.x + HP_BAR_ADJUST_VALUE.x,
			HP_BAR_LEFT_UP_POINT.y + HP_BAR_ADJUST_VALUE.y,
			HP_BAR_RIGHT_DOWN_POINT.x - HP_BAR_ADJUST_VALUE.x,
			HP_BAR_RIGHT_DOWN_POINT.y - HP_BAR_ADJUST_VALUE.y, HPbarRed, true);

		// 緑ゲージの描画 ( HP がゼロでない場合のみ描画 )
		if (!player_->getPlayerData()->isZeroHP()) {
			auto hpPersent = player_->getPlayerData()->getCurrentHPpersent();
			auto HPbarGreen = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/HPbarGreen.bmp");
			DrawExtendGraph(
				HP_BAR_LEFT_UP_POINT.x + HP_BAR_ADJUST_VALUE.x,
				HP_BAR_LEFT_UP_POINT.y + HP_BAR_ADJUST_VALUE.y,
				static_cast<int>((HP_BAR_RIGHT_DOWN_POINT.x * hpPersent)) - HP_BAR_ADJUST_VALUE.x,
				HP_BAR_RIGHT_DOWN_POINT.y - HP_BAR_ADJUST_VALUE.y, HPbarGreen, true);
		}

		// HP の数値表示 ( フォントはレベルを表示しているフォントを流用 )
		auto maxHP = player_->getPlayerData()->getMaxHP();
		auto currentHP = player_->getPlayerData()->getCurrentHP();
		DrawStringToHandleEx(static_cast<float>(HP_STRING_POSITION.x), static_cast<float>(HP_STRING_POSITION.y), -1, LEVEL_STRING_FONT, "HP :  %d / %d ", currentHP, maxHP);
	}

	void DungeonScene::sceneUpdate(float deltaTime) {
		if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT, eKeys::KB_O) && tnl::Input::IsKeyDownTrigger(eKeys::KB_P)) isDebug = !isDebug;

		seq_.update(deltaTime);

		// カメラのアップデート
		player_->getPlayerCamera()->update();
		// スカイボックスのアップデート
		skybox_.update(player_->getPlayerCamera());
		// レンダー ( カメラアップデートの後 )
		render(deltaTime, player_->getPlayerCamera());

		// 2D系の描画
		draw2D(deltaTime);

		{// デバッグモード中に可能な操作
			if (isDebug) {
				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) 	generateDungeon();;
				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);
			}
		}
	}

	void DungeonScene::openMenu() {
		if (menuWindow_) return;
		// メニューを開く
		menuWindow_ = std::make_shared<MenuWindow>(player_->getPlayerData()->getInventory());
		player_->openMenuBook();
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneOpenMenu.ogg", DX_PLAYTYPE_BACK);
	}

	void DungeonScene::closeMenu() {
		if (!menuWindow_) return;
		// メニューウィンドウを閉じる
		menuWindow_.reset();
		player_->closeMenuBook();
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneCloseMenu.ogg", DX_PLAYTYPE_BACK);
	}

	void DungeonScene::soundVolumeFix() {
		auto rManager = ResourceManager::getResourceManager();
		rManager->changeVolumeSoundRes("sound/BGM/DungeonSceneBGM.ogg", 80);
		rManager->changeVolumeSoundRes("sound/SE/DungeonSceneCloseMenu.ogg", 200);
		rManager->changeVolumeSoundRes("sound/SE/DungeonSceneOpenMenu.ogg", 180);
	}

	//---------------------
	// シーケンス
	//---------------------

	// 初期設定 ( DungeonSceneのインスタンス生成後、一度だけ通る ) 
	bool DungeonScene::seqInit(float deltaTime) {
		// フェードは真っ黒からスタート
		FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(255);

		// プレイヤーの生成と初期化
		player_ = std::make_shared<PlayerPawn>();
		player_->initialize(shared_from_this());

		// 現在の階層を 初期化
		currentFloor_ = 0;

		soundVolumeFix();

		ResourceManager::getResourceManager()->playSoundRes("sound/BGM/DungeonSceneBGM.ogg",DX_PLAYTYPE_LOOP);

		// 本シーケンスに遷移
		seq_.change(&DungeonScene::seqToNextFloor);
		return true;
	}

	// 行動フラグオフ用シーケンス
	bool DungeonScene::seqAllTurnFlagOff(float deltaTime) {
		player_->offFlagIsAlreadyTurn();
		for (auto& enemy : enemies_) { enemy->offFlagIsAlreadyTurn(); }

		// リスポーン判定
		++respornTurnTimer_;
		if (respornTurnTimer_ > RESPORN_TURN_COUNT) {
			// タイマーがリスポーンカウントを超えたら、敵が一体スポーンする
			// スポーン先に既にエネミーがいた場合、次のターンにスポーンする
			enemyResporn();
		}

		currentTurn_ = e_turnState::KEY_INPUT;
		seq_.change(&DungeonScene::seqTurnStateProcess);
		return true;
	}

	void DungeonScene::enemyResporn() {
		auto spawnPos = DungeonCreater::getDungeonCreater()->randomChoiceCanSpawnFieldCellPos(player_->getPlayer2Dpos());
		for (const auto& enemy : enemies_) {
			// スポーン先に既に敵がいるかチェック。いたらリターン
			if (enemy->get2Dpos().x == spawnPos.x && enemy->get2Dpos().y == spawnPos.y) {
				return;
			}
		}

		respornTurnTimer_ = 0;
		auto enemy = std::make_shared<EnemyPawn>(spawnPos);
		enemy->assignWeakDungeonScene(shared_from_this());
		enemies_.emplace_back(enemy);
	}

	// 現在ターンに応じて処理実行
	bool DungeonScene::seqTurnStateProcess(float deltaTime) {
		// ターンの特別遷移がある場合、記述
		if (seq_.isStart()) {

			auto& player2Dpos = player_->getPlayer2Dpos();
			{// 階段に乗った場合
				// 階段に乗るフラグが立っていない場合、階段に乗ったフラグを立てる
				if (!isPlayerOnStairs_) {
					auto& stairs2Dpos = originStairs_->get2Dpos();
					if (stairs2Dpos.x == player2Dpos.x && stairs2Dpos.y == player2Dpos.y) {
						isPlayerOnStairs_ = true;
						currentTurn_ = e_turnState::PLAYER_ON_STAIRS;
					}
				}
			}



			{// メニューウィンドウを開いている場合の処理
				if (menuWindow_) {
					seq_.change(&DungeonScene::seqMenuWindow);
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

	bool DungeonScene::seqGameOver(float deltaTime) {
		// 最初のフレームでフェードアウトを行い、フェードアウトが完了したらゲームオーバーシーンに遷移
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			SceneManager::getSceneManager()->changeScene(std::make_shared<GameOverScene>());
		}
		return true;
	}

	void DungeonScene::processKeyInput(float deltaTime) {
		// フェードイン ・ アウト中であれば操作不能
		if (FadeInOutManager::getFadeInOutManager()->isFading()) return;

		// メニューウィンドウ表示中は、移動・攻撃できない
		if (!menuWindow_) {
			if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_A, eKeys::KB_S, eKeys::KB_D)) {
				// プレイヤー側でもキー入力待ちをしていて、プレイヤー操作は 1 フレーム分遅れるので、一回 呼ぶ
				player_->playerUpdate(deltaTime);
				// 移動した場合、階段に乗っているフラグをオフにする
				isPlayerOnStairs_ = false;
				currentTurn_ = e_turnState::PLAYER_MOVE;
			}

			if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
				player_->playerUpdate(deltaTime);
				currentTurn_ = e_turnState::PLAYER_MOVE;
			}
		}

		// 右クリックでメニューウィンドウの生成を行う
		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			if(!menuWindow_) { 
				openMenu();
				seq_.change(&DungeonScene::seqMenuWindow);
			}
		}
	}

	// プレイヤーの移動入力
	void DungeonScene::processPlayerMoveTurn(float deltaTime) {
		// プレイヤーのターン処理 ( 移動 )
		if (!player_->getIsAlreadyTurn()) {
			player_->playerUpdate(deltaTime);
		}

		// 全エネミーの行動が終わったかフラグ作る
		bool allEnemyTurned = true;
		{// エネミーの行動処理
			// エネミー移動処理
			enemyMove(deltaTime, allEnemyTurned);
			// エネミー行動処理
			enemyAction(deltaTime, allEnemyTurned);
		}

		// HP がゼロになり、死亡演出が終わった敵を削除
		deadEnemyErase();
		
		// プレイヤー・エネミーが行動完了したら、シーケンス遷移
		if (player_->getIsAlreadyTurn() && allEnemyTurned) { 
			{// プレイヤーのHPがゼロになっている場合、直接ゲームオーバーシーケンスに遷移
				if (player_->getPlayerData()->isZeroHP()) {
					seq_.change(&DungeonScene::seqGameOver);
					return;
				}
			}

			// 空腹度が0になったら、ゲームオーバーに遷移
			currentSatiety_ -= SATIETY_SUB_VALUE;
			if (currentSatiety_ <= 0) {
				currentSatiety_ = 0;
				seq_.change(&DungeonScene::seqGameOver);
				return;
			}

			// HP 自動回復
			player_->getPlayerData()->changeCurrentHP(EVERY_TURN_HEAL);

			seq_.immediatelyChange(&DungeonScene::seqOnItemPosition); 
		}
	}

	bool DungeonScene::seqOnItemPosition(float deltaTime) {
		auto& player2Dpos = player_->getPlayer2Dpos();
		for (auto it = items_.begin(); it != items_.end();) {
			auto& item2Dpos = (*it)->get2Dpos();
			// アイテムと重なっていない場合、早期　continue
			if (player2Dpos.x != item2Dpos.x || player2Dpos.y != item2Dpos.y) {
				++it;
				continue;
			}

			auto itemData = (*it)->getItemData();
			// インベントリにアイテムデータを格納した時
			if (player_->getPlayerData()->getInventory()->pushbackItemToInventory(itemData)) {
				// テキストログに拾ったアイテム名を出力
				auto& itemName = itemData->getItemName();
				std::string textLog = "　" + itemName + "を拾った";
				TextLogManager::getTextLogManager()->addTextLog(textLog);

				// アイテムを削除
				it = items_.erase(it);
				// １マスにアイテムが二つある事はないので、一個拾ったらbreak
			}
			else
			{
				TextLogManager::getTextLogManager()->addTextLog("荷物がいっぱいで拾えない");
				break;
			}
		}
		seq_.change(&DungeonScene::seqAllTurnFlagOff);
		return true;
	}

	void DungeonScene::enemyMove(float deltaTime, bool& allEnemyTurned) {
		for (auto& enemy : enemies_) {
			if (enemy->getIsAlreadyMove()) { continue; }
			enemy->enemyUpdate(deltaTime);
			allEnemyTurned = false;
		}
	}

	void DungeonScene::enemyAction(float deltaTime, bool& allEnemyTurned) {
		for (auto& enemy : enemies_) {
			if (enemy->getIsAlreadyAction()) { continue; }
			enemy->enemyUpdate(deltaTime);
			allEnemyTurned = false;
		}
	}

	void DungeonScene::deadEnemyErase() {
		for (auto it = enemies_.begin(); it != enemies_.end();) {
			// HP がゼロの敵を発見
			if ((*it)->getEnemyData()->isZeroHP()) {
				// 死亡演出が終わっているかどうか ( Deading なら死亡演出中 )
				if ((*it)->getCurrentState() == EnemyPawn::e_EnemyState::Dead) {
					// エネミーが死亡する時、プレイヤーはエネミーの持っていた経験値を得る
					auto getEXP = (*it)->getEnemyData()->getEnemyExp();
					player_->getPlayerData()->changeCurrentEXP(getEXP);
					// 敵へのポインターを配列から削除
					it = enemies_.erase(it);
					continue;
				}
			}
			++it;
		}
	}

	bool DungeonScene::seqMenuWindow(float deltaTime) {
		if (!menuWindow_) return true; // メニューウィンドウが有効でない場合、早期リターン

		// 右クリックで menuWindowを閉じる
		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			if (menuWindow_) {
				closeMenu();
				seq_.change(&DungeonScene::seqAllTurnFlagOff);
			}
		}

		selectedMenu = menuWindow_->process(deltaTime);

		// MenuWindowクラスから、何を選択中かのインデックスがenum型に変換されて返ってくるので、それに応じて処理
		switch (selectedMenu) {
		case MenuWindow::e_SelectedMenuWindow::NONE: break; // 何も選択されなかったフレームは何もしない
		case MenuWindow::e_SelectedMenuWindow::Item1: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item2: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item3: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item4: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item5: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item6: 
			// 範囲外アクセスチェックをしてから、本当に使うかの確認シーケンスに遷移
			if (player_->getPlayerData()->getInventory()->getItemData(static_cast<int>(selectedMenu)) != nullptr) { seq_.change(&DungeonScene::seqReallyUseItem); } break;
		case MenuWindow::e_SelectedMenuWindow::Setting: break; // 現状何も無し
		case MenuWindow::e_SelectedMenuWindow::CloseMenu: // メニューを閉じる
		{
			closeMenu();
			seq_.change(&DungeonScene::seqAllTurnFlagOff);
			break;
		}
		case MenuWindow::e_SelectedMenuWindow::ReturnToTitle: // タイトルに戻る
		{
			seq_.change(&DungeonScene::seqReallyReturnToTitle);
			break;
		}
		}

		return true;
	}


	// 本当にアイテムを使うか確認
	bool DungeonScene::seqReallyUseItem(float deltaTime) {
		auto& item = player_->getPlayerData()->getInventory()->getItemData(static_cast<int>(selectedMenu));
		if (seq_.isStart()) {
			selectWindow_ = std::make_shared<SelectWindow>(item->getItemName() + "\nを使いますか？");
		}
		if (selectWindow_) {
			if (selectWindow_->windowChoice() == SelectWindow::e_SelectChoice::YES) { // はい、の時の処理
				// アイテム使用してメニューウィンドウを閉じる。エネミーが行動する
				player_->getPlayerData()->getInventory()->useItem(static_cast<int32_t>(selectedMenu));
				selectWindow_.reset();
				closeMenu();
				player_->onFlagIsAlreadyTurn();
				currentTurn_ = e_turnState::PLAYER_MOVE;
				seq_.change(&DungeonScene::seqTurnStateProcess);
			}
			else if (selectWindow_->windowChoice() == SelectWindow::e_SelectChoice::NO) {	// いいえ、の時の処理
				selectWindow_.reset();
				seq_.change(&DungeonScene::seqMenuWindow);
			}
		}
		return true;
	}

	// タイトルに戻るかどうか確認中
	bool DungeonScene::seqReallyReturnToTitle(float deltaTime) {
		if (seq_.isStart()) {
			selectWindow_ = std::make_shared<SelectWindow>("タイトル画面に戻りますか？\n進捗データは残りません");
		}

		if (selectWindow_) {
			if (selectWindow_->windowChoice() == SelectWindow::e_SelectChoice::YES) { // はい、の時の処理
				selectWindow_.reset();
				seq_.change(&DungeonScene::seqReturnToTitle);
			}
			else if (selectWindow_->windowChoice() == SelectWindow::e_SelectChoice::NO) {	// いいえ、の時の処理
				selectWindow_.reset();
				seq_.change(&DungeonScene::seqAllTurnFlagOff);
				currentTurn_ = e_turnState::KEY_INPUT;
			}
		}

		return true;
	}

	// タイトルに戻る処理
	bool DungeonScene::seqReturnToTitle(float deltaTime) {
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			// 待機 
			SEQ_CO_YIELD_RETURN_TIME(nextFloorTransitionTime, deltaTime, [&] {})

			SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
		}

		SEQ_CO_END
	}

	// 階段に乗った時の処理
	void DungeonScene::processPlayerOnStairs(float deltaTime) {
		// ウィンドウがまだ無い場合は生成する
		if (!selectWindow_) selectWindow_ = std::make_shared<SelectWindow>("次の階層に進みますか？");

		// ポインタが有効化どうか
		if (selectWindow_) {
			if (selectWindow_->windowChoice() == SelectWindow::e_SelectChoice::YES) { // はい、の時の処理
				selectWindow_.reset();
				seq_.change(&DungeonScene::seqToNextFloor);

			}
			else if (selectWindow_->windowChoice() == SelectWindow::e_SelectChoice::NO) { // いいえ、の時
				selectWindow_.reset();
				isPlayerOnStairs_ = false;
				currentTurn_ = e_turnState::KEY_INPUT;
			}
		}
	}

	// 次の階層に進む処理
	bool DungeonScene::seqToNextFloor(float deltaTime) {
		// 最初のフレームでフェードアウト開始
		if (seq_.isStart()) {
			// 階段に乗っているフラグをオフにする
			isPlayerOnStairs_ = false;
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
			// フロア数をインクリメント
			++currentFloor_;

		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			// 次階層に遷移中フラグを立てる
			isNextFloorTransition = true;

			// 待機 
			SEQ_CO_YIELD_RETURN_TIME(nextFloorTransitionTime, deltaTime, [&] {})


				// 最大階層に到達したら、クリアシーンに遷移
				if (currentFloor_ >= MAX_FLOOR) {
					SceneManager::getSceneManager()->changeScene(std::make_shared<GameClearScene>());
				}
				else {
					// ダンジョンを生成し、フェードインし画面を表示させる
					generateDungeon();
					FadeInOutManager::getFadeInOutManager()->startFadeIn();
					seq_.change(&DungeonScene::seqAllTurnFlagOff);
				}

			// 次階層へ遷移中フラグをオフに
			isNextFloorTransition = false;
		}

		SEQ_CO_END
	}

	//---------------------
	// ダンジョン生成
	//---------------------

	void DungeonScene::initDungeon() {
		// 地形
		walls_.clear();
		groundTiles_.clear();
		
		// エネミー、アイテム
		enemies_.clear();
		items_.clear();
	}

	void DungeonScene::generateDungeon() {
		initDungeon();

		DungeonCreater::getDungeonCreater()->createDungeon();
		auto& fieldData = DungeonCreater::getDungeonCreater()->getFieldCells();

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
			auto& playerSpawnPos = DungeonCreater::getDungeonCreater()->getPlayerSpawnPos();
			player_->playerSpawn2Dpos(playerSpawnPos);

			// 階段
			auto& stairsSpawnPos = DungeonCreater::getDungeonCreater()->getStairsSpawnPos();
			// セル全長の半分の大きさで生成
			originStairs_ = std::make_shared<Stairs>(stairsSpawnPos, tnl::Vector3{ CELL_FULL_LENGTH / 3,CELL_FULL_LENGTH / 3,CELL_FULL_LENGTH / 3 });

			// エネミー
			auto& enemySpawnPos = DungeonCreater::getDungeonCreater()->getEnemySpawnPos();
			for (int i = 0; i < DungeonCreater::getDungeonCreater()->getEnemySpawnNum(); ++i) {
				auto enemy = std::make_shared<EnemyPawn>(enemySpawnPos[i]);
				enemy->assignWeakDungeonScene(shared_from_this());
				enemies_.emplace_back(enemy);
			}

			// アイテム
			auto& itemSpawnPos = DungeonCreater::getDungeonCreater()->getItemSpawnPos();
			for (int i = 0; i < DungeonCreater::getDungeonCreater()->getItemSpawnNum(); ++i) {
				auto item = std::make_shared<ItemPawn>(itemSpawnPos[i]);
				item->assignWeakDungeonScene(shared_from_this());
				items_.emplace_back(item);
			}
		}

		// テクスチャによるメモリリーク対策
		dxe::Texture::DestroyUnReferenceTextures();
	}

	void DungeonScene::generateWall(int generatePosX, int generatePosZ) {
		Shared<Wall> newWall = std::make_shared<Wall>(originWall_->getMesh());
		newWall->getMesh()->pos_.x = static_cast<float>(generatePosX * CELL_FULL_LENGTH);
		newWall->getMesh()->pos_.y = static_cast<float>(CELL_FULL_LENGTH);
		newWall->getMesh()->pos_.z = static_cast<float>(generatePosZ * CELL_FULL_LENGTH);
		newWall->setMeshSizeVector3(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH, CELL_FULL_LENGTH });
		walls_.emplace_back(newWall);
	}

	void DungeonScene::generateGround(int generatePosX, int generatePosZ) {
		Shared<GroundTile> newGround = std::make_shared<GroundTile>(originGroundTile_->getMesh());
		Shared<GroundTile> newGroundTile = std::make_shared<GroundTile>(originGroundTile_->getMesh());
		newGroundTile->getMesh()->pos_.x = static_cast<float>(generatePosX * CELL_FULL_LENGTH);
		newGroundTile->getMesh()->pos_.z = static_cast<float>(generatePosZ * CELL_FULL_LENGTH);
		groundTiles_.emplace_back(newGroundTile);
	}

	//------------------------
	// デバッグ用
	//------------------------

	void DungeonScene::debug_displayDungeonParam(float deltaTime) {
		DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);

		DrawStringEx(600, 0, -1, "curentTurn ... [ %d ]", currentTurn_);

		// 階段の位置
		if (originStairs_) {
			auto& stairsPos = originStairs_->get2Dpos();
			DrawStringEx(600, 25, -1, "stairsPos ... [ %d, %d ]", stairsPos.x, stairsPos.y);
		}

		DrawStringEx(600, 50, -1, "currentFloor ... [ %d ]", currentFloor_);
		player_->debug_displayPlayerParam(600, 100);

	}

	void DungeonScene::debug_displayMap(float deltaTime) {
		auto dCreater = DungeonCreater::getDungeonCreater();
		auto& field = dCreater->getFieldCells();

		DrawBox(0, 0, 450, 450, GetColor(0, 0, 0), true);

		// フィールド情報の描画
		for (int x = 0; x < field.size(); ++x) {
			for (int y = 0; y < field[x].size(); ++y) {
				if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_ROOM) {
					DrawStringEx((y * 15), (x * 15), -1, " ");
				}
				if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_PATH) {
					DrawStringEx((y * 15), (x * 15), -1, "-");
				}
				if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_WALL) {
					DrawStringEx((y * 15), (x * 15), -1, "*");
				}
			}
		}

		for (const auto& enemy : enemies_) {
			auto& enemyPos = enemy->get2Dpos();
			DrawStringEx(enemyPos.y * 15, enemyPos.x * 15, GetColor(255, 0, 0), "e");
		}

		for (const auto& item : items_) {
			auto& itemPos = item->get2Dpos();
			DrawStringEx(itemPos.y * 15, itemPos.x * 15, GetColor(0, 255, 0), "I");
		}

		auto& player2Dpos = player_->getPlayer2Dpos();
		DrawStringEx(player2Dpos.y * 15, player2Dpos.x * 15, GetColor(0, 0, 255), "P");

		auto& stairs2Dpos = originStairs_->get2Dpos();
		DrawStringEx(stairs2Dpos.y * 15, stairs2Dpos.x * 15, GetColor(200, 200, 200), "S");
	}

}