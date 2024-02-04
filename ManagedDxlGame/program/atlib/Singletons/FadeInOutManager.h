#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class FadeInOutManager final {
	//------------------------------------------
	// �V���O���g���݌v
	public:
		static FadeInOutManager* getFadeInOutManager();
		static void deleteFadeInOutManager();
	private:
		static FadeInOutManager* p_instance_;
	//------------------------------------------
	public:
		// ���݁A�t�F�[�h�����ۂ�
		bool isFading();
		
		// �t�F�[�h�C�� �E �A�E�g�p�̍��F�Z�`�̕`�� ( �펞�`�悷�� )
		void drawFadeBlackRect(float deltaTime);

		// �t�F�[�h�C�� ( �����Ɍ����� )
		inline void startFadeIn() { currentFadeState_ = e_FadeState::FADE_IN; }
		// �t�F�[�h�A�E�g ( �^�����Ɍ����� )
		inline void startFadeOut() { currentFadeState_ = e_FadeState::FADE_OUT; }

		// �t�F�[�h�p���F�Z�`�̓����x�𒼐ڎw�� 0 ... ���� , 255 ... �^����
		// �V�[���̍ŏ��͓����������A�Ȃǂ��w�肷�鎞��
		inline void setFadeAlphaValue(int32_t alphaValue) { fadeAlphaValue_ = alphaValue; }

	private:
		// -----------------------------------
		// �����o�ϐ�
		int32_t fadeAlphaValue_ = 255; // 0 ... �����A 255 ... �^����
		const int32_t FADE_SPEED = 3;
		enum class e_FadeState {
			FADE_NONE, // �t�F�[�h���łȂ�
			FADE_IN, // �t�F�[�h�C���� ( �����Ɍ������Ă��� )
			FADE_OUT // �t�F�[�h�A�E�g�� ( �^�����Ɍ������Ă��� )
		}
		currentFadeState_ = e_FadeState::FADE_NONE;

		// -----------------------------------
		// ���\�b�h
		
		// �t�F�[�h�C�� �E �A�E�g�p���F�Z�a�̓����x�����X�ɕω�
		void fadeBlackRect();
	};

}