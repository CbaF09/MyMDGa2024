#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;
	class Item;

	class ItemPawn final : public Base_MultiMeshObject {
	public:
		~ItemPawn() { itemParticle_.reset(); }

		explicit ItemPawn(const tnl::Vector2i& spawnPos);
		
		void adjustChildsMeshes();
		void renderObjects(const Shared<Atl3DCamera> camera) override;

		void assignWeakPlayer(std::weak_ptr<PlayerPawn> player);

	private:
		std::weak_ptr<PlayerPawn> weakPlayer_;
		Shared<Item> itemData_ = std::make_shared<Item>();
		Shared<dxe::Particle> itemParticle_ = std::make_shared<dxe::Particle>("graphics/particle/ItemEffect.bin");
	};

}