#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... �G�l�~�[�𐶐�����ʒu
		// arg2 ... �G�l�~�[�̑傫��
		// arg3 ... �P�Z���̑S��
		EnemyPawn(const tnl::Vector2i& enemyPos , const tnl::Vector3& enemySize);

		void adjustmentChildMeshes() override;

		// �X�e�[�g�}�V���p enum
		enum class e_EnemyState {
			Wandering,	// �ړI�̂Ȃ��T��
			Searching,	// �ړI�̂���T��
			Chasing,	// �v���C���[��ǐՒ�
		};

		// �Q�b�^�[
		inline const e_EnemyState& getCurrentEnemyState() const { return currentState_; }
		inline const bool getIsAlreadyAction() const { return isAlreadyAction; }

		// �Z�b�^�[
		inline void setEnemySpeed(float enemyMoveSpeed) { enemyMoveSpeed_ = enemyMoveSpeed; }
		
		// �G�l�~�[�̖��t���[���̍s��
		bool enemyUpdate(float deltaTime) {
			return seq_.update(deltaTime);
		}

	private:

		//----------------------
		// ���\�b�h
		// arg ... ���݈ʒu����̈ړ���
		bool checkIsCanMovePos(const tnl::Vector2i& moveToPos);
		bool moveLerp(float deltaTime);

		//-----------------------
		// �����o�ϐ�
		
		bool isAlreadyAction = false;
		tnl::Vector3 enemySize_{ 0,0,0 };

		// �ړ��p
		tnl::Vector3 moveTarget_{ 0,0,0 };
		const float MOVE_LERP_TIME_ = 0.5f;
		float moveLerpTimeCount_ = 0;
		float enemyMoveSpeed_ = 1.0f;			// �ړ����x
		float enemyRotateSpeed_ = 1.0f;			// ��]���x
		float needMoveAmount_ = 0.0f;			// �c��ړ��ʃJ�E���^�[

		// ���݂̃X�e�[�g
		e_EnemyState currentState_ = e_EnemyState::Wandering;

		//-----------------------
		// �V�[�P���X

		SEQUENCE(EnemyPawn, &EnemyPawn::seqCheckCurrentState);
		bool seqCheckCurrentState(float deltaTime);
		bool seqWandering(float deltaTime);
		bool seqSearching(float deltaTime) {/*������*/ };
		bool seqChasing(float deltaTime) {/*������*/ };

		bool seqMoveZplus(float deltaTime);
		bool seqMoveZminus(float deltaTime);
		bool seqMoveXplus(float deltaTime);
		bool seqMoveXminus(float deltaTime);

	};

}