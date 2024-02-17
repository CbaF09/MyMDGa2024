#pragma once

namespace atl {

	class CsvManager {
	//------------------------------------------
	// シングルトン設計
	public:
		static CsvManager* getCsvManager();
		static void deleteCsvManager() { delete getCsvManager(); };
	private:
		CsvManager() {};
		~CsvManager() {};
	//------------------------------------------

	public:

	private:

	};

}