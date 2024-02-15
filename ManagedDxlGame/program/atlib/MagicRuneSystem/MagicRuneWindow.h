#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	class MagicRuneWindow final {

	public:

		// A,D�� currentSelectIndex_ �𑀍삷��
		void process();
		
		// �E�B���h�E��`��
		void draw();

		// �}�W�b�N���[���E�B���h�E���J��������X�C�b�`
		void switchOpenMagicRuneWindow() { isOpen_ = !isOpen_; }
		
		// �Q�b�^�[
		int32_t getCurrentSelectIndex_() { return currentSelectIndex_; }
		bool IsOpenMagicRuneWindow() { return isOpen_; }


	private:
		// �t�H���g
		const int RUNE_NAME_FONT = CreateFontToHandle(NULL, 30, -1,DX_FONTTYPE_ANTIALIASING_EDGE);
		const int RUNE_DESC_FONT = CreateFontToHandle(NULL, 25, -1,DX_FONTTYPE_ANTIALIASING_EDGE);

		// �w�i�摜�̃|�W�V����
		const tnl::Vector2i RUNE_BACKGRAPH_POSITION{ 950,60 };
		// �w�i�摜�̌��� ( X ���� )
		const int32_t RUNE_BACKGRAPH_OFFSET = 70;
		// ���[������`�悷��ʒu
		const tnl::Vector2i RUNE_NAME_STRING_POSITION{ 200 ,250 };
		// ���[���̐�������`�悷��Y�ʒu ( X �͕�����̑傫���ɍ��킹�Ē�����������̂ŕK�v���� )
		const float RUNE_DESC_STRING_POSITION_Y = 300;
		
		
		int32_t currentSelectIndex_ = 0;
		bool isOpen_ = false;
		
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