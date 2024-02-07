#pragma once
#include <array>
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

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
			Setting,
			CloseMenu,
			ReturnToTitle,
			NONE,
		};

		e_SelectedMenuWindow process(float deltaTime);
		void draw(float deltaTime);

	private:
		const tnl::Vector2i TOP_STRINGS_POSITION{ DXE_WINDOW_WIDTH / 3,200 }; // ��ԏ�̍s�̕`��ʒu
		const int32_t STRINGS_OFFSET = 30;		// �s��
		const int32_t ITEM_SYSTEM_OFFSET = 50;  // �A�C�e���ꗗ�ƃV�X�e���I�����̊Ԃ̍s��
		const int SELECTED_COLOR = GetColor(255, 0, 0); // �I�𒆂̃J���[�̐F
		const int UN_SELECTED_COLOR = GetColor(200, 200, 200); // �I�𒆂̃J���[�̐F

		const int MENU_FONT = CreateFontToHandle(NULL,30,-1,DX_FONTTYPE_ANTIALIASING);	// �t�H���g

		// �C���x���g���ւ̎�Q��
		std::weak_ptr<Inventory> weakInventory_;

		// �A�C�e���̑I������
		std::array<std::string,6> itemStrings_ = {
			"( �󂫃X���b�g )",
			"( �󂫃X���b�g )",
			"( �󂫃X���b�g )",
			"( �󂫃X���b�g )",
			"( �󂫃X���b�g )",
			"( �󂫃X���b�g )"
		};

		// �V�X�e���n�̑I����
		std::array<std::string,3> systemStrings_ = {
			"�ݒ�",
			"���j���[�����",
			"�^�C�g���ɖ߂�",
		};

		// ���I������
		int32_t totalStrings = static_cast<int32_t>(itemStrings_.size() + systemStrings_.size());

		int32_t currentSelectIndex_ = 0;
	};

}