#pragma once
#include "Base_ActorData.h"
#include "Inventory.h"
#include "MagicRuneSlot.h"

namespace atl {

	class PlayerData final: public Base_ActorData {
	public:
		PlayerData();

		// �Q�b�^�[
		inline const Shared<Inventory> getInventory() { return playerInventory_; }
		inline const int32_t getCurrentLevel() { return currentLevel_; }

		// �Z�b�^�[
		void changeCurrentEXP(int32_t getExp);

		// �f�o�b�O�p
		// arg ... �\�����鍶����W
		void debug_playerDataParam(int x, int y);

	private:
		// ���݂̃��x��
		int32_t currentLevel_ = 1;
		// ���ݒ~�ϒ��̌o���l
		int32_t currentExp_ = 0;
		// ���x���A�b�v�ɕK�v�Ȍo���l
		int32_t needExp_ = 15;
		const int32_t LEVEL_UP_HP_UP_VALUE = 5;

		// �v���C���[�̃C���x���g��
		Shared<Inventory> playerInventory_ = std::make_shared<Inventory>();
		
		// �v���C���[�̃}�W�b�N���[���X���b�g
		Shared<MagicRuneSlot> playerMagicRuneSlot = std::make_shared<MagicRuneSlot>();

		// ���݌o���l�����x���A�b�v�ɕK�v�Ȍo���l�𒴂������̏����B���x���A�b�v
		void levelUp();
	};

}