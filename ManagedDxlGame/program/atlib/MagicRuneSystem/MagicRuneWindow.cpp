#include "MagicRuneWindow.h"
#include "MagicRuneSystemManager.h"
#include "../Singletons/ResourceManager.h"

namespace atl {
	
	MagicRuneWindow::~MagicRuneWindow() {
		// ���\�[�X���
		ResourceManager::getResourceManager()->deleteResource("graphics/UI/MagicRune/MagicRuneBack.png");
		ResourceManager::getResourceManager()->deleteResource("graphics/UI/MagicRune/RuneSelectWindowBack.png");
		
		// �t�H���g�n���h�����
		DeleteFontToHandle(RUNE_NAME_FONT);
		DeleteFontToHandle(RUNE_DESC_FONT);
	}

	void MagicRuneWindow::process() {
		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		// �������̃��[���������ꍇ�A�������^�[��
		if (runes.size() == 0) return;

		// A,D�ō��E�ɑI��
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_A)) {
			--currentSelectIndex_;
			if (currentSelectIndex_ < 0) {
				currentSelectIndex_ = runes.size() - 1;
			}
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_D)) {
			++currentSelectIndex_;
			currentSelectIndex_ %= runes.size();
		}
	}
	
	void MagicRuneWindow::draw() {
		// �w�i�̋󃋁[����`��
		drawRuneBack();

		// �����A���[���I����ʂłȂ��Ȃ�A�W����Ԃŕ`��
		if (!isOpen_) {
			drawDefaultMode();
		}
		// ���[���I����ʂȂ�A�I����ʏ�Ԃŕ`��
		else {
			drawSelectedMode();
		}
	}

	// �w�i�̋󃋁[����`��
	void MagicRuneWindow::drawRuneBack() {
		auto runeBack = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/MagicRuneBack.png");
		auto maxEquipment = MagicRuneSystemManager::getMagicRuneSystemManager()->getMaxEquipmentRune();

		// �󃋁[����`��
		for (int i = 0; i < maxEquipment; ++i) {
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeBack, true);
		}
	}

	// �E�B���h�E�����삳��Ă��Ȃ����̕`��
	void MagicRuneWindow::drawDefaultMode() {
		// �������Ă��郋�[���ɐݒ肵�Ă���A�C�R���摜��`��
		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		for (int i = 0; i < runes.size(); ++i) {
			auto runeGraph = runes[i]->getRuneGraph();
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeGraph, true);
		}
	}

	// �E�B���h�E�𑀍삵�Ă���Ԃ̕`��
	void MagicRuneWindow::drawSelectedMode() {
		// ��ʑS�̂𔖈Â����邽�߂̃I�[�o�[���C�`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, RUNE_OVERLAY_ALPHA); // �������ݒ�
		DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �`�惂�[�h��ʏ�ɖ߂�

		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		for (int i = 0; i < runes.size(); ++i) {
			// �I�𒆂łȂ����͈̂Â��`��
			if (i != currentSelectIndex_) { 
				SetDrawBright(128, 128, 128);
			}

			auto runeGraph = runes[i]->getRuneGraph();
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeGraph, true);
			
			// �I�𒆂łȂ����͈̂Â��`�悵�Ă����̂����ɖ߂�
			if (i != currentSelectIndex_) {
				SetDrawBright(255, 255, 255);
			}
		}
		
		// �I�𒆂̃��[���̌��ʐ�����`��
		drawSelectedModeRuneDesc();
	}

	// �I�𒆃��[���̌��ʐ���
	void MagicRuneWindow::drawSelectedModeRuneDesc() {
		// ���ʐ����p�̔w�i��`��
		DrawRotaGraph(DXE_WINDOW_WIDTH/2,DXE_WINDOW_HEIGHT/2,1,0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/RuneSelectWindowBack.png"),true);
		
		// ���[���̏����擾
		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		auto& runeName = runes[currentSelectIndex_]->getRuneName();

		// ���[���̖��O��`��
		DrawStringToHandleEx(RUNE_NAME_STRING_POSITION.x, RUNE_NAME_STRING_POSITION.y, -1, RUNE_NAME_FONT, "%s", runeName.c_str());

		// ���[���̐�������`�� ( �������� )

		// ���������ׂ̈̌v�Z
		auto& runeDesc = runes[currentSelectIndex_]->getRuneDesc();
		auto widthFix = GetDrawStringWidthToHandle(runeDesc.c_str(), strlen(runeDesc.c_str()), RUNE_DESC_FONT);
		float drawPosX = static_cast<float>(DXE_WINDOW_WIDTH / 2) - widthFix / 2.0f;
		
		DrawStringToHandleEx(drawPosX, RUNE_DESC_STRING_POSITION_Y, -1, RUNE_DESC_FONT, "%s", runeDesc.c_str());
	}

}