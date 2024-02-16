#include "ItemPawn.h"
#include "PlayerPawn.h"
#include "../Scenes/DungeonScene.h"
#include "../Object/Item.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	ItemPawn::ItemPawn(const tnl::Vector2i& spawnPos) {
		// アイテムの情報がないとテクスチャ設定できないので、メッシュの生成は遅延コンストラクタで行う
		set2Dpos(spawnPos);
	}

	void ItemPawn::renderObject(const Shared<Atl3DCamera> camera, float deltaTime) {
		// アイテムはゆっくり回転する
		getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(ITEM_ROT_SPEED));

		getMesh()->render(camera);

		// パーティクルのレンダー
		dxe::DirectXRenderBegin();
		itemParticle_->render(camera);
		dxe::DirectXRenderEnd();
	}

	void ItemPawn::initialize(std::weak_ptr<DungeonScene> dungeonScene) {
		// ダンジョンシーンへの弱参照を設定
		weakDungeonScene_ = dungeonScene;

		// 保持するアイテムのデータを生成
		item_ = std::make_shared<Item>(weakDungeonScene_);

		// 立方体を生成
		auto& spawnPos = get2Dpos();
		auto cellLength = DungeonScene::getCellLength();
		auto item3Dpos = tnl::Vector3{ static_cast<float>(spawnPos.x * cellLength), ITEM_POSITION_Y, static_cast<float>(spawnPos.y * cellLength) };
		auto texture = item_->getItemIllust();

		auto mesh = dxe::Mesh::CreateBoxMV({ ITEM_PAWN_SIZE,ITEM_PAWN_SIZE,ITEM_PAWN_SIZE },
			texture,
			texture,
			texture,
			texture,
			texture,
			texture
		);
		mesh->pos_ = item3Dpos;
		setMesh(mesh);

		// 立方体の周りに漂うパーティクル
		auto particle = ResourceManager::getResourceManager()->getParticleRes("graphics/particle/ItemEffect.bin");
		itemParticle_ = particle->createClone();
		itemParticle_->setPosition(getMesh()->pos_);
	}
	

}