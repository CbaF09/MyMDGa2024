#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;
	class ItemData;

	class ItemPawn final : public Base_MultiMeshObject {
	public:
		~ItemPawn() { itemParticle_.reset(); }

		explicit ItemPawn(const tnl::Vector2i& spawnPos);
		
		// ゲッター
		inline const Shared<ItemData> getItemData() { return itemData_; }

		//　オーバーライド
		void adjustChildsMeshes() override;
		void renderObjects(const Shared<Atl3DCamera> camera) override;

		// プレイヤーへの弱参照を設定
		void assignWeakPlayer(std::weak_ptr<PlayerPawn> player);

	private:
		// プレイヤーへの弱参照
		std::weak_ptr<PlayerPawn> weakPlayer_;
		// アイテムポーンは、アイテムデータを持つ
		Shared<ItemData> itemData_ = std::make_shared<ItemData>();
		// アイテムの周りに浮かぶパーティクル用
		Shared<dxe::Particle> itemParticle_ = std::make_shared<dxe::Particle>("graphics/particle/ItemEffect.bin");
	};

}