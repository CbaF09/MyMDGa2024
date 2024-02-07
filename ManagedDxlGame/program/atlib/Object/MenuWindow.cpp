#include "../dxlib_ext/dxlib_ext.h"
#include "MenuWindow.h"
#include "../Object/Inventory.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	MenuWindow::~MenuWindow() {
		DeleteFontToHandle(MENU_FONT);
	}

	MenuWindow::MenuWindow(std::weak_ptr<Inventory> inventory) : weakInventory_(inventory) {
		auto inv = weakInventory_.lock();

		for (int i = 0; i < inv->getItemList().size(); ++i) {
			auto& itemData = inv->getItemData(i);
			if (itemData) {
				itemStrings_.at(i) = itemData->getItemName();
				itemDesc_.at(i) = itemData->getItemDescString();
			}
		}
	}

	MenuWindow::e_SelectedMenuWindow MenuWindow::process(float deltaTime) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
			// 最上部を選択中の場合は、上に行けない
			if (currentSelectIndex_ == 0) return e_SelectedMenuWindow::NONE;
			--currentSelectIndex_;
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
			// 最下部を選択中は、下に行けない
			if (currentSelectIndex_ == totalStrings - 1) return e_SelectedMenuWindow::NONE;
			++currentSelectIndex_;
		}

		// スペース or エンター or 左クリックで、currentSelectIndex を返す
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE,eKeys::KB_RETURN) ||
			tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
			return static_cast<e_SelectedMenuWindow>(currentSelectIndex_);
		}

		return e_SelectedMenuWindow::NONE;
	}

	void MenuWindow::draw(float deltaTime) {
		{//　選択肢を描画。選択中の選択肢は強調表示
			tnl::Vector2i actualyPos = TOP_STRINGS_POSITION;

			// アイテムリストの描画
			for (int i = 0; i < itemStrings_.size(); ++i) {

				// 現在選択中の選択肢は強調する
				if (currentSelectIndex_ == i) {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), SELECTED_COLOR, MENU_FONT, itemStrings_.at(i).c_str());
				}
				else {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), UN_SELECTED_COLOR, MENU_FONT, itemStrings_.at(i).c_str());
				}
				actualyPos.y += STRINGS_OFFSET;
			}

			actualyPos.y += ITEM_SYSTEM_OFFSET; // システム文字列の位置に描画位置を移動

			// システム文字列の描画
			for (int i = 0; i < systemStrings_.size(); ++i) {
				// インデックス位置調整
				if (currentSelectIndex_ == i + itemStrings_.size()) {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), SELECTED_COLOR, MENU_FONT, systemStrings_.at(i).c_str());
				}
				else {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), UN_SELECTED_COLOR, MENU_FONT, systemStrings_.at(i).c_str());
				}
				actualyPos.y += STRINGS_OFFSET;
			}
		}

		{// 説明文を記述する UI を描画
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,DESC_UI_ALPHA);
			DrawBox(DESC_UI_POSITION.x, DESC_UI_POSITION.y, DESC_UI_POSITION.x+ DESC_UI_SIZE.x, DESC_UI_POSITION.y + DESC_UI_SIZE.y, GetColor(0, 0, 255), true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		}

		{// 説明文を描画
			auto descString = getDescription(currentSelectIndex_);
			DrawStringToHandleEx(static_cast<float>(DESC_STRING_POSITION.x), static_cast<float>(DESC_STRING_POSITION.y), -1, DESC_FONT, descString.c_str());
		}
	}

	std::string MenuWindow::getDescription(int currentSelectIndex) const {
		// アイテム一覧を指している場合
		if (0 <= currentSelectIndex_ && currentSelectIndex_ < itemStrings_.size()) {
			return itemDesc_[currentSelectIndex];
		}

		// システム一覧を指している場合
		switch (currentSelectIndex_) {
		case static_cast<int>(e_SelectedMenuWindow::Setting):
			return "各種設定";
		case static_cast<int>(e_SelectedMenuWindow::ReturnToTitle):
			return "タイトル画面に戻る。進捗保存はありません";
		case static_cast<int>(e_SelectedMenuWindow::CloseMenu):
			return "メニューを閉じる。右クリックでも閉じる";
		default:
			return "";
		}
	}
}