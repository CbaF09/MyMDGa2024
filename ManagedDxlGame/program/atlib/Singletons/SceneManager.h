#pragma once
#include <memory>
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Scenes/Base_Scene.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

    // シーン管理クラス ( シングルトン )
    // 役割 ... シーンの sceneUpdate 実行 , シーン遷移
    class SceneManager final {
    // --------------------------
    // シングルトン設計
    public:
        // 一番最初に呼び出す際は、起動時の初期シーンを引数に渡してください。std::make_shared< 初期シーンクラス名 >
        static SceneManager* getSceneManager(Shared<Base_Scene> startScene = nullptr);
        static void deleteSceneManager() { delete getSceneManager(); };
    private:
        explicit SceneManager(Shared<Base_Scene> startScene) { p_nowScene_ = startScene; };
        ~SceneManager() {};
    // --------------------------

    public:
        // 次フレームにて、指定したシーンに遷移（現在のシーンは消去されます）
        // arg ... std::make_shared< 次のシーンクラス >()
        inline void changeScene(Shared<Base_Scene> nextScene) { p_nextScene_ = nextScene; };

        // gameMain で毎フレーム実行。シーンクラスの sceneUpdate を呼び出すメソッド
        // arg ... 経過時間。gameMain の引数をそのまま入れればOK
        void nowSceneUpdate(float deltaTime);

        // 現在シーンに対し、指定したシーン へのダウンキャストを行う。
        // 指定したシーンであれば、現在シーンへのポインターが返ってくる。
        // そうでないなら、nullptrが返ってくる
        // 
        // !!!!!!! 絶対に nullptr チェックして使え !!!!!!!!!!
        // 例 : 
        // auto dungeonScene = SceneManager::getSceneManager()->tryGetScene<DungeonScene>();
        // if (dungeonScene) { dungeonScene->hogehoge... }
        template <typename T>
        const Shared<T> tryGetScene() {
            // static_assert => コンパイル時アサート
            // std::is_base_of => 第二引数が、第一引数を継承しているなら true
            static_assert(std::is_base_of<Base_Scene, T>::value, "tryGetSceneにBase_Sceneを継承していないクラスが入力されています");

            auto currentScenePointer = std::dynamic_pointer_cast<T>(p_nowScene_);
            return currentScenePointer;
        }

    private:
        Shared<Base_Scene> p_nowScene_ = nullptr;
        Shared<Base_Scene> p_nextScene_ = nullptr;
    };

};