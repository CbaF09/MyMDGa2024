#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../../atlib/Utilities/Atl3DCamera.h"
#include "../MeshObject/MagicWand.h"
#include "../MeshObject/MagicBook.h"

namespace atl {

	class PlayerPawn final : public std::enable_shared_from_this<PlayerPawn> {
	public:
		PlayerPawn();

		// �Q�b�^�[
		inline const Shared<Atl3DCamera> getPlayerCamera() const { return playerCamera_; }
		inline const tnl::Vector3& getPlayerPos() const { return player3Dpos_; }
		
		void setPlayerAndCamera3Dpos(const tnl::Vector3& newPos);

		// �}�E�X�ŃJ�����𑀍삷��
		void cameraControl();

		// 2D���W��̈ʒu�� Spawn ����
		void playerSpawn2Dpos(const tnl::Vector2i& spawn2Dpos);
		
		void debug_displayPlayerParam(int drawPosX = 0, int drawPosY = 0);

		// �V�[�P���X�Ǘ����s�p
		bool playerUpdate(float deltaTime) {
			bool retV = seq_.update(deltaTime);
			return retV;
		}

		// render�n
		void render(float deltaTime) {
			playerHaveMagicWand->renderObjects(playerCamera_);
			playerHaveMagicBook->renderObject(playerCamera_);
		}

		void initialize() {
			playerHaveMagicWand = std::make_shared<MagicWand>(std::weak_ptr<PlayerPawn>(shared_from_this()));
			playerHaveMagicBook = std::make_shared<MagicBook>(std::weak_ptr<PlayerPawn>(shared_from_this()));
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

		const float CAMERA_ROT_SPEED = 0.3f;	// �J������]���x
		const float MOVE_LERP_SPEED = 3.0f;		// 1�}�X�ړ��ɂ����鑬�x ( �l���傫���قǁA���Ԃ������� )
		const float PLAYER_HEAD_LINE = 500;		// �v���C���[��Y�����i�J�����E�ڐ��̍����j
		float moveLerpTimeCount_ = 0;
		tnl::Vector2i player2Dpos_{ 0,0 };
		tnl::Vector3 player3Dpos_{ 0,0,0 };
		tnl::Vector3 moveTarget_{ 0,0,0 };
		
		Shared<Atl3DCamera> playerCamera_ = nullptr;
		Shared<MagicWand> playerHaveMagicWand = nullptr;
		Shared<MagicBook> playerHaveMagicBook = nullptr;

		// --------------------------------------------------
		// ���\�b�h
		
		// arg ... ���݈ʒu����̈ړ���
		bool checkIsCanMovePos(const tnl::Vector2i& moveToPos);
		bool moveLerp(float deltaTime);
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
	};

}