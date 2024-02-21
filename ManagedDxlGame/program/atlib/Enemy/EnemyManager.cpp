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
        // nullptr �`�F�b�N�@�������^�[��
        if (!currentFactory_) { return; }

        auto newEnemy = currentFactory_->createEnemy();
        
        // enemy ��2D��̈ʒu��ݒ�
        newEnemy->set2Dpos(generate2Dpos);

        // enemy ��3D��̈ʒu��ݒ�
        auto cellLength = DungeonScene::getCellLength();
        newEnemy->getMesh()->pos_ = tnl::Vector3{ static_cast<float>(generate2Dpos.x * cellLength), 0, static_cast<float>(generate2Dpos.y * cellLength) };

        enemyList_.emplace_back(newEnemy);

        // �_���W�����V�[���ւ̎�Q�Ƃ��擾���ݒ�
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