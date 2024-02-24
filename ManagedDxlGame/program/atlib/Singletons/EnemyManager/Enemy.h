#pragma once
#include "../../../dxlib_ext/dxlib_ext.h"
#include "../../Scenes/DungeonScene.h"
#include "../../Object/EnemyData.h"

namespace atl {

	/// <summary>
	/// 
	/// �S�ẴG�l�~�[�̊��N���X
	/// 
	/// protected �ŁA�V�[�P���T�[�ƃV�[�P���X�p�֐����p������B
	/// �p����ŃV�[�P���X�p�֐����I�[�o�[���C�h���鎖�ŁA�����X�^�[�ŗL�̓���������
	/// �p���N���X�𑝂₵����A1 �� 1 �̊֌W�ɂ���t�@�N�g���[�N���X����邱�� => EnemyFactory �Q��
	/// 
	/// <summary>
	/// 

	enum class e_EnemyName {
		NONE = 0,
		BLUE_SLIME = 1,
		GREEN_SLIME = 2,
		RED_SLIME = 3,
		ENEMY_MAX,
	};

	class Base_Enemy : public Base_MeshObject {
	public:
		virtual ~Base_Enemy() {};

		// ������
		virtual void initialize() = 0;

		// �v�Z�E�����p�֐��B��ɃV�[�P���X�̃A�b�v�f�[�g��z��
		virtual void process(float deltaTime) { sequenceUpdate(deltaTime); };

		// �Q�b�^�[ ( ���Ɉړ����I������ )
		inline const bool getIsAlreadyMove() const { return isAlreadyMove_; }
		// �Q�b�^�[ ( ���ɍs�����I������ )
		inline const bool getIsAlreadyAction() const { return isAlreadyAction_; }
		// �Q�b�^�[ ( ���S���o���I���A���S������ )
		inline const bool getIsDead() const { return isDead_; }
		// �Q�b�^�[ ( �G�l�~�[�|�[�������G�l�~�[�f�[�^ )
		inline const Shared<EnemyData> getEnemyData() const { return enemyData_; }

		// �Z�b�^�[ ( �G�l�~�[�f�[�^ �� ID ����ݒ� )
		inline void setEnemyData(int32_t enemyID) { enemyData_ = std::make_shared<EnemyData>(enemyID); }

		// �s���t���O���I�t��
		inline void offFlagIsAlreadyTurn() {
			isAlreadyAction_ = false;
			isAlreadyMove_ = false;
		}

		// �G�l�~�[�f�[�^�����ɁA���b�V��/�e�N�X�`��/�}�e���A�����܂Ƃ߂Đݒ肷��
		void setEnemyVisual(const Shared<EnemyData> enemyData);

	private:
		// �ړ��p
		// �ړ���
		tnl::Vector3 moveTarget_{ 0,0,0 };
		// �ړ��ɂ����鎞�� ( �l���傫���قǈړ��Ɏ��Ԃ������� ) 
		const float MOVE_TIME = 0.3f;
		// �ڕW�n�_�ƌ��݈ʒu�̍������̒l�ȉ��ł���΁A�ړ��͏I�������Ɣ��肳���
		const float MOVE_END_BORDER = 0.1f;
		// lerp�ړ��p�^�C���J�E���^�[
		float moveLerpTimeCount_ = 0;

		// �^�[������p
		// ���Ɉړ����I������
		bool isAlreadyMove_ = false;
		// ���ɍs�����I������ ( �U���Ȃ� )
		bool isAlreadyAction_ = false;
		// ���S���o���I����āA���S������
		bool isDead_ = false;

		// �G�l�~�[�̓G�l�~�[�f�[�^����ێ�����
		Shared<EnemyData> enemyData_ = nullptr;

		// �V�[�P���T�[
		SEQUENCE(Base_Enemy, &Base_Enemy::seqTransition);

	protected:
		// �V�[�P���X�p�֐�

		// �V�[�P���X�A�b�v�f�[�g�p ( �V�[�P���X�ϐ��� private �ɉB�����Ă���̂� )
		inline void sequenceUpdate(float deltaTime) { seq_.update(deltaTime); }

