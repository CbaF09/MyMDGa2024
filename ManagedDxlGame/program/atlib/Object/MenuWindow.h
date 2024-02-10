#pragma once
#include <array>
#include <string>
#include <utility>
#include "../../dxlib_ext/dxlib_ext.h"

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
		const int MENU_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);	// フォント
		const int DESC_FONT = CreateFontToHandle(NULL, 18, -1, DX_FONTTYPE_ANTIALIASING);

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

		// アイテムの選択肢
		std::array<std::string,6> itemStrings_ = {
			"( 空きスロット )",
			"( 空きスロット )",
			"( 空きスロット )",
			"( 空きスロット )",
			"( 空きスロット )",
			"( 空きスロット )"
		};

		// アイテムの説明文
		std::array<std::string, 6> itemDesc_ = {
			"アイテムが無い",
			"アイテムが無い",
			"アイテムが無い",
			"アイテムが無い",
			"アイテムが無い",
			"アイテムが無い",
		};

		// 試しで pair でまとめられないかやってみた。成功。
		// TODO : アイテムの方も　pair でまとめてみる。( インベントリから情報を引き出す処理があるので注意 )
		// システム系の選択肢と説明文。pair でまとまっている。 first->選択肢名。second->説明文
		std::array<std::pair<std::string, std::string>, 3> systemOptions_{
			std::pair{("設定"),("各種設定")},
			std::pair{("メニューを閉じる"),("メニューを閉じる。右クリックでも閉じられる")},
			std::pair{("タイトル画面"),("タイトル画面に戻る。進捗保存はありません")},
		};

		// 総選択肢数
		int32_t totalStrings = static_cast<int32_t>(itemStrings_.size() + systemOptions_.size());

		// 現在選択中のもの
		int32_t currentSelectIndex_ = 0;

		std::string getDescription(int currentSelectIndex) const;

	};

}

