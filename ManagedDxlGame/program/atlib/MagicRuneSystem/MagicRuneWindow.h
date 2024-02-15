#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	class MagicRuneWindow final {

	public:

		// A,D�� currentSelectIndex_ �𑀍삷��
		void process();
		
		// �`��
		void draw();

		// �}�W�b�N���[���E�B���h�E���J��������X�C�b�`
		void switchOpenMagicRuneWindow() { isOpen_ = !isOpen_; }
		
		// �Q�b�^�[
		int32_t getCurrentSelectIndex_() { return currentSelectIndex_; }
		bool IsOpenMagicRuneWindow() { return isOpen_; }


	private:
		const tnl::Vector2i RUNE_BACKGRAPH_POSITION{ 950,60 };
		const int32_t RUNE_BACKGRAPH_OFFSET = 70;
		int32_t currentSelectIndex_ = 0;

		bool isOpen_ = false;
		
		// �w�i���[����`��
		void drawRuneBack();
		// �펞�\�����̏���
		void drawDefaultMode();
		// ���[����I�𒆂̏���
		void drawSelectedMode();
	};

}