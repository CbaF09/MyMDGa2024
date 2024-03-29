#include "TitleScene.h"
#include "DungeonScene.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/SceneManager.h"

namespace atl {
	TitleScene::TitleScene() {
		prorogueText.clear();
		
		// CSV からプロローグの文章を読み込み
		auto csv = tnl::LoadCsv("csv/ProrogueStringCSV.csv");
		for (int i = 1; i < csv.size(); ++i) {
			auto& string = csv[i][1].getString();
			prorogueText.emplace_back(string);
		}
	}

	TitleScene::~TitleScene() {
		{// タイトルシーンで使っているリソースを解放
			
			// 解放するリソースのファイルパスの一時的配列を作成
			std::vector<std::string> tempDeleteRes = {
				"graphics/BackGroundIllust/TitleSceneBack.jpg",
				"graphics/BackGroundIllust/TitleLogo.png",
				"sound/BGM/TitleSceneBGM.ogg",
				"sound/SE/TitleSceneCursorChange.ogg",
				"sound/SE/TitleSceneStart.ogg",
				"sound/SE/NextPaper.ogg",
				"graphics/UI/KeyboardW.png",
				"graphics/UI/TitleSceneCursor.png",
			};

			// リソース解放
			for (const auto& res : tempDeleteRes) {
				ResourceManager::getResourceManager()->deleteResource(res);
			}

			// フォントデータ解放
			DeleteFontToHandle(PROROGUE_FONT);
			DeleteFontToHandle(BUTTON_FONT);
		}
	}

	void TitleScene::sceneUpdate(float deltaTime) {
		draw(deltaTime);
		seq_.update(deltaTime);
	}

	void TitleScene::draw(float deltaTime) {

		// 背景描画
		drawBackground(deltaTime);
		
		if (isDisplayButton) {
			drawButton(deltaTime);
			drawSpaceIsEnter();
		}

		// フェードインアウト用描画
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);

