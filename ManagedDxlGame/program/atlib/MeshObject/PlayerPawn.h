#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Utilities/Atl3DCamera.h"
#include "../MeshObject/MagicWand.h"
#include "../MeshObject/MagicBook.h"
#include "../MeshObject/ForwardArrow.h"

namespace atl {

	class PlayerPawn final : public std::enable_shared_from_this<PlayerPawn> {
	public:
		~PlayerPawn() { explosion_.reset(); }

		// �Q�b�^�[
		inline const Shared<Atl3DCamera> getPlayerCamera() const { return playerCamera_; }
		inline const tnl::Vector3& getPlayerPos() const { return player3Dpos_; }
		inline const tnl::Vector2i& getPlayer2Dpos() const { return player2Dpos_; }
		inline bool getIsAlreadyTurn() const { return isAlreadyTurn_; }

		// �Z�b�^�[
		inline void setIsAlreadyTurn(bool flag = false) { isAlreadyTurn_ = flag; }
		void setPlayerAndCamera3Dpos(const tnl::Vector3& newPos);

		// 2D���W��̈ʒu�� Spawn ����
		void playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos);
		
		void debug_displayPlayerParam(int drawPosX = 0, int drawPosY = 0);

		// �V�[�P���X�Ǘ����s�p
		bool playerUpdate(float deltaTime) {
			seq_.update(deltaTime);
			return false;
		}

		// render�n
		void render(float deltaTime) {
			//forwardArrow_->renderObject(playerCamera_);
			playerHaveMagicWand_->renderObjects(playerCamera_);
			playerHaveMagicBook_->renderObject(playerCamera_);

			dxe::DirectXRenderBegin();
			explosion_->render(playerCamera_);
			dxe::DirectXRenderEnd();
		}

		void initialize() {
			playerHaveMagicWand_ = std::make_shared<MagicWand>(std::weak_ptr<PlayerPawn>(shared_from_this()));
			playerHaveMagicBook_ = std::make_shared<MagicBook>(std::weak_ptr<PlayerPawn>(shared_from_this()));
			forwardArrow_ = std::make_shared<ForwardArrow>(std::weak_ptr<PlayerPawn>(shared_from_this()));
			player3Dpos_ = playerCamera_->pos_;
		}

	private:
		// --------------------------------------------------
		enum class e_XZdir {
			NONE,
			Zplus,
			Zminus,
			Xplus,
			Xminus
		};
		
		// --------------------------------------------------
		// �����o�[�ϐ�

		Shared<Atl3DCamera> playerCamera_ = std::make_shared<Atl3DCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);;
		Shared<MagicWand> playerHaveMagicWand_ = nullptr;
		Shared<MagicBook> playerHaveMagicBook_ = nullptr;
		Shared<ForwardArrow> forwardArrow_ = nullptr;

		tnl::Vector2i player2Dpos_{ 0,0 };
		tnl::Vector3 player3Dpos_{ 0,0,0 };
		const float PLAYER_HEAD_LINE = 500;		// �v���C���[��Y�����i�J�����E�ڐ��̍����j

		// moveLerp�p
		const float MOVE_TIME = 1.5f;		// 1�}�X�ړ��ɂ����鑬�x ( �l���傫���قǁA���Ԃ������� )
		float moveLerpTimeCount_ = 0;
		tnl::Vector3 moveTarget_{ 0,0,0 };
		
		// �p�[�e�B�N���p
		Shared<dxe::Particle> explosion_ = std::make_shared<dxe::Particle>("graphics/particle/explosion.bin");

		// �^�[������p
		bool isAlreadyTurn_ = false;

		// --------------------------------------------------
		// ���\�b�h
		
		// arg ... ���݈ʒu����̈ړ���
		bool isCanMovePos(const tnl::Vector2i& moveToPos);
		e_XZdir checkCurrentFowardDir();
		void calcDirAndMoveSeqChange();

		// --------------------------------------------------
		// �V�[�P���X�p
		
		SEQUENCE(PlayerPawn, &PlayerPawn::seqWaitKeyInput);
		
		bool seqWaitKeyInput(float deltaTime);
		bool seqMoveZplus(float deltaTime);
		bool seqMoveZminus(float deltaTime);
		bool seqMoveXplus(float deltaTime);
		bool seqMoveXminus(float deltaTime);

		bool actionMoveLerp(float deltaTime);
		bool actionAttack(float deltaTime);
	};

}