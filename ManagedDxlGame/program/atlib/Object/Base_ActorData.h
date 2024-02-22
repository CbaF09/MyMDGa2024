#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	///
	///  �v���C���[ �E �G�l�~�[�ɋ��ʂ���f�[�^�Ɗ֐���ێ�
	/// 
	/// </summary>

	class Base_ActorData {
	public:
		virtual ~Base_ActorData() {};

		// �Q�b�^�[
		inline const int32_t getMaxHP() const { return maxHP_; }
		inline const int32_t getCurrentHP() const { return currentHP_; }
		inline const float getCurrentHPpersent() const { return currentHPpersent_; }
		inline const int32_t getAttackPower() const { return attackPower_; }
		inline const int32_t getDefencePower() const { return defencePower_; }
		inline const int32_t getTrueAttackPower() const { return trueAttackPower_; }
		inline const int32_t getTrueDefencePower() const { return trueDefencePower_; }

		// �Z�b�^�[
		inline void setMaxHP(int32_t maxHP) { maxHP_ = maxHP; calcAndSetCurrentHPpersent(); }
		inline void setCurrentHP(int32_t currentHP) { currentHP_ = currentHP; calcAndSetCurrentHPpersent(); }
		inline void setAttackPower(int32_t attackPower) { attackPower_ = attackPower; }
		inline void setDefencePower(int32_t defencePower) { defencePower_ = defencePower; }
		inline void setTrueAttackPower(int32_t trueAttackPower) { trueAttackPower_ = trueAttackPower; }
		inline void setTrueDefencePower(int32_t trueDefencePower) { trueDefencePower_ = trueDefencePower; }

		// �����̒l����HP�𑝌�����
		void changeCurrentHP(int32_t addsumValue);

		// �ʏ�U���ɂ��_���[�W�������������Ăяo���B�v�Z����K�p���Ėh�䑤��HP�����炷�B
		// �Ăяo�����C���X�^���X���K�v���� ( �U�����C���X�^���X���h�䑤�C���X�^���X�������œn���� ) �̂� static �ň�����悤��
		// ret ... ���ۂɗ^�����_���[�W
		// arg1 ... �U��������
		// arg2 ... �U�����ꂽ��
		static int32_t damaged(const Base_ActorData& attackingActor, Base_ActorData& defencingActor);

		// damaged �̃w���p�[�֐�
		// �v�Z���Ɋ�Â��A���ۂ̃_���[�W���Z�o����B���ŗ����g���Ă܂�
		// ret ... ���_���[�W
		// arg1 ... �U��������
		// arg2 ... �U�����ꂽ��
		// �v�Z�� ... �U���������̍U���� = attackPower_ , �h��� = �U�����ꂽ����defencePower_
		// ��{�_���[�W ... �U���� * ((15/16)^�h���)
		// �ŏ��_���[�W ... ��{�_���[�W * ( 7/8 )
		// �ő�_���[�W ... ��{�_���[�W * ( 143/128 )
		// ���_���[�W ... �ŏ��_���[�W�ƍő�_���[�W�̊Ԃŗ����𓾂���A�Œ�l�̕␳������
		static int32_t calcDecideActualyDamage(const Base_ActorData& attackingActor, const Base_ActorData& attakedActor);

		// HP���[�� => true , �[���ł͂Ȃ� => false
		bool isZeroHP();

		// ����HP�ƍő�HP�̊������v�Z����
		void calcAndSetCurrentHPpersent();

	private:
		// �ő�HP
		int32_t maxHP_ = 0;
		// ����HP
		int32_t currentHP_ = 0;
		// ���݂�HP����
		float currentHPpersent_ = 0.0f;

		// �U����
		int32_t attackPower_ = 0;
		// �h���
		int32_t defencePower_ = 0;

		// �Œ�U���_���[�W
		int32_t trueAttackPower_ = 0;
		// �Œ�h���
		int32_t trueDefencePower_ = 0;
	};

}