#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Scenes/DungeonScene.h"
#include "ItemAction.h"

namespace atl {

	/// <summary>
	/// 
	/// �A�C�e���̑��₵��
	/// �@ ItemCSV�ɃA�C�e���̏������
	/// �A e_ItemList��ǉ� ( �A�C�e�����Ɗ֌W�͂Ȃ����A�ǐ��̖ʂ���C�������������ǂ� )
	/// �B ItemAction .h .cpp �ɁAIE_ItemAction���p�������A�A�C�e���̏�������������
	/// �C ItemFactory .cpp �� createItemActionFromItemName �֐��� switch-case �ɁA�V�����A�C�e����e_ItemList�񋓎q���ƐV����ItemAction��std::make_shared��Ή������ď���
	/// 
	/// </summary>

	// �A�C�e������ID����v�����邽�߂�enum
	enum class e_ItemList {
		NONE = 0,
		HealHerb,
		HealPotion,
		ThunderStone,
		ThunderScroll,
		MagicInc,
		FireRune,
		StoneRune,
		ITEM_MAX,
	};

	/// <summary>
	/// 
	/// �A�C�e����\���N���X
	/// �A�C�e���̃f�[�^��ێ�
	/// IE_ItemAction�|�C���^��ێ� ( �X�g���e�W�[�p�^�[�� )
	/// 
	/// ItemPawn �� Inventory ���痘�p�����
	/// </summary>
	
	class Item final {
	public:
		// ����������
		// arg ... ��������A�C�e��
		void initialize(e_ItemList itemName);

		// �Q�b�^�[ ( �A�C�e���̃C���X�g )
		inline const Shared<dxe::Texture> getItemIllust() { return itemIllust_; }
		// �Q�b�^�[ ( �A�C�e���� )
		inline const std::string& getItemName() const { return itemName_; }
		// �Q�b�^�[ ( �A�C�e�������� )
		inline const std::string& getItemDescString() const { return descString_; }

		// �Z�b�^�[ ( �A�C�e������ )
		inline void setItemAction(Shared<IE_ItemAction> itemAction) { itemExecutePerform_ = itemAction; }

		// itemExecutePerform_ �� execute �����s�����BInventory�N���X �� useItem() ����Ăяo�����
		void executeItemPerformAction();

	private:
		// �A�C�e��ID
		e_ItemList itemID_ = e_ItemList::NONE;
		// �Q�[���ɕ\�������A�C�e����
		std::string itemName_ = "";
		// �A�C�e���̐�����
		std::string descString_ = "";
		// �A�C�e���̌����ڗp�e�N�X�`��
		Shared<dxe::Texture> itemIllust_ = nullptr;
		// �A�C�e���̌���
		Shared<IE_ItemAction> itemExecutePerform_ = nullptr;
	};


}