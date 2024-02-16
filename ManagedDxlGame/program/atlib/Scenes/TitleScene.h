#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	class TitleScene final : public atl::Base_Scene {
	public:
		TitleScene();
		~TitleScene();

	private:

		// ----------------------------------
		// 変数

		// 背景関連
		const tnl::Vector2i LOGO_POSITION{ 330,150 };		// タイトルロゴの中心位置
		const float LOGO_DEFAULT_ANGLE = -10.0f;			// タイトルロゴの回転 ( 度数法 )
		const float LOGO_SIZE = 0.7f;						// タイトルロゴのサイズ ( 1 で画像元サイズ )
		
		// UI関連
		
		// ボタン上に表示する文字用フォント
		const int BUTTON_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		// 現在選択中のボタンを表現する enum
		enum class e_SelectTitleButton {
			START,
			PROROGUE,
			EXIT,
			BUTTON_MAX
		}
		currentSelectButton_ = e_SelectTitleButton::START;

		const tnl::Vector2i BUTTON_POSITION{ 1000,400 };	// 一番上のボタンの中心位置
		const tnl::Vector2i BUTTON_OFFSET{ 0,100 };			// 一番上のボタンからオフセットされる量
		const tnl::Vector2i BUTTON_SELECTED_OFFSET{ -50,0 };// 選択中のボタンを追加でオフセットする量
		const tnl::Vector2i BUTTON_STRING_OFFSET{ -60,-15 };	// ボタンに描画される
		const int32_t BUTTON_STRING_FONTSIZE = 30;			// ボタンのフォントサイズ
		const tnl::Vector2i BUTTON_CURSOR_OFFSET{ -300,0 };	// カーソルとボタンの距離
		const tnl::Vector2i CURSOR_KEYBOARD_WS_OFFSET{ 0,80 };	// カーソルとキーボードアイコンの距離
		const tnl::Vector2i CURSOR_KEYBOARD_SPACE_OFFSET{ -100,0 };	// カーソルとキーボードアイコンの距離
		const float BUTTON_CURSOR_SIZE = 0.15f;						// カーソルのサイズ ( 1 で画像元サイズ )
		const float KEYBOARD_WS_SIZE = 0.5;						// キーボードアイコンのサイズ ( 1 で画像元サイズ )
		const float BUTTON_SIZE = 0.5f;						// ボタンのサイズ ( 1 で画像元サイズ )
		const tnl::Vector2i SPACE_IS_ENTER_POSITION{ 800,200 };	// 文字列の背景になるボックスの位置
		const tnl::Vector2i SPACE_IS_ENTER_BACK_LEFTUP_POINT{ 780,180 };	// 文字列の背景になるボックスの位置
		const tnl::Vector2i SPACE_IS_ENTER_BACK_RIGHTDOWN_POINT{ 1160,250 };	// 文字列の背景になるボックスの大きさ

		bool isDisplayButton = false;	// ボタンを表示するか否かのフラグ
		bool isGameEnd = false;			// ゲーム終了状態かのフラグ

		// プロローグ関連
		const int PROROGUE_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		std::vector<std::string> prorogueText{};
		const int32_t PROROGUE_BACKGROUND_BRIGHT = 126;	// プロローグ中の背景の明るさ ( 0 で元と同じ、255 で真っ黒 )
		const tnl::Vector2i TEXT_POSITION{ 25,250 };	// 一番上の行の表示位置
		const tnl::Vector2i TEXT_OFFSET{ 5,40 };		// 一行ごとにオフセットされる量
		const int32_t PROROGUE_FIRST_LOG = 2;			// 一番最初に表示される行数
		const float LOG_LINE_INTERVAL = 5.0f;			// 表示行を増やす間隔 ( 秒 )
		int32_t drawLogLine_ = 0;
		float totalDeltaTime_ = 0.0f;	// 累積時間

		// ----------------------------------
		// メソッド

		// 毎フレーム呼び出し
		void sceneUpdate(float deltaTime) override;

		// 描画をまとめて
		void draw(float deltaTime);
		// 背景描画
		void drawBackground(float deltaTime);
		// 「SPACE で 決定」の文字列を描画
		void drawSpaceIsEnter();
		// ボタン群を描画
		void drawButton(float deltaTime);
		
		// シーケンス用
		SEQUENCE(TitleScene, &TitleScene::seqInit);

		// 初期設定
		bool seqInit(float deltaTime);

		// 音源データのボリューム設定
		void volumeSetting();

		// キー入力待機
		bool seqKeyInputWait(float deltaTime);
		// スタートボタンを押した時の処理
		bool seqStart(float deltaTime);
		// プロローグボタンを押した時の処理
		bool seqProrogue(float deltaTime);
		// ゲーム終了ボタンを押した時の処理
		bool seqExit(float deltaTime);

		// 選択中のボタンを一つ上に
		bool selectButtonUp();
		// 選択中のボタンを一つ下に
		bool selectButtonDown();

		// デバッグ用
		void debug_keyInput();
	};

}