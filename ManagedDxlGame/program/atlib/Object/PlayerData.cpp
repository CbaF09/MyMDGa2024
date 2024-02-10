#include "PlayerData.h"
#include "../Singletons/TextLogManager.h"

namespace atl {

    PlayerData::PlayerData() {
        // �f�o�b�O�p�ŉ��ݒ�
        setAttackPower(10);
        setMaxHP(30);
        setCurrentHP(30);
    }

    void PlayerData::changeCurrentEXP(int32_t getExp) {
        currentExp_ += getExp;
        // ���݌o���l���K�v�o���l�ȏ�ɂȂ�����
        if (currentExp_ >= needExp_) {
            levelUp();
        }
    }

    void PlayerData::debug_playerDataParam(int x, int y) {
        DrawStringEx(x, y, -1, "currentLevel ... [ %d ]", currentLevel_);
        DrawStringEx(x, y + 16, -1, "currentExp_ ... [ %d ]", currentExp_);
        DrawStringEx(x, y + 32, -1, "needExp_ ... [ %d ]", needExp_);
    }

    void PlayerData::levelUp() {
        // ���x������オ��
        ++currentLevel_;
        // �~�όo���l���[����
        currentExp_ = 0;
        TextLogManager::getTextLogManager()->addTextLog("���x�����オ�����I");

    }

}