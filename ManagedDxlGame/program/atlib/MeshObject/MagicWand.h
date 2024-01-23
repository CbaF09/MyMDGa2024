#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;

	class MagicWand final : public Base_MultiMeshObject {
	public:
		MagicWand(std::weak_ptr<const PlayerPawn> player);
		MagicWand(const tnl::Vector2i spawnPos, float initSize);

		void adjustmentChildMeshes() override;

	private:
		std::weak_ptr<const PlayerPawn> weakPlayerPawn;
		float wandSize_ = 0.0f;
		bool isHeldByPlayer = false;
	};

}