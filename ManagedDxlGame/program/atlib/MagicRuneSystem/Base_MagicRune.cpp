#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MagicRune.h"
#include "MagicRuneSystemManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/CsvManager.h"

namespace atl {

	void Base_MagicRune::setRuneData(e_RuneID itemID) {
		// CSV�f�[�^�������Ă���
		auto& csv = CsvManager::getCsvManager()->getMagicRuneCSV();

		int index = static_cast<int>(itemID);

		// ���[������ݒ�
		runeName_ = csv[index][1].getString();
		// ���[���̐�������ݒ�
		runeDesc_ = csv[index][2].getString();
		// ���[���̃A�C�R���摜��ݒ�
		auto& graphHandlePath = csv[index][3].getString();
		graphHandle_ = ResourceManager::getResourceManager()->getGraphRes(graphHandlePath.c_str());
	}

}