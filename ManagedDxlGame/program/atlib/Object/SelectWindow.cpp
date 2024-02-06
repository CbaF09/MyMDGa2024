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
	}


	void SelectWindow::draw(float deltaTime) {
		auto resourceManager = ResourceManager::getResourceManager();
		
		{// �����`��
			// UI��`��
			auto questionTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowQuestionText.png");
			DrawRotaGraph(QUESTION_UI_POSITION.x, QUESTION_UI_POSITION.y, QUESTION_UI_SIZE, 0, questionTextUI, true);

			// ���╶��`��
			int beforeFontSize = GetFontSize();
			SetFontSize(QUESTION_STRING_FONTSIZE);
			
			// ������������ׂ̌v�Z
			int textWidth = GetDrawStringWidth(questionText_.c_str(), strlen(questionText_.c_str()));
			int drawPosX = QUESTION_UI_POSITION.x - textWidth / 2;
			int drawPosY = QUESTION_UI_POSITION.y - QUESTION_STRING_FONTSIZE / 2;

			// �������ŕ`��
			DrawStringEx(drawPosX, drawPosY, GetColor(0,0,0), questionText_.c_str());
			SetFontSize(beforeFontSize);
		}

		{// �͂��A��������`��
			// UI ��`��
			auto yesNoTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowYesNo.png");
			DrawRotaGraph(YES_UI_POSITION.x, YES_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);
			DrawRotaGraph(NO_UI_POSITION.x, NO_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);

			// YES�ENO��`��
			int beforeFontSize = GetFontSize();
			SetFontSize(YES_NO_STRING_FONTSIZE);

			switch (currentSelectedChoice_) {
			case e_SelectChoice::YES:
				DrawStringEx(YES_STRING_POSITION.x, YES_STRING_POSITION.y, GetColor(255, 0, 0), "�͂�");
				DrawStringEx(NO_STRING_POSITION.x, NO_STRING_POSITION.y, GetColor(200, 200, 200), "������");
				break;
			case e_SelectChoice::NO:
				DrawStringEx(YES_STRING_POSITION.x, YES_STRING_POSITION.y, GetColor(200, 200, 200), "�͂�");
				DrawStringEx(NO_STRING_POSITION.x, NO_STRING_POSITION.y, GetColor(255, 0, 0), "������");
				break;
			}
			SetFontSize(beforeFontSize);
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