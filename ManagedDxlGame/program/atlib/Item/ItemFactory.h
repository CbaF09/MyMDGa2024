#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Item.h"
#include "ItemAction.h"

namespace atl {

	class ItemFactory {
	public:
		// �A�C�e�����w�肵�Đ�������
		static Shared<Item> createItem(e_ItemList itemName);

		// �A�C�e���������_����������
		static Shared<Item> createRandomItem();

	private:
		// �w�肵���A�C�e���̌��ʃN���X ( ��ۃN���X ) �𐶐�����
		// createItem ����Ăяo����邽�߁Aprivate
		static Shared<IE_ItemAction> createItemActionFromItemName(e_ItemList itemName);
	};

}