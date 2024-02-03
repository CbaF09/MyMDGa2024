#include "Item.h"

namespace atl {

	Item::Item() {

		auto csv = tnl::LoadCsv("csv/ItemCSV.csv");

		if (!csv.empty()) {
			itemID_ = csv[1][0].getInt();
			itemName_ = csv[1][1].getString();

			auto& filepath = csv[1][2].getString();
			itemIllust_ = dxe::Texture::CreateFromFile(filepath);
		}
	}

}