#include "Item.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/SceneManager.h"
#include "../Singletons/EnemyManager/EnemyManager.h"
#include "../Singletons/CsvManager.h"
#include "../Utilities/AtlRandom.h"
#include "../MeshObject/PlayerPawn.h"
#include "../Utilities/AtlString.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"
#include "../MagicRuneSystem/MagicRune.h"

namespace atl {

	void Item::initialize(e_ItemList itemName) {
		// CSV をロードする
		auto& csv = CsvManager::getCsvManager()->getItemDataCSV();
		// ロードの正否チェック、早期リターン
		if (csv.empty()) { return; }

		// 範囲外チェック、早期リターン
		if (itemName == e_ItemList::NONE || itemName == e_ItemList::ITEM_MAX) { return; }

		auto itemID = static_cast<int>(itemName);

		// Item名をセット
		itemName_ = csv[itemID][1].getString();
		// Item に対応するテクスチャをセット
		auto& filepath = csv[itemID][2].getString();
		itemIllust_ = dxe::Texture::CreateFromFile(filepath);
		// アイテムの説明文をセット
		descString_ = csv[itemID][3].getString();
	}

	void Item::executeItemPerformAction() {
		// nullptr チェック 早期リターン
		if (!itemExecutePerform_) { return; }

		// アイテムの効果発動
		itemExecutePerform_->execute();
	}


}