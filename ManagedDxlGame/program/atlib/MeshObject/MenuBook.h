#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MeshObject.h"

namespace atl {

	/// <summary>
	/// 
	/// ����Ɏ����Ă���{ ( 3D )
	/// �����ڂƁA���j���[���J���Ă��邩�ǂ����̔���
	/// 
	/// </summary>

	class PlayerPawn;

	class MenuBook final : public Base_MeshObject {
	public:
		explicit MenuBook(std::weak_ptr<PlayerPawn> player);
		
		// ��Ԃɉ����Ďp�����ς��̂ŁA�␳���Ă��烌���_�[���邽�߁A�I�[�o�[���C�h
		void renderObject(const Shared<Atl3DCamera> camera, float deltaTime) override;

		// �Q�b�^�[ ( ���j���[�J���Ă����Ԃ� )
		inline const bool isOpenMenu() const { return isOpenMenu_; }
		
		// ���j���[�J���Ă���t���O���I��
		inline void openMenu() { isOpenMenu_ = true; }
		// ���j���[�J���Ă���t���O���I�t
		inline void closeMenu() { isOpenMenu_ = false; }

	private:
		// �ŏ��̉�]��Ԃ�ۑ�
		tnl::Quaternion initRot_;

		// �v���C���[�|�[���ւ̎�Q�� ( �v���C���[�ɒǏ]�����邽�� )
		std::weak_ptr<PlayerPawn> weakPlayerPawn;

		//�@���j���[���J���Ă��邩
		bool isOpenMenu_ = false;

	};

}