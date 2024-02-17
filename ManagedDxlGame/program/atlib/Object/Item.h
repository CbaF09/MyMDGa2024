#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	/// <summary>
	/// 
	/// �A�C�e���̏��Ə���������
	/// ItemPawn �� Inventory ���痘�p�����
	/// 
	/// </summary>

	class Item final {
	public:
		explicit Item(std::weak_ptr<DungeonScene> dungeonScene);

		// �Q�b�^�[ ( �A�C�e���̃C���X�g )
		const Shared<dxe::Texture> getItemIllust() { return itemIllust_; }
		// �Q�b�^�[ ( �A�C�e���� )
		const std::string& getItemName() const { return itemName_; }
		// �Q�b�^�[ ( �A�C�e�������� )
		const std::string& getItemDescString() const { return descString_; }

		// �g�p�����������s����֐��BInventory�N���X �� useItem() ����Ăяo�����
		void executeItemPerformAction();

	private:
		//------------------------------------------
		// �ϐ�

		// �A�C�e������ID����v�����邽�߂�enum
		enum class e_itemList {
			NONE = 0,
			HealHerb,
			HealPotion,
			ThunderStone,
			ThunderScroll,
			MagicInc,
			HealRune,
			FireRune,
			StoneRune,
			ITEM_MAX,
		};

		// �A�C�e��ID
		e_itemList itemID_ = e_itemList::NONE;
		// �Q�[���ɕ\�������A�C�e����
		std::string itemName_ = "";
		// �A�C�e���̐�����
		std::string descString_ = "";
		// �A�C�e���̌����ڗp�e�N�X�`��
		Shared<dxe::Texture> itemIllust_ = nullptr;

		// �_���W�����V�[���ւ̎�Q��
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		//------------------------------------------
		// �֐�

		// �A�C�e�����g�������Ƀ��O�o�͂���w���p�[�֐��B��������getTextLogManager�����̂��߂�ǂ��̂ŁB
		void addTextItemUse(std::string text);

		// �ȉ��A�e�A�C�e���ɑΉ����鏈���B�_���W�����V�[���ւ̎�Q�Ƃ�����̂ŐF�X�ł���͂�

		// �񕜂̑� ... �̗͏���
		const int32_t HERB_HEAL_VALUE = 15;
		void healHerbAction();

		// �񕜖�@... �̗͒���
		const int32_t POTION_HEAL_VALUE = 50;
		void healPotionAction();

		// �v���C���[�Ɠ����G���A�ɑ����Ă���G�S�̂Ƀ_���[�W��^����
		const int32_t THUNDER_STONE_DAMAGE_VALUE = 20;
		void thunderStoneAction();

		// �t�B�[���h�̓G�S���Ƀ_���[�W��^����
		const int32_t THUNDER_SCROLL_DAMAGE_VALUE = 10;
		void thunderScrollAction();

		// ���ҏ�̏�Ԃ��񕜂��� ( �����x���񕜂��� )
		const int32_t MAGIC_INC_HEAL_VALUE = 1000;
		void magicIncAction();

		// �����̃��[������
		void healRuneAction();

		// ���̃��[������
		void fireRuneAction();

		// ��̃��[������
		void stoneRuneAction();
	};

}