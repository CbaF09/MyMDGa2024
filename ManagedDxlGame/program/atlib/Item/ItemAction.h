#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// �A�C�e���̌��ʂ�\�����N���X
	/// �p����ɂČ��ʎ���
	/// 
	/// </summary>
	class IE_ItemAction {
	public:
		virtual ~IE_ItemAction() {};
		virtual void execute() = 0;
	};

	/// <summary>
	/// 
	/// �񕜂̑� ... �̗͏���
	/// 
	/// </summary>
	class HealHerbAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// �񕜗�
		const int32_t HERB_HEAL_VALUE = 15;
	};

	/// <summary>
	/// 
	/// �񕜖�@... �̗͒���
	/// 
	/// </summary>
	class HealPotionAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// �񕜗�
		const int32_t POTION_HEAL_VALUE = 50;
	};

	/// <summary>
	/// 
	/// ���_�̐� ... �v���C���[�Ɠ����G���A�ɑ����Ă���G�S���Ƀ_���[�W��^����
	/// 
	/// </summary>
	class ThundeStoneAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// �^����_���[�W
		const int32_t THUNDER_STONE_DAMAGE_VALUE = 20;
	};

	/// <summary>
	/// 
	/// ���_�̊��� ... �t�B�[���h�̓G�S���Ƀ_���[�W��^����
	/// 
	/// </summary>
	class ThundeScrollAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// �^����_���[�W
		const int32_t THUNDER_SCROLL_DAMAGE_VALUE = 10;
	};


	/// <summary>
	/// 
	/// ���@�̃C���N ... ���ҏ�̏�Ԃ��񕜂��� ( �����x���񕜂��� )
	/// 
	/// </summary>
	class MagicIncAction : public IE_ItemAction {
	public:
		void execute() override;
	private:
		// �񕜗�
		const int32_t MAGIC_INC_HEAL_VALUE = 1000;
	};

	/// <summary>
	/// 
	/// �����̃��[�� ... �����̃��[���𑕔�����
	/// 
	/// </summary>
	class HealRuneAction : public IE_ItemAction {
	public:
		void execute() override;
	};

	/// <summary>
	/// 
	/// ���̃��[�� ... ���̃��[���𑕔�����
	/// 
	/// </summary>
	class FireRuneAction : public IE_ItemAction {
	public:
		void execute() override;
	};

	/// <summary>
	/// 
	/// ��̃��[�� ... ��̃��[���𑕔�����
	/// 
	/// </summary>
	class StoneRuneAction : public IE_ItemAction {
	public:
		void execute() override;
	};

}