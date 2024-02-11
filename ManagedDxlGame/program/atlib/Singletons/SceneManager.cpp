#include "SceneManager.h"

namespace atl {
    

    SceneManager* SceneManager::getSceneManager(Shared<Base_Scene> startScene) {
        static SceneManager* p_instance_ = nullptr;

        if (!p_instance_ && startScene != nullptr) p_instance_ = new SceneManager(std::move(startScene));
        return p_instance_;
    };

    void SceneManager::nowSceneUpdate(float deltaTime) {
        if (p_nextScene_) { 
            p_nowScene_ = p_nextScene_; 
            p_nextScene_.reset();
        }

        if (p_nowScene_) {
            p_nowScene_->sceneUpdate(deltaTime);
        }
        else {
            // TODO: エラーハンドリング
        };
    };

};