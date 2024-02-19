#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class CsvManager final {
	//------------------------------------------
	// シングルトン設計
	public:
		static CsvManager* getCsvManager();
		inline static void deleteCsvManager() { delete getCsvManager(); }
	private:
		CsvManager() {};
		~CsvManager() {};
	//------------------------------------------

	public:
		// ゲッター ( ルーン用CSV )
		inline std::vector<std::vector<tnl::CsvCell>>& getMagicRuneCSV() { return magicRuneCSV_; }

	private:
		// ルーン用 CSV 配列
		std::vector<std::vector<tnl::CsvCell>> magicRuneCSV_ = tnl::LoadCsv("csv/MagicRuneCSV.csv");

	};

}