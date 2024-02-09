#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Scenes/DungeonScene.h"

namespace atl {


	class ItemData final {
	public:
		explicit ItemData(std::weak_ptr<DungeonScene> dungeonScene);

		const Shared<dxe::Texture> getItemIllust() { return itemIllust_; }
		const std::string& getItemName() const { return itemName_; }
		const std::string& getItemDescString() const { return descString_; }

		// �g�p�����ʁBenum�ŊǗ�
		void executeItemPerformAction();

	private:
		enum class e_itemList {
			NONE = 0,
			HealHerb,
			HealPotion,
			ThunderStone,
			ThunderScroll,
			MagicInc,
			ITEM_MAX,
		};
		e_itemList itemID_ = e_itemList::NONE;
		std::string itemName_ = "";
		std::string descString_ = "";
		Shared<dxe::Texture> itemIllust_ = nullptr;

		// �_���W�����V�[���ւ̎�Q��
		std::weak_ptr<DungeonScene> weakDungeonScene_;

		// �A�C�e�����g�������ɏo�͂��郍�O�B��������getTextLogManager�����̂��߂�ǂ��̂ŁB
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
	};

}