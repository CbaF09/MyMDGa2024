#include "EnemyFactory.h"

namespace atl {

    /// <summary>
    /// 
    /// �X���C��
    /// 
    /// </summary>
    Shared<Base_Enemy> SlimeFactory::createEnemy() {
        Shared<Base_Enemy> newSphereEnemy = std::make_shared<SlimeEnemy>();

        newSphereEnemy->initialize();
        
        return newSphereEnemy;
    }
}