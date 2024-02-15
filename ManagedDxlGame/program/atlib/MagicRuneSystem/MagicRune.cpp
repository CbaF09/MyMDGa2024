#include "../../dxlib_ext/dxlib_ext.h"
#include "MagicRune.h"
#include "../Scenes/DungeonScene.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	/// <summary>
	/// �����̃��[��
	/// </summary>
	HealRune::HealRune() {
		setRuneGraph(ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/MagicRuneGreen.png"));
	}

	// �^�[���X�^�[�g���ɁA�ǉ��ő̗͉񕜂���
	void HealRune::onNotify(e_EventType eventType,DungeonScene& dungeonScene) {
		if (eventType == e_EventType::TurnStart) {
			dungeonScene.turnHealHP();
		}
	}
	
	/// <summary>
	/// ���̃��[��
	/// </summary>
	FireRune::FireRune() {
		setRuneGraph(ResourceManager::getResourceManager()->getGraphRes("graphics/UI/MagicRune/MagicRuneRed.png"));
	}
}