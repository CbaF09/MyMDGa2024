#include "EnemyData.h"
#include "../Utilities/AtlRandom.h"

namespace atl {

	EnemyData::EnemyData() {
		auto csv = tnl::LoadCsv("csv/EnemyCSV.csv");

		// CSV‚Ì’†‚©‚çƒ‰ƒ“ƒ_ƒ€‚Éİ’è
		auto rand = mtRandomRangeInt(1, csv.size() - 1);

		enemyID_ = csv[rand][0].getInt();
		enemyName_ = csv[rand][1].getString();
		setMaxHP(csv[rand][2].getInt());
		setCurrentHP(getMaxHP());
		setAttackPower(csv[rand][3].getInt());
		exp_ = csv[rand][4].getInt();

		auto& filePath = csv[rand][5].getString();
		enemyTexture_ = dxe::Texture::CreateFromFile(filePath);
	}

}