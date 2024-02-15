#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	class MagicRuneWindow final {

	public:

		// A,Dで currentSelectIndex_ を操作する
		void process();
		
		// 描画
		void draw();

		// マジックルーンウィンドウを開く閉じるをスイッチ
		void switchOpenMagicRuneWindow() { isOpen_ = !isOpen_; }
		
		// ゲッター
		int32_t getCurrentSelectIndex_() { return currentSelectIndex_; }
		bool IsOpenMagicRuneWindow() { return isOpen_; }


	private:
		const tnl::Vector2i RUNE_BACKGRAPH_POSITION{ 950,60 };
		const int32_t RUNE_BACKGRAPH_OFFSET = 70;
		int32_t currentSelectIndex_ = 0;

		bool isOpen_ = false;
		
		// 背景ルーンを描画
		void drawRuneBack();
		// 常時表示中の処理
		void drawDefaultMode();
		// ルーンを選択中の処理
		void drawSelectedMode();
	};

}