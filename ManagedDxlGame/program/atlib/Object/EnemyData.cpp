#include "EnemyData.h"
#include "../Utilities/AtlRandom.h"

namespace atl {

	EnemyData::EnemyData() {
		auto csv = tnl::LoadCsv("csv/EnemyCSV.csv");

		// CSV‚Ì’†‚©‚çƒ‰ƒ“ƒ_ƒ€‚Éİ’è
		auto rand = mtRandomRangeInt(1, csv.size() - 1);

		enemyID_ = csv[rand][0].getInt();
		setMaxHP(csv[rand][1].getInt());
		setCurrentHP(getMaxHP());
		setAttackPower(csv[rand][2].getInt());

		auto& filePath = csv[rand][3].getString();
		enemyTexture_ = dxe::Texture::CreateFromFile(filePath);
	}

}