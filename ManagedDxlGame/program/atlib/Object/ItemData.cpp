#include "ItemData.h"

namespace atl {

	ItemData::ItemData() {

	}

	ItemData::ItemData(int32_t itemID) {
		auto csv = tnl::LoadCsv("csv/ItemCSV.csv");

		if (!csv.empty()) {
			itemID_ = csv[itemID][0].getInt();
			itemName_ = csv[itemID][1].getString();

			auto& filepath = csv[itemID][2].getString();
			itemIllust_ = dxe::Texture::CreateFromFile(filepath);
		}
	}

}