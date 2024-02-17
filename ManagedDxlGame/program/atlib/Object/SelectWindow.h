#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// 質問に対し、選択肢のあるウィンドウ用 UI
	/// 選択肢は「はい」「いいえ」のみ
	/// 
	/// </summary>

	class SelectWindow final {
	public:
		SelectWindow();
		~SelectWindow();

		// 選択中の選択肢を表現
		enum class e_SelectChoice {	NONE,YES,NO };

		// 質問文やUIを描画する
		void draw(float deltaTime);
		
		// セレクトウィンドウの質問文を設定し、音を鳴らす
		// arg ... 質問文
		void setSelectWindowQuestionText(const std::string& questionText);

		// 左クリックを押した時、選択中の e_SelectChoice を返す。( YES or NO )
		// 右クリックを押すと、Noを選択した扱いになる
		// 呼び出し側で、e_SelectChoice と == で比較する事で、選択肢に応じて処理分岐出来る
		// 何も入力が無い時は NONE が返ってきているので、常時呼び出しても、入力された瞬間だけ検出できる
		// 注意 ： else で条件分岐しようとすると、入力が無い時のNONEを拾って条件分岐してしまいます
		// 使用例： // 毎フレーム呼び出される所で
		// if (selectWindow_.windowChoice() == e_SelectChoice::YES) { // イエスを選んだ時の処理 }
		// else if (selectWindow_.windowChoice() == e_SelectChoice::NO) { // ノーを選んだ時の処理 }
		const e_SelectChoice windowChoice();

	private:
		// 質問のUI位置
		const tnl::Vector2i QUESTION_UI_POSITION{ DXE_WINDOW_WIDTH/2,200 };	
		// 質問のUIサイズ ( 1 で画像元サイズ )
		const float QUESTION_UI_SIZE = 0.6f;	
		// 「はい」の背景UI位置
		const tnl::Vector2i YES_UI_POSITION{ DXE_WINDOW_WIDTH / 4,400 };	
		// 「いいえ」の背景UI位置
		const tnl::Vector2i NO_UI_POSITION{ DXE_WINDOW_WIDTH - DXE_WINDOW_WIDTH / 4,400 };	
		// はい、の文字を表示する位置
		const tnl::Vector2i YES_STRING_POSITION{300,390};
		// いいえ、の文字を表示する位置
		const tnl::Vector2i NO_STRING_POSITION{935,390};
		// AキーのUIを表示する位置
		const tnl::Vector2i KEYBOARD_A_UI_POSITION{ YES_UI_POSITION.x,YES_UI_POSITION.y + 50 };
		// DキーのUIを表示する位置
		const tnl::Vector2i KEYBOARD_D_UI_POSITION{ NO_UI_POSITION.x + 10,NO_UI_POSITION.y + 50 };
		// YES・NO のUIサイズ ( 1 で画像元サイズ )
		const float YES_NO_UI_SIZE = 0.4f;
		// A,DのキーボードUIサイズ ( 1で画像元サイズ )
		const float KEYBOARD_UI_SIZE = 0.5f;

		// フォント
		const int QUESTION_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
		const int YES_NO_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		// 現在選択中の選択肢 ( YES or NO )
		e_SelectChoice currentSelectedChoice_ = e_SelectChoice::YES;
		// 質問文
		std::string questionText_ = "";
	};
}