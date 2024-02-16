#include "ItemPawn.h"
#include "PlayerPawn.h"
#include "../Scenes/DungeonScene.h"
#include "../Object/Item.h"
#include "../Singletons/ResourceManager.h"

namespace atl {

	ItemPawn::ItemPawn(const tnl::Vector2i& spawnPos) {
		// �A�C�e���̏�񂪂Ȃ��ƃe�N�X�`���ݒ�ł��Ȃ��̂ŁA���b�V���̐����͒x���R���X�g���N�^�ōs��
		set2Dpos(spawnPos);
	}

	void ItemPawn::renderObject(const Shared<Atl3DCamera> camera, float deltaTime) {
		// �A�C�e���͂�������]����
		getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(ITEM_ROT_SPEED));

		getMesh()->render(camera);

		// �p�[�e�B�N���̃����_�[
		dxe::DirectXRenderBegin();
		itemParticle_->render(camera);
		dxe::DirectXRenderEnd();
	}

	void ItemPawn::initialize(std::weak_ptr<DungeonScene> dungeonScene) {
		// �_���W�����V�[���ւ̎�Q�Ƃ�ݒ�
		weakDungeonScene_ = dungeonScene;

		// �ێ�����A�C�e���̃f�[�^�𐶐�
		item_ = std::make_shared<Item>(weakDungeonScene_);

		// �����̂𐶐�
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

		// �����̂̎���ɕY���p�[�e�B�N��
		auto particle = ResourceManager::getResourceManager()->getParticleRes("graphics/particle/ItemEffect.bin");
		itemParticle_ = particle->createClone();
		itemParticle_->setPosition(getMesh()->pos_);
	}
	

}