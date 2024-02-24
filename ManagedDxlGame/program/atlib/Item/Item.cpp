#include "Item.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/SceneManager.h"
#include "../Singletons/EnemyManager/EnemyManager.h"
#include "../Singletons/CsvManager.h"
#include "../Utilities/AtlRandom.h"
#include "../MeshObject/PlayerPawn.h"
#include "../Utilities/AtlString.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"
#include "../MagicRuneSystem/MagicRune.h"

namespace atl {

	void Item::initialize(e_ItemList itemName) {
		// CSV �����[�h����
		auto& csv = CsvManager::getCsvManager()->getItemDataCSV();
		// ���[�h�̐��ۃ`�F�b�N�A�������^�[��
		if (csv.empty()) { return; }

		// �͈͊O�`�F�b�N�A�������^�[��
		if (itemName == e_ItemList::NONE || itemName == e_ItemList::ITEM_MAX) { return; }

		auto itemID = static_cast<int>(itemName);

		// Item�����Z�b�g
		itemName_ = csv[itemID][1].getString();
		// Item �ɑΉ�����e�N�X�`�����Z�b�g
		auto& filepath = csv[itemID][2].getString();
		itemIllust_ = dxe::Texture::CreateFromFile(filepath);
		// �A�C�e���̐��������Z�b�g
		descString_ = csv[itemID][3].getString();
	}

	void Item::executeItemPerformAction() {
		// nullptr �`�F�b�N �������^�[��
		if (!itemExecutePerform_) { return; }

		// �A�C�e���̌��ʔ���
		itemExecutePerform_->execute();
	}


}