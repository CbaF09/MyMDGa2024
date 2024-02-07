#include "ItemData.h"
#include "../Singletons/TextLogManager.h"

namespace atl {

	ItemData::ItemData() {

	}

	ItemData::ItemData(int32_t itemID) {
		auto csv = tnl::LoadCsv("csv/ItemCSV.csv");

		if (!csv.empty()) {
			itemID_ = static_cast<e_itemList>(csv[itemID][0].getInt());
			// デバッグ用 末尾にランダムな数字を入れている
			int random = rand() % 30;
			itemName_ = csv[itemID][1].getString() + std::to_string(random);

			auto& filepath = csv[itemID][2].getString();
			itemIllust_ = dxe::Texture::CreateFromFile(filepath);
		}
	}

	void ItemData::executeItemPerformAction() {
		switch (itemID_) {
		case e_itemList::NONE: break;
		case e_itemList::HealPotion: TextLogManager::getTextLogManager()->addTextLog(itemName_ + "を使った"); break;
		}
	}

}