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
		~EnemyPawn();

		// �Q�b�^�[
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }
		inline const bool getIsDead() const{ return isDead_; }
		inline const Shared<EnemyData> getEnemyData() const { return enemyData_; }

		// �Z�b�^�[
		inline void offFlagIsAlreadyTurn() { isAlreadyAction_ = false; isAlreadyMove_ = false; }
		
		// �G�l�~�[�̖��t���[���̍s��
		bool enemyUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return true;
		}

		// �s�����ȕ��̂̕`��
		void renderObjects(const Shared<Atl3DCamera> camera, float deltaTime) override;
		// �����ȕ��̂̕`��ƈʒu�␳
		void renderTransparentObject(const Shared<Atl3DCamera> camera, float deltaTime) override;

		// �v���C���[�ւ̎�Q�Ƃ�ݒ�
		void assignWeakDungeonScene(std::weak_ptr<DungeonScene> player);

	private:
		//-----------------------
		// �ϐ�
		
		// ���b�V���̐����p
		// �G�l�~�[�̑傫��
		const float ENEMY_SIZE_RADIUS = 200;
		// �G�l�~�[��Y�̍���
		const float ENEMY_POS_Y = 400; // 

		// �ړ��p
		tnl::Vector3 moveTarget_{ 0,0,0 };
		const float MOVE_TIME = 0.3f; // �ړ��ɂ����鎞�� ( �l���傫���قǈړ��Ɏ��Ԃ������� ) 
		const float MOVE_END_BORDER = 0.1f; // �ڕW�n�_�ƌ��݈ʒu�̍������̒l�ȉ��ł���΁A�ړ��͏I�������Ɣ��肳���
		float moveLerpTimeCount_ = 0; // lerp�ړ��p�^�C���J�E���^�[
		float hoverSinTimer_ = 0; // sin�֐��ŏ㉺�ɂӂ�ӂ킳����ׂ̃^�C���J�E���^�[
		float hoverSinAmplitude_ = 100.0f; // �㉺�^���̐U��

		// �^�[������p
		bool isAlreadyMove_ = false;
		bool isAlreadyAction_ = false;
		bool isDead_ = false;

		// �_���W�����V�[���ւ̎�Q��
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		// �f�[�^�ێ�
		Shared<EnemyData> enemyData_ = std::make_shared<EnemyData>();

		//----------------------
		// ���\�b�h

		// ���b�V���Q�̈ʒu�𒲐�����BrenderObjects����Ăяo�����
		void adjustChildsMeshes(float deltaTime) override;

		// �ړ��ł��邩�ǂ����̔���
		// ret ... �ړ��ł��� => true , �ړ��ł��Ȃ� => false
		// arg ... ���݈ʒu����̈ړ���
		bool isCanMove(const tnl::Vector2i& moveToPos);
		// �ړ���̐ݒ� ( �V�[�P���X��seqMoveToTarget�ňړ����� )
		void setMoveTarget(const tnl::Vector2i& moveToPos);
		// �v���C���[�ƑO�㍶�E�ŗאڂ��Ă��邩
		bool isNeighborPlayer();
		// �v���C���[�Ɠ����G���A�ɂ��邩
		bool isSameAreaPlayer();

		//-----------------------
		// �V�[�P���X

		SEQUENCE(EnemyPawn, &EnemyPawn::seqStateTransition);
		bool seqStateTransition(float deltaTime);

		// �����_���ړ�
		bool seqWandering(float deltaTime);
		// �v���C���[�Ɍ������Ĉړ�
		bool seqChasePlayer(float deltaTime);
		// �v���C���[�ɗאڂ��Ă��鎞
		bool seqPlayerNeighboring(float deltaTime);
		// ���S���̉��o
		bool seqDeading(float deltaTime);

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