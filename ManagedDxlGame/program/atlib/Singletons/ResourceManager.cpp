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

	// フライウェイトパターン グラフィックハンドルを取得、無い場合はロードする
	int ResourceManager::getGraphRes(const std::string& filepath) {
		// graphResourceMap_ に filepath が見つかった場合、それと対応するグラフィックハンドルを返す
		if (graphResourceMap_.find(filepath) != graphResourceMap_.end()) return graphResourceMap_[filepath];

		int newGrHandle = LoadGraph(filepath.c_str());
		if (newGrHandle != -1) { // エラーの場合 -1 が返ってくるので、エラーでない場合はuiResourceMapに追加する
			graphResourceMap_.insert(std::make_pair(filepath, newGrHandle));
		}
		return newGrHandle;
	}

	// 解放に成功した時 => true , 解放しなかった時 => false
	bool ResourceManager::deleteResource(const std::string& filepath) {
		// graphResourceMap_ を走査
		if (auto it = graphResourceMap_.find(filepath); it != graphResourceMap_.end()) {
			// 発見した場合、解放
			DeleteGraph(it->second);
			graphResourceMap_.erase(filepath);
			return true;
		}
		
		// 発見できなかった場合
		return false;
	}
}