#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Utilities/Atl3DCamera.h"
#include "../Scenes/DungeonScene.h"

#include "../Object/PlayerData.h"

namespace atl {

	class DungeonScene;
	class MagicWand;
	class MenuBook;
	class ForwardArrow;

	class PlayerPawn final : public std::enable_shared_from_this<PlayerPawn> {
	public:
		PlayerPawn();
		~PlayerPawn();

		// �x���R���X�g���N�^�B������Ɉ�񂾂��ĂԎ�
		void initialize(std::weak_ptr<DungeonScene> dungeonScene);

		// �f�o�b�O�p�BdungeonScene�ւ̎�Q�Ƃ������Ȃ�
		void initialize();
		
		// �Q�b�^�[
		inline const Shared<Atl3DCamera> getPlayerCamera() const { return playerCamera_; }
		inline const tnl::Vector3& getPlayerPos() const { return player3Dpos_; }
		inline const tnl::Vector2i& getPlayer2Dpos() const { return player2Dpos_; }
		inline bool getIsAlreadyTurn() const { return isAlreadyTurn_; }
		inline const Shared<PlayerData> getPlayerData() const { return playerData_; }

		// �Z�b�^�[
		inline void offFlagIsAlreadyTurn() { isAlreadyTurn_ = false; }
		inline void onFlagIsAlreadyTurn() { isAlreadyTurn_ = true; }
		void setPlayerAndCamera3Dpos(const tnl::Vector3& newPos);

		// ���j���[���J�� ( �{�𐳖ʂ̈ʒu�� ) 
		void openMenuBook();
		// ���j���[����� ( �{�����̈ʒu�� ) 
		void closeMenuBook();

		// 2D���W��̈ʒu�� Spawn ����
		void playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos);
		

		// �V�[�P���X�Ǘ����s�p
		bool playerUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return false;
		}

		// render�n
		void render(float deltaTime);


		void debug_displayPlayerParam(int drawPosX = 0, int drawPosY = 0);

	private:
		// --------------------------------------------------
		// �����o�[�ϐ�

		Shared<Atl3DCamera> playerCamera_ = std::make_shared<Atl3DCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
		Shared<MagicWand> playerHaveMagicWand_ = nullptr;
		Shared<MenuBook> playerHaveMenuBook_ = nullptr;
		Shared<ForwardArrow> forwardArrow_ = nullptr;
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		tnl::Vector2i player2Dpos_{ 0,0 };
		tnl::Vector3 player3Dpos_{ 0,0,0 };
		const float PLAYER_HEAD_LINE = 500;		// �v���C���[��Y�����i�J�����E�ڐ��̍����j

		// moveLerp�p
		const float MOVE_TIME = 0.6f;		// 1�}�X�ړ��ɂ����鑬�x ( �l���傫���قǁA���Ԃ������� )
		const float MOVE_END_BORDER = 0.1f; // �ڕW�n�_�ƌ��݈ʒu�̍������̒l�ȉ��ł���΁A�ړ��͏I�������Ɣ��肳���
		float moveLerpTimeCount_ = 0;
		tnl::Vector3 moveTarget_{ 0,0,0 };
		
		// �p�[�e�B�N���p
		Shared<dxe::Particle> explosion_ = std::make_shared<dxe::Particle>("graphics/particle/explosion.bin");

		// �^�[������p
		bool isAlreadyTurn_ = false;
		float totalDeltaTimer_ = 0.0f; // ��~���ԗp�^�C�}�[
		float waitTime_ = 0.0f; // ���ۂ̒�~���Ԑݒ�p
		const float ATTACK_TIME = 1.5f;	// �U���������̒�~����
		const float ATTACK_MISS_TIME = 0.5f; // �U������U�肵�����̒�~����


		// --------------------------------------------------
		// �X�e�[�^�X�p
		Shared<PlayerData> playerData_ = std::make_shared<PlayerData>();

		// --------------------------------------------------
		// ���\�b�h
		
		// �ړ��ł��邩����
		bool isCanMovePos(const tnl::Vector2i& moveToPos);
		// �ړ�����w�� ( moveLerp�p ) 
		void setMoveTarget(const tnl::Vector2i& moveToPos);
		void changeMoveDirSeq();

		// --------------------------------------------------
		// �V�[�P���X�p
		
		SEQUENCE(PlayerPawn, &PlayerPawn::seqWaitKeyInput);
		
		bool seqWaitKeyInput(float deltaTime);

		bool actionMoveLerp(float deltaTime);
		bool actionAttack(float deltaTime);
		void attackHitEffectAndLog(const Shared<atl::EnemyPawn>& enemy);
	};

}