		// �ړ��ł��邩�ǂ����̔���
		// ret ... �ړ��ł��� => true , �ړ��ł��Ȃ� => false
		// arg ... ���݈ʒu����̈ړ���
		bool isCanMove(const tnl::Vector2i& moveToPos) const;

		// �ړ���̐ݒ� ( �V�[�P���X��seqMoveToTarget�ňړ����� )
		void setMoveTarget(const tnl::Vector2i& moveToPos);

		// �v���C���[�ƑO�㍶�E�ŗאڂ��Ă��邩
		// ret ... �אڂ��Ă��� => true, �אڂ��Ă��Ȃ� => false
		bool isNeighborPlayer() const;

		// �v���C���[�Ɠ����G���A�ɂ��邩
		// ret ... �v���C���[�Ɠ����G���A�ɂ��� => true, ���Ȃ� => false
		bool isSameAreaPlayer() const;

		// �V�[�P���X�̑J�ڂ��m�F����
		virtual bool seqTransition(float deltaTime);

		// �����_���ړ�
		virtual bool seqWandering(float deltaTime);
		// �v���C���[�Ɍ������Ĉړ�
		virtual bool seqChasePlayer(float deltaTime);
		// �v���C���[�ɗאڂ��Ă��鎞
		virtual bool seqPlayerNeighboring(float deltaTime);
		// ���S���̉��o
		virtual bool seqDeading(float deltaTime);

		// �ړ� ( �׈�}�X�� )
		virtual bool seqMoveToTarget(float deltaTime);
		// �ړ� ( ������ )
		virtual bool seqMoveNone(float deltaTime);

		// �s�� ( �U�� )
		virtual bool seqActionAttack(float deltaTime);
		// �s�� ( ���� )
		virtual bool seqActionNone(float deltaTime);


	};

	/// <summary>
	/// 
	/// �X���C��
	/// 
	/// </summary>
	class BlueSlime final : public Base_Enemy {
	public:
		void initialize() override;

		// �ӂ�ӂ킵�������������邽�߁ArenderObject ���I�[�o�[���C�h
		void renderObject(Shared<Atl3DCamera> camera, float deltaTime) override;

	private:
		// �����g�łӂ�ӂ킳����ׂ̃^�C�}�[
		float totalDeltaTimer_ = 0;
		// sin �̐U�ꕝ
		const float AMPLITUDE = 64;
		// sin �̎��g��
		const float FREQUENCY = 2;

		// �ӂ�ӂ킵�����ɏ��ɓ˂�������̂�h���ׂ̌v�Z�p Y���W
		const float INIT_POS_Y = AMPLITUDE;
	};

	/// <summary>
	/// 
	/// �΃X���C��
	/// 
	/// </summary>
	class GreenSlime final : public Base_Enemy {
	public:
		void initialize() override;

		// �ӂ�ӂ킵�������������邽�߁ArenderObject ���I�[�o�[���C�h
		void renderObject(Shared<Atl3DCamera> camera, float deltaTime) override;

	private:
		// �����g�łӂ�ӂ킳����ׂ̃^�C�}�[
		float totalDeltaTimer_ = 0;
		// sin �̐U�ꕝ
		const float AMPLITUDE = 64;
		// sin �̎��g��
		const float FREQUENCY = 2.5f;

		// �ӂ�ӂ킵�����ɏ��ɓ˂�������̂�h���ׂ̌v�Z�p Y���W
		const float INIT_POS_Y = AMPLITUDE;
	};


	/// <summary>
	/// 
	/// �ԃX���C��
	/// 
	/// </summary>
	class RedSlime final : public Base_Enemy {
	public:
		void initialize() override;

		// �ӂ�ӂ킵�������������邽�߁ArenderObject ���I�[�o�[���C�h
		void renderObject(Shared<Atl3DCamera> camera, float deltaTime) override;

	private:
		// �����g�łӂ�ӂ킳����ׂ̃^�C�}�[
		float totalDeltaTimer_ = 0;
		// sin �̐U�ꕝ
		const float AMPLITUDE = 64;
		// sin �̎��g��
		const float FREQUENCY = 3;

		// �ӂ�ӂ킵�����ɏ��ɓ˂�������̂�h���ׂ̌v�Z�p Y���W
		const float INIT_POS_Y = AMPLITUDE;
	};

}