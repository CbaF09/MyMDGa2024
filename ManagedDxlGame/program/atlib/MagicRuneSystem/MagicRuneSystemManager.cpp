#include "MagicRuneSystemManager.h"

namespace atl {

    MagicRuneSystemManager* MagicRuneSystemManager::getMagicRuneSystemManager() {
        static MagicRuneSystemManager* p_instance_ = nullptr;

        if (!p_instance_) p_instance_ = new MagicRuneSystemManager();
        return p_instance_;
    }

    bool MagicRuneSystemManager::equipRune(Shared<Base_MagicRune> newRune) {
        if (equipmentMagicRuneVector_.size() < MAX_EQUIPMENT_RUNE) {
            equipmentMagicRuneVector_.emplace_back(newRune);
            return true;
        }
        else {
            return false;
        }
    }

    void MagicRuneSystemManager::removeRune(int index) {
        // 範囲外アクセスチェック、早期リターン
        if (index < 0  || index >= equipmentMagicRuneVector_.size()) return;

        equipmentMagicRuneVector_.erase(equipmentMagicRuneVector_.begin() + index);
    }

    void MagicRuneSystemManager::notifyOnEvent(e_EventType eventType,DungeonScene& dungeonScene) {
        for (auto& rune : equipmentMagicRuneVector_) {
            rune->onNotify(eventType,dungeonScene);
        }
    }
}