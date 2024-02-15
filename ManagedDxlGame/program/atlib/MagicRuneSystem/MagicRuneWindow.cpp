#include "MagicRuneWindow.h"
#include "MagicRuneSystemManager.h"
#include "../Singletons/ResourceManager.h"

namespace atl {
	
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
		drawRuneBack();

		if (!isOpen_) {
			drawDefaultMode();
		}
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
		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		for (int i = 0; i < runes.size(); ++i) {
			if (i != currentSelectIndex_) { // �I�𒆂łȂ����͈̂Â��`��
				SetDrawBright(128, 128, 128);
			}
			auto runeGraph = runes[i]->getRuneGraph();
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeGraph, true);
			if (i != currentSelectIndex_) {
				SetDrawBright(255, 255, 255);
			}
		}
	}

}