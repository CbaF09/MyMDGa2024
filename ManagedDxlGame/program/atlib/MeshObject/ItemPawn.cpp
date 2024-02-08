#include "ItemPawn.h"
#include "PlayerPawn.h"
#include "../Scenes/DungeonScene.h"
#include "../Object/ItemData.h"

namespace atl {

	ItemPawn::ItemPawn(const tnl::Vector2i& spawnPos) {
		set2Dpos(spawnPos);
		auto cellLength = DungeonScene::getCellLength();
		auto item3Dpos = tnl::Vector3{ static_cast<float>(spawnPos.x * cellLength), 200, static_cast<float>(spawnPos.y * cellLength) };

		auto rootMesh = dxe::Mesh::CreateCubeMV(180);
		rootMesh->pos_ = item3Dpos;
		setRootMesh(rootMesh);

		itemParticle_->setPosition(getRootMesh()->pos_);
	}

	void ItemPawn::adjustChildsMeshes(float deltaTime) {
		getRootMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(0.5f));

		auto& childs = getChildMeshes();

		tnl::Vector3 playerPos{ 0,0,0 };
		auto lock = weakDungeonScene_.lock();
		if (lock) {
			playerPos = lock->getPlayerPawn()->getPlayerPos();
		}

		childs[0]->rot_ = tnl::Quaternion::RotationAxis({ 1,0,0 },tnl::ToRadian(-30)) * tnl::Quaternion::LookAt(childs[0]->pos_, playerPos, { 0,1,0 });
	}

	void ItemPawn::renderObjects(const Shared<Atl3DCamera> camera,float deltaTime) {
		adjustChildsMeshes(deltaTime);

		getRootMesh()->render(camera);
		auto& childs = getChildMeshes();
		for (const auto& child : childs) {
			child->render(camera);
		}

		dxe::DirectXRenderBegin();
		itemParticle_->render(camera);
		dxe::DirectXRenderEnd();
	}

	void ItemPawn::assignWeakDungeonScene(std::weak_ptr<DungeonScene> dungeonScene) {
		weakDungeonScene_ = dungeonScene;
		itemData_ = std::make_shared<ItemData>(weakDungeonScene_);

		// itemData ‚ªo—ˆ‚Ä‚©‚çAPlane ¶¬‚·‚é
		auto plane = dxe::Mesh::CreatePlaneMV({ 200,200,0 });
		plane->setTexture(itemData_->getItemIllust());
		plane->pos_ = getRootMesh()->pos_ + tnl::Vector3{0,200,0};
		addChildMesh(plane);
	}

	

}