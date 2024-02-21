#include "EnemyManager.h"
#include "../Singletons/SceneManager.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

    EnemyManager* EnemyManager::getEnemyManager() {
        static EnemyManager* p_instance_ = nullptr;
        
        if (!p_instance_) p_instance_ = new EnemyManager;
        return p_instance_;
    }

    void EnemyManager::generateEnemy(const tnl::Vector2i& generate2Dpos) {
        // nullptr チェック　早期リターン
        if (!currentFactory_) { return; }

        auto newEnemy = currentFactory_->createEnemy();
        
        // enemy の2D上の位置を設定
        newEnemy->set2Dpos(generate2Dpos);

        // enemy の3D上の位置を設定
        auto cellLength = DungeonScene::getCellLength();
        newEnemy->getMesh()->pos_ = tnl::Vector3{ static_cast<float>(generate2Dpos.x * cellLength), 0, static_cast<float>(generate2Dpos.y * cellLength) };

        // リストに追加
        enemyList_.emplace_back(newEnemy);
    }

    void EnemyManager::renderAllEnemy(Shared<Atl3DCamera> camera,float deltaTime) {
        for (auto& enemy : enemyList_) {
            enemy->renderObject(camera,deltaTime);
        }
    }

    bool EnemyManager::moveAllEnemy(float deltaTime) {
        bool retV = true;

        for (auto& enemy : enemyList_) {
            if (enemy->getIsAlreadyMove()) { continue;  }
            enemy->process(deltaTime);
            retV = false;
        }

        return retV;
    }

    bool EnemyManager::actionAllEnemy(float deltaTime) {
        bool retV = true;

        for (auto& enemy : enemyList_) {
            if (enemy->getIsAlreadyAction()) { continue; }
            enemy->process(deltaTime);
            retV = false;
        }

        return retV;
    }

    void EnemyManager::deadEnemyErase() {
        for (auto it = enemyList_.begin(); it != enemyList_.end();) {
            // もし死亡フラグが立っていたら、リストから除去
            if ((*it)->getIsDead()) {
                it = enemyList_.erase(it);
                continue;
            }
            else {
                ++it;
            }
        }
    }

    void EnemyManager::allEnemyOffFlagIsAlreadyTurn() {
        for (auto& enemy : enemyList_) {
            enemy->offFlagIsAlreadyTurn();
        }
    }

}