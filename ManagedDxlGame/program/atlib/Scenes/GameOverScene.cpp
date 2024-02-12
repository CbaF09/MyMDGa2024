#include "GameOverScene.h"
#include "TitleScene.h"
#include "../Singletons/SceneManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	GameOverScene::~GameOverScene() {
		auto rManager = ResourceManager::getResourceManager();
		rManager->deleteResource("sound/SE/GameOver.ogg");
		rManager->deleteResource("sound/SE/GameOverContinue.ogg");
	}

	void GameOverScene::sceneUpdate(float deltaTime) {
		seq_.update(deltaTime);
	}

	bool GameOverScene::seqInit(float deltaTime) {
		// 背景を真っ黒にする
		FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(255);

		ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/SE/GameOver.ogg", 160);
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/GameOver.ogg",DX_PLAYTYPE_BACK);

		// テクスチャによるメモリリーク対策
		dxe::Texture::DestroyUnReferenceTextures();

		seq_.change(&GameOverScene::seqUpdata);
		return false;
	}

	bool GameOverScene::seqUpdata(float deltaTime) {
		// 真っ黒背景を描画
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);

		// 文字列を描画
		DrawStringToHandleEx(STRING_POSITION.x, STRING_POSITION.y, -1, GAMEOVER_FONT,
			"塔から追い出されてしまった……\n( エンターキーかスペースキーを押すとタイトルに戻る )");

		// エンターかスペースが押されたら、タイトルシーンに遷移
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN, eKeys::KB_SPACE)) {
			ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/SE/GameOverContinue.ogg", 150);
			ResourceManager::getResourceManager()->playSoundRes("sound/SE/GameOverContinue.ogg", DX_PLAYTYPE_NORMAL);
			SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
		}
		return false;
	}

}