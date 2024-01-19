#pragma once

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"
#include "../atlib/Utilities/AtlRandom.h"
#include "../atlib/MeshObject/Wall.h"
#include "../atlib/Singletons/SceneManager.h"
#include "../atlib/Scenes/DungeonScene.h"
#include "../atlib/Scenes/Scene_Dummy.h"
#include "gm_main.h"

void gameStart() {
	atl::mtRandomInit();

	//	SetFogEnable(true);
	//	SetFogColor(32, 32, 32);
	//	SetFogStartEnd(500.0f, 1500.0f);
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);

	// atl::SceneManager::getSceneManager(std::make_shared<atl::Scene_Dummy>());
	atl::SceneManager::getSceneManager(std::make_shared<atl::DungeonScene>());
}

void gameMain(float delta_time) {
	atl::SceneManager::getSceneManager()->nowSceneUpdate(delta_time);
}

void gameEnd() {

}