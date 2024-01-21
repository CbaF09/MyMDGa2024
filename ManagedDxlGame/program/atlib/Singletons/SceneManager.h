#pragma once

#include <memory>
#include "../Scenes/Base_Scene.h"

namespace atl {

    // �V�[���Ǘ��N���X ( �V���O���g�� )
    // ���� ... �V�[���� sceneUpdate ���s , �V�[���J��
    class SceneManager final {
    // --------------------------
    // �V���O���g���݌v
    public:
        // ��ԍŏ��ɌĂяo���ۂ́A�N�����̏����V�[���������ɓn���Ă��������Bstd::make_shared< �����V�[���N���X�� >
        static SceneManager* getSceneManager(std::shared_ptr<Base_Scene> startScene = nullptr);
        static void deleteSceneManager();
    private:
        static SceneManager* p_instance_;
    // --------------------------

    public:
        explicit SceneManager(std::shared_ptr<Base_Scene> startScene) { p_nowScene_ = std::move(startScene); };

        // ���t���[���ɂāA�w�肵���V�[���ɑJ�ځi���݂̃V�[���͏�������܂��j
        // arg ... std::make_shared< ���̃V�[���N���X >()
        void changeScene(std::shared_ptr<Base_Scene> nextScene) { p_nextScene_ = std::move(nextScene); };

        // gameMain �Ŗ��t���[�����s�B�V�[���N���X�� sceneUpdate ���Ăяo�����\�b�h
        // arg ... �o�ߎ��ԁBgameMain �̈��������̂܂ܓ�����OK
        void nowSceneUpdate(float deltaTime);



    private:
        std::shared_ptr<Base_Scene> p_nowScene_ = nullptr;
        std::shared_ptr<Base_Scene> p_nextScene_ = nullptr;
    };

};

/* Scene_Dummy���Ăяo���T���v��

#pragma once

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Singletons/SceneManager.h"
#include "../atlib/Scenes/Scene_Dummy.h"
#include "../atlib/Utilities/AtlRandom.h"
#include "gm_main.h"

void gameStart() {
    SetFontSize(15);
    atl::mtRandomInit();
    atl::SceneManager::getSceneManager(std::make_shared<atl::Scene_Dummy>());
}

void gameMain(float delta_time) {

    atl::SceneManager::getSceneManager()->nowSceneUpdate(delta_time);
}

void gameEnd() {

}

*/