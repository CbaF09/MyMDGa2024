#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	// 選択肢のある 2D UI用
	// 対応している選択肢は二つのみ
	class SelectWindow {
	public:
		// コンストラクタで文言（階段を登りますか？等）を指定する
		SelectWindow(std::string questionText);

		~SelectWindow();

		// 選択中の選択肢を表現
		enum class e_SelectChoice {	NONE,YES,NO };

		// 質問文言やUIを描画する
		void draw(float deltaTime);
		
		// エンター or スペースを押した時、currentSelectedChoice == YES なら true を返す。NO なら false を返す。
		const e_SelectChoice& windowChoice();

	private:
		e_SelectChoice currentSelectedChoice_ = e_SelectChoice::YES;
		std::string questionText_;

		const tnl::Vector2i QUESTION_UI_POSITION{ DXE_WINDOW_WIDTH/2,200 };	// 質問のUI位置
		const float QUESTION_UI_SIZE = 0.6f;									// 質問のUIサイズ ( 1 で画像元サイズ )
		const tnl::Vector2i YES_UI_POSITION{ DXE_WINDOW_WIDTH / 4,400 };	// YES の選択肢の位置
		const tnl::Vector2i NO_UI_POSITION{ DXE_WINDOW_WIDTH - DXE_WINDOW_WIDTH / 4,400 };	// NO の選択肢の位置
		const tnl::Vector2i YES_STRING_POSITION{300,390};	// はい、の文字を表示する位置
		const tnl::Vector2i NO_STRING_POSITION{935,390};	// いいえ、の文字を表示する位置
		const float YES_NO_UI_SIZE = 0.4f;									// YES・NO のUIサイズ ( 1 で画像元サイズ )

		const int QUESTION_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);
		const int YES_NO_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

	};


}