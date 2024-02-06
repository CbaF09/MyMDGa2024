#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;
	class ItemData;

	class ItemPawn final : public Base_MultiMeshObject {
	public:
		~ItemPawn() { itemParticle_.reset(); }

		explicit ItemPawn(const tnl::Vector2i& spawnPos);
		
		// �Q�b�^�[
		inline const Shared<ItemData> getItemData() { return itemData_; }

		//�@�I�[�o�[���C�h
		void adjustChildsMeshes() override;
		void renderObjects(const Shared<Atl3DCamera> camera) override;

		// �v���C���[�ւ̎�Q�Ƃ�ݒ�
		void assignWeakPlayer(std::weak_ptr<PlayerPawn> player);

	private:
		// �v���C���[�ւ̎�Q��
		std::weak_ptr<PlayerPawn> weakPlayer_;
		// �A�C�e���|�[���́A�A�C�e���f�[�^������
		Shared<ItemData> itemData_ = std::make_shared<ItemData>();
		// �A�C�e���̎���ɕ����ԃp�[�e�B�N���p
		Shared<dxe::Particle> itemParticle_ = std::make_shared<dxe::Particle>("graphics/particle/ItemEffect.bin");
	};

}