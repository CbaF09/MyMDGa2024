#include "TitleScene.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/SceneManager.h"
#include "DungeonScene.h"

namespace atl {

	TitleScene::~TitleScene() {
		{// �^�C�g���V�[���Ŏg���Ă��郊�\�[�X�����
			
			// ������郊�\�[�X�̃t�@�C���p�X�̈ꎞ�I�z����쐬
			std::vector<std::string> tempDeleteRes = {
				"graphics/BackGroundIllust/TitleSceneBack.jpg"
				"graphics/BackGroundIllust/TitleLogo.png"
			};
			
			// �t�@�C���p�X���������B����̐������s���f�o�b�O���O�ɏo��
			for (const auto& res : tempDeleteRes) {
				bool isDelete = ResourceManager::getResourceManager()->deleteResource(res);
				if(!isDelete) { tnl::DebugTrace("\n------------------------------\nTitleScene::�f�X�g���N�^ ��������� => ����ɉ������Ă��܂���"); }
				else { tnl::DebugTrace("\n------------------------------\nTitleScene::�f�X�g���N�^ ��������� => ����"); }
			}
			tnl::DebugTrace("\n------------------------------\n"); // ���O�����Â炢�̂ōŌ�ɉ��s�Ɛ؂����������
		}
	}
	void TitleScene::sceneUpdate(float deltaTime) {
		draw(deltaTime);
		seq_.update(deltaTime);
	}

