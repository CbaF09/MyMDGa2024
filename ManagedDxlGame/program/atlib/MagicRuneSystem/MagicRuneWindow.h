#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	class MagicRuneWindow final {

	public:

		// A,Dで currentSelectIndex_ を操作する
		void process();
		
		// ウィンドウを描画
		void draw();

		// マジックルーンウィンドウを開く閉じるをスイッチ
		void switchOpenMagicRuneWindow() { isOpen_ = !isOpen_; }
		
		// ゲッター
		int32_t getCurrentSelectIndex_() { return currentSelectIndex_; }
		bool IsOpenMagicRuneWindow() { return isOpen_; }


	private:
		// フォント
		const int RUNE_NAME_FONT = CreateFontToHandle(NULL, 30, -1,DX_FONTTYPE_ANTIALIASING_EDGE);
		const int RUNE_DESC_FONT = CreateFontToHandle(NULL, 25, -1,DX_FONTTYPE_ANTIALIASING_EDGE);

		// 背景画像のポジション
		const tnl::Vector2i RUNE_BACKGRAPH_POSITION{ 950,60 };
		// 背景画像の隙間 ( X 方向 )
		const int32_t RUNE_BACKGRAPH_OFFSET = 70;
		// ルーン名を描画する位置
		const tnl::Vector2i RUNE_NAME_STRING_POSITION{ 200 ,250 };
		// ルーンの説明文を描画するY位置 ( X は文字列の大きさに合わせて中央揃えするので必要無し )
		const float RUNE_DESC_STRING_POSITION_Y = 300;
		
		
		int32_t currentSelectIndex_ = 0;
		bool isOpen_ = false;
		
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