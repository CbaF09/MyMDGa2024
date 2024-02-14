#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;

	class MagicWand final : public Base_MultiMeshObject {
	public:
		explicit MagicWand(std::weak_ptr<PlayerPawn> player);

		void adjustChildsMeshes(float deltaTime) override;

	private:
		tnl::Quaternion initRot_;
		const float WAND_SIZE = 25.0f;
		std::weak_ptr<PlayerPawn> weakPlayerPawn;
	};

}