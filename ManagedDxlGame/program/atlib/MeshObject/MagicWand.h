#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;

	class MagicWand final : public Base_MultiMeshObject {
	public:
		explicit MagicWand(std::weak_ptr<const PlayerPawn> player);
		MagicWand(const tnl::Vector2i spawnPos, float initSize);

		void adjustChildsMeshes() override;

	private:
		float wandSize_ = 0.0f;
		tnl::Quaternion initRot_;
		std::weak_ptr<const PlayerPawn> weakPlayerPawn;
		bool isHeldByPlayer = false;
	};

}