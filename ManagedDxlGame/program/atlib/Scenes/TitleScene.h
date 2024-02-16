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
		// �ϐ�

		// �w�i�֘A
		const tnl::Vector2i LOGO_POSITION{ 330,150 };		// �^�C�g�����S�̒��S�ʒu
		const float LOGO_DEFAULT_ANGLE = -10.0f;			// �^�C�g�����S�̉�] ( �x���@ )
		const float LOGO_SIZE = 0.7f;						// �^�C�g�����S�̃T�C�Y ( 1 �ŉ摜���T�C�Y )
		
		// UI�֘A
		
		// �{�^����ɕ\�����镶���p�t�H���g
		const int BUTTON_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		// ���ݑI�𒆂̃{�^����\������ enum
		enum class e_SelectTitleButton {
			START,
			PROROGUE,
			EXIT,
			BUTTON_MAX
		}
		currentSelectButton_ = e_SelectTitleButton::START;

		const tnl::Vector2i BUTTON_POSITION{ 1000,400 };	// ��ԏ�̃{�^���̒��S�ʒu
		const tnl::Vector2i BUTTON_OFFSET{ 0,100 };			// ��ԏ�̃{�^������I�t�Z�b�g������
		const tnl::Vector2i BUTTON_SELECTED_OFFSET{ -50,0 };// �I�𒆂̃{�^����ǉ��ŃI�t�Z�b�g�����
		const tnl::Vector2i BUTTON_STRING_OFFSET{ -60,-15 };	// �{�^���ɕ`�悳���
		const int32_t BUTTON_STRING_FONTSIZE = 30;			// �{�^���̃t�H���g�T�C�Y
		const tnl::Vector2i BUTTON_CURSOR_OFFSET{ -300,0 };	// �J�[�\���ƃ{�^���̋���
		const tnl::Vector2i CURSOR_KEYBOARD_WS_OFFSET{ 0,80 };	// �J�[�\���ƃL�[�{�[�h�A�C�R���̋���
		const tnl::Vector2i CURSOR_KEYBOARD_SPACE_OFFSET{ -100,0 };	// �J�[�\���ƃL�[�{�[�h�A�C�R���̋���
		const float BUTTON_CURSOR_SIZE = 0.15f;						// �J�[�\���̃T�C�Y ( 1 �ŉ摜���T�C�Y )
		const float KEYBOARD_WS_SIZE = 0.5;						// �L�[�{�[�h�A�C�R���̃T�C�Y ( 1 �ŉ摜���T�C�Y )
		const float BUTTON_SIZE = 0.5f;						// �{�^���̃T�C�Y ( 1 �ŉ摜���T�C�Y )
		const tnl::Vector2i SPACE_IS_ENTER_POSITION{ 800,200 };	// ������̔w�i�ɂȂ�{�b�N�X�̈ʒu
		const tnl::Vector2i SPACE_IS_ENTER_BACK_LEFTUP_POINT{ 780,180 };	// ������̔w�i�ɂȂ�{�b�N�X�̈ʒu
		const tnl::Vector2i SPACE_IS_ENTER_BACK_RIGHTDOWN_POINT{ 1160,250 };	// ������̔w�i�ɂȂ�{�b�N�X�̑傫��

		bool isDisplayButton = false;	// �{�^����\�����邩�ۂ��̃t���O
		bool isGameEnd = false;			// �Q�[���I����Ԃ��̃t���O

		// �v�����[�O�֘A
		const int PROROGUE_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		std::vector<std::string> prorogueText{};
		const int32_t PROROGUE_BACKGROUND_BRIGHT = 126;	// �v�����[�O���̔w�i�̖��邳 ( 0 �Ō��Ɠ����A255 �Ő^���� )
		const tnl::Vector2i TEXT_POSITION{ 25,250 };	// ��ԏ�̍s�̕\���ʒu
		const tnl::Vector2i TEXT_OFFSET{ 5,40 };		// ��s���ƂɃI�t�Z�b�g������
		const int32_t PROROGUE_FIRST_LOG = 2;			// ��ԍŏ��ɕ\�������s��
		const float LOG_LINE_INTERVAL = 5.0f;			// �\���s�𑝂₷�Ԋu ( �b )
		int32_t drawLogLine_ = 0;
		float totalDeltaTime_ = 0.0f;	// �ݐώ���

		// ----------------------------------
		// ���\�b�h

		// ���t���[���Ăяo��
		void sceneUpdate(float deltaTime) override;

		// �`����܂Ƃ߂�
		void draw(float deltaTime);
		// �w�i�`��
		void drawBackground(float deltaTime);
		// �uSPACE �� ����v�̕������`��
		void drawSpaceIsEnter();
		// �{�^���Q��`��
		void drawButton(float deltaTime);
		
		// �V�[�P���X�p
		SEQUENCE(TitleScene, &TitleScene::seqInit);

		// �����ݒ�
		bool seqInit(float deltaTime);

		// �����f�[�^�̃{�����[���ݒ�
		void volumeSetting();

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