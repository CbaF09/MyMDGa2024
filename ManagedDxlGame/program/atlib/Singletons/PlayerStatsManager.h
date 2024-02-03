#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class PlayerStatsManager final {
	//------------------------------------------
	// �V���O���g���݌v
	public:
		static PlayerStatsManager* getPlayerStatsManager();
		static void deletePlayerStatsManager();
	private:
		static PlayerStatsManager* p_instance_;
	//------------------------------------------

	public:
		// �Q�b�^�[
		inline const int32_t& getPlayerCurrentHP() const { return playerCurrentHP_; }
		inline const float getPlayerCurrentHPpersent() const { return playerCurrentHPpersent; }

		// �����̒l����HP�𑝌�����
		void changePlayerCurrentHP(int32_t addsumValue);

		// �v���C���[���U�����ꂽ���ɌĂяo��
		// �@ �v�Z����K�p���Ă���v���C���[��HP�����炷�B
		// �A �󂯂��_���[�W���e�L�X�g���O�擪�ɒǉ�����
		// �v�Z�� ... �ő�_���[�W = attackPower, �ŏ��_���[�W = attackPower * 7/8 , �̊ԂŃ����_���Ȓl
		// arg ... �U�����̍U����
		void damagedPlayer(int32_t attackPower);

		// �v���C���[��HP���[�� => true , �[���ł͂Ȃ� => false
		bool isZeroPlayerHP();
		
		// �v���C���[�̌���HP�ƍő�HP�̊������v�Z����
		float calcAndSetCurrentPlayerHPpersent();

	private:
		int32_t playerCurrentHP_ = 100;
		int32_t playerMaxHP_ = 100;
		float playerCurrentHPpersent = 1.0f;
		
	};
}