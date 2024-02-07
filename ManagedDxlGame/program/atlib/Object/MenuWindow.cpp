#include "../dxlib_ext/dxlib_ext.h"
#include "MenuWindow.h"
#include "../Object/Inventory.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	MenuWindow::~MenuWindow() {
		DeleteFontToHandle(MENU_FONT);
	}

	MenuWindow::MenuWindow(std::weak_ptr<Inventory> inventory) : weakInventory_(inventory) {
		auto inv = weakInventory_.lock();

		for (int i = 0; i < inv->getItemList().size(); ++i) {
			auto& itemData = inv->getItemData(i);
			if (itemData) {
				itemStrings_.at(i) = itemData->getItemName();
				itemDesc_.at(i) = itemData->getItemDescString();
			}
		}
	}

	MenuWindow::e_SelectedMenuWindow MenuWindow::process(float deltaTime) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
			// �ŏ㕔��I�𒆂̏ꍇ�́A��ɍs���Ȃ�
			if (currentSelectIndex_ == 0) return e_SelectedMenuWindow::NONE;
			--currentSelectIndex_;
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
			// �ŉ�����I�𒆂́A���ɍs���Ȃ�
			if (currentSelectIndex_ == totalStrings - 1) return e_SelectedMenuWindow::NONE;
			++currentSelectIndex_;
		}

		// �X�y�[�X or �G���^�[ or ���N���b�N�ŁAcurrentSelectIndex ��Ԃ�
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE,eKeys::KB_RETURN) ||
			tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
			return static_cast<e_SelectedMenuWindow>(currentSelectIndex_);
		}

		return e_SelectedMenuWindow::NONE;
	}

	void MenuWindow::draw(float deltaTime) {
		{//�@�I������`��B�I�𒆂̑I�����͋����\��
			tnl::Vector2i actualyPos = TOP_STRINGS_POSITION;

			// �A�C�e�����X�g�̕`��
			for (int i = 0; i < itemStrings_.size(); ++i) {

				// ���ݑI�𒆂̑I�����͋�������
				if (currentSelectIndex_ == i) {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), SELECTED_COLOR, MENU_FONT, itemStrings_.at(i).c_str());
				}
				else {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), UN_SELECTED_COLOR, MENU_FONT, itemStrings_.at(i).c_str());
				}
				actualyPos.y += STRINGS_OFFSET;
			}

			actualyPos.y += ITEM_SYSTEM_OFFSET; // �V�X�e��������̈ʒu�ɕ`��ʒu���ړ�

			// �V�X�e��������̕`��
			for (int i = 0; i < systemStrings_.size(); ++i) {
				// �C���f�b�N�X�ʒu����
				if (currentSelectIndex_ == i + itemStrings_.size()) {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), SELECTED_COLOR, MENU_FONT, systemStrings_.at(i).c_str());
				}
				else {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), UN_SELECTED_COLOR, MENU_FONT, systemStrings_.at(i).c_str());
				}
				actualyPos.y += STRINGS_OFFSET;
			}
		}

		{// ���������L�q���� UI ��`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,DESC_UI_ALPHA);
			DrawBox(DESC_UI_POSITION.x, DESC_UI_POSITION.y, DESC_UI_POSITION.x+ DESC_UI_SIZE.x, DESC_UI_POSITION.y + DESC_UI_SIZE.y, GetColor(0, 0, 255), true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		}

		{// ��������`��
			auto descString = getDescription(currentSelectIndex_);
			DrawStringToHandleEx(static_cast<float>(DESC_STRING_POSITION.x), static_cast<float>(DESC_STRING_POSITION.y), -1, DESC_FONT, descString.c_str());
		}
	}

	std::string MenuWindow::getDescription(int currentSelectIndex) const {
		// �A�C�e���ꗗ���w���Ă���ꍇ
		if (0 <= currentSelectIndex_ && currentSelectIndex_ < itemStrings_.size()) {
			return itemDesc_[currentSelectIndex];
		}

		// �V�X�e���ꗗ���w���Ă���ꍇ
		switch (currentSelectIndex_) {
		case static_cast<int>(e_SelectedMenuWindow::Setting):
			return "�e��ݒ�";
		case static_cast<int>(e_SelectedMenuWindow::ReturnToTitle):
			return "�^�C�g����ʂɖ߂�B�i���ۑ��͂���܂���";
		case static_cast<int>(e_SelectedMenuWindow::CloseMenu):
			return "���j���[�����B�E�N���b�N�ł�����";
		default:
			return "";
		}
	}
}