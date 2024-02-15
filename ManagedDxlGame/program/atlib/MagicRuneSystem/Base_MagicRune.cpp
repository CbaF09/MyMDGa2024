#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MagicRune.h"
#include "MagicRuneSystemManager.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	void Base_MagicRune::setRuneData(e_RuneID itemID) {
		auto& csv = MagicRuneSystemManager::getMagicRuneSystemManager()->getMagicRuneCSV();
		int id = static_cast<int>(itemID);

		// ���[������ݒ�
		runeName_ = csv[id][1].getString();
		// ���[���̐�������ݒ�
		runeDesc_ = csv[id][2].getString();
		// ���[���̃A�C�R���摜��ݒ�
		auto& graphHandlePath = csv[id][3].getString();
		graphHandle_ = ResourceManager::getResourceManager()->getGraphRes(graphHandlePath.c_str());
	}

}