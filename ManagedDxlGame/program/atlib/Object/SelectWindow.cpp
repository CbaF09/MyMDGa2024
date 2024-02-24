#include "SelectWindow.h"
#include "../Singletons/ResourceManager.h"

namespace atl {
	SelectWindow::SelectWindow() {
		ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/SE/DungeonSceneOpenSelectMenu.ogg", 160);
	}

	SelectWindow::~SelectWindow(){
		// ���\�[�X���
		std::vector<std::string> tempDeleteRes = {
		"graphics/UI/SelectWindowQuestionText.png",
		"graphics/UI/SelectWindowYesNo.png",
		"graphics/UI/KeyboardA.png",
		"graphics/UI/KeyboardD.png",
		};

		for (const auto& res : tempDeleteRes) {
			ResourceManager::getResourceManager()->deleteResource(res);
		}

		// �t�H���g�n���h�����
		DeleteFontToHandle(QUESTION_FONT);
		DeleteFontToHandle(YES_NO_FONT);
	}
	
	void SelectWindow::draw(float deltaTime) {
		auto resourceManager = ResourceManager::getResourceManager();
		
		{// �����`��
			// UI��`��
			auto questionTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowQuestionText.png");
			DrawRotaGraph(QUESTION_UI_POSITION.x, QUESTION_UI_POSITION.y, QUESTION_UI_SIZE, 0, questionTextUI, true);
			
			// ���������Ŏ��╶��`��
			int textWidth = static_cast<int>(GetDrawStringWidthToHandle(questionText_.c_str(), strlen(questionText_.c_str()),QUESTION_FONT));
			float drawPosX = static_cast<float>(QUESTION_UI_POSITION.x - textWidth / 2);
			float drawPosY = static_cast<float>(QUESTION_UI_POSITION.y - GetFontSizeToHandle(QUESTION_FONT) / 2);

			DrawStringToHandleEx(drawPosX, drawPosY, -1,QUESTION_FONT,questionText_.c_str());
		}

		{// �͂��A��������`��
			
			// UI ��`��
			auto yesNoTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowYesNo.png");
			DrawRotaGraph(YES_UI_POSITION.x, YES_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);
			DrawRotaGraph(NO_UI_POSITION.x, NO_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);
			// A,D�L�[��UI�`��
			auto keyboardA = resourceManager->getGraphRes("graphics/UI/KeyboardA.png");
			DrawRotaGraph(KEYBOARD_A_UI_POSITION.x, KEYBOARD_A_UI_POSITION.y - 100, KEYBOARD_UI_SIZE, 0, keyboardA, true);
			auto keyboardD = resourceManager->getGraphRes("graphics/UI/KeyboardD.png");
			DrawRotaGraph(KEYBOARD_D_UI_POSITION.x, KEYBOARD_D_UI_POSITION.y - 100, KEYBOARD_UI_SIZE, 0, keyboardD, true);


			// �͂��E�������̕�����`��
			switch (currentSelectedChoice_) {
			case e_SelectChoice::YES:
				DrawStringToHandleEx(static_cast<float>(YES_STRING_POSITION.x), static_cast<float>(YES_STRING_POSITION.y), GetColor(255, 0, 0),YES_NO_FONT, "�͂�");
				DrawStringToHandleEx(static_cast<float>(NO_STRING_POSITION.x), static_cast<float>(NO_STRING_POSITION.y), GetColor(200, 200, 200), YES_NO_FONT, "������");
				break;
			case e_SelectChoice::NO:
				DrawStringToHandleEx(static_cast<float>(YES_STRING_POSITION.x), static_cast<float>(YES_STRING_POSITION.y), GetColor(200, 200, 200), YES_NO_FONT, "�͂�");
				DrawStringToHandleEx(static_cast<float>(NO_STRING_POSITION.x), static_cast<float>(NO_STRING_POSITION.y), GetColor(255, 0, 0), YES_NO_FONT, "������");
				break;
			}
		}

		// A,D�őI������ς���
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_A)) {
			currentSelectedChoice_ = e_SelectChoice::YES;
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_D)) {
			currentSelectedChoice_ = e_SelectChoice::NO;
		}
	}

	void SelectWindow::setSelectWindowQuestionText(const std::string& questionText) {
		questionText_ = questionText;
	}

	// ���N���b�N�����������AcurrentSelectedChoice == YES �Ȃ� YES ��Ԃ��BNO �Ȃ� NO ��Ԃ��B
	// �E�N���b�N�����������́ANO��I�񂾔���
	const SelectWindow::e_SelectChoice SelectWindow::windowChoice() const {
		
		// ���N���b�N����������
		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
			return currentSelectedChoice_;
		}
		// �E�N���b�N�����������A��������I�񂾔���
		else if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			return e_SelectChoice::NO;
		}

		return e_SelectChoice::NONE;
	}
}