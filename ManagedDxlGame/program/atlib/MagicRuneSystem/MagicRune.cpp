#include "../../dxlib_ext/dxlib_ext.h"
#include "MagicRune.h"
#include "MagicRuneSystemManager.h"
#include "../Scenes/DungeonScene.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	/// <summary>
	/// �����̃��[��
	/// </summary>

	void HealRune::onNotify(e_EventType eventType,DungeonScene& dungeonScene) {
		if (eventType == e_EventType::TurnStart) {
			dungeonScene.turnHealHP();
		}
	}
	
	/// <summary>
	/// ���̃��[��
	/// </summary>
}