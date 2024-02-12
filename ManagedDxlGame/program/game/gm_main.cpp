#pragma once

#include <time.h>
#include <string>
#include <numbers>
#include <functional>

#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"
#include "../atlib/Utilities/AtlRandom.h"
#include "../atlib/Singletons/SceneManager.h"
#include "../atlib/Singletons/FadeInOutManager.h"
#include "../atlib/Singletons/DungeonCreater.h"
#include "../atlib/Singletons/ResourceManager.h"
#include "../atlib/Singletons/TextLogManager.h"
#include "../atlib/Scenes/DungeonScene.h"
#include "../atlib/Scenes/TitleScene.h"
#include "../atlib/Scenes/Scene_Dummy.h"
#include "../atlib/Scenes/GameClearScene.h"
#include "../atlib/Scenes/GameOverScene.h"
#include "gm_main.h"

void gameStart() {
	atl::mtRandomInit();

    // フォグのオンオフは、Skyboxクラスで行われています
	SetFogColor(32, 32, 32);
	SetFogStartEnd(4000.0f, 5000.0f);

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
    
    // マウスカーソルを非表示に
	SetMouseDispFlag(false);
    // マウスカーソルをウィンドウに固定
    LockCursorToWindow();

    // atl::SceneManager::getSceneManager(std::make_shared<atl::Scene_Dummy>());

	atl::SceneManager::getSceneManager(std::make_shared<atl::TitleScene>());
}

void gameMain(float delta_time) {
	atl::SceneManager::getSceneManager()->nowSceneUpdate(delta_time);
}

void gameEnd() {
    // シングルトン群を解放
    atl::SceneManager::deleteSceneManager();
    atl::DungeonCreater::deleteDungeonCreater();
    atl::TextLogManager::deleteTextLogManager();
    atl::FadeInOutManager::deleteFadeInOutManager();
    atl::ResourceManager::deleteResourceManager();

    // DXlib の関数でメモリをまとめて解放
    InitGraph();
    InitFontToHandle();
    InitSoundMem();
}

// マウスカーソルをウィンドウに固定させる関数
void LockCursorToWindow() {
    HWND hwnd = GetMainWindowHandle(); // ウィンドウハンドルを取得
    RECT rect;
    GetClientRect(hwnd, &rect); // クライアント領域のサイズを取得

    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;

    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;

    MapWindowPoints(hwnd, nullptr, &ul, 1); // クライアント領域の左上のスクリーン座標への変換
    MapWindowPoints(hwnd, nullptr, &lr, 1); // クライアント領域の右下のスクリーン座標への変換

    RECT clipRect;
    clipRect.left = ul.x;
    clipRect.top = ul.y;
    clipRect.right = lr.x;
    clipRect.bottom = lr.y;

    ClipCursor(&clipRect); // カーソルを制限
}

//#pragma once
//
////---------------------------------------------------------------------------------------------------------------
////
////
//// 3D スタートアップサンプル
////
////
////---------------------------------------------------------------------------------------------------------------
//
//#include <time.h>
//#include <string>
//#include <numbers>
//#include <functional>
//#include "../dxlib_ext/dxlib_ext.h"
//#include "gm_main.h"
//
//Shared<dxe::Camera> camera = nullptr;
//Shared<dxe::Mesh> mesh = nullptr;
//Shared<dxe::Texture> texture = nullptr;
//
//
////------------------------------------------------------------------------------------------------------------
//// ゲーム起動時に１度だけ実行されます
//void gameStart() {
//    srand(time(0));
//
//    SetFogEnable(true);
//    SetFogColor(32, 32, 32);
//    SetFogStartEnd(500.0f, 1500.0f);
//    ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
//    SetBackgroundColor(32, 32, 32);
//
//
//    camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
//
//    texture = dxe::Texture::CreateFromFile("graphics/test.jpg");
//
//    mesh = dxe::Mesh::CreateSphereMV(100, 20, 20);
//    mesh->setTexture(texture);
//    mesh->setBlendMode(DX_BLENDMODE_ALPHA);
//    mesh->setAlpha(0.5);
//}
//
//
////------------------------------------------------------------------------------------------------------------
//// 毎フレーム実行されます
//void gameMain(float delta_time) {
//
//    mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(1));
//
//    camera->update();
//
//
//    DrawGridGround(camera, 50, 20);
//
//    mesh->render(camera);
//    mesh->drawGuiMaterialControlloer();
//
//    DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
//
//}
//
////------------------------------------------------------------------------------------------------------------
//// ゲーム終了時に１度だけ実行されます
//void gameEnd() {
//
//}
//
//
