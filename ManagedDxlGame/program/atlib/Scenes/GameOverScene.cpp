#include "GameOverScene.h"
#include "TitleScene.h"
#include "../Singletons/SceneManager.h"
#include "../Singletons/FadeInOutManager.h"

void atl::GameOverScene::sceneUpdate(float deltaTime) {
	seq_.update(deltaTime);
}

bool atl::GameOverScene::seqInit(float deltaTime) {
	// �w�i��^�����ɂ���
	FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(255);

	seq_.change(&GameOverScene::seqDraw);
	return false;
}

bool atl::GameOverScene::seqDraw(float deltaTime) {
	// �^�����w�i��`��
	FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);

	// �t�H���g�T�C�Y�T�C�Y��ύX���ĕ������`��
	int beforeFontSize = GetFontSize();
	SetFontSize(FONT_SIZE);
	DrawStringEx(STRING_POSITION.x, STRING_POSITION.y, -1,
		"�͐s���Ă��܂����c�c\n( �G���^�[�L�[���X�y�[�X�L�[�������ƃ^�C�g���ɖ߂� )");
	// �t�H���g�T�C�Y��߂�	
	SetFontSize(beforeFontSize);

	// �G���^�[���X�y�[�X�������ꂽ��A�^�C�g���V�[���ɑJ��
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN, eKeys::KB_SPACE)) {
		SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
	}


	return false;
}
