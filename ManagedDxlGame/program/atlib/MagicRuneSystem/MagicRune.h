#pragma once
#include "Base_MagicRune.h"

namespace atl {

	// �����̃��[��
	class HealRune : public Base_MagicRune {
	public:
		HealRune();
		void onNotify(e_EventType eventType,DungeonScene& dungeonScene) override;
	};

	// ���̃��[��
	class FireRune : public Base_MagicRune {
	public:
		FireRune();
		// �������L�q
		void onNotify(e_EventType eventType, DungeonScene& dungeonScene) override {};
	};

}