#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// ����ɑ΂��A�I�����̂���E�B���h�E�p UI
	/// �I�����́u�͂��v�u�������v�̂�
	/// 
	/// </summary>

	class SelectWindow final {
	public:
		SelectWindow();
		~SelectWindow();

		// �I�𒆂̑I������\��
		enum class e_SelectChoice {	NONE,YES,NO };

		// ���╶��UI��`�悷��
		void draw(float deltaTime);
		
		// �Z���N�g�E�B���h�E�̎��╶��ݒ肵�A����炷
		// arg ... ���╶
		void setSelectWindowQuestionText(const std::string& questionText);

		// ���N���b�N�����������A�I�𒆂� e_SelectChoice ��Ԃ��B( YES or NO )
		// �E�N���b�N�������ƁANo��I�����������ɂȂ�
		// �Ăяo�����ŁAe_SelectChoice �� == �Ŕ�r���鎖�ŁA�I�����ɉ����ď�������o����
		// �������͂��������� NONE ���Ԃ��Ă��Ă���̂ŁA�펞�Ăяo���Ă��A���͂��ꂽ�u�Ԃ������o�ł���
		// ���� �F else �ŏ������򂵂悤�Ƃ���ƁA���͂���������NONE���E���ď������򂵂Ă��܂��܂�
		// �g�p��F // ���t���[���Ăяo����鏊��
		// if (selectWindow_.windowChoice() == e_SelectChoice::YES) { // �C�G�X��I�񂾎��̏��� }
		// else if (selectWindow_.windowChoice() == e_SelectChoice::NO) { // �m�[��I�񂾎��̏��� }
		const e_SelectChoice windowChoice();

	private:
		// �����UI�ʒu
		const tnl::Vector2i QUESTION_UI_POSITION{ DXE_WINDOW_WIDTH/2,200 };	
		// �����UI�T�C�Y ( 1 �ŉ摜���T�C�Y )
		const float QUESTION_UI_SIZE = 0.6f;	
		// �u�͂��v�̔w�iUI�ʒu
		const tnl::Vector2i YES_UI_POSITION{ DXE_WINDOW_WIDTH / 4,400 };	
		// �u�������v�̔w�iUI�ʒu
		const tnl::Vector2i NO_UI_POSITION{ DXE_WINDOW_WIDTH - DXE_WINDOW_WIDTH / 4,400 };	
		// �͂��A�̕�����\������ʒu
		const tnl::Vector2i YES_STRING_POSITION{300,390};
		// �������A�̕�����\������ʒu
		const tnl::Vector2i NO_STRING_POSITION{935,390};
		// A�L�[��UI��\������ʒu
		const tnl::Vector2i KEYBOARD_A_UI_POSITION{ YES_UI_POSITION.x,YES_UI_POSITION.y + 50 };
		// D�L�[��UI��\������ʒu
		const tnl::Vector2i KEYBOARD_D_UI_POSITION{ NO_UI_POSITION.x + 10,NO_UI_POSITION.y + 50 };
		// YES�ENO ��UI�T�C�Y ( 1 �ŉ摜���T�C�Y )
		const float YES_NO_UI_SIZE = 0.4f;
		// A,D�̃L�[�{�[�hUI�T�C�Y ( 1�ŉ摜���T�C�Y )
		const float KEYBOARD_UI_SIZE = 0.5f;

		// �t�H���g
		const int QUESTION_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
		const int YES_NO_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		// ���ݑI�𒆂̑I���� ( YES or NO )
		e_SelectChoice currentSelectedChoice_ = e_SelectChoice::YES;
		// ���╶
		std::string questionText_ = "";
	};
}