#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;

	class MagicWand final : public Base_MultiMeshObject {
	public:
		explicit MagicWand(std::weak_ptr<const PlayerPawn> player);

		void adjustChildsMeshes(float deltaTime) override;

	private:
		tnl::Quaternion initRot_;
		std::weak_ptr<const PlayerPawn> weakPlayerPawn;
	};

}