	void TitleScene::draw(float deltaTime) {

		// �w�i�`��
		drawBackground(deltaTime);
		
		if (isDisplayButton) {
			drawButton(deltaTime);
		}
		// �t�F�[�h�C���A�E�g�p�`��
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);
	}

	// �w�i�ƃ��S��`��
	void TitleScene::drawBackground(float deltaTime){
		auto resourceManager = ResourceManager::getResourceManager();
		// �w�i�`��
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, resourceManager->getGraphRes("graphics/BackGroundIllust/TitleSceneBack.jpg"), true);
		// ���S�`��
		DrawRotaGraph(LOGO_POSITION.x, LOGO_POSITION.y, LOGO_SIZE, tnl::ToRadian(LOGO_DEFAULT_ANGLE), resourceManager->getGraphRes("graphics/BackGroundIllust/TitleLogo.png"), true);
	}

	void TitleScene::drawButton(float deltaTime) {
		auto resourceManager = ResourceManager::getResourceManager();

		// ���̃t�H���g�T�C�Y��ۑ�
		int beforeFontSize = GetFontSize();
		SetFontSize(30);

		// �{�^�� enum �̐������A�{�^����`�悷��
		for (int i = 0; i < static_cast<int>(e_SelectTitleButton::BUTTON_MAX); ++i) {
			
			// ���ۂ̃{�^���̈ʒu
			tnl::Vector2i actualButtonPos = { BUTTON_POSITION.x + (i * BUTTON_OFFSET.x), BUTTON_POSITION.y + (i * BUTTON_OFFSET.y) };

			// ���ݑI�𒆂̃{�^���̏ꍇ�́A���ʂȏ�����������
			if (static_cast<e_SelectTitleButton>(i) == currentSelectButton_) {
				// �F��ς���
				SetDrawBright(200, 200, 255);
				// �ʒu��␳
				actualButtonPos += BUTTON_SELECTED_OFFSET;
			}

			DrawRotaGraph(actualButtonPos.x, actualButtonPos.y, BUTTON_SIZE, 0, resourceManager->getGraphRes("graphics/UI/TitleSceneButtonUI.png"), true);

			std::string buttonName;

			switch (static_cast<e_SelectTitleButton>(i)) {
			case e_SelectTitleButton::START: buttonName = "�Q�[���J�n";
				break;
			case e_SelectTitleButton::PROROGUE: buttonName = "�v�����[�O";
				break;
			case e_SelectTitleButton::EXIT: buttonName = "�@�I��"; // �󔒂ŕ`��ʒu����
				break;
			}

			DrawStringEx(actualButtonPos.x + BUTTON_STRING_OFFSET.x, actualButtonPos.y + BUTTON_STRING_OFFSET.y, -1, buttonName.c_str());

			if (static_cast<e_SelectTitleButton>(i) == currentSelectButton_) {
				// ���ɖ߂�
				SetDrawBright(255, 255, 255);
			}
		}
		
		// �t�H���g�T�C�Y�����ɖ߂�
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
		// �t�F�[�h���͑���s�\ ( �������^�[�� )
		if (FadeInOutManager::getFadeInOutManager()->isFading()) return true;

		// �X�y�[�X,�G���^�[�L�[�����������A���ݑI�𒆂̃{�^���ɉ����ăV�[�P���X��J��
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN, eKeys::KB_SPACE)) {

			// �I�𒆂̃{�^��
			switch (currentSelectButton_) {
				// �X�^�[�g�{�^��
			case e_SelectTitleButton::START:	seq_.change(&TitleScene::seqStart);
				break;

				// �v�����[�O�{�^��
			case e_SelectTitleButton::PROROGUE:	seq_.change(&TitleScene::seqProrogue);
				break;

				// �Q�[���I���{�^��
			case e_SelectTitleButton::EXIT:		seq_.change(&TitleScene::seqExit);
				break;

				// �f�t�H���g�Ȃ牽�����Ȃ�
			default: break;
			}
		}
		
		// ��A���[ or W �����������A�I�𒆂̃{�^�������̂��̂ɂ���
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP, eKeys::KB_W)) {
			selectButtonUp();
		}
		// ���A���[ or S �����������A�I�𒆂̃{�^��������̂��̂ɂ���
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN,eKeys::KB_S)) {
			selectButtonDown();
		}

		return true;
	}

	// �I�𒆂̃{�^�������̂��̂ɂ���
	bool TitleScene::selectButtonUp() {
		int currentButtonInt = static_cast<int>(currentSelectButton_);

		--currentButtonInt;

		// �ォ�炳��ɏ���͂�����A���ɖ߂�
		if (currentButtonInt < 0) {
			currentButtonInt = (static_cast<int>(e_SelectTitleButton::BUTTON_MAX) - 1);
		}

		// �X�V�����C���f�b�N�X�őI�𒆂̃{�^�����X�V
		currentSelectButton_ = static_cast<e_SelectTitleButton>(currentButtonInt);
		return true;
	}

	// �I�𒆂̃{�^��������̂��̂ɂ���
	bool TitleScene::selectButtonDown() {
		int currentButtonInt = static_cast<int>(currentSelectButton_);

		// �C���f�b�N�X���C���N�������g���āA����̃{�^����I��
		currentButtonInt++;
		// �C���f�b�N�X���͈͊O�ɂȂ����ꍇ�́A�ŏ㕔�̃{�^����I��
		if (currentButtonInt >= static_cast<int>(e_SelectTitleButton::BUTTON_MAX)) {
			currentButtonInt = 0;
		}

		// �X�V�����C���f�b�N�X�őI�𒆂̃{�^�����X�V
		currentSelectButton_ = static_cast<e_SelectTitleButton>(currentButtonInt);
		return true;
	}

	bool TitleScene::seqStart(float deltaTime) {
		// �ŏ��� 1 �t���[���ŁA�t�F�[�h�A�E�g���J�n
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		// �t�F�[�h���I�������A�_���W�����V�[���ɐ؂�ւ���
		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			SceneManager::getSceneManager()->changeScene(std::make_shared<DungeonScene>());
		}

		return true;
	}

	bool TitleScene::seqProrogue(float deltaTime) {
		if(seq_.isStart()) {
			// �w�i�������Â�����
			FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(PROROGUE_BACKGROUND_BRIGHT);

			// �\�����ׂ��s����������
			drawLogLine_ = PROROGUE_FIRST_LOG;
			// Timer�̗ݐώ��Ԃ�������
			totalDeltaTime_ = 0.0f;
			// �{�^��������
			isDisplayButton = false;

		}


		totalDeltaTime_ += deltaTime;
		// �\������s���c���Ă���ꍇ�̂�
		if (drawLogLine_ < prorogueText.size()) {
			// totalDeltaTime �� LOG_LINE_INTERVAL �𒴂�����A�\������s�𑝂₷
			if (totalDeltaTime_ >= LOG_LINE_INTERVAL) {
				totalDeltaTime_ = 0.0f;
				++drawLogLine_; // �\���s���₷
			}
		}

		int beforeFontSize = GetFontSize();
		SetFontSize(PROROGUE_STRING_FONTSIZE);
		// �\�����ׂ��s��S�ĕ\������
		for (int i = 0; i < drawLogLine_; ++i) {
			DrawStringEx(TEXT_POSITION.x + (i * TEXT_OFFSET.x), TEXT_POSITION.y + (i * TEXT_OFFSET.y), -1, "%s", prorogueText[i].c_str());
		}
		SetFontSize(beforeFontSize);

		// �G���^�[�L�[���X�y�[�X�L�[�ŃV�[�P���X�J��
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE,eKeys::KB_RETURN)) {
			// �{�^���ĕ\��
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