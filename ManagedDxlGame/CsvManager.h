#pragma once

namespace atl {

	class CsvManager {
	//------------------------------------------
	// �V���O���g���݌v
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