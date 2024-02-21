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
#include "../MagicRuneSystem/MagicRuneSystemManager.h"
#include "../MagicRuneSystem/MagicRune.h"
#include "../MeshObject/Wall.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/ItemPawn.h"
#include "../Object/SelectWindow.h"
#include "../Object/MenuWindow.h"
#include "../Utilities/Atl3DCamera.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/EnemyFactory.h"

namespace atl {

	DungeonScene::DungeonScene() {
		walls_.clear();
		groundTiles_.clear();
		items_.clear();

		player_ = std::make_shared<PlayerPawn>();
		menuWindow_ = std::make_shared<MenuWindow>(player_->getPlayerData()->getInventory());
	}

	DungeonScene::~DungeonScene() {
		// 解放するリソースのファイルパスの一時的配列を作成
		std::vector<std::string> tempDeleteRes = {
			"graphics/UI/HPbarBackGround.png",
			"graphics/UI/HPbarRed.bmp",
			"graphics/UI/HPbarGreen.bmp",
			"graphics/UI/Instruction.png",
			"sound/BGM/DungeonSceneBGM.ogg",
			"sound/SE/DungeonSceneCloseMenu.ogg",
			"sound/SE/DungeonSceneOpenMenu.ogg",
			"sound/SE/DungeonSceneOpenSelectMenu.ogg",
		};

		// リソース解放
		for (const auto& res : tempDeleteRes) {
			ResourceManager::getResourceManager()->deleteResource(res);
		}

		// フォントデータの解放
		DeleteFontToHandle(NEXT_FLOOR_FONT);
		DeleteFontToHandle(LEVEL_STRING_FONT);
	}

	void DungeonScene::render(float deltaTime, const Shared<Atl3DCamera> camera) {
		// スカイスフィアのアップデート
		skysphere_.update(camera);

		for (const auto& ground : groundTiles_) { ground->renderObject(camera); }
		for (const auto& wall : walls_) { wall->renderObject(camera); }
		if (stairs_) { stairs_->renderObjects(camera, deltaTime); }
		if (player_) { player_->render(deltaTime); };
		for (const auto& item : items_) { item->renderObject(camera, deltaTime); }

		// 敵描画
		EnemyManager::getEnemyManager()->renderAllEnemy(camera, deltaTime);
	}

	void DungeonScene::draw2D(float deltaTime) {

		drawUI(deltaTime);

		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);

		// 現在階層の描画は真っ黒な画面の上にやりたいので、この位置
		if (isNextFloorTransition) { drawNextFloorTransition(); }

