#include "EnemyData.h"
#include "../Utilities/AtlRandom.h"
#include "../Singletons/CsvManager.h"

namespace atl {

	EnemyData::EnemyData(int32_t enemyID) {
		// CSVから読み込む関係上、enemyIDが 0 以下の場合はリターンする
		if (enemyID <= 0) { return; } 
		
		auto& csv = CsvManager::getCsvManager()->getEnemyDataCSV();

		// 敵の名前
		enemyName_ = csv[enemyID][1].getString();

		// ステータス関連
		setMaxHP(csv[enemyID][2].getInt());
		setCurrentHP(getMaxHP());

		setAttackPower(csv[enemyID][3].getInt());
		setDefencePower(csv[enemyID][4].getInt());
		setTrueAttackPower(csv[enemyID][5].getInt());
		setTrueDefencePower(csv[enemyID][6].getInt());

		exp_ = csv[enemyID][7].getInt();

		// 敵の見た目 ( メッシュ等 ) 関連
		enemyMeshFilepath_ = csv[enemyID][8].getString();
		enemySizeScale_ = csv[enemyID][9].getFloat();
		enemyTextureFilepath_ = csv[enemyID][10].getString();
		enemyMaterialFilepath_ = csv[enemyID][11].getString();
	}

}