#include "ItemData.h"
#include "../Singletons/TextLogManager.h"

namespace atl {

	ItemData::ItemData() {

	}

	// itemID ������ƁA���̃A�C�e�������������
	ItemData::ItemData(int32_t itemID) {
		auto csv = tnl::LoadCsv("csv/ItemCSV.csv");

		if (!csv.empty()) {
			// ID �� enum �ɕϊ����Ă���Z�b�g
			itemID_ = static_cast<e_itemList>(csv[itemID][0].getInt());
			// Item�����Z�b�g
			itemName_ = csv[itemID][1].getString();
			// Item �ɑΉ�����e�N�X�`�����Z�b�g
			auto& filepath = csv[itemID][2].getString();
			itemIllust_ = dxe::Texture::CreateFromFile(filepath);
			// �A�C�e���̐��������Z�b�g
			descString_ = csv[itemID][3].getString();
		}
	}

	void ItemData::executeItemPerformAction() {
		switch (itemID_) {
		case e_itemList::NONE: break;
		case e_itemList::HealPotion: TextLogManager::getTextLogManager()->addTextLog(itemName_ + "���g����"); break;
		}
	}

}