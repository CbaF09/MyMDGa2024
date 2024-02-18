#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// �������� MagicRune �� ��ʂɕ`�悵����A�I�������肷�鎞��UI�p�N���X
	/// 
	/// </summary>

	class MagicRuneWindow final {

	public:
		~MagicRuneWindow();

		// A,D�� currentSelectIndex_ �𑀍삷��
		void process();

		// �E�B���h�E��`��
		// isOpen�̏�Ԃɉ����āA�ǂ��`�悳��邩�ς��
		void draw();

		// ���[���I����ʂ��J��������X�C�b�`
		void switchOpenMagicRuneWindow() { isOpen_ = !isOpen_; }

		// �Q�b�^�[ ( ���ݑI�𒆂̃��[�� )
		int32_t getCurrentSelectRune_() { return currentSelectIndex_; }
		// �Q�b�^�[ ( ���݁A���[���I����ʂ��ǂ��� )
		bool IsOpenMagicRuneWindow() { return isOpen_; }

		// ���ݑI�𒆂̃��[���p�C���f�b�N�X�� 0 �ɐݒ肷��
		void resetIndex() { currentSelectIndex_ = 0; }

	private:
		// �t�H���g
		const int RUNE_NAME_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
		const int RUNE_DESC_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		// �w�i�摜�̈ʒu
		const tnl::Vector2i RUNE_BACKGRAPH_POSITION{ 950,60 };
		// �w�i�摜�̌��� ( X ���� )
		const int32_t RUNE_BACKGRAPH_OFFSET = 70;
		// �w�i�摜�̔w�i�̃I�[�o�[���C ( �ȉ~ ) �̈ʒu
		const tnl::Vector2i RUNE_BACK_OVERLAY_POSITION{ 1090,60 };
		// �I�[�o�[���C ( �ȉ~ ) �̑傫��
		const tnl::Vector2i RUNE_BACK_OVERLAY_SIZE{ 200,50 };
		// �I�[�o�[���C ( �ȉ~ ) �̓����x
		const float RUNE_BACK_OVERLAY_ALPHA = 200;
		// ���[������`�悷��ʒu
		const tnl::Vector2i RUNE_NAME_STRING_POSITION{ 200 ,250 };
		// ���[���̐�������`�悷��Y�ʒu ( X �͕�����̑傫���ɍ��킹�Ē�����������̂ŕK�v���� )
		const float RUNE_DESC_STRING_POSITION_Y = 300;
		// ���[����I�𒆂̔w�i���Â�����I�[�o�[���C�̓����x
		const int32_t RUNE_OVERLAY_ALPHA = 128;
		// ���[����I��

		// ���ݑI�𒆂̃��[��
		int32_t currentSelectIndex_ = 0;
		// ���݁A�E�B���h�E���J����Ă��邩
		bool isOpen_ = false;

		// draw �֐��̃w���p�[�֐��Q
		// �w�i���[����`��
		void drawRuneBack();
		// �펞�\�����̏���
		void drawDefaultMode();
		// ���[����I�𒆂̏���
		void drawSelectedMode();
		// ���[����I�𒆂̒ǉ��`�� ( ���[���̌��ʐ��� )
		void drawSelectedModeRuneDesc();
	};

}