#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;
	class ItemData;
	class DungeonScene;

	class ItemPawn final : public Base_MultiMeshObject {
	public:
		~ItemPawn() { itemParticle_.reset(); }

		explicit ItemPawn(const tnl::Vector2i& spawnPos);
		
		// ゲッター
		inline const Shared<ItemData> getItemData() { return itemData_; }

		//　オーバーライド
		void adjustChildsMeshes(float deltaTime) override;
		void renderObjects(const Shared<Atl3DCamera> camera,float deltaTime) override;

		// ダンジョンシーンへの弱参照を設定
		void assignWeakDungeonScene(std::weak_ptr<DungeonScene> dungeonScene);

	private:
		// ダンジョンシーンへの弱参照
		std::weak_ptr<DungeonScene> weakDungeonScene_;
		// アイテムポーンは、アイテムデータを持つ
		Shared<ItemData> itemData_;
		// アイテムの周りに浮かぶパーティクル用
		Shared<dxe::Particle> itemParticle_ = std::make_shared<dxe::Particle>("graphics/particle/ItemEffect.bin");
	};

}