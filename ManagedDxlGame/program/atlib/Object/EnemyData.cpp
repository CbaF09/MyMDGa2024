#include "EnemyData.h"
#include "../Utilities/AtlRandom.h"

namespace atl {

	EnemyData::EnemyData() {
		auto csv = tnl::LoadCsv("csv/EnemyCSV.csv");

		// CSVの中からランダムに設定
		auto rand = mtRandomRangeInt(1, csv.size() - 1);

		enemyID_ = csv[rand][0].getInt();
		enemyName_ = csv[rand][1].getString();
		setMaxHP(csv[rand][2].getInt());
		setCurrentHP(getMaxHP());
		setAttackPower(csv[rand][3].getInt());
		exp_ = csv[rand][4].getInt();

		enemyMaterialFilepath_ = csv[rand][5].getString();
	}

}