#include "PlayerData.h"
#include "../Singletons/TextLogManager.h"

namespace atl {

    PlayerData::PlayerData() {
        // �f�o�b�O�p�ŉ��ݒ�
        setAttackPower(10);
        setDefencePower(2);
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

    void PlayerData::levelUp() {
        // ���x������オ��
        ++currentLevel_;
        // �~�όo���l���[����
        currentExp_ = 0;
        // MAX HP �� ������
        setMaxHP(getMaxHP() + LEVEL_UP_HP_UP_VALUE);
        TextLogManager::getTextLogManager()->addTextLog("���x�����オ�����I");

    }

    void PlayerData::debug_playerDataParam(int x, int y) {
        DrawStringEx(x, y, -1, "AttackPower ... [ %d ]", getAttackPower());
        DrawStringEx(x, y + 16, -1, "DefencePower ... [ %d ]", getDefencePower());
    }

}