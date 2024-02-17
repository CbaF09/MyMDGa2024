#include "MagicRuneWindow.h"
#include "MagicRuneSystemManager.h"
#include "../Singletons/ResourceManager.h"

namespace atl {
	
	MagicRuneWindow::~MagicRuneWindow() {
		// リソース解放
		ResourceManager::getResourceManager()->deleteResource("graphics/UI/MagicRune/MagicRuneBack.png");
		ResourceManager::getResourceManager()->deleteResource("graphics/UI/MagicRune/RuneSelectWindowBack.png");
		
		// フォントハンドル解放
		DeleteFontToHandle(RUNE_NAME_FONT);
		DeleteFontToHandle(RUNE_DESC_FONT);
	}

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
		// 背景の空ルーンを描画
		drawRuneBack();

		// もし、ルーン選択画面でないなら、標準状態で描画
		if (!isOpen_) {
			drawDefaultMode();
		}
		// ルーン選択画面なら、選択画面状態で描画
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
		// 画面全体を薄暗くするためのオーバーレイ描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, RUNE_OVERLAY_ALPHA); // 半透明設定
		DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 描画モードを通常に戻す

		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		for (int i = 0; i < runes.size(); ++i) {
			// 選択中でないものは暗く描画
			if (i != currentSelectIndex_) { 
				SetDrawBright(128, 128, 128);
			}

			auto runeGraph = runes[i]->getRuneGraph();
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeGraph, true);
			
			// 選択中でないものは暗く描画していたのを元に戻す
			if (i != currentSelectIndex_) {
				SetDrawBright(255, 255, 255);
			}
		}
		
		// 選択中のルーンの効果説明を描画
		drawSelectedModeRuneDesc();
	}

	// 選択中ルーンの効果説明
	void MagicRuneWindow::drawSelectedModeRuneDesc() {
		// 効果説明用の背景を描画
		DrawRotaGraph(DXE_WINDOW_WIDTH/2,DXE_WINDOW_HEIGHT/2,1,0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/RuneSelectWindowBack.png"),true);
		
		// ルーンの情報を取得
		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		auto& runeName = runes[currentSelectIndex_]->getRuneName();

		// ルーンの名前を描画
		DrawStringToHandleEx(RUNE_NAME_STRING_POSITION.x, RUNE_NAME_STRING_POSITION.y, -1, RUNE_NAME_FONT, "%s", runeName.c_str());

		// ルーンの説明文を描画 ( 中央揃え )

		// 中央揃えの為の計算
		auto& runeDesc = runes[currentSelectIndex_]->getRuneDesc();
		auto widthFix = GetDrawStringWidthToHandle(runeDesc.c_str(), strlen(runeDesc.c_str()), RUNE_DESC_FONT);
		float drawPosX = static_cast<float>(DXE_WINDOW_WIDTH / 2) - widthFix / 2.0f;
		
		DrawStringToHandleEx(drawPosX, RUNE_DESC_STRING_POSITION_Y, -1, RUNE_DESC_FONT, "%s", runeDesc.c_str());
	}

}