#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Item/Item.h"

namespace atl {

	class Inventory final {
	public:
		
		Inventory() {};
		~Inventory();

		// �Q�b�^�[ ( �A�C�e����vector�z����擾 )
		const std::vector<Shared<Item>>& getItemList() const { return itemList_; }

		// �Q�b�^�[ ( �C���f�b�N�X���w�肷�鎖�ŁA���X�g���璼�ڃA�C�e�����擾���� )
		const Shared<Item> getItem(int index) const;
		
		// �A�C�e����z��ɒǉ�����B�ő吔�����܂��Ă���A�ǉ��ł����ꍇ�� true ,�ł��Ȃ������ꍇ�� false ��Ԃ�
		bool pushbackItemToInventory(const Shared<Item> newItemData);

		// �A�C�e���̎g�p���ʂ�����������A�z�񂩂�폜����
		// arg ... �g���A�C�e�����C���f�b�N�X�Ŏw��
		void useItem(int32_t index);
		
	private:
		// ���Ă�A�C�e���̍ő吔
		const int32_t INVENTORY_MAX = 6;
		
		// �C���x���g���́A�A�C�e���̃��X�g������
		std::vector<Shared<Item>> itemList_{};
	};

}