#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;
	class Item;
	class DungeonScene;

	class ItemPawn final : public Base_MultiMeshObject {
	public:
		~ItemPawn() { itemParticle_.reset(); }

		explicit ItemPawn(const tnl::Vector2i& spawnPos);
		
		// �Q�b�^�[
		inline const Shared<Item> getItem() { return item_; }

		//�@�I�[�o�[���C�h
		void adjustChildsMeshes(float deltaTime) override;
		void renderObjects(const Shared<Atl3DCamera> camera,float deltaTime) override;

		// �_���W�����V�[���ւ̎�Q�Ƃ�ݒ�
		void assignWeakDungeonScene(std::weak_ptr<DungeonScene> dungeonScene);

	private:
		// �_���W�����V�[���ւ̎�Q��
		std::weak_ptr<DungeonScene> weakDungeonScene_;
		// �A�C�e���|�[���́A�A�C�e���f�[�^������
		Shared<Item> item_ = nullptr;
		// �A�C�e���̎���ɕ����ԃp�[�e�B�N���p
		Shared<dxe::Particle> itemParticle_ = nullptr;
	};

}