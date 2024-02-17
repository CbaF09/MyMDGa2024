#pragma once
#include <array>
#include <string>
#include <utility>
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// ダンジョンシーンで右クリックした時に開けるメニューUI
	/// 
	/// </summary>

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
			EraseMagicRune,
			CloseMenu,
			ReturnToTitle,
			NONE,
		};

		// 入力に応じて選択肢を変える。毎フレーム呼ぶ
		e_SelectedMenuWindow process(float deltaTime);
		// 選択肢の強調表示など。毎フレーム呼ぶ
		void draw(float deltaTime);
		// メニューを開いた時にのみ呼ぶ。アイテム一覧のstringを更新する
		void itemWindowsUpdate();


	private:
		const int MENU_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);	// フォント
		const int DESC_FONT = CreateFontToHandle(NULL, 18, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		const tnl::Vector2i TOP_STRINGS_POSITION{ 50,130 }; // 一番上の行の描画位置
		const int32_t STRINGS_OFFSET = 40;		// 行間
		const int32_t ITEM_SYSTEM_OFFSET = 90;  // アイテム一覧とシステム選択肢の間の行間
		const int SELECTED_COLOR = GetColor(255, 0, 0); // 選択中のカラーの色
		const int UN_SELECTED_COLOR = GetColor(200, 200, 200); // 選択中のカラーの色


		const tnl::Vector2i DESC_UI_POSITION{ 70,380 }; // 説明文を記述する UI の位置 ( 左上座標 )
		const tnl::Vector2i DESC_UI_SIZE{ 420,60 }; // 説明文を記述する UI のサイズ
		const int32_t DESC_UI_ALPHA = 128; // 説明文を記述する UI の透明度
		const tnl::Vector2i DESC_STRING_POSITION{ 80,400 }; // 説明文を記述する位置

		// インベントリへの弱参照
		std::weak_ptr<Inventory> weakInventory_;

		// アイテムの選択肢と説明文。pair でまとめている。 first->選択肢名。second->説明文
		std::array<std::pair<std::string, std::string>, 6> itemWindows_{};

		// システム系の選択肢と説明文。pair でまとめている。 first->選択肢名。second->説明文
		std::array<std::pair<std::string, std::string>, 3> systemWindows_{
			std::pair{("ルーンの確認・削除"),("装備中のルーンを確認・削除します")},
			std::pair{("メニューを閉じる"),("メニューを閉じる")},
			std::pair{("タイトル画面"),("タイトル画面に戻る")},
		};

		// 総選択肢数
		int32_t totalStrings = static_cast<int32_t>(itemWindows_.size() + systemWindows_.size());

		// 現在選択中のもの
		int32_t currentSelectIndex_ = 0;

		const std::string& getDescription(int currentSelectIndex) const;

	};

}

