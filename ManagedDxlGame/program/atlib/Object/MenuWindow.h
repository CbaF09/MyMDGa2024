#pragma once
#include <array>
#include <string>
#include <utility>
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// �_���W�����V�[���ŉE�N���b�N�������ɊJ���郁�j���[UI
	/// 
	/// </summary>

	class Inventory;

	class MenuWindow final {
	public:
		~MenuWindow();

		explicit MenuWindow(std::weak_ptr<Inventory> inventory);

		enum class e_SelectedMenuWindow {
			Item1,
			Item2,
			Item3,
			Item4,
			Item5,
			Item6,
			EraseMagicRune,
			CloseMenu,
			ReturnToTitle,
			NONE,
		};

		// ���͂ɉ����đI������ς���B���t���[���Ă�
		e_SelectedMenuWindow process(float deltaTime);
		// �I�����̋����\���ȂǁB���t���[���Ă�
		void draw(float deltaTime);
		// ���j���[���J�������ɂ̂݌ĂԁB�A�C�e���ꗗ��string���X�V����
		void itemWindowsUpdate();


	private:
		const int MENU_FONT = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);	// �t�H���g
		const int DESC_FONT = CreateFontToHandle(NULL, 18, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		const tnl::Vector2i TOP_STRINGS_POSITION{ 50,130 }; // ��ԏ�̍s�̕`��ʒu
		const int32_t STRINGS_OFFSET = 40;		// �s��
		const int32_t ITEM_SYSTEM_OFFSET = 90;  // �A�C�e���ꗗ�ƃV�X�e���I�����̊Ԃ̍s��
		const int SELECTED_COLOR = GetColor(255, 0, 0); // �I�𒆂̃J���[�̐F
		const int UN_SELECTED_COLOR = GetColor(200, 200, 200); // �I�𒆂̃J���[�̐F


		const tnl::Vector2i DESC_UI_POSITION{ 70,380 }; // ���������L�q���� UI �̈ʒu ( ������W )
		const tnl::Vector2i DESC_UI_SIZE{ 420,60 }; // ���������L�q���� UI �̃T�C�Y
		const int32_t DESC_UI_ALPHA = 128; // ���������L�q���� UI �̓����x
		const tnl::Vector2i DESC_STRING_POSITION{ 80,400 }; // ���������L�q����ʒu

		// �C���x���g���ւ̎�Q��
		std::weak_ptr<Inventory> weakInventory_;

		// �A�C�e���̑I�����Ɛ������Bpair �ł܂Ƃ߂Ă���B first->�I�������Bsecond->������
		std::array<std::pair<std::string, std::string>, 6> itemWindows_{};

		// �V�X�e���n�̑I�����Ɛ������Bpair �ł܂Ƃ߂Ă���B first->�I�������Bsecond->������
		std::array<std::pair<std::string, std::string>, 3> systemWindows_{
			std::pair{("���[���̊m�F�E�폜"),("�������̃��[�����m�F�E�폜���܂�")},
			std::pair{("���j���[�����"),("���j���[�����")},
			std::pair{("�^�C�g�����"),("�^�C�g����ʂɖ߂�")},
		};

		// ���I������
		int32_t totalStrings = static_cast<int32_t>(itemWindows_.size() + systemWindows_.size());

		// ���ݑI�𒆂̂���
		int32_t currentSelectIndex_ = 0;

		const std::string& getDescription(int currentSelectIndex) const;

	};

}

