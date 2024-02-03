#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	// �v���C���[ �E �G�l�~�[�ɋ��ʂ���f�[�^�Ɗ֐���ێ�
	// EnemyData, PlayerData �ȂǂɌp��
	class Base_ActorData {
	public:
		virtual ~Base_ActorData() {};

		// �Q�b�^�[
		inline const int32_t getMaxHP() const { return maxHP_; }
		inline const int32_t getCurrentHP() const { return currentHP_; }
		inline const float getCurrentHPpersent() const { return currentHPpersent; }
		inline const int32_t getattackPower() const { return attackPower_; }

		// �Z�b�^�[
		inline void setMaxHP(int32_t maxHP) { maxHP_ = maxHP; }
		inline void setCurrentHP(int32_t currentHP) { currentHP_ = currentHP; calcAndSetCurrentHPpersent(); }
		inline void setAttackPower(int32_t attackPower) { attackPower_ = attackPower; }

		// �����̒l����HP�𑝌�����
		void changeCurrentHP(int32_t addsumValue);

		// �U�����ꂽ���ɌĂяo���B�v�Z����K�p����HP�����炷�B
		// �v�Z�� ... �ő�_���[�W = attackPower, �ŏ��_���[�W = attackPower * 7/8 , �̊ԂŃ����_���Ȓl
		// arg ... �U�����̍U����
		// ret ... ���ۂɗ^�����_���[�W
		int32_t damaged(int32_t attackedPower);

		// HP���[�� => true , �[���ł͂Ȃ� => false
		bool isZeroHP();

		// ����HP�ƍő�HP�̊������v�Z����
		void calcAndSetCurrentHPpersent();

	private:
		int32_t maxHP_ = 0;
		int32_t currentHP_ = 0;
		float currentHPpersent = 0.0f;
		int32_t attackPower_ = 0;
	};

}