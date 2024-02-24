#include "GameClearScene.h"
#include "TitleScene.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/SceneManager.h"

namespace atl {
	GameClearScene::GameClearScene() {
		// �e�N�X�`���ɂ�郁�������[�N�΍�
		dxe::Texture::DestroyUnReferenceTextures();

		// �G�s���[�O�̕������CSV����ǂݍ���
		auto csv = tnl::LoadCsv("csv/EpirogueStringCSV.csv");
		for (int i = 1; i < csv.size(); ++i) {
			auto& string = csv[i][1].getString();
			epilogueString_.emplace_back(string);
		}
	}

	GameClearScene::~GameClearScene() {
		{// ���\�[�X���
			// ������郊�\�[�X�̃t�@�C���p�X�̈ꎞ�I�z����쐬
			std::vector<std::string> tempDeleteRes = {
				"graphics/BackgroundIllust/ClearSceneBack.jpg",
				"sound/BGM/GameClearBGM.ogg",
				"sound/SE/NextPaper.ogg",
			};

			// ���\�[�X�����
			for (const auto& res : tempDeleteRes) {
				ResourceManager::getResourceManager()->deleteResource(res);
			}

			// �t�H���g�f�[�^���
			DeleteFontToHandle(EPILOGUE_FONT);
		}
	}

	void GameClearScene::sceneUpdate(float deltaTime) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) seq_.change(&GameClearScene::seqToTitleScene);

		drawBackground(deltaTime);
		seq_.update(deltaTime);
	}

	void GameClearScene::drawBackground(float deltaTime) {

		DrawExtendGraph(0,0,DXE_WINDOW_WIDTH,DXE_WINDOW_HEIGHT,ResourceManager::getResourceManager()->getGraphRes("graphics/BackgroundIllust/ClearSceneBack.jpg"),true);
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);

	}

	bool GameClearScene::seqInit(float deltaTime) {
		auto fadeInOutManager = FadeInOutManager::getFadeInOutManager();
		if (seq_.isStart()) {
			// �ŏ��͐^��������t�F�[�h�C�����Ă���
			fadeInOutManager->setFadeAlphaValue(255);
			fadeInOutManager->startFadeIn();
		}

		// ������x�̈Â��ɂȂ�����X�g�b�v���A�V�[�P���X�J��
		if (fadeInOutManager->getFadeAlphaValue() < EPILOGUE_BACKGROUND_BRIGHT) {
			fadeInOutManager->setFadeAlphaValue(EPILOGUE_BACKGROUND_BRIGHT);
			fadeInOutManager->stopFade();
			ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/BGM/GameClearBGM.ogg", 125);
			ResourceManager::getResourceManager()->playSoundRes("sound/BGM/GameClearBGM.ogg",DX_PLAYTYPE_LOOP);
			seq_.change(&GameClearScene::seqEpilogue);
		}

		return true;
	}

	bool GameClearScene::seqEpilogue(float deltaTime) {
		// ��ɕ\�����镶��
		DrawStringToHandleEx(static_cast<float>(CAN_BACK_TEXT_POSITION.x), static_cast<float>(CAN_BACK_TEXT_POSITION.y), GetColor(255, 255, 255), EPILOGUE_FONT, "�X�y�[�X�L�[ �� �^�C�g����ʂɖ߂�܂�\nW,S�L�[�ŏ㉺�ɃX�N���[�����܂�");

		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
			--currentTopLogIndex_;
			// �C���f�b�N�X���͈͉��������������͈͓��ɃN�����v
			if (currentTopLogIndex_ < 0) {
				currentTopLogIndex_ = 0;
			}
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
			++currentTopLogIndex_;
			// �C���f�b�N�X���͈͏���𒴂�����͈͓��ɃN�����v
			if (currentTopLogIndex_ > epilogueString_.size() - 1) {
				currentTopLogIndex_ = static_cast<int32_t>(epilogueString_.size() - 1);
			}
		}

		// ���݂̃g�b�v�s������A�Ō�܂ł�`��
		for (int i = currentTopLogIndex_; i < epilogueString_.size(); ++i) {
			DrawStringToHandleEx(static_cast<float>(TEXT_POSITION.x), static_cast<float>(TEXT_POSITION.y + ((i - currentTopLogIndex_) * TEXT_OFFSET.y)), -1, EPILOGUE_FONT,epilogueString_[i].c_str());
		}

		return true;
	}

	bool GameClearScene::seqToTitleScene(float deltaTime) {
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			ResourceManager::getResourceManager()->stopSoundRes("sound/BGM/GameClearBGM.ogg");
			SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
		}
		return true;
	}
}