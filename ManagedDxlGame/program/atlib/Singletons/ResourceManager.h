#pragma once
#include <unordered_map>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class ResourceManager final {
	//------------------------------------------
	// �V���O���g���݌v
	public:
		static ResourceManager* getResourceManager();
		static void deleteResourceManager();
	private:
		static ResourceManager* p_instance_;
	//------------------------------------------
	public:
		int getUiRes(const std::string& filepath);
		int getIllustRes(const std::string& filepath);

		// �w��p�X�̃��\�[�X���������B���� => false , ���s => false
		bool deleteResource(const std::string& filepath);

	private:
		std::unordered_map<std::string, int> uiResourceMap_;
		std::unordered_map<std::string, int> illustResourceMap_;
	};

}