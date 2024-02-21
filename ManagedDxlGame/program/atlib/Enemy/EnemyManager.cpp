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

        enemyList_.emplace_back(newEnemy);

        // ダンジョンシーンへの弱参照を取得し設定
        auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
        if (!dungeonScene) { return; }
        newEnemy->setWeakDungeonScene(dungeonScene);
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