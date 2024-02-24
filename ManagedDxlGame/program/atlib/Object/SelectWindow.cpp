#include "SelectWindow.h"
#include "../Singletons/ResourceManager.h"

namespace atl {
	SelectWindow::SelectWindow() {
		ResourceManager::getResourceManager()->changeVolumeSoundRes("sound/SE/DungeonSceneOpenSelectMenu.ogg", 160);
	}

	SelectWindow::~SelectWindow(){
		// リソース解放
		std::vector<std::string> tempDeleteRes = {
		"graphics/UI/SelectWindowQuestionText.png",
		"graphics/UI/SelectWindowYesNo.png",
		"graphics/UI/KeyboardA.png",
		"graphics/UI/KeyboardD.png",
		};

		for (const auto& res : tempDeleteRes) {
			ResourceManager::getResourceManager()->deleteResource(res);
		}

		// フォントハンドル解放
		DeleteFontToHandle(QUESTION_FONT);
		DeleteFontToHandle(YES_NO_FONT);
	}
	
	void SelectWindow::draw(float deltaTime) {
		auto resourceManager = ResourceManager::getResourceManager();
		
		{// 質問を描画
			// UIを描画
			auto questionTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowQuestionText.png");
			DrawRotaGraph(QUESTION_UI_POSITION.x, QUESTION_UI_POSITION.y, QUESTION_UI_SIZE, 0, questionTextUI, true);
			
			// 中央揃えで質問文を描画
			int textWidth = static_cast<int>(GetDrawStringWidthToHandle(questionText_.c_str(), strlen(questionText_.c_str()),QUESTION_FONT));
			float drawPosX = static_cast<float>(QUESTION_UI_POSITION.x - textWidth / 2);
			float drawPosY = static_cast<float>(QUESTION_UI_POSITION.y - GetFontSizeToHandle(QUESTION_FONT) / 2);

			DrawStringToHandleEx(drawPosX, drawPosY, -1,QUESTION_FONT,questionText_.c_str());
		}

		{// はい、いいえを描画
			
			// UI を描画
			auto yesNoTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowYesNo.png");
			DrawRotaGraph(YES_UI_POSITION.x, YES_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);
			DrawRotaGraph(NO_UI_POSITION.x, NO_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);
			// A,DキーのUI描画
			auto keyboardA = resourceManager->getGraphRes("graphics/UI/KeyboardA.png");
			DrawRotaGraph(KEYBOARD_A_UI_POSITION.x, KEYBOARD_A_UI_POSITION.y - 100, KEYBOARD_UI_SIZE, 0, keyboardA, true);
			auto keyboardD = resourceManager->getGraphRes("graphics/UI/KeyboardD.png");
			DrawRotaGraph(KEYBOARD_D_UI_POSITION.x, KEYBOARD_D_UI_POSITION.y - 100, KEYBOARD_UI_SIZE, 0, keyboardD, true);


			// はい・いいえの文字を描画
			switch (currentSelectedChoice_) {
			case e_SelectChoice::YES:
				DrawStringToHandleEx(static_cast<float>(YES_STRING_POSITION.x), static_cast<float>(YES_STRING_POSITION.y), GetColor(255, 0, 0),YES_NO_FONT, "はい");
				DrawStringToHandleEx(static_cast<float>(NO_STRING_POSITION.x), static_cast<float>(NO_STRING_POSITION.y), GetColor(200, 200, 200), YES_NO_FONT, "いいえ");
				break;
			case e_SelectChoice::NO:
				DrawStringToHandleEx(static_cast<float>(YES_STRING_POSITION.x), static_cast<float>(YES_STRING_POSITION.y), GetColor(200, 200, 200), YES_NO_FONT, "はい");
				DrawStringToHandleEx(static_cast<float>(NO_STRING_POSITION.x), static_cast<float>(NO_STRING_POSITION.y), GetColor(255, 0, 0), YES_NO_FONT, "いいえ");
				break;
			}
		}

		// A,Dで選択肢を変える
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

	// 左クリックを押した時、currentSelectedChoice == YES なら YES を返す。NO なら NO を返す。
	// 右クリックを押した時は、NOを選んだ判定
	const SelectWindow::e_SelectChoice SelectWindow::windowChoice() const {
		
		// 左クリックを押した時
		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
			return currentSelectedChoice_;
		}
		// 右クリックを押した時、いいえを選んだ判定
		else if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			return e_SelectChoice::NO;
		}

		return e_SelectChoice::NONE;
	}
}