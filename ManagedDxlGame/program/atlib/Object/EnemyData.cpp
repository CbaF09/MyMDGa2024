#include "EnemyData.h"
#include "../Utilities/AtlRandom.h"
#include "../Singletons/CsvManager.h"

namespace atl {

	EnemyData::EnemyData(int32_t enemyID) {
		// CSV����ǂݍ��ފ֌W��AenemyID�� 0 �ȉ��̏ꍇ�̓��^�[������
		if (enemyID <= 0) { return; } 
		
		auto& csv = CsvManager::getCsvManager()->getEnemyDataCSV();

		// �G�̖��O
		enemyName_ = csv[enemyID][1].getString();

		// �X�e�[�^�X�֘A
		setMaxHP(csv[enemyID][2].getInt());
		setCurrentHP(getMaxHP());

		setAttackPower(csv[enemyID][3].getInt());
		setDefencePower(csv[enemyID][4].getInt());
		setTrueAttackPower(csv[enemyID][5].getInt());
		setTrueDefencePower(csv[enemyID][6].getInt());

		exp_ = csv[enemyID][7].getInt();

		// �G�̌����� ( ���b�V���� ) �֘A
		enemyMeshFilepath_ = csv[enemyID][8].getString();
		enemySizeScale_ = csv[enemyID][9].getFloat();
		enemyTextureFilepath_ = csv[enemyID][10].getString();
		enemyMaterialFilepath_ = csv[enemyID][11].getString();
	}

}