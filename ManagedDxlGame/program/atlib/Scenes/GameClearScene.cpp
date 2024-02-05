#include "GameClearScene.h"
#include "TitleScene.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/SceneManager.h"

namespace atl {

	GameClearScene::~GameClearScene() {
		{// ���\�[�X���

			// ������郊�\�[�X�̃t�@�C���p�X�̈ꎞ�I�z����쐬
			std::vector<std::string> tempDeleteRes = {
				"graphics/BackgroundIllust/ClearSceneBack.jpg",
			};

			// �t�@�C���p�X���������B����̐������s���f�o�b�O���O�ɏo��
			for (const auto& res : tempDeleteRes) {
				bool isDelete = ResourceManager::getResourceManager()->deleteResource(res);
				if (!isDelete) { tnl::DebugTrace("\n------------------------------\nDungeonScene::�f�X�g���N�^ ��������� => ����ɉ������Ă��܂���"); }
				else { tnl::DebugTrace("\n------------------------------\nDungeonScene::�f�X�g���N�^ ��������� => ����"); }
			}
			tnl::DebugTrace("\n------------------------------\n"); // ���O�����Â炢�̂ōŌ�ɉ��s�Ɛ؂����������
		}
	}

	void GameClearScene::sceneUpdate(float deltaTime) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);


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
			// �ŏ��͐^��������t�F�[�h�C�����Ă���
			fadeInOutManager->setFadeAlphaValue(255);
			fadeInOutManager->startFadeIn();
		}

		// ������x�̈Â��ɂȂ�����X�g�b�v���A�V�[�P���X�J��
		if (fadeInOutManager->getFadeAlphaValue() < EPILOGUE_BACKGROUND_BRIGHT) {
			fadeInOutManager->setFadeAlphaValue(EPILOGUE_BACKGROUND_BRIGHT);
			fadeInOutManager->stopFade();
			seq_.change(&GameClearScene::seqEpilogue1);
		}

		return true;
	}

	bool GameClearScene::seqEpilogue1(float deltaTime) {
		// �ŏ��� 1 �t���[��
		if (seq_.isStart()) {
			drawLogLine_ = 0;
			textAlpha_ = 255;
			// 3�b���ƂɁA�`�悷��s�𑝂₷
			seq_.invokeRepeating(&GameClearScene::invokeDrawLogLineIncrement, 0, LOG_LINE_INTERVAL);
		}
		
		// �T�C�Y�𒴂��� drawLogLine �������Ȃ��悤��
		if (drawLogLine_ >= epiloguePage[static_cast<int>(currentPage_)].size()) {
			seq_.cancelInvoke(&GameClearScene::invokeDrawLogLineIncrement);
		}

		// �t�H���g�T�C�Y�Ɠ����x��ݒ肵�Ȃ���e�L�X�g��`��
		int beforeFontSize = GetFontSize();
		SetFontSize(EPILOGUE_STRING_FONTSIZE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, textAlpha_);
		// �\�����ׂ��s��S�ĕ\������
		for (int i = 0; i < drawLogLine_; ++i) {
			DrawStringEx(TEXT_POSITION.x + (i * TEXT_OFFSET.x), TEXT_POSITION.y + (i * TEXT_OFFSET.y), -1, "%s", epiloguePage[static_cast<int>(currentPage_)][i].c_str());
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		SetFontSize(beforeFontSize);

		// �S�s�\�����ꂽ��A�������ɓ���
		if (drawLogLine_ == epiloguePage[static_cast<int>(currentPage_)].size()) {
			// �ҋ@
			SEQ_CO_YIELD_RETURN_TIME(FULL_ALPHA_TIME, deltaTime, [&] {});

			// alpha ���}�C�i�X����
			SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {
				textAlpha_ -= ALPHA_MINUS_SPEED;
				// alpha �� 0 �ɂȂ�����R���[�`���j��
				if (textAlpha_ < 0) {
					textAlpha_ = 0;
					// �C���f�b�N�X�� 1 �i�߂�
					currentPage_ = static_cast<e_EpiloguePage>(static_cast<int>(currentPage_) + 1);
					
					// �y�[�W���c���Ă�����V�[�P���X�Ď��s�A�c���Ă��Ȃ�������V�[�P���X�J��
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
		++drawLogLine_; // �\���s���₷
	}

	bool GameClearScene::seqToTitleScene(float deltaTime) {
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			// �ҋ@
			SEQ_CO_YIELD_RETURN_TIME(FULL_ALPHA_TIME, deltaTime, [&] {});

			SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
		}

		SEQ_CO_END;

	}

}