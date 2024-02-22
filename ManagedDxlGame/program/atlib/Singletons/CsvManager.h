#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// CSV �̃��[�h�ƕێ��A�n�����s��
	/// �t�@�C�����[�h���s���^�C�~���O�����炷�ړI
	/// ���� : tnl::CsvCell �́Aconst �ň����Ȃ��݌v�ɂȂ��Ă���悤�Ȃ̂ŁA�Q�b�^�[�� const ���t���Ă��Ȃ�
	/// ( ������CsvCell�Ƃ��Ċi�[�����f�[�^���A�ʂ̌^�ɕϊ����Ă���֌W�H )
	/// ���� : �Q�b�^�[�������̂́Atnl::CsvCell�����@�\�ȊO�̑�������Ȃ����� ( emplace_back �Ƃ������肷��� )
	/// 
	/// </summary>
	
	class CsvManager final {
	//------------------------------------------
	// �V���O���g���݌v
	public:
		static CsvManager* getCsvManager();
		inline static void deleteCsvManager() { delete getCsvManager(); }
	private:
		CsvManager() {};
		~CsvManager() {};
	//------------------------------------------

	public:
		// �Q�b�^�[ ( ���[���p )
		inline std::vector<std::vector<tnl::CsvCell>>& getMagicRuneCSV() { return magicRuneCSV_; }
		
		// �Q�b�^�[ ( �G�f�[�^�p )
		inline std::vector<std::vector<tnl::CsvCell>>& getEnemyDataCSV() { return enemyDataCSV_; }

	private:
		// ���[���p CSV �z��
		std::vector<std::vector<tnl::CsvCell>> magicRuneCSV_ = tnl::LoadCsv("csv/MagicRuneCSV.csv");

		// �G�f�[�^�p CSV �z��
		std::vector<std::vector<tnl::CsvCell>> enemyDataCSV_ = tnl::LoadCsv("csv/EnemyCSV.csv");

	};

}