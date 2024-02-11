#include "FadeInOutManager.h"

namespace atl {

    FadeInOutManager* FadeInOutManager::getFadeInOutManager() {
        static FadeInOutManager* p_instance_ = nullptr;

        if (!p_instance_) p_instance_ = new FadeInOutManager();
        return p_instance_;
    }

    bool FadeInOutManager::isFading() {
        // ( 三項演算 ) 現在のステートが、FADE_NONE なら、Falseを返す。そうでないなら、trueを返す
        bool retV =
            currentFadeState_ == e_FadeState::FADE_NONE ? false : true;

        return retV;
    }

    void FadeInOutManager::drawFadeBlackRect(float deltaTime) {
        fadeBlackRect();
        // 完全透明の場合は描画しない
        if (fadeAlphaValue_ == 0) return;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlphaValue_);
        DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(0, 0, 0), true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    void FadeInOutManager::fadeBlackRect() {
        switch (currentFadeState_) {
            // フェード状態でないなら何もしない
        case e_FadeState::FADE_NONE: return;

            // フェードイン ( 画面を見えるようにしていく )
        case e_FadeState::FADE_IN:
            fadeAlphaValue_ -= FADE_SPEED;
            if (fadeAlphaValue_ < 0) {
                fadeAlphaValue_ = 0;
                currentFadeState_ = e_FadeState::FADE_NONE;
            }
            return;

            // フェードアウト ( 画面を真っ黒に向かわせる )
        case e_FadeState::FADE_OUT:
            fadeAlphaValue_ += FADE_SPEED;
            if (fadeAlphaValue_ > 255) {
                fadeAlphaValue_ = 255;
                currentFadeState_ = e_FadeState::FADE_NONE;
            }
            return;
        }
    }

}