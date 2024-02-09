#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	// ゲームクリア時のシーン
	// エピローグとタイトルに戻る処理のみ
	
	class GameClearScene final : public Base_Scene{
	public:
		GameClearScene() {
			// テクスチャによるメモリリーク対策
			dxe::Texture::DestroyUnReferenceTextures();
		}
		~GameClearScene();

	private:
		// -------------------------------------
		// 変数

		// プロローグ関連
		const int EPILOGUE_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

		const int32_t EPILOGUE_BACKGROUND_BRIGHT = 126;	// 背景の明るさ ( 0 で元と同じ、255 で真っ黒 )
		const tnl::Vector2i TEXT_POSITION{ 25,150 };	// 一番上の行の表示位置
		const tnl::Vector2i TEXT_OFFSET{ 0,45 };		// 一行ごとにオフセットされる量
		const float LOG_LINE_INTERVAL = 5.0f;			// 表示行を増やす間隔 ( 秒 )
		int32_t drawLogLine_ = 0;		// 描画される行数
		float totalDeltaTime_ = 0.0f;	// 累積時間
		int32_t textAlpha_ = 255;		// テキストの透明度用変数
		const int32_t ALPHA_MINUS_SPEED = 2; // テキストが透明になっていく速度
		const float FULL_ALPHA_TIME = 4.5;	// 全テキストが描画された後、最大透明度のままの待機時間
		enum class e_EpiloguePage {
			ZERO = 0,
			ONE,
			TWO,
			THREE,
			FOUR,
			PAGE_MAX
		}currentPage_ = e_EpiloguePage::ZERO;
		const std::vector<std::vector<std::string>> epiloguePage = {
			{// 0ページ目
				"最上階に辿り着いた。",
				"中央には、光り輝く巨大な杖が鎮座している。",
				"「はじめまして、ワイズマン」",
				"私が杖に向かって頭を下げると、朗らかな声がどこからか聞こえてきた。",
				"『やあ、君か。待っていたよ、招待状を出してからずっとね』",
				"ワイズマンの大魔道具が一つ――物見の杖――この杖にはワイズマンの魔力と意思が宿っている。",
			},
			{ // 1ページ目
				"『わざわざご足労頂いてすまないね。何せこの身体だ』",
				"心なしか、声色に合わせて物見の杖が明滅しているように見える。",
				"「あなたは……今のご自分について、どうお考えなのですか？」",
				"『まあ、悪くはないね』",
				"少し踏み込んだ質問をしたつもりが、あっさりと返されてしまった。",
				"もしかしたら、もう何度も聞かれてきた事なのかもしれない。",
				"ワイズマンは何百年も昔の魔法使いだ。とっくに息を引き取っている。",
				"この杖はワイズマンの作品であると同時に、故ワイズマンその人となったのだ。",
			},
			{// 2ページ目
				"「なぜ、私に招待状を」",
				"『長年生きていると、たまには話し相手が欲しくなるものさ』",
				"\"ワイズマンの招待状は、若い見習い魔法使いの元に現れる\"",
				"これを祝福や啓示と捉える者もいる。試練と捉える者、あるいは侮蔑と捉える者……",
				"私は未だ、この招待状をどう捉えればいいのか分からずにいた。今もなお。",
				"『知っているだろうが、招待状の期限はあまり長くはない。君のものは、そろそろ……』",
				"道中と、塔を登りきるまでの時間。そして招待状の期限。塔に赴く者は実の所、多くない。",
			},
			{// 3ページ目
				"それでも私はここへ来た。",
				"「なぜ、こんな事を続けるのですか」",
				"ワイズマンは、少し間を空けてから答えた。",
				"『……そうだな、簡潔に言うなら――』",
				"招待状が光りはじめた。期限切れだ。",
				"『――幸せになってほしいから、といった所かな』",
			},
			{// 4ページ目
				"招待状の光に包まれ、気付くと私は塔の外に居た。",
				"もう招待状は無かった。私はため息をつくと、来た道を辿って帰る事にした。",
				"家に着いたら、少し休んで、また冒険に出掛けよう。",
			}
		};
		
		// -------------------------------------
		// メソッド

		void sceneUpdate(float deltaTime) override;
		void draw(float deltaTime);

		SEQUENCE(GameClearScene,&GameClearScene::seqInit);

		bool seqInit(float deltaTime);
		bool seqEpilogue1(float deltaTime);

		void invokeDrawLogLineIncrement();

		bool seqToTitleScene(float deltaTime);

	};

}