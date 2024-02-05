#pragma once
#include "Base_MultiMeshObject.h"
#include "../Object/EnemyData.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	class PlayerPawn;

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... �G�l�~�[�𐶐�����ʒu
		EnemyPawn(const tnl::Vector2i& enemyPos);

		void adjustChildsMeshes() override;

		// �X�e�[�g�p enum
		enum class e_EnemyState {
			Wandering,			// �ړI�̂Ȃ��T��
			ChasePlayer,		// �v���C���[�Ɍ�����
			PlayerNeighboring,	// ���̏�ő�����,�v���C���[�̕�������
			Deading,			// ���S�� ( ���X�ɏ����Ă����Ȃǂ̏��� )
			Dead,				// ���S ( Deading ����J�� )
		};

		// �Q�b�^�[
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }
		inline const e_EnemyState& getCurrentState() const{ return currentState_; }
		inline const Shared<EnemyData> getEnemyData() const { return enemyData_; }

		// �Z�b�^�[
		inline void offFlagIsAlreadyTurn() { isAlreadyAction_ = false; isAlreadyMove_ = false; }
		
		// �G�l�~�[�̖��t���[���̍s��
		bool enemyUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return true;
		}

		// �v���C���[�ւ̎�Q�Ƃ�ݒ�
		void assignWeakDungeonScene(std::weak_ptr<DungeonScene> player);

	private:
		//----------------------
		// ���\�b�h
		
		// ret ... �ړ��ł��� => true , �ړ��ł��Ȃ� => false
		// arg ... ���݈ʒu����̈ړ���
		bool isCanMovePos(const tnl::Vector2i& moveToPos);
		void setMoveTarget(const tnl::Vector2i& moveToPos);
		// �v���C���[�ƑO�㍶�E�ŗאڂ��Ă��邩
		bool isNeighborPlayer();


		//-----------------------
		// �����o�ϐ�
		
		// ���b�V���̐����p
		const tnl::Vector3 ENEMY_SIZE{ 200,200,200 };

		// �ړ��p
		tnl::Vector3 moveTarget_{ 0,0,0 };
		const float MOVE_TIME = 1.0f; // �ړ��ɂ����鎞�� ( �l���傫���قǈړ��Ɏ��Ԃ������� ) 
		const float MOVE_END_BORDER = 0.1f; // �ڕW�n�_�ƌ��݈ʒu�̍������̒l�ȉ��ł���΁A�ړ��͏I�������Ɣ��肳���
		float moveLerpTimeCount_ = 0;

		// �^�[������p
		e_EnemyState currentState_ = e_EnemyState::Wandering;
		bool isAlreadyMove_ = false;
		bool isAlreadyAction_ = false;

		// �_���W�����V�[���ւ̎�Q��
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		// �f�[�^�ێ�
		Shared<EnemyData> enemyData_ = std::make_shared<EnemyData>();

		//-----------------------
		// �V�[�P���X

		SEQUENCE(EnemyPawn, &EnemyPawn::seqStateTransition);
		bool seqStateTransition(float deltaTime);

		bool seqWandering(float deltaTime);
		bool seqPlayerNeighboring(float deltaTime);
		bool seqDeading(float deltaTime);
		bool seqDead(float deltaTime);

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