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
        // �X�|�[���\���X�g����Ȃ瑁�����^�[��
        if (spawnableEnemyFactoryList_.empty()) { return; }

        int32_t randomIndex = mtRandomRangeInt(0, static_cast<int32_t>(spawnableEnemyFactoryList_.size() - 1));
        currentFactory_ = spawnableEnemyFactoryList_[randomIndex];
    }

    void EnemyManager::spawnEnemy(const tnl::Vector2i& spawn2Dpos) {
        // nullptr �`�F�b�N�@�������^�[��
        if (!currentFactory_) { return; }

        auto newEnemy = currentFactory_->createEnemy();
        
        // enemy ��2D��̈ʒu��ݒ�
        newEnemy->set2Dpos(spawn2Dpos);

        // enemy ��3D��̈ʒu��ݒ�
        auto cellLength = DungeonScene::getCellLength();
        newEnemy->getMesh()->pos_ = tnl::Vector3{ static_cast<float>(spawn2Dpos.x * cellLength), 0, static_cast<float>(spawn2Dpos.y * cellLength) };

        // ���X�g�ɒǉ�
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

    // �S�G�l�~�[�̈ړ������s����B�ړ����������Ă��Ȃ��G�l�~�[�������ꍇ�Areturn false, �S�G�l�~�[���ړ��������Ă���ꍇ�Areturn true
    bool EnemyManager::moveAllEnemy(float deltaTime) {
        bool retV = true;

        for (auto& enemy : enemyList_) {
            // ���Ɉړ������ς݂̃G�l�~�[�̏ꍇ�́A�������� continue
            if (enemy->getIsAlreadyMove()) { continue; }

            // �ړ����������Ă��Ȃ��G�l�~�[�̏ꍇ�Aprocess�����s��AretV ��false�ɂ���
            enemy->process(deltaTime);
            retV = false;
        }

        return retV;
    }

    // �S�G�l�~�[�̍s�������s����B�s�����������Ă��Ȃ��G�l�~�[�������ꍇ�Areturn false, �S�G�l�~�[���s���������Ă���ꍇ�Areturn true
    bool EnemyManager::actionAllEnemy(float deltaTime) {
        bool retV = true;

        for (auto& enemy : enemyList_) {
            // ���ɍs�������ς݂̃G�l�~�[�̏ꍇ�A�������� continue
            if (enemy->getIsAlreadyAction()) { continue; }

            // �s�����������Ă��Ȃ��G�l�~�[�̏ꍇ�Aprocess�����s��AretV �� false�ɂ���
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