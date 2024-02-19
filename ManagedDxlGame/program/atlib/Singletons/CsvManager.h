#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

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
		// �Q�b�^�[ ( ���[���pCSV )
		inline std::vector<std::vector<tnl::CsvCell>>& getMagicRuneCSV() { return magicRuneCSV_; }

	private:
		// ���[���p CSV �z��
		std::vector<std::vector<tnl::CsvCell>> magicRuneCSV_ = tnl::LoadCsv("csv/MagicRuneCSV.csv");

	};

}