#include "../../dxlib_ext/dxlib_ext.h"
#include "MenuWindow.h"
#include "../Object/Inventory.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	MenuWindow::~MenuWindow() {
		DeleteFontToHandle(MENU_FONT);
		DeleteFontToHandle(DESC_FONT);
	}

	MenuWindow::MenuWindow(std::weak_ptr<Inventory> inventory) 
		: weakInventory_(inventory) {
	}

	MenuWindow::e_SelectedMenuWindow MenuWindow::process(float deltaTime) {
		// ���W�����[���Z�ŁA�I�����ŏ㕔�ƍŉ��������[�v����悤�ɂ��Ă���
		// W �L�[����������
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
			--currentSelectIndex_;
			currentSelectIndex_ = (currentSelectIndex_ + totalStrings) % totalStrings;
		}
		// S�L�[����������
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
			++currentSelectIndex_;
			currentSelectIndex_ = (currentSelectIndex_ + totalStrings) % totalStrings;
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
			for (int i = 0; i < itemWindows_.size(); ++i) {

				// ���ݑI�𒆂̑I�����͋�������
				if (currentSelectIndex_ == i) {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), SELECTED_COLOR, MENU_FONT, itemWindows_.at(i).first.c_str());
				}
				else {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), UN_SELECTED_COLOR, MENU_FONT, itemWindows_.at(i).first.c_str());
				}
				actualyPos.y += STRINGS_OFFSET;
			}

			// �V�X�e��������̈ʒu�ɕ`��ʒu���ړ�
			actualyPos.y += ITEM_SYSTEM_OFFSET;

			// �V�X�e��������̕`��
			for (int i = 0; i < systemWindows_.size(); ++i) {
				// �C���f�b�N�X�ʒu����
				if (currentSelectIndex_ == i + itemWindows_.size()) {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), SELECTED_COLOR, MENU_FONT, systemWindows_.at(i).first.c_str());
				}
				else {
					DrawStringToHandleEx(static_cast<float>(actualyPos.x), static_cast<float>(actualyPos.y), UN_SELECTED_COLOR, MENU_FONT, systemWindows_.at(i).first.c_str());
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
			auto& descString = getDescription(currentSelectIndex_);
			DrawStringToHandleEx(static_cast<float>(DESC_STRING_POSITION.x), static_cast<float>(DESC_STRING_POSITION.y), -1, DESC_FONT, descString.c_str());
		}
	}

	void MenuWindow::itemWindowsUpdate() {
		auto inv = weakInventory_.lock();
		for (int i = 0; i < itemWindows_.size(); ++i) {
			auto& item = inv->getItem(i);
			// �A�C�e������������
			if (item) {
				// �A�C�e���̖��O�Ɛ��������Z�b�g
				itemWindows_.at(i).first = item->getItemName();
				itemWindows_.at(i).second = item->getItemDescString();
			}
			// �A�C�e�����Ȃ�������
			else {
				// �f�t�H�̖��O�Ɛ��������Z�b�g
				itemWindows_.at(i).first = "( �󂫃X���b�g )";
				itemWindows_.at(i).second = "�A�C�e��������";
			}
		}
	}

	const std::string& MenuWindow::getDescription(int currentSelectIndex) const {
		// �A�C�e���ꗗ���w���Ă���ꍇ
		if (0 <= currentSelectIndex_ && currentSelectIndex_ < itemWindows_.size()) {
			// �A�C�e���ꗗ�̐�������Ԃ�
			return itemWindows_.at(currentSelectIndex).second;
		}
		else {// �V�X�e���ꗗ���w���Ă���ꍇ
			// �V�X�e���ꗗ�̐�������Ԃ�
			// �A�C�e���̉��ɃV�X�e���̃{�^��������
			return systemWindows_.at(currentSelectIndex_ - itemWindows_.size()).second;
		}
	}
}