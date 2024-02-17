#include "MagicRuneSystemManager.h"

namespace atl {

	MagicRuneSystemManager* MagicRuneSystemManager::getMagicRuneSystemManager() {
		static MagicRuneSystemManager* p_instance_ = nullptr;

		if (!p_instance_) {
			p_instance_ = new MagicRuneSystemManager();
		}
		return p_instance_;
	}

	bool MagicRuneSystemManager::equipRune(Shared<Base_MagicRune> newRune, DungeonScene& dungeonScene) {
		// もし装備可能最大数を超えていなければ、追加
		if (equipmentMagicRunes_.size() < MAX_EQUIPMENT_RUNE) {
			equipmentMagicRunes_.emplace_back(newRune);
			newRune->onEquipMomentNotify(dungeonScene);
			return true;
		}
		else {
			return false;
		}
	}

	void MagicRuneSystemManager::removeRune(int index, DungeonScene& dungeonScene) {
		// 範囲外アクセスチェック、早期リターン
		if (index < 0 || index >= equipmentMagicRunes_.size()) return;

		equipmentMagicRunes_[index]->onRemoveMomentNotify(dungeonScene);
		equipmentMagicRunes_.erase(equipmentMagicRunes_.begin() + index);
	}

	void MagicRuneSystemManager::notifyOnEvent(e_EventType eventType, DungeonScene& dungeonScene) {
		for (auto& rune : equipmentMagicRunes_) {
			rune->onNotify(eventType, dungeonScene);
		}
	}
}