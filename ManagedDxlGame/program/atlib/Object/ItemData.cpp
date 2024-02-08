#include "ItemData.h"
#include "../Singletons/TextLogManager.h"

namespace atl {

	ItemData::ItemData() {

	}

	// itemID を入れると、そのアイテムが生成される
	ItemData::ItemData(int32_t itemID) {
		auto csv = tnl::LoadCsv("csv/ItemCSV.csv");

		if (!csv.empty()) {
			// ID を enum に変換してからセット
			itemID_ = static_cast<e_itemList>(csv[itemID][0].getInt());
			// Item名をセット
			itemName_ = csv[itemID][1].getString();
			// Item に対応するテクスチャをセット
			auto& filepath = csv[itemID][2].getString();
			itemIllust_ = dxe::Texture::CreateFromFile(filepath);
			// アイテムの説明文をセット
			descString_ = csv[itemID][3].getString();
		}
	}

	void ItemData::executeItemPerformAction() {
		switch (itemID_) {
		case e_itemList::NONE: break;
		case e_itemList::HealPotion: TextLogManager::getTextLogManager()->addTextLog(itemName_ + "を使った"); break;
		}
	}

}