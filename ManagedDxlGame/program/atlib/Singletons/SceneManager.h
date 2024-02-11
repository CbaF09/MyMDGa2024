#pragma once
#include <memory>
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Scenes/Base_Scene.h"

namespace atl {

    // �V�[���Ǘ��N���X ( �V���O���g�� )
    // ���� ... �V�[���� sceneUpdate ���s , �V�[���J��
    class SceneManager final {
    // --------------------------
    // �V���O���g���݌v
    public:
        // ��ԍŏ��ɌĂяo���ۂ́A�N�����̏����V�[���������ɓn���Ă��������Bstd::make_shared< �����V�[���N���X�� >
        static SceneManager* getSceneManager(Shared<Base_Scene> startScene = nullptr);
        static void deleteSceneManager() { delete getSceneManager(); };
    private:
        explicit SceneManager(Shared<Base_Scene> startScene) { p_nowScene_ = startScene; };
        ~SceneManager() {};
    // --------------------------
    public:
        // ���t���[���ɂāA�w�肵���V�[���ɑJ�ځi���݂̃V�[���͏�������܂��j
        // arg ... std::make_shared< ���̃V�[���N���X >()
        inline void changeScene(Shared<Base_Scene> nextScene) { p_nextScene_ = nextScene; };

        // gameMain �Ŗ��t���[�����s�B�V�[���N���X�� sceneUpdate ���Ăяo�����\�b�h
        // arg ... �o�ߎ��ԁBgameMain �̈��������̂܂ܓ�����OK
        void nowSceneUpdate(float deltaTime);

    private:
        Shared<Base_Scene> p_nowScene_ = nullptr;
        Shared<Base_Scene> p_nextScene_ = nullptr;
    };

};