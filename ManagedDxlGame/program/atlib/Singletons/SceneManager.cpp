#include "SceneManager.h"

namespace atl {
    
    SceneManager* SceneManager::p_instance_ = nullptr;

    SceneManager* SceneManager::getSceneManager(std::shared_ptr<Base_Scene> startScene) {
        if (!p_instance_ && startScene != nullptr) p_instance_ = new SceneManager(std::move(startScene));
        return p_instance_;
    };

    void SceneManager::deleteSceneManager() {
        if (p_instance_) {
            delete p_instance_;
            p_instance_ = nullptr;
        };
    };

    void SceneManager::nowSceneUpdate(float deltaTime) {
        if (p_nextScene_) { p_nowScene_ = std::move(p_nextScene_); }

        if (p_nowScene_) {
            p_nowScene_->sceneUpdate(deltaTime);
        }
        else {
            // TODO: エラーハンドリング
        };
    };

};