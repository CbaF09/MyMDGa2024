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
				"graphics/BackGroundIllust/TitleSceneBack.png"
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
		seq_.update(deltaTime);
		draw(deltaTime);
	}

	void TitleScene::draw(float deltaTime) {
		DrawExtendGraph(0,0,DXE_WINDOW_WIDTH,DXE_WINDOW_HEIGHT,ResourceManager::getResourceManager()->getIllustRes("graphics/BackGroundIllust/TitleSceneBack.png"),true);
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);
	}

	bool TitleScene::seqInit(float deltaTime) {

		auto fadeManager = FadeInOutManager::getFadeInOutManager();
		fadeManager->setFadeAlphaValue(255);
		fadeManager->startFadeIn();
		
		seq_.change(&TitleScene::seqKeyInputWait);

		return false;
	}

	bool TitleScene::seqKeyInputWait(float deltaTime) {
		debug_keyInput();

		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			seq_.change(&TitleScene::seqToDungeonScene);
		}

		return true;
	}

	bool TitleScene::seqToDungeonScene(float deltaTime) {
		// 最初の 1 フレームで、フェードアウトを開始
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			SceneManager::getSceneManager()->changeScene(std::make_shared<DungeonScene>());
		}

		return true;
	}

	void TitleScene::debug_keyInput() {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);
	}

}