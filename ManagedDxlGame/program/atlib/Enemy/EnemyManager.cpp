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

        // ���X�g�ɒǉ�
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
            // �������S�t���O�������Ă�����A���X�g���珜��
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