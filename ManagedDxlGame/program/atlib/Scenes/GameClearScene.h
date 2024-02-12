#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	// ゲームクリア時のシーン
	// エピローグとタイトルに戻る処理のみ
	
	class GameClearScene final : public Base_Scene{
	public:
		GameClearScene();
		~GameClearScene();

	private:
		// -------------------------------------
		// 変数

		// プロローグ関連
		const int EPILOGUE_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

		const int32_t EPILOGUE_BACKGROUND_BRIGHT = 126;	// 背景の明るさ ( 0 で元と同じ、255 で真っ黒 )
		const tnl::Vector2i CAN_BACK_TEXT_POSITION{ 10,10 }; // 「スペースキーでタイトルに戻れます」を表示する位置
		const tnl::Vector2i TEXT_POSITION{ 25,150 };	// 一番上の行の表示位置
		const tnl::Vector2i TEXT_OFFSET{ 0,45 };		// 一行ごとにオフセットされる量

		// エピローグの文字列
		std::vector<std::string> epilogueString_{};
		int32_t currentTopLogIndex_ = 0;

		// -------------------------------------
		// メソッド

		// 毎フレーム呼ぶ
		void sceneUpdate(float deltaTime) override;
		
		// 背景描画
		void drawBackground(float deltaTime);

		// シーケンス ( 文字の描画も行う )
		SEQUENCE(GameClearScene,&GameClearScene::seqInit);

		// 初期化 ( 最初に一度だけ呼ばれる )
		bool seqInit(float deltaTime);

		// 本シーケンス
		bool seqEpilogue(float deltaTime);

		//void invokeDrawLogLineIncrement();

		// タイトルシーンに遷移するフェードインアウト
		bool seqToTitleScene(float deltaTime);

	};

}