#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	// ゲームオーバーした時にやってくるシーン
	// 真っ黒背景に文字列が出ているだけ
	// エンターキーかスペースキーを押すとタイトルに戻る

	class GameOverScene final : public Base_Scene {

	public:

	private:

		const int GAMEOVER_FONT = CreateFontToHandle(NULL, 25, -1);

		const int FONT_SIZE = 25;		// フォントサイズ
		const tnl::Vector2i STRING_POSITION{ 200,200 };	// 文字列の位置

		void sceneUpdate(float deltaTime);

		SEQUENCE(GameOverScene, &GameOverScene::seqInit);
		bool seqInit(float deltaTime);
		bool seqDraw(float deltaTime);

	};

}