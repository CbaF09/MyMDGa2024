#include "ResourceManager.h"

namespace atl {

	ResourceManager* ResourceManager::p_instance_ = nullptr;

	ResourceManager* ResourceManager::getResourceManager() {
		if (!p_instance_) p_instance_ = new ResourceManager();
		return p_instance_;
	};

	void ResourceManager::deleteResourceManager() {
		if (p_instance_) {
			delete p_instance_;
			p_instance_ = nullptr;
		};
	}

	Shared<dxe::Texture> ResourceManager::loadTexture(const std::string& filePath) {
		auto it = textureResourceMap_.find(filePath);
		if (it != textureResourceMap_.end()) return textureResourceMap_[filePath];
		Shared<dxe::Texture> retTexture = dxe::Texture::CreateFromFile(filePath);
		textureResourceMap_.insert(std::make_pair(filePath, retTexture));
		return retTexture;
	}

}