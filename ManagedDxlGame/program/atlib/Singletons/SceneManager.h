#pragma once

#include <memory>
#include "../Scenes/Base_Scene.h"

namespace atl {

    // シーン管理クラス ( シングルトン )
    // 役割 ... シーンの sceneUpdate 実行 , シーン遷移
    class SceneManager final {
    // --------------------------
    // シングルトン設計
    public:
        // 一番最初に呼び出す際は、起動時の初期シーンを引数に渡してください。std::make_shared< 初期シーンクラス名 >
        static SceneManager* getSceneManager(std::shared_ptr<Base_Scene> startScene = nullptr);
        static void deleteSceneManager();
    private:
        static SceneManager* p_instance_;
    // --------------------------

    public:
        explicit SceneManager(std::shared_ptr<Base_Scene> startScene) { p_nowScene_ = std::move(startScene); };

        // 次フレームにて、指定したシーンに遷移（現在のシーンは消去されます）
        // arg ... std::make_shared< 次のシーンクラス >()
        void changeScene(std::shared_ptr<Base_Scene> nextScene) { p_nextScene_ = std::move(nextScene); };

        // gameMain で毎フレーム実行。シーンクラスの sceneUpdate を呼び出すメソッド
        // arg ... 経過時間。gameMain の引数をそのまま入れればOK
        void nowSceneUpdate(float deltaTime);



    private:
        std::shared_ptr<Base_Scene> p_nowScene_ = nullptr;
        std::shared_ptr<Base_Scene> p_nextScene_ = nullptr;
    };

};

/* Scene_Dummyを呼び出すサンプル

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