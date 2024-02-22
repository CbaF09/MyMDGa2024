#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// CSV のロードと保持、渡しを行う
	/// ファイルロードを行うタイミングを減らす目的
	/// 注意 : tnl::CsvCell は、const で扱えない設計になっているようなので、ゲッターに const が付いていない
	/// ( 内部でCsvCellとして格納したデータを、別の型に変換している関係？ )
	/// 注意 : ゲッターしたものは、tnl::CsvCellが持つ機能以外の操作をしないこと ( emplace_back とかしたりするな )
	/// 
	/// </summary>
	
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
		// ゲッター ( ルーン用 )
		inline std::vector<std::vector<tnl::CsvCell>>& getMagicRuneCSV() { return magicRuneCSV_; }
		
		// ゲッター ( 敵データ用 )
		inline std::vector<std::vector<tnl::CsvCell>>& getEnemyDataCSV() { return enemyDataCSV_; }

	private:
		// ルーン用 CSV 配列
		std::vector<std::vector<tnl::CsvCell>> magicRuneCSV_ = tnl::LoadCsv("csv/MagicRuneCSV.csv");

		// 敵データ用 CSV 配列
		std::vector<std::vector<tnl::CsvCell>> enemyDataCSV_ = tnl::LoadCsv("csv/EnemyCSV.csv");

	};

}