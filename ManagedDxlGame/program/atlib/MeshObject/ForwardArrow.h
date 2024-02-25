#pragma once
#include "Base_MeshObject.h"

namespace atl {

	/// <summary>
	/// 
	/// �v���C���[�������Ă������ ( �J�����̌����ł͂Ȃ��A�ړ���E�U����ƂȂ���p ) �𕪂���₷������ׂ� 3DUI
	/// �v���C���[�������Ă���n�ʂɁA�g�̂悤�Ȋ����ŕ\������
	/// 
	/// </summary>

	class PlayerPawn;

	// �萔�n
	namespace {
		// �g�̑傫�� ( ���a )
		const float FORWARD_ARROW_SIZE = 300;
		// �g�̍��� ( -500 �Œn�ʂƓ����ʒu�B�n�ʂ��͂��ɏ�ɐݒ� )
		const float FORWARD_ARROW_Y = -480;
	}

	class ForwardArrow final : public Base_MeshObject {
	public:
		explicit ForwardArrow(std::weak_ptr<const PlayerPawn> player);

		void renderObject(const Shared<Atl3DCamera> camera,float deltaTime = 0) override;

	private:
		// �v���C���[�ւ̎�Q��
		std::weak_ptr<const PlayerPawn> weakPlayerPawn_;
	};

}