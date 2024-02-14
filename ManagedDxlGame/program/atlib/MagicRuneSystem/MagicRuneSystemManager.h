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
		~MagicRuneSystemManager() {};
	//------------------------------------------

	public:
		// ���[����ǉ�����
		bool equipRune(Shared<Base_MagicRune> newRune);
		
		// ���[�����폜����
		void removeRune(int index);

		// ���[���𔭓�������
		// arg ... �����^�C�~���O�BBase_MagicRune �Œ�`
		void notifyOnEvent(e_EventType eventType, DungeonScene& dungeonScene);

	private:
		// �ő呕������10�܂�
		int32_t MAX_EQUIPMENT_RUNE = 10;
		// �������̃��[���̔z��
		std::vector<Shared<Base_MagicRune>> equipmentMagicRuneVector_{};
	};

}