#pragma once
#include "Base_ActorData.h"

namespace atl {

	class PlayerData : public Base_ActorData {
	public:
		PlayerData();

	private:
		int32_t level;
	};

}