#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	class PlayerPawn;

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... �G�l�~�[�𐶐�����ʒu
		// arg2 ... �G�l�~�[�̑傫��
		// arg3 ... �P�Z���̑S��
		EnemyPawn(const tnl::Vector2i& enemyPos , const tnl::Vector3& enemySize);

		void adjustmentChildMeshes() override;

		// �X�e�[�g�p enum
		enum class e_EnemyState {
			Wandering,	// �ړI�̂Ȃ��T��
			PlayerNeighboring, // ���̏�ő�����,�v���C���[�̕�������
		};

		// �Q�b�^�[
		inline const e_EnemyState& getCurrentEnemyState() const { return currentState_; }
		inline const bool getIsAlreadyAction() const { return isAlreadyAction; }

		// �Z�b�^�[
		inline void setEnemyState(const e_EnemyState& newState) { currentState_ = newState; }
		inline void setIsAlreadyAction(bool flag = false) { isAlreadyAction = flag; }
		
		// �G�l�~�[�̖��t���[���̍s��
		// setEnemyState �ŃX�e�[�g��ݒ肵�Ă���Ăяo�����ŁA�X�e�[�g�ɉ������s�������
		bool enemyUpdate(float deltaTime) {
			return seq_.update(deltaTime);
		}

		// �v���C���[�̈ʒu��2D���W�n�œ���
		const tnl::Vector2i& searchPlayerPos(Shared<PlayerPawn> player);
		// �v���C���[�ƑO�㍶�E�ŗאڂ��Ă��邩
		bool isNeighborPlayer(Shared<PlayerPawn> player);

	private:

		//----------------------
		// ���\�b�h
		// arg ... ���݈ʒu����̈ړ���
		bool isCanMovePos(const tnl::Vector2i& moveToPos);
		void moveLerp(float deltaTime);

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
		bool seqPlayerNeighboring(float deltaTime);

		bool seqMoveZplus(float deltaTime);
		bool seqMoveZminus(float deltaTime);
		bool seqMoveXplus(float deltaTime);
		bool seqMoveXminus(float deltaTime);

	};

}