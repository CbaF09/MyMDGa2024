#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MagicRune.h"
#include "MagicRuneSystemManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/CsvManager.h"

namespace atl {

	void Base_MagicRune::setRuneData(e_RuneID itemID) {
		// CSVデータを持ってくる
		auto& csv = CsvManager::getCsvManager()->getMagicRuneCSV();

		int index = static_cast<int>(itemID);

		// ルーン名を設定
		runeName_ = csv[index][1].getString();
		// ルーンの説明文を設定
		runeDesc_ = csv[index][2].getString();
		// ルーンのアイコン画像を設定
		auto& graphHandlePath = csv[index][3].getString();
		graphHandle_ = ResourceManager::getResourceManager()->getGraphRes(graphHandlePath.c_str());
	}

}