		// デバッグモード中に描画するマップやその他情報
		if (isDebug) {
			debug_displayMap(deltaTime);
			debug_displayDungeonParam(deltaTime);
		}
	}

	// 次の階層を描画する
	void DungeonScene::drawNextFloorTransition() {
		// 現在階層が、最上階 ( クリア階 ) でない場合の描画
		if (currentFloor_ != MAX_FLOOR) {
			DrawStringToHandleEx(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, -1, NEXT_FLOOR_FONT, "ワイズマンの修練塔 [%d / %d]", currentFloor_, MAX_FLOOR);
		}
		else { // クリア階の場合の描画
			DrawStringToHandleEx(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, -1, NEXT_FLOOR_FONT, "ワイズマンの修練塔 [ 最上階 ]", currentFloor_);
		}
	}

	void DungeonScene::drawUI(float deltaTime) {
		// HP バー 表示
		drawHPbar();

		// 操作説明の描画
		drawInstruction();

		// メニューを開いている時はログ表示無し,レベル表示無し,満腹度表示無し
		if (!player_->getIsMenuOpen()) {
			TextLogManager::getTextLogManager()->displayTextLog(TEXT_LOG_POSITION.x, TEXT_LOG_POSITION.y, deltaTime);
			drawLevel();
			drawInvatation();
		}

		// メニューを開いている間の描画
		if (player_->getIsMenuOpen()) {
			menuWindow_->draw(deltaTime);
			drawMinimap(deltaTime);
		}

		// ルーン装備ウィンドウ描画
		magicRuneWindow_.draw();

		// セレクトウィンドウがある時は、描画する
		if (isSelectWindow_) selectWindow_.draw(deltaTime);
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

	void DungeonScene::drawLevel() {
		DrawStringToHandleEx(static_cast<float>(LEVEL_STRING_POSITION.x), static_cast<float>(LEVEL_STRING_POSITION.y), -1, LEVEL_STRING_FONT, "レベル ... [ %d ]", player_->getPlayerData()->getCurrentLevel());
	}

	void DungeonScene::drawInstruction() {
		// 操作説明の背景描画
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 125);
		DrawBoxEx({ static_cast<float>(INSTRUCTION_POSITION.x),static_cast<float>(INSTRUCTION_POSITION.y),0 }, static_cast<float>(INSTRUCTION_BACK_BOX_SIZE.x), static_cast<float>(INSTRUCTION_BACK_BOX_SIZE.y), true, GetColor(0, 0, 255));
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

		// 操作説明の画像描画
		DrawRotaGraph(INSTRUCTION_POSITION.x, INSTRUCTION_POSITION.y, INSTRUCTION_SIZE, 0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/Instruction.png"), true);
	}


	void DungeonScene::drawInvatation() {
		// 招待状 UI を描画
		// 現在空腹度の10分の1を輝度に
		SetDrawBright(currentSatiety_ / 10, currentSatiety_ / 10, currentSatiety_ / 10);
		DrawRotaGraph(INVATATION_POSITION.x, INVATATION_POSITION.y, INVATATION_SIZE, tnl::ToRadian(INVATATION_ANGLE), ResourceManager::getResourceManager()->getGraphRes("graphics/UI/Invatation.png"), true);
		// 描画輝度を元に戻す
		SetDrawBright(255, 255, 255);

		// 文字列「招待状」を描画。レベルを表示しているフォントを流用。
		DrawStringToHandleEx(static_cast<float>(INVATATION_STRING_POSITION.x), static_cast<float>(INVATATION_STRING_POSITION.y), -1, LEVEL_STRING_FONT, "招待状");
	}

	void DungeonScene::drawMinimap(float deltaTime) {
		auto& field = DungeonCreater::getDungeonCreater()->getFieldCells();

		// フィールド情報の描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MINIMAP_ALPHA);
		for (int x = 0; x < field.size(); ++x) {
			for (int y = 0; y < field[x].size(); ++y) {
				// 何もない場合、早期continue
				if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_NONE) { continue; }
				// 未発見の場合、早期continue
				if (field[x][y].isDiscoverByPlayer == false) { continue; }

				// cellType に応じて色を変える
				int drawColor = 0;
				if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_ROOM) {
					// 白
					drawColor = GetColor(255, 255, 255);
				}
				else if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_PATH) {
					// 青
					drawColor = GetColor(0, 0, 255);
				}
				// 描画位置の計算
				tnl::Vector2i drawPos = calcDrawMinimapPos(x, y);
				// 描画
				DrawBoxEx({ static_cast<float>(drawPos.x),static_cast<float>(drawPos.y) ,0 }, static_cast<float>(MINIMAP_CELL_SIZE), static_cast<float>(MINIMAP_CELL_SIZE), true, drawColor);
			}
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 壁は透明度無しの黒で描画
		for (int x = 0; x < field.size(); ++x) {
			for (int y = 0; y < field[x].size();++y) {
				// 壁以外は早期リターン
				if (field[x][y].cellType_ != DungeonCreater::e_FieldCellType::CELL_TYPE_WALL) { continue; }
				// 未発見の場合、早期continue
				if (field[x][y].isDiscoverByPlayer == false) { continue; }

				// 描画位置の計算
				tnl::Vector2i drawPos = calcDrawMinimapPos(x, y);
				// 描画 ( 黒 )
				DrawBoxEx({ (float)drawPos.x,(float)drawPos.y ,0 }, static_cast<float>(MINIMAP_CELL_SIZE), static_cast<float>(MINIMAP_CELL_SIZE), true, GetColor(0, 0, 0));
			}
		}

		{// プレイヤーの位置描画
			auto& player2Dpos = player_->getPlayer2Dpos();
			tnl::Vector2i playerDrawPos = calcDrawMinimapPos(player2Dpos.x, player2Dpos.y);
			DrawCircle(playerDrawPos.x, playerDrawPos.y, MINIMAP_PLAYER_SIZE, GetColor(255, 255, 0));
		}
	}

	const tnl::Vector2i DungeonScene::calcDrawMinimapPos(int32_t x, int32_t y) {
		// Y と X が逆になっているのは、三次元空間との整合性を取る為なので、問題ないです。
		int32_t drawPosX = (y * MINIMAP_CELL_SIZE) + MINIMAP_LEFTUP_POSITION.x;
		int32_t drawPosY = (x * MINIMAP_CELL_SIZE) + MINIMAP_LEFTUP_POSITION.y;
		return { drawPosX,drawPosY };
	};

	void DungeonScene::sceneUpdate(float deltaTime) {
		// シーケンスアップデート
		seq_.update(deltaTime);

		// カメラのアップデート
		player_->getPlayerCamera()->update();

		// レンダー ( カメラアップデートの後 )
		render(deltaTime, player_->getPlayerCamera());

		// 2D系の描画
		draw2D(deltaTime);

		{// デバッグモード関連
			// O + P キーで、デバッグモードに切り替え ( OverPower )
			if (tnl::Input::IsKeyDown(eKeys::KB_O) && tnl::Input::IsKeyDownTrigger(eKeys::KB_P)) { isDebug = !isDebug; }
			// デバッグモード中にスペースを押すと、ダンジョン再生成する
			if (isDebug) {
				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) 	generateDungeon();;
			}
		}
	}

	void DungeonScene::openMenu() {
		player_->openMenuBook();
		// アイテム一覧の表示を更新する
		menuWindow_->itemWindowsUpdate();
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneOpenMenu.ogg", DX_PLAYTYPE_BACK);
	}

	void DungeonScene::closeMenu() {
		player_->closeMenuBook();
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneCloseMenu.ogg", DX_PLAYTYPE_BACK);
	}

	void DungeonScene::openSelectWindow(const std::string& question) {
		selectWindow_.setSelectWindowQuestionText(question);
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneOpenSelectMenu.ogg", DX_PLAYTYPE_BACK);
		isSelectWindow_ = true;
	}

	void DungeonScene::closeSelectWindow() {
		isSelectWindow_ = false;
	}

	void DungeonScene::minimapUpdate(const tnl::Vector2i& openCellPos) {
		auto dCreater = DungeonCreater::getDungeonCreater();

		// 開こうとしているセルのセルタイプを取得
		auto openCellType = dCreater->getFieldCellType(openCellPos);

		// セルタイプに応じて分岐
		switch (openCellType) {
			// 何も無いならブレーク
		case DungeonCreater::e_FieldCellType::CELL_TYPE_NONE:
			break;
			// 壁なら、そこをオープンしてブレーク
		case DungeonCreater::e_FieldCellType::CELL_TYPE_WALL:
			dCreater->discoverFieldCell(openCellPos);
			break;
			// 通路なら、そこをオープンしてブレーク
		case DungeonCreater::e_FieldCellType::CELL_TYPE_PATH:
			dCreater->discoverFieldCell(openCellPos);
			dCreater->discoverFieldCell(openCellPos + tnl::Vector2i{ 0,1 });
			dCreater->discoverFieldCell(openCellPos + tnl::Vector2i{ 0,-1 });
			dCreater->discoverFieldCell(openCellPos + tnl::Vector2i{ 1,0 });
			dCreater->discoverFieldCell(openCellPos + tnl::Vector2i{ -1,0 });
			break;
			// 部屋なら、そこをオープンしてから、既に発見済みの場所でない事を確認してから再帰処理
		case DungeonCreater::e_FieldCellType::CELL_TYPE_ROOM:
			dCreater->discoverFieldCell(openCellPos);
			if (!dCreater->isDiscoverFieldCell(openCellPos + tnl::Vector2i{ 0,1 })) {
				minimapUpdate(openCellPos + tnl::Vector2i{ 0,1 });
			}
			if (!dCreater->isDiscoverFieldCell(openCellPos + tnl::Vector2i{ 0,-1 })) {
				minimapUpdate(openCellPos + tnl::Vector2i{ 0,-1 });
			}
			if (!dCreater->isDiscoverFieldCell(openCellPos + tnl::Vector2i{ 1,0 })) {
				minimapUpdate(openCellPos + tnl::Vector2i{ 1,0 });
			}
			if (!dCreater->isDiscoverFieldCell(openCellPos + tnl::Vector2i{ -1,0 })) {
				minimapUpdate(openCellPos + tnl::Vector2i{ -1,0 });
			}
			break;
		}
	}

	void DungeonScene::soundVolumeFix() {
		auto rManager = ResourceManager::getResourceManager();
		rManager->changeVolumeSoundRes("sound/BGM/DungeonSceneBGM.ogg", 80);
		rManager->changeVolumeSoundRes("sound/SE/DungeonSceneCloseMenu.ogg", 200);
		rManager->changeVolumeSoundRes("sound/SE/DungeonSceneOpenMenu.ogg", 180);
	}

	// タイマーがリスポーンカウントを超えたら、敵が一体スポーンする
	// スポーン先に既にエネミーがいた場合、次のターンにスポーンする
	void DungeonScene::enemyResporn() {
		++respornTurnTimer_;

		// スポーン条件を満たしていないなら早期リターン
		if (!(respornTurnTimer_ > RESPORN_TURN_COUNT)) { return; }

		// スポーン位置を決める ( プレイヤーとは違うエリアの位置が返ってくる ) 
		auto spawnPos = DungeonCreater::getDungeonCreater()->randomChoiceEnemyRespawnPos(player_->getPlayer2Dpos());

		// スポーン位置に既に敵がいたら早期リターンする
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (const auto& enemy : enemyList) {
			auto& enemyPos = enemy->get2Dpos();
			// スポーン先に既に敵がいるかチェック。いたらリターン
			if (enemyPos.x == spawnPos.x && enemyPos.y == spawnPos.y) { return; }
		}

		respornTurnTimer_ = 0;
		EnemyManager::getEnemyManager()->setCurrentFactory(std::make_shared<SlimeFactory>());
		EnemyManager::getEnemyManager()->generateEnemy(spawnPos);
	}

	// 未移動のエネミーのアップデートを回す
	void DungeonScene::enemyMove(float deltaTime) {
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) {
			// 既に移動終わっているエネミーは早期リターン
			if (enemy->getIsAlreadyMove()) { continue; }
			enemy->process(deltaTime);
		}
	}

	// 未行動のエネミーのアップデートを回す
	void DungeonScene::enemyAction(float deltaTime) {
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) {
			// 既に移動終わっているエネミーは早期リターン
			if (enemy->getIsAlreadyAction()) { continue; }
			enemy->process(deltaTime);
		}
	}

	void DungeonScene::pickUpItem() {
		auto& player2Dpos = player_->getPlayer2Dpos();
		for (auto it = items_.begin(); it != items_.end();) {
			auto& item2Dpos = (*it)->get2Dpos();
			// アイテムと重なっていない場合、早期　continue
			if (player2Dpos.x != item2Dpos.x || player2Dpos.y != item2Dpos.y) {
				++it;
				continue;
			}

			auto item = (*it)->getItem();
			// インベントリにアイテムデータを格納した時
			if (player_->getPlayerData()->getInventory()->pushbackItemToInventory(item)) {
				// テキストログに拾ったアイテム名を出力
				auto& itemName = item->getItemName();
				std::string textLog = "　" + itemName + "を拾った";
				TextLogManager::getTextLogManager()->addTextLog(textLog);

				// アイテムを削除
				it = items_.erase(it);
				// １マスにアイテムが二つある事はないので、一個拾ったらbreak
				break;
			}
			else
			{
				TextLogManager::getTextLogManager()->addTextLog("荷物がいっぱいで拾えない");
				break;
			}
		}
	}

	void DungeonScene::turnHealHP() {
		// HP 自動回復
		player_->getPlayerData()->changeCurrentHP(EVERY_TURN_HEAL);
	}

	//---------------------
	// シーケンス
	//---------------------

	// 初期設定 ( DungeonSceneのインスタンス生成後、一度だけ通る ) 
	bool DungeonScene::seqInit(float deltaTime) {
		// フェードは真っ黒からスタート
		FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(255);

		// プレイヤーの生成と初期化
		player_->initialize(shared_from_this());

		// 現在の階層を 初期化
		currentFloor_ = 0;
		// サウンド調整
		soundVolumeFix();
		// BGM再生
		ResourceManager::getResourceManager()->playSoundRes("sound/BGM/DungeonSceneBGM.ogg", DX_PLAYTYPE_LOOP);

		/*発表用*/
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<HealRune>(), *shared_from_this());
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<StoneRune>(), *shared_from_this());
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<FireRune>(), *shared_from_this());
		/*発表用*/

		// 本シーケンスに遷移
		seq_.change(&DungeonScene::seqToNextFloor);
		return true;
	}

	// 毎ターン初期化処理
	bool DungeonScene::seqTurnInit(float deltaTime) {
		// プレイヤーの行動フラグをオフ
		player_->offFlagIsAlreadyTurn();
		
		// エネミーの行動フラグをオフ
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) { enemy->offFlagIsAlreadyTurn(); }

		// ターン開始時処理へ
		seq_.change(&DungeonScene::seqTurnStart);
		return true;
	}

	// ターン開始時処理
	bool DungeonScene::seqTurnStart(float deltaTime) {
		// HP 自動回復
		turnHealHP();

		// 敵のリスポーン判定
		enemyResporn();

		// ミニマップ更新用
		minimapUpdate(player_->getPlayer2Dpos());

		// キー入力待ちへ
		seq_.change(&DungeonScene::seqKeyInput);

		return true;
	}

	// キー入力待ち
	bool DungeonScene::seqKeyInput(float deltaTime) {
		// フェードイン ・ アウト中であれば操作不能
		if (FadeInOutManager::getFadeInOutManager()->isFading()) return true;

		// WASDで移動
		if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_A, eKeys::KB_S, eKeys::KB_D)) {
			// 移動可能か判定してから、シーケンス遷移
			if (player_->startMove()) {
				// 移動した場合、階段に乗っているフラグをオフにする
				isPlayerOnStairs_ = false;
				seq_.change(&DungeonScene::seqPlayerMoveTurn);
			}
			return true;
		}
		// 左クリックで攻撃
		else if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
			player_->startAttack();
			seq_.change(&DungeonScene::seqPlayerActionTurn);
			return true;
		}
		// 右クリックでメニューウィンドウ
		else if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			openMenu();
			seq_.change(&DungeonScene::seqMenuWindow);
		}

		return true;
	}

	bool DungeonScene::seqPlayerMoveTurn(float deltaTime) {
		SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {

			player_->playerUpdate(deltaTime);

			enemyMove(deltaTime);

			// 全エネミー移動完了フラグ
			bool allEnemyMoved = true;
			auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
			for (const auto& enemy : enemyList) {
				// もし移動が完了していなければ、全エネミー移動完了フラグをオフに
				if (!enemy->getIsAlreadyMove()) { allEnemyMoved = false; }
			}

			// プレイヤーと全てのエネミーの移動が完了していたら、遷移
			if (player_->getIsAlreadyTurn() && allEnemyMoved) { SEQ_CO_BREAK; }
			});

		SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {
			// 未行動エネミーの行動
			enemyAction(deltaTime);

			// 全てのエネミーの行動が完了しているか
			bool allEnemyAction = true;
			auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
			for (const auto& enemy : enemyList) {
				// もし移動が完了していなければ、全エネミー移動完了フラグをオフに
				if (!enemy->getIsAlreadyAction()) { allEnemyAction = false; }
			}

			// 全てのエネミーの行動が完了していたら、遷移
			if (allEnemyAction) {
				// ターンエンド処理へ
				seq_.change(&DungeonScene::seqTurnEnd);
			}
			});

		SEQ_CO_END;
	}

	// プレイヤーが攻撃を選択したターン
	bool DungeonScene::seqPlayerActionTurn(float deltaTime) {
		SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {
			// プレイヤーのターン処理 ( 攻撃 )
			if (!player_->getIsAlreadyTurn()) {
				player_->playerUpdate(deltaTime);
			}

			// 未行動エネミーの行動
			enemyAction(deltaTime);

			// 全てのエネミーの行動が完了しているか
			bool allEnemyAction = true;
			auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
			for (const auto& enemy : enemyList) {
				// もし移動が完了していなければ、全エネミー移動完了フラグをオフに
				if (!enemy->getIsAlreadyAction()) { allEnemyAction = false; }
			}

			// 全てのエネミーの行動が完了していたらコルーチン終了
			if (allEnemyAction) {
				SEQ_CO_BREAK;
			}
			});

		// エネミーの移動
		SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {
			// 未移動のエネミーの移動処理
			enemyMove(deltaTime);

			// 全てのエネミーの移動が完了しているか
			bool allEnemyMoved = true;
			auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
			for (const auto& enemy : enemyList) {
				// もし移動が完了していなければ、全エネミー移動完了フラグをオフに
				if (!enemy->getIsAlreadyMove()) { allEnemyMoved = false; }
			}

			// プレイヤーと全てのエネミーの移動が完了していたら、コルーチン終了
			if (player_->getIsAlreadyTurn() && allEnemyMoved) {
				SEQ_CO_BREAK;
			}
			});

		// ターンエンド処理へ
		seq_.change(&DungeonScene::seqTurnEnd);

		SEQ_CO_END;
	}

	// ターンエンド処理
	bool DungeonScene::seqTurnEnd(float deltaTime) {

		// プレイヤーのHPがゼロになっている場合、直接ゲームオーバーシーケンスに遷移
		if (player_->getPlayerData()->isZeroHP()) {
			seq_.change(&DungeonScene::seqGameOver);
			return true;;
		}

		// 空腹度を減らし、0になったら、ゲームオーバーに遷移
		currentSatiety_ -= SATIETY_SUB_VALUE;
		if (currentSatiety_ <= 0) {
			currentSatiety_ = 0;
			seq_.change(&DungeonScene::seqGameOver);
			return true;;
		}

		// 階段に乗った時、階段に乗っているフラグが立っていない場合、階段シーケンスに遷移
		auto& player2Dpos = player_->getPlayer2Dpos();
		auto& stairs2Dpos = stairs_->get2Dpos();
		if (stairs2Dpos.x == player2Dpos.x && stairs2Dpos.y == player2Dpos.y && !isPlayerOnStairs_) {
			seq_.change(&DungeonScene::seqOnStairs);
			return true;
		}

		// 足元にアイテムがあったら拾う
		pickUpItem();

		// ターンの初期化処理へ
		seq_.change(&DungeonScene::seqTurnInit);
		return true;
	}

	bool DungeonScene::seqGameOver(float deltaTime) {
		// 最初のフレームでフェードアウトを行い、フェードアウトが完了したらゲームオーバーシーンに遷移
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			ResourceManager::getResourceManager()->stopSoundRes("sound/BGM/DungeonSceneBGM.ogg");
			SceneManager::getSceneManager()->changeScene(std::make_shared<GameOverScene>());
		}
		return true;
	}

	// 死んだ敵を配列から削除
	//void DungeonScene::deadEnemyErase() {
	//	for (auto it = enemies_.begin(); it != enemies_.end();) {
	//		// HP がゼロの敵を発見
	//		if ((*it)->getEnemyData()->isZeroHP()) {
	//			// 死亡しているかどうか
	//			if ((*it)->getIsDead()) {
	//				// エネミーが死亡する時、プレイヤーはエネミーの持っていた経験値を得る
	//				auto getEXP = (*it)->getEnemyData()->getEnemyExp();
	//				player_->getPlayerData()->changeCurrentEXP(getEXP);
	//				// 敵へのポインターを配列から削除
	//				it = enemies_.erase(it);
	//				continue;
	//			}
	//		}
	//		++it;
	//	}
	//}

	bool DungeonScene::seqMenuWindow(float deltaTime) {
		// メニューウィンドウが有効でない場合、早期リターン
		if (!player_->getIsMenuOpen()) { return false; }

		// 右クリックで menuWindowを閉じる
		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			closeMenu();
			seq_.change(&DungeonScene::seqKeyInput);
		}

		selectedMenu_ = menuWindow_->process(deltaTime);

		// MenuWindowクラスから、何を選択中かのインデックスがenum型に変換されて返ってくるので、それに応じて処理
		switch (selectedMenu_) {
		case MenuWindow::e_SelectedMenuWindow::NONE: break; // 何も選択されなかったフレームは何もしない
		case MenuWindow::e_SelectedMenuWindow::Item1: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item2: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item3: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item4: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item5: // ブレイクスルー
		case MenuWindow::e_SelectedMenuWindow::Item6:
			// nullptrチェックをしてから、本当に使うかの確認シーケンスに遷移
			if (player_->getPlayerData()->getInventory()->getItem(static_cast<int>(selectedMenu_)) != nullptr) { seq_.change(&DungeonScene::seqReallyUseItem); } break;
		case MenuWindow::e_SelectedMenuWindow::EraseMagicRune: { // 装備中のルーンを確認する
			// 装備中のルーンがあれば
			if (!MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes().empty()) {
				// シーケンス遷移
				seq_.change(&DungeonScene::seqEraseMagicRuneWindow);
			}
			// なければ
			else {
				// TODO : 処理記述
			}
			break;
		}
		case MenuWindow::e_SelectedMenuWindow::CloseMenu: {// メニューを閉じる
			closeMenu();
			seq_.change(&DungeonScene::seqKeyInput);
			break;
		}
		case MenuWindow::e_SelectedMenuWindow::ReturnToTitle: {// タイトルに戻る
			seq_.change(&DungeonScene::seqReallyReturnToTitle);
			break;
		}
		}

		return true;
	}

	bool DungeonScene::seqEraseMagicRuneWindow(float deltaTime) {
		if (seq_.isStart()) {
			// ウィンドウをオープン状態に
			magicRuneWindow_.switchOpenMagicRuneWindow();
		}

		// A,Dで選択肢切り替え
		magicRuneWindow_.process();

		// 左クリック で、選んだルーンを削除する
		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
			auto selectRune = magicRuneWindow_.getCurrentSelectRune_();
			MagicRuneSystemManager::getMagicRuneSystemManager()->removeRune(selectRune, *shared_from_this());
			magicRuneWindow_.resetIndex();
			magicRuneWindow_.switchOpenMagicRuneWindow();
			seq_.change(&DungeonScene::seqMenuWindow);
		}
		// 右クリック で、キャンセルしてメニューに戻る
		else if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			magicRuneWindow_.switchOpenMagicRuneWindow();
			seq_.change(&DungeonScene::seqMenuWindow);
		}

		return false;
	}

	// 本当にアイテムを使うか確認
	bool DungeonScene::seqReallyUseItem(float deltaTime) {
		auto& item = player_->getPlayerData()->getInventory()->getItem(static_cast<int>(selectedMenu_));
		if (seq_.isStart()) {
			openSelectWindow(item->getItemName() + "\nを使いますか？");
		}
		if (isSelectWindow_) {
			if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::YES) { // はい、の時の処理
				// アイテム使用してメニューウィンドウを閉じる。エネミーが行動する
				player_->getPlayerData()->getInventory()->useItem(static_cast<int32_t>(selectedMenu_));
				closeMenu();
				closeSelectWindow();
				player_->onFlagIsAlreadyTurn();
				seq_.change(&DungeonScene::seqPlayerActionTurn);
			}
			else if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::NO) {	// いいえ、の時の処理
				closeSelectWindow();
				seq_.change(&DungeonScene::seqMenuWindow);
			}
		}
		return true;
	}

	// 本当にタイトルに戻るかどうか確認中
	bool DungeonScene::seqReallyReturnToTitle(float deltaTime) {
		if (seq_.isStart()) {
			openSelectWindow("タイトル画面に戻りますか？\n進捗データは残りません");
		}

		if (isSelectWindow_) {
			if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::YES) { // はい、の時の処理
				seq_.change(&DungeonScene::seqReturnToTitle);
			}
			else if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::NO) {	// いいえ、の時の処理
				closeSelectWindow();
				seq_.change(&DungeonScene::seqMenuWindow);
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
			// 1秒 待機
			SEQ_CO_YIELD_RETURN_TIME(1, deltaTime, [&] {});

			ResourceManager::getResourceManager()->stopSoundRes("sound/BGM/DungeonSceneBGM.ogg");
			SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
		}

		SEQ_CO_END
	}

	// 階段に乗った時の処理
	bool DungeonScene::seqOnStairs(float deltaTime) {
		// ウィンドウがまだ無い場合は生成する
		if (seq_.isStart()) {
			isPlayerOnStairs_ = true;
			openSelectWindow("次の階層に進みますか？");
		}

		if (isSelectWindow_) {
			// はい、の時の処理
			if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::YES) {
				closeSelectWindow();
				seq_.change(&DungeonScene::seqToNextFloor);

			}
			// いいえ、の時
			else if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::NO) {
				closeSelectWindow();
				seq_.change(&DungeonScene::seqTurnInit);
			}
		}

		return true;
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

		// フェード中は処理しない
		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			// 次階層に遷移中フラグを立てる
			isNextFloorTransition = true;

			// 待機 
			SEQ_CO_YIELD_RETURN_TIME(nextFloorTransitionTime, deltaTime, [&] {});

			// 最大階層に到達したら、クリアシーンに遷移
			if (currentFloor_ >= MAX_FLOOR) {
				ResourceManager::getResourceManager()->stopSoundRes("sound/BGM/DungeonSceneBGM.ogg");
				SceneManager::getSceneManager()->changeScene(std::make_shared<GameClearScene>());
			}
			else {
				// ダンジョンを生成し、フェードインし画面を表示させる
				generateDungeon();
				FadeInOutManager::getFadeInOutManager()->startFadeIn();
				seq_.change(&DungeonScene::seqTurnInit);
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
		// 地形初期化
		walls_.clear();
		groundTiles_.clear();

		// アイテム初期化
		items_.clear();
	}

	void DungeonScene::generateDungeon() {
		initDungeon();

		DungeonCreater::getDungeonCreater()->createDungeon();
		auto& fieldData = DungeonCreater::getDungeonCreater()->getFieldCells();

		for (int x = 0; x < fieldData.size(); ++x) {
			for (int y = 0; y < fieldData[x].size(); ++y) {
				// 何もない場所なら早期リターン
				if (fieldData[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_NONE) { continue; }

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
			stairs_ = std::make_shared<Stairs>(stairsSpawnPos);

			// エネミー
			auto& enemySpawnPos = DungeonCreater::getDungeonCreater()->getEnemySpawnPos();
			EnemyManager::getEnemyManager()->setCurrentFactory(std::make_shared<SlimeFactory>());
			for (int i = 0; i < DungeonCreater::getDungeonCreater()->getEnemySpawnNum(); ++i) {
				EnemyManager::getEnemyManager()->generateEnemy(enemySpawnPos[i]);
			}

			// アイテム
			auto& itemSpawnPos = DungeonCreater::getDungeonCreater()->getItemSpawnPos();
			for (int i = 0; i < DungeonCreater::getDungeonCreater()->getItemSpawnNum(); ++i) {
				auto item = std::make_shared<ItemPawn>(itemSpawnPos[i]);
				item->initialize();
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

		// 階段の位置
		if (stairs_) {
			auto& stairsPos = stairs_->get2Dpos();
			DrawStringEx(600, 25, -1, "stairsPos ... [ %d, %d ]", stairsPos.x, stairsPos.y);
		}

		player_->debug_displayPlayerParam(600, 100);
		player_->getPlayerData()->debug_playerDataParam(600, 300);


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
				else if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_PATH) {
					DrawStringEx((y * 15), (x * 15), -1, "-");
				}
				else if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_WALL) {
					DrawStringEx((y * 15), (x * 15), -1, "*");
				}
			}
		}

		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (const auto& enemy : enemyList) {
			auto& enemyPos = enemy->get2Dpos();
			DrawStringEx(enemyPos.y * 15, enemyPos.x * 15, GetColor(255, 0, 0), "e");
		}

		for (const auto& item : items_) {
			auto& itemPos = item->get2Dpos();
			DrawStringEx(itemPos.y * 15, itemPos.x * 15, GetColor(0, 255, 0), "I");
		}

		auto& player2Dpos = player_->getPlayer2Dpos();
		DrawStringEx(player2Dpos.y * 15, player2Dpos.x * 15, GetColor(0, 0, 255), "P");

		if (stairs_) {
			auto& stairs2Dpos = stairs_->get2Dpos();
			DrawStringEx(stairs2Dpos.y * 15, stairs2Dpos.x * 15, GetColor(200, 200, 200), "S");
		}
	}

}