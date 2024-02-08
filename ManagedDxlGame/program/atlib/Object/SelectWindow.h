#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	// �I�����̂��� 2D UI�p
	// �Ή����Ă���I�����͓�̂�
	class SelectWindow {
	public:
		// �R���X�g���N�^�ŕ����i�K�i��o��܂����H���j���w�肷��
		SelectWindow(std::string questionText);

		~SelectWindow();

		// �I�𒆂̑I������\��
		enum class e_SelectChoice {	NONE,YES,NO };

		// ���╶����UI��`�悷��
		void draw(float deltaTime);
		
		// �G���^�[ or �X�y�[�X�����������AcurrentSelectedChoice == YES �Ȃ� true ��Ԃ��BNO �Ȃ� false ��Ԃ��B
		const e_SelectChoice& windowChoice();

	private:
		e_SelectChoice currentSelectedChoice_ = e_SelectChoice::YES;
		std::string questionText_;

		const tnl::Vector2i QUESTION_UI_POSITION{ DXE_WINDOW_WIDTH/2,200 };	// �����UI�ʒu
		const float QUESTION_UI_SIZE = 0.6f;									// �����UI�T�C�Y ( 1 �ŉ摜���T�C�Y )
		const tnl::Vector2i YES_UI_POSITION{ DXE_WINDOW_WIDTH / 4,400 };	// YES �̑I�����̈ʒu
		const tnl::Vector2i NO_UI_POSITION{ DXE_WINDOW_WIDTH - DXE_WINDOW_WIDTH / 4,400 };	// NO �̑I�����̈ʒu
		const tnl::Vector2i YES_STRING_POSITION{300,390};	// �͂��A�̕�����\������ʒu
		const tnl::Vector2i NO_STRING_POSITION{935,390};	// �������A�̕�����\������ʒu
		const float YES_NO_UI_SIZE = 0.4f;									// YES�ENO ��UI�T�C�Y ( 1 �ŉ摜���T�C�Y )

		const int QUESTION_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);
		const int YES_NO_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

	};


}