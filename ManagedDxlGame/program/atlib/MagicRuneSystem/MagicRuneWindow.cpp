#include "MagicRuneWindow.h"
#include "MagicRuneSystemManager.h"
#include "../Singletons/ResourceManager.h"

namespace atl {
	
	void MagicRuneWindow::process() {
		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		// 装備中のルーンが無い場合、早期リターン
		if (runes.size() == 0) return;

		// A,Dで左右に選択
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_A)) {
			--currentSelectIndex_;
			if (currentSelectIndex_ < 0) {
				currentSelectIndex_ = runes.size() - 1;
			}
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_D)) {
			++currentSelectIndex_;
			currentSelectIndex_ %= runes.size();
		}
	}
	
	void MagicRuneWindow::draw() {
		drawRuneBack();

		if (!isOpen_) {
			drawDefaultMode();
		}
		else {
			drawSelectedMode();
		}
	}

	// 背景の空ルーンを描画
	void MagicRuneWindow::drawRuneBack() {
		auto runeBack = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/MagicRuneBack.png");
		auto maxEquipment = MagicRuneSystemManager::getMagicRuneSystemManager()->getMaxEquipmentRune();

		// 空ルーンを描画
		for (int i = 0; i < maxEquipment; ++i) {
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeBack, true);
		}
	}

	// ウィンドウが操作されていない時の描画
	void MagicRuneWindow::drawDefaultMode() {
		// 装備しているルーンに設定しているアイコン画像を描画
		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		for (int i = 0; i < runes.size(); ++i) {
			auto runeGraph = runes[i]->getRuneGraph();
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeGraph, true);
		}
	}

	// ウィンドウを操作している間の描画
	void MagicRuneWindow::drawSelectedMode() {
		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		for (int i = 0; i < runes.size(); ++i) {
			if (i != currentSelectIndex_) { // 選択中でないものは暗く描画
				SetDrawBright(128, 128, 128);
			}
			auto runeGraph = runes[i]->getRuneGraph();
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeGraph, true);
			if (i != currentSelectIndex_) {
				SetDrawBright(255, 255, 255);
			}
		}
		
		drawSelectedModeRuneDesc();
	}

	// ルーンを選択中の追加描画 ( ルーンの効果説明 )
	void MagicRuneWindow::drawSelectedModeRuneDesc() {
		DrawRotaGraph(DXE_WINDOW_WIDTH/2,DXE_WINDOW_HEIGHT/2,1,0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/RuneSelectWindowBack.png"),true);
		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		
		// ルーンの名前を描画
		auto& runeName = runes[currentSelectIndex_]->getRuneName();
		DrawStringToHandleEx(RUNE_NAME_STRING_POSITION.x, RUNE_NAME_STRING_POSITION.y, -1, RUNE_NAME_FONT, "%s", runeName.c_str());
		
		// ルーンの説明文を描画

		// 中央揃えの為の計算
		auto& runeDesc = runes[currentSelectIndex_]->getRuneDesc();
		auto widthFix = GetDrawStringWidthToHandle(runeDesc.c_str(), strlen(runeDesc.c_str()), RUNE_DESC_FONT);
		float drawPosX = static_cast<float>(DXE_WINDOW_WIDTH / 2) - widthFix / 2.0f;
		
		DrawStringToHandleEx(drawPosX, RUNE_DESC_STRING_POSITION_Y, -1, RUNE_DESC_FONT, "%s", runeDesc.c_str());
	}

}