#pragma once
#include "Base_MultiMeshObject.h"
#include "../Object/EnemyData.h"

namespace atl {

	class PlayerPawn;

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... �G�l�~�[�𐶐�����ʒu
		// arg2 ... �G�l�~�[�̑傫��
		// arg3 ... �P�Z���̑S��
		EnemyPawn(const tnl::Vector2i& enemyPos , const tnl::Vector3& enemySize);

		void adjustChildsMeshes() override;

		// �X�e�[�g�p enum
		enum class e_EnemyMoveState {
			Wandering,	// �ړI�̂Ȃ��T��
			PlayerNeighboring, // ���̏�ő�����,�v���C���[�̕�������
		};

		// �Q�b�^�[
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }

		// �Z�b�^�[
		inline void setIsAlreadyTurn(bool flag = false) { isAlreadyAction_ = flag; isAlreadyMove_ = flag; }
		
		// �G�l�~�[�̖��t���[���̍s��
		bool enemyUpdate(float deltaTime) {
			return seq_.update(deltaTime);
		}

		// �v���C���[�ւ̎�Q�Ƃ�ݒ�
		void assignWeakPlayer(std::weak_ptr<PlayerPawn> player);

	private:
		//----------------------
		// ���\�b�h
		
		// ret ... �ړ��ł��� => true , �ړ��ł��Ȃ� => false
		// arg ... ���݈ʒu����̈ړ���
		bool assignMoveTarget(const tnl::Vector2i& moveToPos);
		// �v���C���[�ƑO�㍶�E�ŗאڂ��Ă��邩
		bool isNeighborPlayer();


		//-----------------------
		// �����o�ϐ�
		
		bool isAlreadyMove_ = false;
		bool isAlreadyAction_ = false;
		tnl::Vector3 enemySize_{ 0,0,0 };

		// �ړ��p
		tnl::Vector3 moveTarget_{ 0,0,0 };
		const float MOVE_TIME = 1.0f; // �ړ��ɂ����鎞�� ( �l���傫���قǈړ��Ɏ��Ԃ������� ) 
		float moveLerpTimeCount_ = 0;

		// ���݂̃X�e�[�g
		e_EnemyMoveState turnState_ = e_EnemyMoveState::Wandering;

		// �v���C���[�ւ̎�Q��
		std::weak_ptr<PlayerPawn> weakPlayer;

		// �f�[�^�ێ�
		Shared<EnemyData> enemyData_ = std::make_shared<EnemyData>();

		//-----------------------
		// �V�[�P���X

		SEQUENCE(EnemyPawn, &EnemyPawn::seqStateTransition);
		bool seqStateTransition(float deltaTime);

		bool seqWandering(float deltaTime);
		bool seqPlayerNeighboring(float deltaTime);

		// �ړ� ( �׈�}�X�� )
		bool seqMoveToTarget(float deltaTime);
		// �ړ� ( ������ )
		bool seqMoveNone(float deltaTime);

		// �s�� ( �U�� )
		bool seqActionAttack(float deltaTime);
		// �s�� ( ���� )
		bool seqActionNone(float deltaTime);

	};

}