#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MagicRune.h"
#include "MagicRuneSystemManager.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	void Base_MagicRune::setRuneData(e_RuneID itemID) {
		auto& csv = MagicRuneSystemManager::getMagicRuneSystemManager()->getMagicRuneCSV();
		int id = static_cast<int>(itemID);

		// ルーン名を設定
		runeName_ = csv[id][1].getString();
		// ルーンの説明文を設定
		runeDesc_ = csv[id][2].getString();
		// ルーンのアイコン画像を設定
		auto& graphHandlePath = csv[id][3].getString();
		graphHandle_ = ResourceManager::getResourceManager()->getGraphRes(graphHandlePath.c_str());
	}

}