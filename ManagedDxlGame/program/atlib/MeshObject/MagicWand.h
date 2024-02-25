#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {
	
	/// <summary>
	/// 
	/// �E��Ɏ����Ă���� ( 3D )
	/// �����ڂ���
	/// 
	/// </summary>

	class PlayerPawn;

	// �萔�n
	namespace {
		// ��̑傫��
		const float WAND_SIZE = 25.0f;
	}

	class MagicWand final : public Base_MultiMeshObject {
	public:
		// �R���X�g���N�^
		explicit MagicWand(std::weak_ptr<PlayerPawn> player);

		// �q���b�V���̕␳ ( �ڍׂ� Base_MultiMeshObject )
		void adjustChildsMeshes(float deltaTime) override;

	private:
		// RootMesh �̍ŏ��̉�]��Ԃ�ۑ�
		tnl::Quaternion initRot_;

		// �v���C���[�|�[���ւ̎�Q�� ( �v���C���[�ɒǏ]�����邽�� )
		std::weak_ptr<PlayerPawn> weakPlayerPawn;
	};

}