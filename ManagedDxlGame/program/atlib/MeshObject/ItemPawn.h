#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;

	class ItemPawn : public Base_MultiMeshObject {
	public:
		~ItemPawn() { itemParticle_.reset(); }

		explicit ItemPawn(const tnl::Vector2i& spawnPos);
		
		void adjustmentChildMeshes();
		void renderObjects(const Shared<Atl3DCamera> camera) override;

		void assignWeakPlayer(std::weak_ptr<PlayerPawn> player);

	private:
		std::weak_ptr<PlayerPawn> weakPlayer_;
		Shared<dxe::Particle> itemParticle_ = std::make_shared<dxe::Particle>("graphics/particle/ItemEffect.bin");
	};

}