#pragma once
#include "Base_MeshObject.h"

namespace atl {

	class PlayerPawn;

	class ForwardArrow final : public Base_MeshObject {
	public:
		explicit ForwardArrow(std::weak_ptr<const PlayerPawn> player);

		void renderObject(const Shared<Atl3DCamera> camera,float deltaTime = 0) override;

	private:
		// プレイヤーへの弱参照
		std::weak_ptr<const PlayerPawn> weakPlayerPawn;

		const float FORWARD_ARROW_Y = -480;
	};

}