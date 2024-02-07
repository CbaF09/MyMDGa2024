#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	class TitleScene final : public atl::Base_Scene {
	public:
		~TitleScene();

	private:

		// ----------------------------------
		// �ϐ�

		// �w�i�֘A
		const tnl::Vector2i LOGO_POSITION{ 330,150 };		// �^�C�g�����S�̒��S�ʒu
		const float LOGO_DEFAULT_ANGLE = -10.0f;			// �^�C�g�����S�̉�] ( �x���@ )
		const float LOGO_SIZE = 0.7f;						// �^�C�g�����S�̃T�C�Y ( 1 �ŉ摜���T�C�Y )
		
		// UI�֘A
		
		const int BUTTON_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);

		enum class e_SelectTitleButton {
			START,
			PROROGUE,
			EXIT,
			BUTTON_MAX
		} // ���ݑI�𒆂̃{�^����\������ enum
		currentSelectButton_ = e_SelectTitleButton::START;
		const tnl::Vector2i BUTTON_POSITION{ 1000,400 };	// ��ԏ�̃{�^���̒��S�ʒu
		const tnl::Vector2i BUTTON_OFFSET{ 0,100 };			// ��ԏ�̃{�^������I�t�Z�b�g������
		const tnl::Vector2i BUTTON_SELECTED_OFFSET{ -50,0 };// �I�𒆂̃{�^����ǉ��ŃI�t�Z�b�g�����
		const tnl::Vector2i BUTTON_STRING_OFFSET{ -60,-15 };	// �{�^���ɕ`�悳���
		const int32_t BUTTON_STRING_FONTSIZE = 30;			// �{�^���̃t�H���g�T�C�Y
		const float BUTTON_SIZE = 0.5f;						// �{�^���̃T�C�Y ( 1 �ŉ摜���T�C�Y )
		bool isDisplayButton = false;		// �{�^����\�����邩�ۂ��̃t���O

		// �v�����[�O�֘A
		const int PROROGUE_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

		const std::vector<std::string> prorogueText = {
			"(�G���^�[�L�[���X�y�[�X�L�[�Ŗ߂�)",
			"",
			"�\�\���C�Y�}���ƌĂ΂��喂�@�g���������B",
			"�ނ��Z��ł������ɂ́A���͂Ȍ��E���\���Ă���A",
			"\"���ҏ�\" ���������҂łȂ���΁A���̓��ɗ������鎖�͏o���Ȃ��B",
			"�s�v�c�Ȏ��ɁA���� \"���ҏ�\" �́A���K�����@�g���̌��ɁA���R�ƌ����B",
			"�����A����\"���ҏ�\" �����߂Ȃ���A���x�����̈Ӗ����l���Ă����B",
			"���C�Y�}���̏C�����\�\",
			"�����炩�����Ă΂��悤�ɂȂ������̓��ɁA���͂悤�₭�H�蒅�����B",
			"(�G���^�[�L�[���X�y�[�X�L�[�Ŗ߂�)",
		};
		const int32_t PROROGUE_BACKGROUND_BRIGHT = 126;	// �v�����[�O���̔w�i�̖��邳 ( 0 �Ō��Ɠ����A255 �Ő^���� )
		const tnl::Vector2i TEXT_POSITION{ 25,250 };	// ��ԏ�̍s�̕\���ʒu
		const tnl::Vector2i TEXT_OFFSET{ 5,40 };		// ��s���ƂɃI�t�Z�b�g������
		const int32_t PROROGUE_FIRST_LOG = 3;			// ��ԍŏ��ɕ\�������s��
		const float LOG_LINE_INTERVAL = 5.0f;			// �\���s�𑝂₷�Ԋu ( �b )
		int32_t drawLogLine_ = 0;
		float totalDeltaTime_ = 0.0f;	// �ݐώ���

		// ----------------------------------
		// ���\�b�h

		void sceneUpdate(float deltaTime) override;

		// �`����܂Ƃ߂�
		void draw(float deltaTime);
		void drawBackground(float deltaTime);
		// �{�^���Q��`��
		void drawButton(float deltaTime);
		
		// �V�[�P���X�p
		SEQUENCE(TitleScene, &TitleScene::seqInit);

		// �����ݒ�
		bool seqInit(float deltaTime);

		// �L�[���͑ҋ@
		bool seqKeyInputWait(float deltaTime);
		// �X�^�[�g�{�^�������������̏���
		bool seqStart(float deltaTime);
		// �v�����[�O�{�^�������������̏���
		bool seqProrogue(float deltaTime);
		// �Q�[���I���{�^�������������̏���
		bool seqExit(float deltaTime);

		// �I�𒆂̃{�^��������
		bool selectButtonUp();
		// �I�𒆂̃{�^���������
		bool selectButtonDown();

		// �f�o�b�O�p
		void debug_keyInput();
	};

}