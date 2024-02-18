#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// 装備中の MagicRune を 画面に描画したり、選択したりする時のUI用クラス
	/// 
	/// </summary>

	class MagicRuneWindow final {

	public:
		~MagicRuneWindow();

		// A,Dで currentSelectIndex_ を操作する
		void process();

		// ウィンドウを描画
		// isOpenの状態に応じて、どう描画されるか変わる
		void draw();

		// ルーン選択画面を開く閉じるをスイッチ
		void switchOpenMagicRuneWindow() { isOpen_ = !isOpen_; }

		// ゲッター ( 現在選択中のルーン )
		int32_t getCurrentSelectRune_() { return currentSelectIndex_; }
		// ゲッター ( 現在、ルーン選択画面かどうか )
		bool IsOpenMagicRuneWindow() { return isOpen_; }

		// 現在選択中のルーン用インデックスを 0 に設定する
		void resetIndex() { currentSelectIndex_ = 0; }

	private:
		// フォント
		const int RUNE_NAME_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
		const int RUNE_DESC_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		// 背景画像の位置
		const tnl::Vector2i RUNE_BACKGRAPH_POSITION{ 950,60 };
		// 背景画像の隙間 ( X 方向 )
		const int32_t RUNE_BACKGRAPH_OFFSET = 70;
		// 背景画像の背景のオーバーレイ ( 楕円 ) の位置
		const tnl::Vector2i RUNE_BACK_OVERLAY_POSITION{ 1090,60 };
		// オーバーレイ ( 楕円 ) の大きさ
		const tnl::Vector2i RUNE_BACK_OVERLAY_SIZE{ 200,50 };
		// オーバーレイ ( 楕円 ) の透明度
		const float RUNE_BACK_OVERLAY_ALPHA = 200;
		// ルーン名を描画する位置
		const tnl::Vector2i RUNE_NAME_STRING_POSITION{ 200 ,250 };
		// ルーンの説明文を描画するY位置 ( X は文字列の大きさに合わせて中央揃えするので必要無し )
		const float RUNE_DESC_STRING_POSITION_Y = 300;
		// ルーンを選択中の背景を暗くするオーバーレイの透明度
		const int32_t RUNE_OVERLAY_ALPHA = 128;
		// ルーンを選択中

		// 現在選択中のルーン
		int32_t currentSelectIndex_ = 0;
		// 現在、ウィンドウが開かれているか
		bool isOpen_ = false;

		// draw 関数のヘルパー関数群
		// 背景ルーンを描画
		void drawRuneBack();
		// 常時表示中の処理
		void drawDefaultMode();
		// ルーンを選択中の処理
		void drawSelectedMode();
		// ルーンを選択中の追加描画 ( ルーンの効果説明 )
		void drawSelectedModeRuneDesc();
	};

}