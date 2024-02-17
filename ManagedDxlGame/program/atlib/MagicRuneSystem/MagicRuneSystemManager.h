#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MagicRune.h"

namespace atl {
	
	/// <summary>
	/// 
	/// ���[���Ɋւ���}�l�[�W���[�N���X�B
	/// �I�u�U�[�o�[�p�^�[���̃I�u�U�[�o�[�ɂ�����B
	/// Base_MagicRune�̔z���ێ��B
	/// notifyOnEvent �ŁABase_MagicRune�̋�ۃN���X�Œ�`�����onNotify�𔭉΂�����B
	/// 
	/// </summary>
	
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
		// �Q�b�^�[ ( ���[���pCSV )
		inline std::vector<std::vector<tnl::CsvCell>>& getMagicRuneCSV() { return magicRuneCSV_; }

		// ���[����ǉ�����
		bool equipRune(Shared<Base_MagicRune> newRune);
		
		// ���[�����폜����
		void removeRune(int index);

		// ���[���𔭓�������
		// arg ... ���΂�����C�x���g�BBase_MagicRune �Œ�`
		void notifyOnEvent(e_EventType eventType, DungeonScene& dungeonScene);

	private:
		// �ő呕������5�܂�
		int32_t MAX_EQUIPMENT_RUNE = 5;

		// �������̃��[���̔z��
		std::vector<Shared<Base_MagicRune>> equipmentMagicRunes_{};
		
		// ���[���pCSV ��ێ����Ă����ׂ̔z��
		std::vector<std::vector<tnl::CsvCell>> magicRuneCSV_ = tnl::LoadCsv("csv/MagicRuneCSV.csv");

	};

}