#include "EnemyFactory.h"

namespace atl {

    /// <summary>
    /// 
    /// �X���C��
    /// 
    /// </summary>
    Shared<Base_Enemy> BlueSlimeFactory::createEnemy() {
        Shared<Base_Enemy> newEnemy = std::make_shared<BlueSlime>();
        newEnemy->initialize();
        return newEnemy;
    }

    /// <summary>
    /// 
    /// �΃X���C��
    /// 
    /// </summary>
    Shared<Base_Enemy> GreenSlimeFactory::createEnemy() {
        Shared<Base_Enemy> newEnemy = std::make_shared<GreenSlime>();
        newEnemy->initialize();
        return newEnemy;
    }

    /// <summary>
    /// 
    /// �ԃX���C��
    /// 
    /// </summary>
    Shared<Base_Enemy> RedSlimeFactory::createEnemy() {
        Shared<Base_Enemy> newEnemy = std::make_shared<RedSlime>();
        newEnemy->initialize();
        return newEnemy;

    }
}