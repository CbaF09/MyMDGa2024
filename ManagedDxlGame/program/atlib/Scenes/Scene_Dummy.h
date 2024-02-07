#pragma once

#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

    // �_�~�[�V�[��
    // ����   ... �f�o�b�O�p
    class Scene_Dummy final :
        public Base_Scene {

    public:

    private:

        void sceneUpdate(float deltaTime) override {
            seq_.update(deltaTime);
            render(deltaTime);
        };

        void render(float deltaTime) {
            
//            DrawDefaultLightGuiController();
//            DrawGridGround(player_->getPlayerCamera(), 50, 20);
            DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);
        }

        // �V�[�P���X
        SEQUENCE(Scene_Dummy, &Scene_Dummy::seqInit);

        bool seqInit(float deltaTime) {

            seq_.change(&Scene_Dummy::seqProcess);
            return true;
        }

        bool seqProcess(float deltaTime) {
            
            {// �f�o�b�O�p�BESC�L�[�ŃE�B���h�E���Ƃ��B
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
                    exit(1);
                }
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {

                }
                if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

                }
            }

            return true;
        }


    };
};