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

	// サウンドリソース、フライウェイトパターン実装部
	int ResourceManager::getSoundRes(const std::string& filepath) {
		// ロード済みの場合、そのサウンドハンドルを返す
		if (soundResouceMap_.find(filepath) != soundResouceMap_.end()) {
			return soundResouceMap_[filepath];
		}

		// ロード済みでない場合、ロードする
		int newSoundHandle = LoadSoundMem(filepath.c_str());

		// ロードでエラーを起こしたらリターン
		if (newSoundHandle == -1) return -1;

		// ロードしたものを map に追加する
		soundResouceMap_.insert(std::make_pair(filepath, newSoundHandle));
		return newSoundHandle;
	}

	// 再生
	void ResourceManager::playSoundRes(const std::string& filepath, int playType, int topPositionFlag) {
		auto handle = getSoundRes(filepath);
		PlaySoundMem(handle, playType, topPositionFlag);
	}

	// 音量変更
	void ResourceManager::changeVolumeSoundRes(const std::string& filepath, int volume) {
		auto handle = getSoundRes(filepath);
		// ヴォリュームの値が正しいかチェック
		if (0 <= volume || volume <= 255) {
			// playSoundMemとハンドルを入れる引数の順が違いますが、DXlibの仕様です
			ChangeVolumeSoundMem(volume, handle);
		}
	}

	// 停止
	void ResourceManager::stopSoundRes(const std::string& filepath) {
		auto handle = getSoundRes(filepath);
		StopSoundMem(handle);
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

		// soundResourceMap_ を走査
		if (auto it = soundResouceMap_.find(filepath); it != graphResourceMap_.end()) {
			DeleteSoundMem(it->second);
			soundResouceMap_.erase(filepath);
		}
		
		// 発見できなかった場合
		tnl::DebugTrace("\n----------------------------------\n ResourceManager のデータ解放が正常に成功していません \n----------------------------------\n");
		return false;
	}


}