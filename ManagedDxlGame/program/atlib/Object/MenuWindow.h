#pragma once
#include <array>
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class Inventory;

	class MenuWindow final {
	public:
		~MenuWindow();

		explicit MenuWindow(std::weak_ptr<Inventory> inventory);

		enum class e_SelectedMenuWindow {
			Item1,
			Item2,
			Item3,
			Item4,
			Item5,
			Item6,
			Setting,
			CloseMenu,
			ReturnToTitle,
			NONE,
		};

		e_SelectedMenuWindow process(float deltaTime);
		void draw(float deltaTime);

	private:
		const tnl::Vector2i TOP_STRINGS_POSITION{ DXE_WINDOW_WIDTH / 3,200 }; // 一番上の行の描画位置
		const int32_t STRINGS_OFFSET = 30;		// 行間
		const int32_t ITEM_SYSTEM_OFFSET = 50;  // アイテム一覧とシステム選択肢の間の行間
		const int SELECTED_COLOR = GetColor(255, 0, 0); // 選択中のカラーの色
		const int UN_SELECTED_COLOR = GetColor(200, 200, 200); // 選択中のカラーの色

		const int MENU_FONT = CreateFontToHandle(NULL,30,-1,DX_FONTTYPE_ANTIALIASING);	// フォント

		// インベントリへの弱参照
		std::weak_ptr<Inventory> weakInventory_;

		// アイテムの選択肢数
		std::array<std::string,6> itemStrings_ = {
			"( 空きスロット )",
			"( 空きスロット )",
			"( 空きスロット )",
			"( 空きスロット )",
			"( 空きスロット )",
			"( 空きスロット )"
		};

		// システム系の選択肢
		std::array<std::string,3> systemStrings_ = {
			"設定",
			"メニューを閉じる",
			"タイトルに戻る",
		};

		// 総選択肢数
		int32_t totalStrings = static_cast<int32_t>(itemStrings_.size() + systemStrings_.size());

		int32_t currentSelectIndex_ = 0;
	};

}