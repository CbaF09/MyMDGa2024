#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	class TitleScene final : public atl::Base_Scene {
	public:
		~TitleScene();

	private:
		void sceneUpdate(float deltaTime) override;

		// 描画をまとめて
		void draw(float deltaTime);

		
		// シーケンス用
		SEQUENCE(TitleScene, &TitleScene::seqInit);
		// 初期設定
		bool seqInit(float deltaTime);
		bool seqKeyInputWait(float deltaTime);
		bool seqToDungeonScene(float deltaTime);

		// デバッグ用
		void debug_keyInput();
	};

}