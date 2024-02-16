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
	/// </summary>

	/// <summary>
	/// �����̃��[��
	/// </summary>
	class HealRune : public Base_MagicRune {
	public:
		HealRune() { setRuneData(e_RuneID::HealRune); }
		void onNotify(e_EventType eventType,DungeonScene& dungeonScene) override;
	};

	/// <summary>
	/// ���̃��[��
	/// </summary>
	class FireRune : public Base_MagicRune {
	public:
		FireRune() { setRuneData(e_RuneID::FireRune); }
		// �������L�q
		void onNotify(e_EventType eventType, DungeonScene& dungeonScene) override {};
	};

}