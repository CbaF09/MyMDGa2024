#pragma once

#include <time.h>
#include <string>
#include <numbers>
#include <functional>

#include "gm_main.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"
#include "../atlib/Utilities/AtlRandom.h"
#include "../atlib/Singletons/SceneManager.h"
#include "../atlib/Singletons/FadeInOutManager.h"
#include "../atlib/Singletons/DungeonCreater.h"
#include "../atlib/Singletons/ResourceManager.h"
#include "../atlib/Singletons/TextLogManager.h"
#include "../atlib/Singletons/CsvManager.h"
#include "../atlib/Scenes/DungeonScene.h"
#include "../atlib/Scenes/TitleScene.h"
#include "../atlib/Scenes/Scene_Dummy.h"
#include "../atlib/Scenes/GameClearScene.h"
#include "../atlib/Scenes/GameOverScene.h"

// 最初に一回呼ばれる関数
void gameStart() {
	atl::mtRandomInit();

    // フォグのオンオフは、Skysphereクラスで行われています
	SetFogColor(32, 32, 32);
	SetFogStartEnd(4000.0f, 5000.0f);

    // 直射光を設定
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));

    // 背景色設定
	SetBackgroundColor(32, 32, 32);
    
    // マウスカーソルを非表示に
	SetMouseDispFlag(false);
    // マウスカーソルをウィンドウに固定
    LockCursorToWindow();

    // デバッグ用ダミーシーン
    //atl::SceneManager::getSceneManager(std::make_shared<atl::Scene_Dummy>());
    
    // 本編の初期シーン設定
	atl::SceneManager::getSceneManager(std::make_shared<atl::TitleScene>());
}

// ゲームメインループ
void gameMain(float delta_time) {
	atl::SceneManager::getSceneManager()->nowSceneUpdate(delta_time);
}

// 終了時に一回呼ばれる関数
void gameEnd() {
    // シングルトン群を解放
    atl::SceneManager::deleteSceneManager();
    atl::DungeonCreater::deleteDungeonCreater();
    atl::TextLogManager::deleteTextLogManager();
    atl::FadeInOutManager::deleteFadeInOutManager();
    atl::MagicRuneSystemManager::deleteMagicRuneSystemManager();
    atl::CsvManager::deleteCsvManager();

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