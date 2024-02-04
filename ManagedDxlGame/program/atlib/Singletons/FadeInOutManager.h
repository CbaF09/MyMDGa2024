#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class FadeInOutManager final {
	//------------------------------------------
	// シングルトン設計
	public:
		static FadeInOutManager* getFadeInOutManager();
		static void deleteFadeInOutManager();
	private:
		static FadeInOutManager* p_instance_;
	//------------------------------------------
	public:
		// 現在、フェード中か否か
		bool isFading();
		
		// フェードイン ・ アウト用の黒色短形の描画 ( 常時描画する )
		void drawFadeBlackRect(float deltaTime);

		// フェードイン ( 透明に向かう )
		inline void startFadeIn() { currentFadeState_ = e_FadeState::FADE_IN; }
		// フェードアウト ( 真っ黒に向かう )
		inline void startFadeOut() { currentFadeState_ = e_FadeState::FADE_OUT; }

		// フェード用黒色短形の透明度を直接指定 0 ... 透明 , 255 ... 真っ黒
		// シーンの最初は透明か黒か、などを指定する時に
		inline void setFadeAlphaValue(int32_t alphaValue) { fadeAlphaValue_ = alphaValue; }

	private:
		// -----------------------------------
		// メンバ変数
		int32_t fadeAlphaValue_ = 255; // 0 ... 透明、 255 ... 真っ黒
		const int32_t FADE_SPEED = 3;
		enum class e_FadeState {
			FADE_NONE, // フェード中でない
			FADE_IN, // フェードイン中 ( 透明に向かっている )
			FADE_OUT // フェードアウト中 ( 真っ黒に向かっている )
		}
		currentFadeState_ = e_FadeState::FADE_NONE;

		// -----------------------------------
		// メソッド
		
		// フェードイン ・ アウト用黒色短径の透明度を徐々に変化
		void fadeBlackRect();
	};

}