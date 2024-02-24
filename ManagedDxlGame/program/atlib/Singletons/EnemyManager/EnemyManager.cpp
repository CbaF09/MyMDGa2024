#include "EnemyManager.h"
#include "../SceneManager.h"
#include "../../Utilities/Atl3DCamera.h"
#include "../../Utilities/AtlRandom.h"

namespace atl {

    EnemyManager* EnemyManager::getEnemyManager() {
        static EnemyManager* p_instance_ = nullptr;
        
        if (!p_instance_) p_instance_ = new EnemyManager;
        return p_instance_;
    }

    void EnemyManager::setRandomEnemyFactory() {
        // スポーン可能リストが空なら早期リターン
        if (spawnableEnemyFactoryList_.empty()) { return; }

        int32_t randomIndex = mtRandomRangeInt(0, static_cast<int32_t>(spawnableEnemyFactoryList_.size() - 1));
        currentFactory_ = spawnableEnemyFactoryList_[randomIndex];
    }

    void EnemyManager::spawnEnemy(const tnl::Vector2i& spawn2Dpos) {
        // nullptr チェック　早期リターン
        if (!currentFactory_) { return; }

        auto newEnemy = currentFactory_->createEnemy();
        
        // enemy の2D上の位置を設定
        newEnemy->set2Dpos(spawn2Dpos);

        // enemy の3D上の位置を設定
        auto cellLength = DungeonScene::getCellLength();
        newEnemy->getMesh()->pos_ = tnl::Vector3{ static_cast<float>(spawn2Dpos.x * cellLength), 0, static_cast<float>(spawn2Dpos.y * cellLength) };

        // リストに追加
        enemyList_.emplace_back(newEnemy);
    }

    void EnemyManager::spawnRandomEnemy(const tnl::Vector2i spawn2Dpos) {
        setRandomEnemyFactory();
        spawnEnemy(spawn2Dpos);
    }

    void EnemyManager::renderAllEnemy(Shared<Atl3DCamera> camera,float deltaTime) {
        for (auto& enemy : enemyList_) {
            enemy->renderObject(camera,deltaTime);
        }
    }

    // 全エネミーの移動を実行する。移動が完了していないエネミーがいた場合、return false, 全エネミーが移動完了している場合、return true
    bool EnemyManager::moveAllEnemy(float deltaTime) {
        bool retV = true;

        for (auto& enemy : enemyList_) {
            // 既に移動完了済みのエネミーの場合は、何もせず continue
            if (enemy->getIsAlreadyMove()) { continue; }

            // 移動が完了していないエネミーの場合、processを実行後、retV をfalseにする
            enemy->process(deltaTime);
            retV = false;
        }

        return retV;
    }

    // 全エネミーの行動を実行する。行動が完了していないエネミーがいた場合、return false, 全エネミーが行動完了している場合、return true
    bool EnemyManager::actionAllEnemy(float deltaTime) {
        bool retV = true;

        for (auto& enemy : enemyList_) {
            // 既に行動完了済みのエネミーの場合、何もせず continue
            if (enemy->getIsAlreadyAction()) { continue; }

            // 行動が完了していないエネミーの場合、processを実行後、retV を falseにする
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