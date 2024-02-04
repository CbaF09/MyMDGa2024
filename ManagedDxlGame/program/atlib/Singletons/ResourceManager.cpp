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

	// フライウェイトパターン UI用のグラフィックハンドルを取得、無い場合はロードする
	int ResourceManager::getUiRes(const std::string& filepath) {
		// uiResourceMap_ に filepath が見つかった場合、それと対応するグラフィックハンドルを返す
		if (uiResourceMap_.find(filepath) != uiResourceMap_.end()) return uiResourceMap_[filepath];

		int newGrHandle = LoadGraph(filepath.c_str());
		if (newGrHandle != -1) { // エラーの場合 -1 が返ってくるので、エラーでない場合はuiResourceMapに追加する
			uiResourceMap_.insert(std::make_pair(filepath, newGrHandle));
		}

		return newGrHandle;
	}
	int ResourceManager::getIllustRes(const std::string& filepath) {
		if (illustResourceMap_.find(filepath) != illustResourceMap_.end()) return illustResourceMap_[filepath];

		int newGrHandle = LoadGraph(filepath.c_str());
		if (newGrHandle != -1) {
			illustResourceMap_.insert(std::make_pair(filepath, newGrHandle));
		}

		return 0;
	}

	bool ResourceManager::deleteResource(const std::string& filepath) {
		// illustResource を走査
		if (auto it = illustResourceMap_.find(filepath); it != illustResourceMap_.end()) {
			// 発見した場合、解放
			DeleteGraph(it->second);
			illustResourceMap_.erase(filepath);
			return true;
		}
		// uiResource を走査
		else if (auto it = uiResourceMap_.find(filepath);it != uiResourceMap_.end()) {
			DeleteGraph(it->second);
			uiResourceMap_.erase(filepath);
			return true;
		}
		
		// 発見できなかった場合
		return false;
	}
}