		// 真っ黒画面の上に表示したいのでこの位置
		if (isGameEnd) {
			DrawStringEx(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2, -1, "ウィンドウの×ボタンを押してね");
		}
	}

	// 背景とロゴを描画
	void TitleScene::drawBackground(float deltaTime){
		auto resourceManager = ResourceManager::getResourceManager();
		// 背景描画
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, resourceManager->getGraphRes("graphics/BackGroundIllust/TitleSceneBack.jpg"), true);
		// ロゴ描画
		DrawRotaGraph(LOGO_POSITION.x, LOGO_POSITION.y, LOGO_SIZE, tnl::ToRadian(LOGO_DEFAULT_ANGLE), resourceManager->getGraphRes("graphics/BackGroundIllust/TitleLogo.png"), true);
	}

	void TitleScene::drawSpaceIsEnter() {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 155);
		DrawBox(SPACE_IS_ENTER_BACK_LEFTUP_POINT.x, SPACE_IS_ENTER_BACK_LEFTUP_POINT.y, SPACE_IS_ENTER_BACK_RIGHTDOWN_POINT.x, SPACE_IS_ENTER_BACK_RIGHTDOWN_POINT.y, GetColor(100, 100, 100), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ボタン用のフォントを流用
		DrawStringToHandleEx(static_cast<float>(SPACE_IS_ENTER_POSITION.x), static_cast<float>(SPACE_IS_ENTER_POSITION.y), -1, BUTTON_FONT, "スペースキー で 決定");
	}

	void TitleScene::drawButton(float deltaTime) {
		auto resourceManager = ResourceManager::getResourceManager();

		// 実際のボタンの位置
		tnl::Vector2i actualButtonPos{};

		// ボタン enum の数だけ、ボタンを描画する
		for (int i = 0; i < static_cast<int>(e_SelectTitleButton::BUTTON_MAX); ++i) {
			
			actualButtonPos = { BUTTON_POSITION.x + (i * BUTTON_OFFSET.x), BUTTON_POSITION.y + (i * BUTTON_OFFSET.y) };

			// 現在選択中のボタンの場合は、特別な処理を加える
			if (static_cast<e_SelectTitleButton>(i) == currentSelectButton_) {
				// カーソルを表示する
				DrawRotaGraph(actualButtonPos.x + BUTTON_CURSOR_OFFSET.x, actualButtonPos.y + BUTTON_CURSOR_OFFSET.y, BUTTON_CURSOR_SIZE, tnl::ToRadian(-90), ResourceManager::getResourceManager()->getGraphRes("graphics/UI/TitleSceneCursor.png"), true);
				DrawRotaGraph(actualButtonPos.x + BUTTON_CURSOR_OFFSET.x, actualButtonPos.y + BUTTON_CURSOR_OFFSET.y + CURSOR_KEYBOARD_WS_OFFSET.y, KEYBOARD_WS_SIZE, 0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/KeyboardS.png"), true);
				DrawRotaGraph(actualButtonPos.x + BUTTON_CURSOR_OFFSET.x, actualButtonPos.y + BUTTON_CURSOR_OFFSET.y - CURSOR_KEYBOARD_WS_OFFSET.y, KEYBOARD_WS_SIZE, 0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/KeyboardW.png"), true);
				// 色を変える
				SetDrawBright(200, 200, 255);
				// 位置を補正
				actualButtonPos += BUTTON_SELECTED_OFFSET;
			}

			// ボタンUIを描画
			DrawRotaGraph(actualButtonPos.x, actualButtonPos.y, BUTTON_SIZE, 0, resourceManager->getGraphRes("graphics/UI/TitleSceneButtonUI.png"), true);

			std::string buttonName;

			switch (static_cast<e_SelectTitleButton>(i)) {
			case e_SelectTitleButton::START:
				buttonName = "ゲーム開始";
				break;
			case e_SelectTitleButton::PROROGUE: 
				buttonName = "プロローグ";
				break;
			case e_SelectTitleButton::EXIT: 
				buttonName = "　終了"; // 空白で描画位置調整
				break;
			}

			DrawStringToHandleEx(static_cast<float>(actualButtonPos.x + BUTTON_STRING_OFFSET.x), static_cast<float>(actualButtonPos.y + BUTTON_STRING_OFFSET.y), -1, BUTTON_FONT, buttonName.c_str());

			// 色を元に戻す
			SetDrawBright(255, 255, 255);
		}


	}

	bool TitleScene::seqInit(float deltaTime) {
		auto fadeManager = FadeInOutManager::getFadeInOutManager();
		fadeManager->setFadeAlphaValue(255);
		fadeManager->startFadeIn();

		volumeSetting();
		
		ResourceManager::getResourceManager()->playSoundRes("sound/BGM/TitleSceneBGM.ogg",DX_PLAYTYPE_LOOP);
		
		// テクスチャによるメモリリーク対策
		dxe::Texture::DestroyUnReferenceTextures();

		seq_.change(&TitleScene::seqKeyInputWait);
		isDisplayButton = true;

		return false;
	}

	void TitleScene::volumeSetting() {
		auto rManager = ResourceManager::getResourceManager();
		// BGM
		rManager->changeVolumeSoundRes("sound/BGM/TitleSceneBGM.ogg", 130);

		// SE
		rManager->changeVolumeSoundRes("sound/SE/TitleSceneCursorChange.ogg", 120);
		rManager->changeVolumeSoundRes("sound/SE/TitleSceneStart.ogg", 150);
		rManager->changeVolumeSoundRes("sound/SE/NextPaper.ogg", 150);
	}

	bool TitleScene::seqKeyInputWait(float deltaTime) {
		// フェード中は操作不能 ( 早期リターン )
		if (FadeInOutManager::getFadeInOutManager()->isFading()) return true;

		// スペース,エンターキー,左クリックを押した時、現在選択中のボタンに応じてシーケンスを遷移
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN, eKeys::KB_SPACE)) {
			// 選択中のボタン
			switch (currentSelectButton_) {
				// スタートボタン
			case e_SelectTitleButton::START:	seq_.change(&TitleScene::seqStart);
				break;
				// プロローグボタン
			case e_SelectTitleButton::PROROGUE:	seq_.change(&TitleScene::seqProrogue);
				break;
				// ゲーム終了ボタン
			case e_SelectTitleButton::EXIT:		seq_.change(&TitleScene::seqExit);
				break;
				// デフォルトなら何もしない
			default: break;
			}
		}
		
		// 上アロー or W を押した時、選択中のボタンを一つ上のものにする
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP, eKeys::KB_W)) {
			selectButtonUp();
		}
		// 下アロー or S を押した時、選択中のボタンを一つ下のものにする
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN,eKeys::KB_S)) {
			selectButtonDown();
		}

		return true;
	}

	// 選択中のボタンを一つ上のものにする
	bool TitleScene::selectButtonUp() {
		int currentButtonInt = static_cast<int>(currentSelectButton_);

		ResourceManager::getResourceManager()->playSoundRes("sound/SE/TitleSceneCursorChange.ogg",DX_PLAYTYPE_BACK);
		--currentButtonInt;

		// 上からさらに上入力したら、下に戻る
		if (currentButtonInt < 0) {
			currentButtonInt = (static_cast<int>(e_SelectTitleButton::BUTTON_MAX) - 1);
		}

		// 更新したインデックスで選択中のボタンを更新
		currentSelectButton_ = static_cast<e_SelectTitleButton>(currentButtonInt);
		return true;
	}

	// 選択中のボタンを一つ下のものにする
	bool TitleScene::selectButtonDown() {
		int currentButtonInt = static_cast<int>(currentSelectButton_);

		// インデックスをインクリメントして、一つ下のボタンを選択
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/TitleSceneCursorChange.ogg", DX_PLAYTYPE_BACK);
		currentButtonInt++;
		// インデックスが範囲外になった場合は、最上部のボタンを選択
		if (currentButtonInt >= static_cast<int>(e_SelectTitleButton::BUTTON_MAX)) {
			currentButtonInt = 0;
		}

		// 更新したインデックスで選択中のボタンを更新
		currentSelectButton_ = static_cast<e_SelectTitleButton>(currentButtonInt);
		return true;
	}

	bool TitleScene::seqStart(float deltaTime) {
		// 最初の 1 フレームで、フェードアウトを開始
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
			ResourceManager::getResourceManager()->playSoundRes("sound/SE/TitleSceneStart.ogg", DX_PLAYTYPE_BACK);
		}

		// フェードが終わったら、ダンジョンシーンに切り替える
		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			ResourceManager::getResourceManager()->stopSoundRes("sound/SE/TitleSceneCursorChange.ogg");
			SceneManager::getSceneManager()->changeScene(std::make_shared<DungeonScene>());
		}

		return true;
	}

	bool TitleScene::seqProrogue(float deltaTime) {
		if(seq_.isStart()) {
			// 背景を少し暗くする
			FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(PROROGUE_BACKGROUND_BRIGHT);

			// 表示すべき行数を初期化
			drawLogLine_ = PROROGUE_FIRST_LOG;
			// Timerの累積時間を初期化
			totalDeltaTime_ = 0.0f;
			// ボタンを消す
			isDisplayButton = false;
			ResourceManager::getResourceManager()->playSoundRes("sound/SE/NextPaper.ogg", DX_PLAYTYPE_BACK);
		}


		totalDeltaTime_ += deltaTime;
		// 表示する行が残っている場合のみ
		if (drawLogLine_ < prorogueText.size()) {
			// totalDeltaTime が LOG_LINE_INTERVAL を超えたら、表示する行を増やす
			if (totalDeltaTime_ >= LOG_LINE_INTERVAL) {
				totalDeltaTime_ = 0.0f;
				++drawLogLine_; // 表示行増やす
				ResourceManager::getResourceManager()->playSoundRes("sound/SE/NextPaper.ogg", DX_PLAYTYPE_BACK);
			}
		}

		// 表示すべき行を全て表示する
		for (int i = 0; i < drawLogLine_; ++i) {
			DrawStringToHandleEx(static_cast<float>(TEXT_POSITION.x + (i * TEXT_OFFSET.x)), static_cast<float>(TEXT_POSITION.y + (i * TEXT_OFFSET.y)), -1,PROROGUE_FONT, "%s", prorogueText[i].c_str());
		}

		// エンターキーかスペースキーでシーケンス遷移
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE,eKeys::KB_RETURN)) {
			// ボタン再表示
			isDisplayButton = true;
			FadeInOutManager::getFadeInOutManager()->startFadeIn();
			seq_.change(&TitleScene::seqKeyInputWait);
		}

		return true;
	}

	bool TitleScene::seqExit(float deltaTime) {
		if (seq_.isStart()) {
			ClipCursor(NULL);
			SetMouseDispFlag(true);
			isDisplayButton = false;
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			isGameEnd = true;
		}

		// 終了処理。記述しない
		// exit 関数で終了していたが、これではgameEnd()やDxLib_End()が呼ばれない。
		// TODO : やり方聞く
		return true;
	}

	void TitleScene::debug_keyInput() {

	}

}