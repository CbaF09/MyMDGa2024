#pragma once
#include <unordered_map>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class ResourceManager final {
	//------------------------------------------
	// シングルトン設計
	public:
		static ResourceManager* getResourceManager();
		static void deleteResourceManager();
	private:
		static ResourceManager* p_instance_;
	//------------------------------------------

	public:
		int getUIres(const std::string& filepath);

	private:
		std::unordered_map<std::string, int> uiResourceMap_;
	};

}