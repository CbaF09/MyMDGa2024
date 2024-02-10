#include "TitleScene.h"
#include "DungeonScene.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/SceneManager.h"

namespace atl {

	TitleScene::~TitleScene() {
		{// �^�C�g���V�[���Ŏg���Ă��郊�\�[�X�����
			
			// ������郊�\�[�X�̃t�@�C���p�X�̈ꎞ�I�z����쐬
			std::vector<std::string> tempDeleteRes = {
				"graphics/BackGroundIllust/TitleSceneBack.jpg",
				"graphics/BackGroundIllust/TitleLogo.png",
				"sound/BGM/TitleSceneBGM.ogg",
				"sound/SE/TitleSceneCursorChange.ogg",
				"sound/SE/TitleSceneStart.ogg",
				"sound/SE/NextPaper.ogg",
				"graphics/UI/KeyboardW.png",
				"graphics/UI/TitleSceneCursor.png",
			};

			// ���\�[�X���
			for (const auto& res : tempDeleteRes) {
				ResourceManager::getResourceManager()->deleteResource(res);
			}

			// �t�H���g�f�[�^���
			DeleteFontToHandle(PROROGUE_FONT);
			DeleteFontToHandle(BUTTON_FONT);
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
			drawSpaceIsEnter();
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

	void TitleScene::drawSpaceIsEnter() {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 155);
		DrawBox(SPACE_IS_ENTER_BACK_LEFTUP_POINT.x, SPACE_IS_ENTER_BACK_LEFTUP_POINT.y, SPACE_IS_ENTER_BACK_RIGHTDOWN_POINT.x, SPACE_IS_ENTER_BACK_RIGHTDOWN_POINT.y, GetColor(100, 100, 100), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �{�^���p�̃t�H���g�𗬗p
		DrawStringToHandleEx(static_cast<float>(SPACE_IS_ENTER_POSITION.x), static_cast<float>(SPACE_IS_ENTER_POSITION.y), -1, BUTTON_FONT, "�X�y�[�X�L�[ �� ����");
	}

	void TitleScene::drawButton(float deltaTime) {
		auto resourceManager = ResourceManager::getResourceManager();

		// ���ۂ̃{�^���̈ʒu
		tnl::Vector2i actualButtonPos{};

		// �{�^�� enum �̐������A�{�^����`�悷��
		for (int i = 0; i < static_cast<int>(e_SelectTitleButton::BUTTON_MAX); ++i) {
			
			actualButtonPos = { BUTTON_POSITION.x + (i * BUTTON_OFFSET.x), BUTTON_POSITION.y + (i * BUTTON_OFFSET.y) };

			// ���ݑI�𒆂̃{�^���̏ꍇ�́A���ʂȏ�����������
			if (static_cast<e_SelectTitleButton>(i) == currentSelectButton_) {
				// �J�[�\����\������
				DrawRotaGraph(actualButtonPos.x + BUTTON_CURSOR_OFFSET.x, actualButtonPos.y + BUTTON_CURSOR_OFFSET.y, BUTTON_CURSOR_SIZE, tnl::ToRadian(-90), ResourceManager::getResourceManager()->getGraphRes("graphics/UI/TitleSceneCursor.png"), true);
				DrawRotaGraph(actualButtonPos.x + BUTTON_CURSOR_OFFSET.x, actualButtonPos.y + BUTTON_CURSOR_OFFSET.y + CURSOR_KEYBOARD_WS_OFFSET.y, KEYBOARD_WS_SIZE, 0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/KeyboardS.png"), true);
				DrawRotaGraph(actualButtonPos.x + BUTTON_CURSOR_OFFSET.x, actualButtonPos.y + BUTTON_CURSOR_OFFSET.y - CURSOR_KEYBOARD_WS_OFFSET.y, KEYBOARD_WS_SIZE, 0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/KeyboardW.png"), true);
				// �F��ς���
				SetDrawBright(200, 200, 255);
				// �ʒu��␳
				actualButtonPos += BUTTON_SELECTED_OFFSET;
			}

			// �{�^��UI��`��
			DrawRotaGraph(actualButtonPos.x, actualButtonPos.y, BUTTON_SIZE, 0, resourceManager->getGraphRes("graphics/UI/TitleSceneButtonUI.png"), true);

			std::string buttonName;

			switch (static_cast<e_SelectTitleButton>(i)) {
			case e_SelectTitleButton::START:
				buttonName = "�Q�[���J�n";
				break;
			case e_SelectTitleButton::PROROGUE: 
				buttonName = "�v�����[�O";
				break;
			case e_SelectTitleButton::EXIT: 
				buttonName = "�@�I��"; // �󔒂ŕ`��ʒu����
				break;
			}

			DrawStringToHandleEx(static_cast<float>(actualButtonPos.x + BUTTON_STRING_OFFSET.x), static_cast<float>(actualButtonPos.y + BUTTON_STRING_OFFSET.y), -1, BUTTON_FONT, buttonName.c_str());

			// �F�����ɖ߂�
			SetDrawBright(255, 255, 255);
		}


	}

	bool TitleScene::seqInit(float deltaTime) {
		auto fadeManager = FadeInOutManager::getFadeInOutManager();
		fadeManager->setFadeAlphaValue(255);
		fadeManager->startFadeIn();

		volumeSetting();
		
		ResourceManager::getResourceManager()->playSoundRes("sound/BGM/TitleSceneBGM.ogg",DX_PLAYTYPE_LOOP);
		
		// �e�N�X�`���ɂ�郁�������[�N�΍�
		dxe::Texture::DestroyUnReferenceTextures();

		seq_.change(&TitleScene::seqKeyInputWait);
		isDisplayButton = true;

		return false;
	}

	void TitleScene::volumeSetting() {
		auto rManager = ResourceManager::getResourceManager();
		// BGM
		rManager->changeVolumeSoundRes("sound/BGM/TitleSceneBGM.ogg", 130);

		// SE
		rManager->changeVolumeSoundRes("sound/SE/TitleSceneCursorChange.ogg", 120);
		rManager->changeVolumeSoundRes("sound/SE/TitleSceneStart.ogg", 150);
		rManager->changeVolumeSoundRes("sound/SE/NextPaper.ogg", 150);
	}

	bool TitleScene::seqKeyInputWait(float deltaTime) {
		// �t�F�[�h���͑���s�\ ( �������^�[�� )
		if (FadeInOutManager::getFadeInOutManager()->isFading()) return true;

		// �X�y�[�X,�G���^�[�L�[,���N���b�N�����������A���ݑI�𒆂̃{�^���ɉ����ăV�[�P���X��J��
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

		ResourceManager::getResourceManager()->playSoundRes("sound/SE/TitleSceneCursorChange.ogg",DX_PLAYTYPE_BACK);
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
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/TitleSceneCursorChange.ogg", DX_PLAYTYPE_BACK);
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
			ResourceManager::getResourceManager()->playSoundRes("sound/SE/TitleSceneStart.ogg", DX_PLAYTYPE_BACK);
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
			ResourceManager::getResourceManager()->playSoundRes("sound/SE/NextPaper.ogg", DX_PLAYTYPE_BACK);
		}


		totalDeltaTime_ += deltaTime;
		// �\������s���c���Ă���ꍇ�̂�
		if (drawLogLine_ < prorogueText.size()) {
			// totalDeltaTime �� LOG_LINE_INTERVAL �𒴂�����A�\������s�𑝂₷
			if (totalDeltaTime_ >= LOG_LINE_INTERVAL) {
				totalDeltaTime_ = 0.0f;
				++drawLogLine_; // �\���s���₷
				ResourceManager::getResourceManager()->playSoundRes("sound/SE/NextPaper.ogg", DX_PLAYTYPE_BACK);
			}
		}

		// �\�����ׂ��s��S�ĕ\������
		for (int i = 0; i < drawLogLine_; ++i) {
			DrawStringToHandleEx(static_cast<float>(TEXT_POSITION.x + (i * TEXT_OFFSET.x)), static_cast<float>(TEXT_POSITION.y + (i * TEXT_OFFSET.y)), -1,PROROGUE_FONT, "%s", prorogueText[i].c_str());
		}

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