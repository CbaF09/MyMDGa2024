#include <cmath>
#include "Base_ActorData.h"
#include "../Utilities/AtlRandom.h"

namespace atl {

	void Base_ActorData::changeCurrentHP(int32_t addsubValue) {
		currentHP_ += addsubValue;
		// HP���[�������������A�[���ɃN�����v
		if (currentHP_ < 0) { currentHP_ = 0; }
		// HP �� maxHP �𒴂�����AmaxHP �ɃN�����v
		if (currentHP_ > maxHP_) { currentHP_ = maxHP_; }

		// ����HP�̊������v�Z
		calcAndSetCurrentHPpersent();
	}

	int32_t Base_ActorData::damaged(const Base_ActorData& attackingActor, Base_ActorData& defencingActor) {
		int32_t actualyDamage = calcDecideActualyDamage(attackingActor, defencingActor);

		defencingActor.changeCurrentHP(-actualyDamage);

		return actualyDamage;
	}

	// damaged �̃w���p�[�֐��B
	// �v�Z���Ɋ�Â��A���ۂ̃_���[�W���Z�o����B���ŗ����g���Ă܂�
	// ret ... ���_���[�W
	// arg1 ... �U��������
	// arg2 ... �U�����ꂽ��
	// �v�Z�� ... �U���������̍U���� = attackPower_ , �h��� = �U�����ꂽ����defencePower_
	// ��{�_���[�W ... �U���� * ((15/16)^�h���)
	// �ŏ��_���[�W ... ��{�_���[�W * ( 7/8 )
	// �ő�_���[�W ... ��{�_���[�W * ( 143/128 )
	// ���_���[�W ... �ŏ��_���[�W�ƍő�_���[�W�̊Ԃŗ����𓾂���A�Œ�l�̕␳������
	int32_t Base_ActorData::calcDecideActualyDamage(const Base_ActorData& attackingActor, const Base_ActorData& defencingActor) {
		// �v�Z�� float �^�ōs��
		float attackPower = static_cast<float>(attackingActor.getAttackPower());
		float defencePower = static_cast<float>(defencingActor.getDefencePower());

		// ��{�_���[�W
		float baseAttack = attackPower * pow((15.0f / 16.0f), defencePower);
		// �ŏ��_���[�W
		float smallerDamage = baseAttack * (7.0f / 8.0f);
		// �ő�_���[�W
		float biggerDamage = baseAttack * (143.0f / 128.0f);

		// �����Œl�����
		float randomDamage = mtRandomRangeFloat(smallerDamage, biggerDamage);

		// �Œ�l�ɂ��_���[�W�̑����v�Z
		float trueAttack = static_cast<float>(attackingActor.getTrueAttackPower());
		float trueDefence = static_cast<float>(defencingActor.getTrueDefencePower());

		// �Œ�_���[�W�l�̕␳
		float fixedDamage = randomDamage + trueAttack - trueDefence;

		// int32_t�^�ɕϊ�
		int32_t retV = static_cast<int32_t>(fixedDamage);

		return retV;
	}

	void Base_ActorData::calcAndSetCurrentHPpersent() {
	
		if (maxHP_ > 0) { // �[�����Z����AmaxHP���[�����傫���ꍇ�Ɍ���v�Z
			currentHPpersent_ =
				static_cast<float>(currentHP_) / static_cast<float>(maxHP_);
		}

		assert(maxHP_ > 0 && "�ő�HP���[���ȉ��ɂȂ��Ă��܂��BcalcAndSetCurrentHPpersent()");

	}

}