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
		// �w�i��^�����ɂ���
		FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(255);

		ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/SE/GameOver.ogg", 160);
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/GameOver.ogg",DX_PLAYTYPE_BACK);

		// �e�N�X�`���ɂ�郁�������[�N�΍�
		dxe::Texture::DestroyUnReferenceTextures();

		seq_.change(&GameOverScene::seqUpdata);
		return false;
	}

	bool GameOverScene::seqUpdata(float deltaTime) {
		// �^�����w�i��`��
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);

		// �������`��
		DrawStringToHandleEx(STRING_POSITION.x, STRING_POSITION.y, -1, GAMEOVER_FONT,
			"������ǂ��o����Ă��܂����c�c\n( �G���^�[�L�[���X�y�[�X�L�[�������ƃ^�C�g���ɖ߂� )");

		// �G���^�[���X�y�[�X�������ꂽ��A�^�C�g���V�[���ɑJ��
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN, eKeys::KB_SPACE)) {
			ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/SE/GameOverContinue.ogg", 150);
			ResourceManager::getResourceManager()->playSoundRes("sound/SE/GameOverContinue.ogg", DX_PLAYTYPE_NORMAL);
			SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
		}
		return false;
	}

}