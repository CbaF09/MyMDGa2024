#pragma once
#include "Base_MultiMeshObject.h"
#include "../Object/EnemyData.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	/// <summary>
	/// 
	/// 3D��ł̃G�l�~�[
	/// �����ڂƓ����𐧌�
	/// 
	/// </summary>

	class PlayerPawn;

	class EnemyPawn : public Base_MultiMeshObject {
	public:
		// arg1 ... �G�l�~�[�𐶐�����ʒu
		EnemyPawn(const tnl::Vector2i& enemyPos);
		~EnemyPawn();

		// �Q�b�^�[ ( ���Ɉړ����I������ )
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		// �Q�b�^�[ ( ���ɍs�����I������ )
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }
		// �Q�b�^�[ ( ���S���o���I���A���S������ )
		inline const bool getIsDead() const{ return isDead_; }
		// �Q�b�^�[ ( �G�l�~�[�|�[�������G�l�~�[�f�[�^ )
		inline const Shared<EnemyData> getEnemyData() const { return enemyData_; }

		// �s���t���O���I�t��
		inline void offFlagIsAlreadyTurn() { 
			isAlreadyAction_ = false; 
			isAlreadyMove_ = false; 
		}
		
		// �G�l�~�[�̃V�[�P���X�A�b�v�f�[�g
		inline bool enemyUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return true;
		}

		// �e���b�V���������Ȃ̂ŁA�e���b�V���̕`��Ǝq���b�V���̕`��𕪂���
		// �s�����ȕ��̂̕`��
		void renderObjects(const Shared<Atl3DCamera> camera, float deltaTime) override;
		// �����ȕ��̂̕`��ƈʒu�␳
		void renderTransparentObject(const Shared<Atl3DCamera> camera, float deltaTime) override;

		// �_���W�����V�[���ւ̎�Q�Ƃ�ݒ�
		void assignWeakDungeonScene(std::weak_ptr<DungeonScene> dungeonScene);

	private:
		//-----------------------
		// �ϐ�
		
		// ���b�V���̐����p
		// �G�l�~�[�̑傫��
		const float ENEMY_SIZE_RADIUS = 200;
		// �G�l�~�[��Y�̍���
		const float ENEMY_POS_Y = 400;

		// �ړ��p
		// �ړ���
		tnl::Vector3 moveTarget_{ 0,0,0 };
		// �ړ��ɂ����鎞�� ( �l���傫���قǈړ��Ɏ��Ԃ������� ) 
		const float MOVE_TIME = 0.3f; 
		// �ڕW�n�_�ƌ��݈ʒu�̍������̒l�ȉ��ł���΁A�ړ��͏I�������Ɣ��肳���
		const float MOVE_END_BORDER = 0.1f; 
		// lerp�ړ��p�^�C���J�E���^�[
		float moveLerpTimeCount_ = 0; 
		// sin�֐��ŏ㉺�ɂӂ�ӂ킳����ׂ̃^�C���J�E���^�[
		float hoverSinTimer_ = 0; 
		// �㉺�^���̐U��
		float hoverSinAmplitude_ = 100.0f; 

		// �^�[������p
		// ���Ɉړ����I������
		bool isAlreadyMove_ = false;
		// ���ɍs�����I������ ( �U���Ȃ� )
		bool isAlreadyAction_ = false;
		// ���S���o���I����āA���S������
		bool isDead_ = false;

		// �_���W�����V�[���ւ̎�Q��
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		// �G�l�~�[�̓G�l�~�[�f�[�^����ێ�����
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
		// ret ... �אڂ��Ă��� => true, �אڂ��Ă��Ȃ� => false
		bool isNeighborPlayer();
		
		// �v���C���[�Ɠ����G���A�ɂ��邩
		// ret ... �v���C���[�Ɠ����G���A�ɂ��� => true, ���Ȃ� => false
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