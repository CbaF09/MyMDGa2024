#include "MagicRuneWindow.h"
#include "MagicRuneSystemManager.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	void MagicRuneWindow::draw() {
		auto runeBack = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/MagicRuneBack.png");
		auto maxEquipment = MagicRuneSystemManager::getMagicRuneSystemManager()->getMaxEquipmentRune();

		// ‹óƒ‹[ƒ“‚ğ•`‰æ
		for (int i = 0; i < maxEquipment; ++i) {
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeBack, true);
		}

		auto& runes = MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes();
		for (int i = 0; i < runes.size(); ++i) {
			auto runeGraph = runes[i]->getRuneGraph();
			DrawRotaGraph(RUNE_BACKGRAPH_POSITION.x + i * RUNE_BACKGRAPH_OFFSET, RUNE_BACKGRAPH_POSITION.y, 1, 0, runeGraph, true);
		}

	}

}