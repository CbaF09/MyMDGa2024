#include "SelectWindow.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	SelectWindow::SelectWindow(std::string questionText) : questionText_(questionText) {

	}

	SelectWindow::~SelectWindow(){
		auto resourceManager = ResourceManager::getResourceManager();

		std::vector<std::string> tempDeleteRes = {
		"graphics/UI/SelectWindowQuestionText.png",
		"graphics/UI/SelectWindowYesNo.png"
		};

		// �t�@�C���p�X���������B����̐������s���f�o�b�O���O�ɏo��
		for (const auto& res : tempDeleteRes) {
			bool isDelete = ResourceManager::getResourceManager()->deleteResource(res);
			if (!isDelete) { tnl::DebugTrace("\n------------------------------\nSelectWindow::�f�X�g���N�^ ��������� => ����ɉ������Ă��܂���"); }
			else { tnl::DebugTrace("\n------------------------------\nSelectWindow::�f�X�g���N�^ ��������� => ����"); }
		}
		tnl::DebugTrace("\n------------------------------\n"); // ���O�����Â炢�̂ōŌ�ɉ��s�Ɛ؂����������

		DeleteFontToHandle(QUESTION_FONT);
		DeleteFontToHandle(YES_NO_FONT);
	}


	void SelectWindow::draw(float deltaTime) {
		auto resourceManager = ResourceManager::getResourceManager();
		
		{// �����`��
			// UI��`��
			auto questionTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowQuestionText.png");
			DrawRotaGraph(QUESTION_UI_POSITION.x, QUESTION_UI_POSITION.y, QUESTION_UI_SIZE, 0, questionTextUI, true);

			
			// ������������ׂ̌v�Z
			int textWidth = GetDrawStringWidth(questionText_.c_str(), strlen(questionText_.c_str()));
			int drawPosX = QUESTION_UI_POSITION.x - textWidth / 2;
			int drawPosY = QUESTION_UI_POSITION.y - GetFontSizeToHandle(QUESTION_FONT) / 2;

			// �������ŕ`��
			DrawStringToHandleEx(drawPosX, drawPosY, GetColor(0,0,0),QUESTION_FONT,questionText_.c_str());
		}

		{// �͂��A��������`��
			// UI ��`��
			auto yesNoTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowYesNo.png");
			DrawRotaGraph(YES_UI_POSITION.x, YES_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);
			DrawRotaGraph(NO_UI_POSITION.x, NO_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);

			// YES�ENO��`��
			switch (currentSelectedChoice_) {
			case e_SelectChoice::YES:
				DrawStringToHandleEx(YES_STRING_POSITION.x, YES_STRING_POSITION.y, GetColor(255, 0, 0),YES_NO_FONT, "�͂�");
				DrawStringToHandleEx(NO_STRING_POSITION.x, NO_STRING_POSITION.y, GetColor(200, 200, 200), YES_NO_FONT, "������");
				break;
			case e_SelectChoice::NO:
				DrawStringToHandleEx(YES_STRING_POSITION.x, YES_STRING_POSITION.y, GetColor(200, 200, 200), YES_NO_FONT, "�͂�");
				DrawStringToHandleEx(NO_STRING_POSITION.x, NO_STRING_POSITION.y, GetColor(255, 0, 0), YES_NO_FONT, "������");
				break;
			}
		}

		// A,D�őI������ς���
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_A)) {
			currentSelectedChoice_ = e_SelectChoice::YES;
		}
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_D)) {
			currentSelectedChoice_ = e_SelectChoice::NO;
		}
	}

	// �G���^�[ or �X�y�[�X�����������AcurrentSelectedChoice == YES �Ȃ� true ��Ԃ��BNO �Ȃ� false ��Ԃ��B
	const SelectWindow::e_SelectChoice& SelectWindow::windowChoice() {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN, eKeys::KB_SPACE)) {
			return currentSelectedChoice_;
		}
		return e_SelectChoice::NONE;
	}
}