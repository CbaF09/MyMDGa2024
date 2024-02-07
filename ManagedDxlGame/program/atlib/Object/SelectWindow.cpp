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

		// ファイルパスを一つずつ解放。解放の成功失敗をデバッグログに出力
		for (const auto& res : tempDeleteRes) {
			bool isDelete = ResourceManager::getResourceManager()->deleteResource(res);
			if (!isDelete) { tnl::DebugTrace("\n------------------------------\nSelectWindow::デストラクタ メモリ解放 => 正常に解放されていません"); }
			else { tnl::DebugTrace("\n------------------------------\nSelectWindow::デストラクタ メモリ解放 => 正常"); }
		}
		tnl::DebugTrace("\n------------------------------\n"); // ログが見づらいので最後に改行と切り取り線を入れる

		DeleteFontToHandle(QUESTION_FONT);
		DeleteFontToHandle(YES_NO_FONT);
	}


	void SelectWindow::draw(float deltaTime) {
		auto resourceManager = ResourceManager::getResourceManager();
		
		{// 質問を描画
			// UIを描画
			auto questionTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowQuestionText.png");
			DrawRotaGraph(QUESTION_UI_POSITION.x, QUESTION_UI_POSITION.y, QUESTION_UI_SIZE, 0, questionTextUI, true);

			
			// 中央揃えする為の計算
			int textWidth = GetDrawStringWidth(questionText_.c_str(), strlen(questionText_.c_str()));
			int drawPosX = QUESTION_UI_POSITION.x - textWidth / 2;
			int drawPosY = QUESTION_UI_POSITION.y - GetFontSizeToHandle(QUESTION_FONT) / 2;

			// 黒文字で描画
			DrawStringToHandleEx(drawPosX, drawPosY, GetColor(0,0,0),QUESTION_FONT,questionText_.c_str());
		}

		{// はい、いいえを描画
			// UI を描画
			auto yesNoTextUI = resourceManager->getGraphRes("graphics/UI/SelectWindowYesNo.png");
			DrawRotaGraph(YES_UI_POSITION.x, YES_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);
			DrawRotaGraph(NO_UI_POSITION.x, NO_UI_POSITION.y, YES_NO_UI_SIZE, 0, yesNoTextUI,true);

			// YES・NOを描画
			switch (currentSelectedChoice_) {
			case e_SelectChoice::YES:
				DrawStringToHandleEx(YES_STRING_POSITION.x, YES_STRING_POSITION.y, GetColor(255, 0, 0),YES_NO_FONT, "はい");
				DrawStringToHandleEx(NO_STRING_POSITION.x, NO_STRING_POSITION.y, GetColor(200, 200, 200), YES_NO_FONT, "いいえ");
				break;
			case e_SelectChoice::NO:
				DrawStringToHandleEx(YES_STRING_POSITION.x, YES_STRING_POSITION.y, GetColor(200, 200, 200), YES_NO_FONT, "はい");
				DrawStringToHandleEx(NO_STRING_POSITION.x, NO_STRING_POSITION.y, GetColor(255, 0, 0), YES_NO_FONT, "いいえ");
				break;
			}
		}

		// A,Dで選択肢を変える
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_A)) {
			currentSelectedChoice_ = e_SelectChoice::YES;
		}
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_D)) {
			currentSelectedChoice_ = e_SelectChoice::NO;
		}
	}

	// エンター or スペースを押した時、currentSelectedChoice == YES なら true を返す。NO なら false を返す。
	const SelectWindow::e_SelectChoice& SelectWindow::windowChoice() {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN, eKeys::KB_SPACE)) {
			return currentSelectedChoice_;
		}
		return e_SelectChoice::NONE;
	}
}