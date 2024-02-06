#pragma once
#include "Base_ActorData.h"
#include "Inventory.h"

namespace atl {

	class PlayerData : public Base_ActorData {
	public:
		PlayerData();

		inline const Shared<Inventory> getInventory() { return playerInventory_; }

	private:
		// �v���C���[�̃C���x���g��
		Shared<Inventory> playerInventory_ = std::make_shared<Inventory>();
	};

}