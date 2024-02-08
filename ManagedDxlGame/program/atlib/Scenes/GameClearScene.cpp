#include "GameClearScene.h"
#include "TitleScene.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/SceneManager.h"

namespace atl {

	GameClearScene::~GameClearScene() {
		{// リソース解放

			// 解放するリソースのファイルパスの一時的配列を作成
			std::vector<std::string> tempDeleteRes = {
				"graphics/BackgroundIllust/ClearSceneBack.jpg",
				"sound/BGM/GameClearBGM.ogg",
				"sound/SE/NextPaper.ogg",
			};

			// リソースを解放
			for (const auto& res : tempDeleteRes) {
				ResourceManager::getResourceManager()->deleteResource(res);
			}

			// フォントデータ解放
			DeleteFontToHandle(EPILOGUE_FONT);
		}
	}

	void GameClearScene::sceneUpdate(float deltaTime) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) seq_.change(&GameClearScene::seqToTitleScene);


		draw(deltaTime);
		seq_.update(deltaTime);
	}

	void GameClearScene::draw(float deltaTime) {
		DrawExtendGraph(0,0,DXE_WINDOW_WIDTH,DXE_WINDOW_HEIGHT,ResourceManager::getResourceManager()->getGraphRes("graphics/BackgroundIllust/ClearSceneBack.jpg"),true);
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);
	}

	bool GameClearScene::seqInit(float deltaTime) {
		auto fadeInOutManager = FadeInOutManager::getFadeInOutManager();
		if (seq_.isStart()) {
			// 最初は真っ黒からフェードインしていく
			fadeInOutManager->setFadeAlphaValue(255);
			fadeInOutManager->startFadeIn();
		}

		// ある程度の暗さになったらストップし、シーケンス遷移
		if (fadeInOutManager->getFadeAlphaValue() < EPILOGUE_BACKGROUND_BRIGHT) {
			fadeInOutManager->setFadeAlphaValue(EPILOGUE_BACKGROUND_BRIGHT);
			fadeInOutManager->stopFade();
			ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/BGM/GameClearBGM.ogg", 125);
			ResourceManager::getResourceManager()->playSoundRes("sound/BGM/GameClearBGM.ogg",DX_PLAYTYPE_LOOP);
			seq_.change(&GameClearScene::seqEpilogue1);
		}

		return true;
	}

	bool GameClearScene::seqEpilogue1(float deltaTime) {
		// 最初の 1 フレーム
		if (seq_.isStart()) {
			drawLogLine_ = 0;
			textAlpha_ = 255;
			// 3秒ごとに、描画する行を増やす
			seq_.invokeRepeating(&GameClearScene::invokeDrawLogLineIncrement, 0, LOG_LINE_INTERVAL);
		}
		
		// サイズを超えて drawLogLine が増えないように
		if (drawLogLine_ >= epiloguePage[static_cast<int>(currentPage_)].size()) {
			seq_.cancelInvoke(&GameClearScene::invokeDrawLogLineIncrement);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, textAlpha_);
		// 表示すべき行を全て表示する
		for (int i = 0; i < drawLogLine_; ++i) {
			DrawStringToHandleEx(static_cast<float>(TEXT_POSITION.x + (i * TEXT_OFFSET.x)), static_cast<float>(TEXT_POSITION.y + (i * TEXT_OFFSET.y)), -1,EPILOGUE_FONT, "%s", epiloguePage[static_cast<int>(currentPage_)][i].c_str());
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

		// 全行表示されたら、透明化に入る
		if (drawLogLine_ == epiloguePage[static_cast<int>(currentPage_)].size()) {
			// 待機
			SEQ_CO_YIELD_RETURN_TIME(FULL_ALPHA_TIME, deltaTime, [&] {});

			// alpha をマイナスする
			SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {
				textAlpha_ -= ALPHA_MINUS_SPEED;
				// alpha が 0 になったらコルーチン破棄
				if (textAlpha_ < 0) {
					textAlpha_ = 0;
					// インデックスを 1 進める
					currentPage_ = static_cast<e_EpiloguePage>(static_cast<int>(currentPage_) + 1);
					
					// ページが残っていたらシーケンス再実行、残っていなかったらシーケンス遷移
					if (currentPage_ != e_EpiloguePage::PAGE_MAX) {
						seq_.change(&GameClearScene::seqEpilogue1);
					}
					else {
						seq_.change(&GameClearScene::seqToTitleScene);
					}
					SEQ_CO_BREAK;
				}
				});
		}



		SEQ_CO_END;
	}

	void GameClearScene::invokeDrawLogLineIncrement() {
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/NextPaper.ogg", DX_PLAYTYPE_BACK);
		++drawLogLine_; // 表示行増やす
	}

	bool GameClearScene::seqToTitleScene(float deltaTime) {
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			// 待機
			SEQ_CO_YIELD_RETURN_TIME(FULL_ALPHA_TIME, deltaTime, [&] {});

			SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
		}

		SEQ_CO_END;

	}

}