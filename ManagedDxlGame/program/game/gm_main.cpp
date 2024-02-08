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

	SetFogColor(32, 32, 32);
	SetFogStartEnd(5000.0f, 6000.0f);
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
	SetMouseDispFlag(false);
    LockCursorToWindow();

    //atl::SceneManager::getSceneManager(std::make_shared<atl::Scene_Dummy>());

	atl::SceneManager::getSceneManager(std::make_shared<atl::DungeonScene>());
}

void gameMain(float delta_time) {
	atl::SceneManager::getSceneManager()->nowSceneUpdate(delta_time);
}

void gameEnd() {
    // シングルトン群を解放
    atl::TextLogManager::deleteTextLogManager();
    atl::DungeonCreater::deleteDungeonCreater();
    atl::FadeInOutManager::deleteFadeInOutManager();
    atl::ResourceManager::deleteResourceManager();
    atl::SceneManager::deleteSceneManager();

    // DXlib の関数でメモリをまとめて解放
    InitGraph();
    InitFontToHandle();
    InitSoundMem();
}

// カーソルをウィンドウに固定させる関数
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


//-----------------------------------------------------------------------------------------------------------
//
// スカイボックス利用サンプル
//
// ※ フォグを利用する場合はスカイボックスを描画した後に設定しましょう
// ※ スカイボックスは大きく作るのでカメラの far 設定次第では描画されなくなるので注意
//
//-----------------------------------------------------------------------------------------------------------

//#include <time.h>
//#include <string>
//#include <numbers>
//#include <functional>
//#include "../dxlib_ext/dxlib_ext.h"
//#include "gm_main.h"
//
//Shared<dxe::Camera> camera = nullptr;
//Shared<dxe::Mesh> skybox = nullptr;
//
////------------------------------------------------------------------------------------------------------------
//// ゲーム起動時に１度だけ実行されます
//void gameStart() {
//	srand(time(0));
//
//	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
//	SetBackgroundColor(32, 32, 32);
//	SetDefaultLightParameter("directional_light_parameter.bin");
//
//	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
//
//	skybox = dxe::Mesh::CreateBoxMV({ 30000, 30000,30000 },
//		dxe::Texture::CreateFromFile("graphics/skybox/SkyboxLeft.jpg"),
//		dxe::Texture::CreateFromFile("graphics/skybox/SkyboxLeft.jpg"),
//		dxe::Texture::CreateFromFile("graphics/skybox/SkyboxLeft.jpg"),
//		dxe::Texture::CreateFromFile("graphics/skybox/SkyboxLeft.jpg"),
//		dxe::Texture::CreateFromFile("graphics/skybox/SkyboxLeft.jpg"),
//		dxe::Texture::CreateFromFile("graphics/skybox/SkyboxLeft.jpg"),
//		30,
//		30
//		);
//	//skybox->loadMaterial("material.bin");
//
//}
//
//
////------------------------------------------------------------------------------------------------------------
//// 毎フレーム実行されます
//void gameMain(float delta_time) {
//
//	//-------------------------------------------------------------------------------
//	//
//	// update
//	//
//	//-------------------------------------------------------------------------------
//
//	skybox->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.01f));
//
//	camera->update();
//
//
//	//-------------------------------------------------------------------------------
//	//
//	// draw
//	//
//	//-------------------------------------------------------------------------------
//
//	skybox->render(camera);
//
//	DrawGridGround(camera, 50, 20);
//	DrawDefaultLightGuiController();
//
//	skybox->drawGuiMaterialControlloer();
//
//	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
//
//}
//
////------------------------------------------------------------------------------------------------------------
//// ゲーム終了時に１度だけ実行されます
//void gameEnd() {
//
//}