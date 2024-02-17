#pragma once
#include "Base_MagicRune.h"

namespace atl {

	/// <summary>
	/// 
	/// Base_MagicRune���p�������A���[���̋�ۃN���X ( �I�u�U�[�o�[�p�^�[���̃T�u�W�F�N�g�̋�ۃN���X )
	/// 
	/// ���[���̑��₵��
	/// 1. CSV�ɁA���₵�������[���̏������
	/// 2. e_RuneID�ɁA���₵�����[���p�̗񋓎q��ǉ�
	/// 3. ��ۃN���X�����A�R���X�g���N�^�� setRuneData ���L�q����B
	/// 4. onNotify�ɁA�������L�q
	/// 
	/// ���[���𑕔�����A�C�e���́A�ʂŒǉ����K�v
	/// 
	/// </summary>

	/// <summary>
	/// �����̃��[��
	/// </summary>
	class HealRune : public Base_MagicRune {
	public:
		HealRune() { setRuneData(e_RuneID::HealRune); }
		void onNotify(e_EventType eventType, DungeonScene& dungeonScene) override;
	};

	/// <summary>
	/// ���̃��[��
	/// </summary>
	class FireRune : public Base_MagicRune {
	public:
		FireRune() { setRuneData(e_RuneID::FireRune); }
		inline void onNotify(e_EventType eventType, DungeonScene& dungeonScene) override { /*���L�q*/ };
		void onEquipMomentNotify(DungeonScene& dungeonScene) override;
		void onRemoveMomentNotify(DungeonScene& dungeonScene) override;
	private:
		const int32_t ATTACK_UP_VALUE = 3;
	};

	/// <summary>
	/// ��̃��[��
	/// </summary>
	class StoneRune : public Base_MagicRune {
	public:
		StoneRune() { setRuneData(e_RuneID::StoneRune); }
		inline void onNotify(e_EventType eventType, DungeonScene& dungeonScene) override { /*���L�q*/ };
		void onEquipMomentNotify(DungeonScene& dungeonScene) override;
		void onRemoveMomentNotify(DungeonScene& dungeonScene) override;
	private:
		const int32_t DEFENCE_UP_VALUE = 5;
	};

}