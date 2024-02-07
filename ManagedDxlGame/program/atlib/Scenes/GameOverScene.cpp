#include "GameOverScene.h"
#include "TitleScene.h"
#include "../Singletons/SceneManager.h"
#include "../Singletons/FadeInOutManager.h"

void atl::GameOverScene::sceneUpdate(float deltaTime) {
	seq_.update(deltaTime);
}

bool atl::GameOverScene::seqInit(float deltaTime) {
	// 背景を真っ黒にする
	FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(255);

	seq_.change(&GameOverScene::seqDraw);
	return false;
}

bool atl::GameOverScene::seqDraw(float deltaTime) {
	// 真っ黒背景を描画
	FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);

	// 文字列を描画
	DrawStringToHandleEx(STRING_POSITION.x, STRING_POSITION.y, -1, GAMEOVER_FONT,
		"力尽きてしまった……\n( エンターキーかスペースキーを押すとタイトルに戻る )");

	// エンターかスペースが押されたら、タイトルシーンに遷移
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN, eKeys::KB_SPACE)) {
		SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
	}


	return false;
}
