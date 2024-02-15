#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MagicRune.h"

namespace atl {

	class Base_MagicRune;

	class MagicRuneSystemManager final {
	//------------------------------------------
	// �V���O���g���݌v
	public:
		static MagicRuneSystemManager* getMagicRuneSystemManager();
		static void deleteMagicRuneSystemManager() { delete getMagicRuneSystemManager(); };
	private:
		MagicRuneSystemManager() {};
		~MagicRuneSystemManager() { equipmentMagicRunes_.clear(); };
	//------------------------------------------

	public:

		// �Q�b�^�[ ( �ő呕���� )
		inline const int32_t getMaxEquipmentRune() const { return MAX_EQUIPMENT_RUNE; }
		// �Q�b�^�[ ( �������̃��[���z�� )
		inline const std::vector<Shared<Base_MagicRune>>& getEquipmentMagicRunes() const { return equipmentMagicRunes_;}

		// ���[����ǉ�����
		bool equipRune(Shared<Base_MagicRune> newRune);
		
		// ���[�����폜����
		void removeRune(int index);

		// ���[���𔭓�������
		// arg ... �����^�C�~���O�BBase_MagicRune �Œ�`
		void notifyOnEvent(e_EventType eventType, DungeonScene& dungeonScene);


	private:
		// �ő呕������5�܂�
		int32_t MAX_EQUIPMENT_RUNE = 5;
		// �������̃��[���̔z��
		std::vector<Shared<Base_MagicRune>> equipmentMagicRunes_{};
	};

}