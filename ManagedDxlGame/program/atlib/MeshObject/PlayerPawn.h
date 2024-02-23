#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Utilities/Atl3DCamera.h"
#include "../Scenes/DungeonScene.h"
#include "../Object/PlayerData.h"
#include "../Enemy/Enemy.h"
#include "MenuBook.h"

namespace atl {

	/// <summary>
	/// 
	/// 3D��Ńv���C���[�����삷��v���C���[�|�[��
	/// �v���C���[�̓��͂ɑ΂��Ĕ�������N���X
	/// 
	/// </summary>

	class DungeonScene;
	class MagicWand;
	class ForwardArrow;

	class PlayerPawn final : public std::enable_shared_from_this<PlayerPawn> {
	public:
		PlayerPawn();
		~PlayerPawn();

		// �x���R���X�g���N�^�B������Ɉ�񂾂��ĂԎ�
		void initialize();

		// �Q�b�^�[ ( �J���� )
		inline const Shared<Atl3DCamera> getPlayerCamera() const { return playerCamera_; }
		// �Q�b�^�[ ( �v���C���[�|�[���������Ă���v���C���[�̃X�e�[�^�X��� )
		inline const Shared<PlayerData> getPlayerData() const { return playerData_; }
		// �Q�b�^�[ ( ���ɍs���I�������� )
		inline bool getIsAlreadyTurn() const { return isAlreadyTurn_; }
		// �Q�b�^�[ ( ���j���[���J���Ă����Ԃ� )
		inline bool getIsMenuOpen() const { return playerHaveMenuBook_->isOpenMenu(); }
		// �Q�b�^�[ ( 3D��̈ʒu )
		inline const tnl::Vector3& getPlayerPos() const { return player3Dpos_; }
		// �Q�b�^�[ ( 2D��̍��W )
		inline const tnl::Vector2i& getPlayer2Dpos() const { return player2Dpos_; }

		// �Z�b�^�[ ( �s�������t���O���I�t�ɂ��� )
		inline void offFlagIsAlreadyTurn() { isAlreadyTurn_ = false; }
		// �Z�b�^�[ ( �s�������t���O���I���ɂ���ɂ��� )
		inline void onFlagIsAlreadyTurn() { isAlreadyTurn_ = true; }

		// ���j���[���J�� ( �{�𐳖ʂ̈ʒu�� ) 
		void openMenuBook();
		// ���j���[����� ( �{�����̈ʒu�� ) 
		void closeMenuBook();

		// �X�|�[���������s���ׂ̊֐�
		// arg ... �X�|�[������2D��̍��W
		void playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos);

		// �V�[�P���X���s�p
		inline bool playerUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return false;
		}

		// �ړ��A�N�V�������X�^�[�g
		// ret ... �ړ��悪�ړ��\ => true , �ړ��s�\ => false
		bool startMove();
		// �U���A�N�V�������X�^�[�g
		void startAttack();

		// render�n
		void render(float deltaTime);

		// --------------------------------------------------
		// �f�o�b�O�p
		// �v���C���[�|�[���̏���F�X�o��
		void debug_displayPlayerParam(int drawPosX = 0, int drawPosY = 0);

	private:
		// --------------------------------------------------
		// �����o�[�ϐ�
		
		// �v���C���[��2D��̍��W
		tnl::Vector2i player2Dpos_{ 0,0 };
		// �v���C���[��3D��̈ʒu
		tnl::Vector3 player3Dpos_{ 0,0,0 };
		// �v���C���[��Y�����i�J�����E�ڐ��̍���, 0 �Œn�ʂƓ����j
		const float PLAYER_HEAD_LINE = 500;

		// seqMoveLerp�p
 		// �^�C�}�[
		float moveLerpTimeCount_ = 0;
		// �ڕW�n�_
		tnl::Vector3 moveTarget_{ 0,0,0 };
		// 1�}�X�ړ��ɂ����鑬�x ( �l���傫���قǁA���Ԃ������� )
		const float MOVE_TIME = 0.35f;		
		// �ڕW�n�_�ƌ��݈ʒu�̍������̒l�ȉ��ł���΁A�ړ��͏I�������Ɣ��肳���
		const float MOVE_END_BORDER = 0.1f; 


		// ���ɍs��������
		bool isAlreadyTurn_ = false;
		// ��~���ԗp�^�C�}�[
		float totalDeltaTimer_ = 0.0f; 
		// ���ۂ̒�~���Ԑݒ�p
		float waitTime_ = 0.0f; 
		// �U���������̒�~����
		const float ATTACK_TIME = 0.5f;	
		// �U������U�肵�����̒�~����
		const float ATTACK_MISS_TIME = 0.05f; 

		// �J����
		Shared<Atl3DCamera> playerCamera_ = std::make_shared<Atl3DCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
		// �E��Ɏ����Ă����
		Shared<MagicWand> playerHaveMagicWand_ = nullptr;
		// ����̖{
		Shared<MenuBook> playerHaveMenuBook_ = nullptr;
		// �����Ă�������𕪂���₷������ׂ̘g
		Shared<ForwardArrow> forwardArrow_ = nullptr;
		// �U�����̃p�[�e�B�N�� ( ���� )
		Shared<dxe::Particle> explosion_ = std::make_shared<dxe::Particle>("graphics/particle/explosion.bin");

		// �X�e�[�^�X�p
		Shared<PlayerData> playerData_ = std::make_shared<PlayerData>();

		// --------------------------------------------------
		// ���\�b�h

		// �ړ��ł��邩����
		bool isCanMovePos(const tnl::Vector2i& moveToPos);
		// �ړ�����w�� ( moveLerp�p ) 
		void setMoveTarget(const tnl::Vector2i& moveToPos);
		// �U���������̉��o
		void attackHitEffectAndLog(const Shared<atl::Base_Enemy> enemy);


		// --------------------------------------------------
		// �V�[�P���X�p

		SEQUENCE(PlayerPawn, &PlayerPawn::seqWait);

		// �ҋ@���
		bool seqWait(float deltaTime);

		// lerp�ňړ�����
		bool seqMoveLerp(float deltaTime);

		// �U���������̑҂����� ( ��U�莞�ƃq�b�g���ňႤ )
		bool seqActionAttackDelay(float deltaTime);

	};

}