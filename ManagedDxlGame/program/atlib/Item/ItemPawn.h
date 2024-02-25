#pragma once
#include "../Item/Item.h"

namespace atl {

	/// <summary>
	/// 
	/// �_���W�����ɗ����Ă���A�C�e�� ( 3D )
	/// �A�C�e���̏��������Ă���A�E�����ŃC���x���g���ɒǉ��ł���
	/// 
	/// </summary>

	class PlayerPawn;
	class DungeonScene;

	// �萔�n
	namespace {
		// �A�C�e���|�[�� ( ������ ) �̑傫��
		const float ITEM_PAWN_SIZE = 200;
		// �A�C�e���|�[����Y���� ( �n�� �� -500 )
		const float ITEM_POSITION_Y = 250;
		// �A�C�e���|�[���̉�]���x
		const float ITEM_ROT_SPEED = 0.5f;
	}

	class ItemPawn final : public Base_MeshObject {
	public:
		~ItemPawn() { itemParticle_.reset(); }

		explicit ItemPawn(const tnl::Vector2i& spawnPos);
		
		// �Q�b�^�[
		inline const Shared<Item> getItem() { return item_; }

		//�@�I�[�o�[���C�h
		void renderObject(const Shared<Atl3DCamera> camera,float deltaTime) override;

		// �x���R���X�g���N�^
		void initialize();

	private:
		// �A�C�e���|�[���́A�A�C�e���f�[�^������
		Shared<Item> item_ = nullptr;
		// �A�C�e���̎���ɕ����ԃp�[�e�B�N���p
		Shared<dxe::Particle> itemParticle_ = nullptr;

	};

}