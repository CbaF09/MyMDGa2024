#include "EnemyManager.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

    EnemyManager* EnemyManager::getEnemyManager() {
        static EnemyManager* p_instance_ = nullptr;
        
        if (!p_instance_) p_instance_ = new EnemyManager;
        return p_instance_;
    }

    void EnemyManager::generateEnemy() {
        // nullptr チェック　早期リターン
        if (!currentFactory_) { return; }

        auto newEnemy = currentFactory_->createEnemy();
        enemyList_.emplace_back(newEnemy);
    }

    void EnemyManager::processAllEnemy(float deltaTime) {
        for (auto& enemy : enemyList_) {
            enemy->process(deltaTime);
        }
    }

    void EnemyManager::renderAllEnemy(Shared<Atl3DCamera> camera,float deltaTime) {
        for (auto& enemy : enemyList_) {
            enemy->renderObject(camera);
        }
    }

}