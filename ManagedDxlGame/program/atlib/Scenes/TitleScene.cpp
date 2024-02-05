#include "TitleScene.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/SceneManager.h"
#include "DungeonScene.h"

namespace atl {

	TitleScene::~TitleScene() {
		{// タイトルシーンで使っているリソースを解放
			
			// 解放するリソースのファイルパスの一時的配列を作成
			std::vector<std::string> tempDeleteRes = {
				"graphics/BackGroundIllust/TitleSceneBack.jpg"
				"graphics/BackGroundIllust/TitleLogo.png"
			};
			
			// ファイルパスを一つずつ解放。解放の成功失敗をデバッグログに出力
			for (const auto& res : tempDeleteRes) {
				bool isDelete = ResourceManager::getResourceManager()->deleteResource(res);
				if(!isDelete) { tnl::DebugTrace("\n------------------------------\nTitleScene::デストラクタ メモリ解放 => 正常に解放されていません"); }
				else { tnl::DebugTrace("\n------------------------------\nTitleScene::デストラクタ メモリ解放 => 正常"); }
			}
			tnl::DebugTrace("\n------------------------------\n"); // ログが見づらいので最後に改行と切り取り線を入れる
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
		}
		// フェードインアウト用描画
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);
	}

	// 背景とロゴを描画
	void TitleScene::drawBackground(float deltaTime){
		auto resourceManager = ResourceManager::getResourceManager();
		// 背景描画
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, resourceManager->getGraphRes("graphics/BackGroundIllust/TitleSceneBack.jpg"), true);
		// ロゴ描画
		DrawRotaGraph(LOGO_POSITION.x, LOGO_POSITION.y, LOGO_SIZE, tnl::ToRadian(LOGO_DEFAULT_ANGLE), resourceManager->getGraphRes("graphics/BackGroundIllust/TitleLogo.png"), true);
	}

	void TitleScene::drawButton(float deltaTime) {
		auto resourceManager = ResourceManager::getResourceManager();

		// 元のフォントサイズを保存
		int beforeFontSize = GetFontSize();
		SetFontSize(30);

		// ボタン enum の数だけ、ボタンを描画する
		for (int i = 0; i < static_cast<int>(e_SelectTitleButton::BUTTON_MAX); ++i) {
			
			// 実際のボタンの位置
			tnl::Vector2i actualButtonPos = { BUTTON_POSITION.x + (i * BUTTON_OFFSET.x), BUTTON_POSITION.y + (i * BUTTON_OFFSET.y) };

			// 現在選択中のボタンの場合は、特別な処理を加える
			if (static_cast<e_SelectTitleButton>(i) == currentSelectButton_) {
				// 色を変える
				SetDrawBright(200, 200, 255);
				// 位置を補正
				actualButtonPos += BUTTON_SELECTED_OFFSET;
			}

			DrawRotaGraph(actualButtonPos.x, actualButtonPos.y, BUTTON_SIZE, 0, resourceManager->getGraphRes("graphics/UI/TitleSceneButtonUI.png"), true);

			std::string buttonName;

			switch (static_cast<e_SelectTitleButton>(i)) {
			case e_SelectTitleButton::START: buttonName = "ゲーム開始";
				break;
			case e_SelectTitleButton::PROROGUE: buttonName = "プロローグ";
				break;
			case e_SelectTitleButton::EXIT: buttonName = "　終了"; // 空白で描画位置調整
				break;
			}

			DrawStringEx(actualButtonPos.x + BUTTON_STRING_OFFSET.x, actualButtonPos.y + BUTTON_STRING_OFFSET.y, -1, buttonName.c_str());

			if (static_cast<e_SelectTitleButton>(i) == currentSelectButton_) {
				// 元に戻す
				SetDrawBright(255, 255, 255);
			}
		}
		
		// フォントサイズを元に戻す
		SetFontSize(beforeFontSize);
	}

	bool TitleScene::seqInit(float deltaTime) {
		auto fadeManager = FadeInOutManager::getFadeInOutManager();
		fadeManager->setFadeAlphaValue(255);
		fadeManager->startFadeIn();
		
		seq_.change(&TitleScene::seqKeyInputWait);
		isDisplayButton = true;

		return false;
	}

	bool TitleScene::seqKeyInputWait(float deltaTime) {
		// フェード中は操作不能 ( 早期リターン )
		if (FadeInOutManager::getFadeInOutManager()->isFading()) return true;

		// スペース,エンターキーを押した時、現在選択中のボタンに応じてシーケンスを遷移
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
		}

		// フェードが終わったら、ダンジョンシーンに切り替える
		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
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

		}


		totalDeltaTime_ += deltaTime;
		// 表示する行が残っている場合のみ
		if (drawLogLine_ < prorogueText.size()) {
			// totalDeltaTime が LOG_LINE_INTERVAL を超えたら、表示する行を増やす
			if (totalDeltaTime_ >= LOG_LINE_INTERVAL) {
				totalDeltaTime_ = 0.0f;
				++drawLogLine_; // 表示行増やす
			}
		}

		int beforeFontSize = GetFontSize();
		SetFontSize(PROROGUE_STRING_FONTSIZE);
		// 表示すべき行を全て表示する
		for (int i = 0; i < drawLogLine_; ++i) {
			DrawStringEx(TEXT_POSITION.x + (i * TEXT_OFFSET.x), TEXT_POSITION.y + (i * TEXT_OFFSET.y), -1, "%s", prorogueText[i].c_str());
		}
		SetFontSize(beforeFontSize);

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
		exit(1);
		return true;
	}

	void TitleScene::debug_keyInput() {
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);

	}

}