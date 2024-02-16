#include "MagicRuneSystemManager.h"

namespace atl {

    MagicRuneSystemManager* MagicRuneSystemManager::getMagicRuneSystemManager() {
        static MagicRuneSystemManager* p_instance_ = nullptr;

        if (!p_instance_) { 
            p_instance_ = new MagicRuneSystemManager();
        }
        return p_instance_;
    }

    bool MagicRuneSystemManager::equipRune(Shared<Base_MagicRune> newRune) {
        // ���������\�ő吔�𒴂��Ă��Ȃ���΁A�ǉ�
        if (equipmentMagicRunes_.size() < MAX_EQUIPMENT_RUNE) {
            equipmentMagicRunes_.emplace_back(newRune);
            return true;
        }
        else {
            return false;
        }
    }

    void MagicRuneSystemManager::removeRune(int index) {
        // �͈͊O�A�N�Z�X�`�F�b�N�A�������^�[��
        if (index < 0  || index >= equipmentMagicRunes_.size()) return;

        equipmentMagicRunes_.erase(equipmentMagicRunes_.begin() + index);
    }

    void MagicRuneSystemManager::notifyOnEvent(e_EventType eventType,DungeonScene& dungeonScene) {
        for (auto& rune : equipmentMagicRunes_) {
            rune->onNotify(eventType,dungeonScene);
        }
    }
}