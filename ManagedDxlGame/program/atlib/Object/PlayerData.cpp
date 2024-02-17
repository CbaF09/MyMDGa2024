#include "PlayerData.h"
#include "../Singletons/TextLogManager.h"

namespace atl {

    PlayerData::PlayerData() {
        // デバッグ用で仮設定
        setAttackPower(10);
        setDefencePower(2);
        setMaxHP(30);
        setCurrentHP(30);
    }

    void PlayerData::changeCurrentEXP(int32_t getExp) {
        currentExp_ += getExp;
        // 現在経験値が必要経験値以上になった時
        if (currentExp_ >= needExp_) {
            levelUp();
        }
    }

    void PlayerData::levelUp() {
        // レベルが一つ上がる
        ++currentLevel_;
        // 蓄積経験値がゼロに
        currentExp_ = 0;
        // MAX HP が 増える
        setMaxHP(getMaxHP() + LEVEL_UP_HP_UP_VALUE);
        TextLogManager::getTextLogManager()->addTextLog("レベルが上がった！");

    }

    void PlayerData::debug_playerDataParam(int x, int y) {
        DrawStringEx(x, y, -1, "AttackPower ... [ %d ]", getAttackPower());
        DrawStringEx(x, y + 16, -1, "DefencePower ... [ %d ]", getDefencePower());
    }

}