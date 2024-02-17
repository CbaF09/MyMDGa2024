#include "../../dxlib_ext/dxlib_ext.h"
#include "MenuWindow.h"
#include "../Object/Inventory.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	MenuWindow::~MenuWindow() {
		DeleteFontToHandle(MENU_FONT);
		DeleteFontToHandle(DESC_FONT);
	}

	MenuWindow::MenuWindow(std::weak_ptr<Inventory> inventory) 
		: weakInventory_(inventory) {
	}

	MenuWindow::e_SelectedMenuWindow MenuWindow::process(float deltaTime) {
		// モジュラー演算で、選択肢最上部と最下部がループするようにしている
		// W キーを押した時
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
			--currentSelectIndex_;
			currentSelectIndex_ = (currentSelectIndex_ + totalStrings) % totalStrings;
		}
		// Sキーを押した時
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
			++currentSelectIndex_;
			currentSelectIndex_ = (currentSelectIndex_ + totalStrings) % totalStrings;
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
			for (int i = 0; i < itemWindows_.size(); ++i) {

				// 現在選択中の選択肢は強調する
				if (currentSelectIndex_ == i) {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), SELECTED_COLOR, MENU_FONT, itemWindows_.at(i).first.c_str());
				}
				else {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), UN_SELECTED_COLOR, MENU_FONT, itemWindows_.at(i).first.c_str());
				}
				actualyPos.y += STRINGS_OFFSET;
			}

			// システム文字列の位置に描画位置を移動
			actualyPos.y += ITEM_SYSTEM_OFFSET;

			// システム文字列の描画
			for (int i = 0; i < systemWindows_.size(); ++i) {
				// インデックス位置調整
				if (currentSelectIndex_ == i + itemWindows_.size()) {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), SELECTED_COLOR, MENU_FONT, systemWindows_.at(i).first.c_str());
				}
				else {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), UN_SELECTED_COLOR, MENU_FONT, systemWindows_.at(i).first.c_str());
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
			auto& descString = getDescription(currentSelectIndex_);
			DrawStringToHandleEx(static_cast<float>(DESC_STRING_POSITION.x), static_cast<float>(DESC_STRING_POSITION.y), -1, DESC_FONT, descString.c_str());
		}
	}

	void MenuWindow::itemWindowsUpdate() {
		auto inv = weakInventory_.lock();
		for (int i = 0; i < itemWindows_.size(); ++i) {
			auto& item = inv->getItem(i);
			// アイテムがあったら
			if (item) {
				// アイテムの名前と説明文をセット
				itemWindows_.at(i).first = item->getItemName();
				itemWindows_.at(i).second = item->getItemDescString();
			}
			// アイテムがなかったら
			else {
				// デフォの名前と説明文をセット
				itemWindows_.at(i).first = "( 空きスロット )";
				itemWindows_.at(i).second = "アイテムが無い";
			}
		}
	}

	const std::string& MenuWindow::getDescription(int currentSelectIndex) const {
		// アイテム一覧を指している場合
		if (0 <= currentSelectIndex_ && currentSelectIndex_ < itemWindows_.size()) {
			// アイテム一覧の説明文を返す
			return itemWindows_.at(currentSelectIndex).second;
		}
		else {// システム一覧を指している場合
			// システム一覧の説明文を返す
			// アイテムの下にシステムのボタンがある
			return systemWindows_.at(currentSelectIndex_ - itemWindows_.size()).second;
		}
	}
}