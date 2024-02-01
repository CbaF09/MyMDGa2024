#pragma once
#include "Base_MeshObject.h"

namespace atl {

	class PlayerPawn;

	class ForwardArrow : public Base_MeshObject {
	public:
		explicit ForwardArrow(std::weak_ptr<const PlayerPawn> player);

		void renderObject(const Shared<Atl3DCamera> camera) const override;

	private:
		std::weak_ptr<const PlayerPawn> weakPlayerPawn;
	};

}