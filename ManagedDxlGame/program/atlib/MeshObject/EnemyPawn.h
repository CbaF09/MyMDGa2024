#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... �G�l�~�[�𐶐�����ʒu
		// arg2 ... �G�l�~�[�̑傫��
		// arg3 ... �P�Z���̑S��
		EnemyPawn(const tnl::Vector3& enemyPos , const tnl::Vector3& enemySize, float CellLength);

		void adjustmentChildMeshes() override;

		// �X�e�[�g�}�V���p enum
		enum class e_EnemyState {
			Wandering,	// �ړI�̂Ȃ��T��
			Searching,	// �ړI�̂���T��
			Chasing,	// �v���C���[��ǐՒ�
		};

		// �Q�b�^�[
		inline const e_EnemyState& getCurrentEnemyState() const { return currentState_; }
		
		// �Z�b�^�[
		inline void setEnemySpeed(float enemyMoveSpeed) { enemyMoveSpeed_ = enemyMoveSpeed; }
		
		// �ڕW
		void registerTarget(const tnl::Vector3& target);

		// �G�l�~�[�̖��t���[���̍s��
		void enemyUpdate(float deltaTime);

	private:

		//----------------------
		// ���\�b�h
		
		inline void setNeedMoveAmount(float needMoveAmount) { needMoveAmount_ = needMoveAmount; }

		//-----------------------
		// �����o�ϐ�

		tnl::Vector3 enemySize_{ 0,0,0 };

		// �ړ��p
		tnl::Vector3 targetPos_ = { 0,0,0 };		// �ړI�n
		tnl::Quaternion targetRot_;
		float enemyMoveSpeed_ = 1.0f;			// �ړ����x
		float enemyRotateSpeed_ = 1.0f;			// ��]���x
		float oneCellLength_ = 0.0f;			// �P�}�X�̑傫��
		float needMoveAmount_ = 0.0f;			// �c��ړ��ʃJ�E���^�[

		// ���݂̃X�e�[�g
		e_EnemyState currentState_ = e_EnemyState::Wandering;

		//-----------------------
		// �V�[�P���X

		SEQUENCE(EnemyPawn, &EnemyPawn::seqCheckCurrentState);
		bool seqInit(float deltaTime) {/*������*/ };
		bool seqCheckCurrentState(float deltaTime);
		bool seqWandering(float deltaTime);
		bool seqSearching(float deltaTime) {/*������*/ };
		bool seqChasing(float deltaTime) {/*������*/ };
		bool seqRotate(float deltaTime);
		bool seqMove(float deltaTime);

	};

}