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

	// �t���C�E�F�C�g�p�^�[�� UI�p�̃O���t�B�b�N�n���h�����擾�A�����ꍇ�̓��[�h����
	int ResourceManager::getUiRes(const std::string& filepath) {
		// uiResourceMap_ �� filepath �����������ꍇ�A����ƑΉ�����O���t�B�b�N�n���h����Ԃ�
		if (uiResourceMap_.find(filepath) != uiResourceMap_.end()) return uiResourceMap_[filepath];

		int newGrHandle = LoadGraph(filepath.c_str());
		if (newGrHandle != -1) { // �G���[�̏ꍇ -1 ���Ԃ��Ă���̂ŁA�G���[�łȂ��ꍇ��uiResourceMap�ɒǉ�����
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
		// illustResource �𑖍�
		if (auto it = illustResourceMap_.find(filepath); it != illustResourceMap_.end()) {
			// ���������ꍇ�A���
			DeleteGraph(it->second);
			illustResourceMap_.erase(filepath);
			return true;
		}
		// uiResource �𑖍�
		else if (auto it = uiResourceMap_.find(filepath);it != uiResourceMap_.end()) {
			DeleteGraph(it->second);
			uiResourceMap_.erase(filepath);
			return true;
		}
		
		// �����ł��Ȃ������ꍇ
		return false;
	}
}