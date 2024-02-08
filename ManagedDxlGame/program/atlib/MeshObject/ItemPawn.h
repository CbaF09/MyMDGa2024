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
		
		// �Q�b�^�[
		inline const Shared<ItemData> getItemData() { return itemData_; }

		//�@�I�[�o�[���C�h
		void adjustChildsMeshes(float deltaTime) override;
		void renderObjects(const Shared<Atl3DCamera> camera,float deltaTime) override;

		// �_���W�����V�[���ւ̎�Q�Ƃ�ݒ�
		void assignWeakDungeonScene(std::weak_ptr<DungeonScene> dungeonScene);

	private:
		// �_���W�����V�[���ւ̎�Q��
		std::weak_ptr<DungeonScene> weakDungeonScene_;
		// �A�C�e���|�[���́A�A�C�e���f�[�^������
		Shared<ItemData> itemData_;
		// �A�C�e���̎���ɕ����ԃp�[�e�B�N���p
		Shared<dxe::Particle> itemParticle_ = std::make_shared<dxe::Particle>("graphics/particle/ItemEffect.bin");
	};

}