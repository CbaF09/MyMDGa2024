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
	int ResourceManager::getUIres(const std::string& filepath) {
		auto it = uiResourceMap_.find(filepath);
		// uiResourceMap_ に filepath が見つかった場合、それと対応するグラフィックハンドルを返す
		if (it != uiResourceMap_.end()) return uiResourceMap_[filepath];

		int newGrHandle = LoadGraph(filepath.c_str());
		if (newGrHandle != -1) { // エラーの場合 -1 が返ってくるので、エラーでない場合はuiResourceMapに追加する
			uiResourceMap_.insert(std::make_pair(filepath, newGrHandle));
		}

		return newGrHandle;
	}
}