#include "FadeInOutManager.h"

namespace atl {

    FadeInOutManager* FadeInOutManager::getFadeInOutManager() {
        static FadeInOutManager* p_instance_ = nullptr;

        if (!p_instance_) p_instance_ = new FadeInOutManager();
        return p_instance_;
    }

    bool FadeInOutManager::isFading() {
        // ( �O�����Z ) ���݂̃X�e�[�g���AFADE_NONE �Ȃ�AFalse��Ԃ��B�����łȂ��Ȃ�Atrue��Ԃ�
        bool retV =
            currentFadeState_ == e_FadeState::FADE_NONE ? false : true;

        return retV;
    }

    void FadeInOutManager::drawFadeBlackRect(float deltaTime) {
        fadeBlackRect();
        // ���S�����̏ꍇ�͕`�悵�Ȃ�
        if (fadeAlphaValue_ == 0) return;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlphaValue_);
        DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(0, 0, 0), true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    void FadeInOutManager::fadeBlackRect() {
        switch (currentFadeState_) {
            // �t�F�[�h��ԂłȂ��Ȃ牽�����Ȃ�
        case e_FadeState::FADE_NONE: return;

            // �t�F�[�h�C�� ( ��ʂ�������悤�ɂ��Ă��� )
        case e_FadeState::FADE_IN:
            fadeAlphaValue_ -= FADE_SPEED;
            if (fadeAlphaValue_ < 0) {
                fadeAlphaValue_ = 0;
                currentFadeState_ = e_FadeState::FADE_NONE;
            }
            return;

            // �t�F�[�h�A�E�g ( ��ʂ�^�����Ɍ����킹�� )
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