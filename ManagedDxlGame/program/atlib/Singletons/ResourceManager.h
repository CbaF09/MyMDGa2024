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
		Shared<dxe::Texture> loadTexture(const std::string& filepath);

	private:
		std::unordered_map<std::string, Shared<dxe::Texture>> textureResourceMap_;
	};

}