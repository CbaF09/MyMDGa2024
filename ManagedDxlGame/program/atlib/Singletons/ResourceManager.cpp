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
	int ResourceManager::getUIres(const std::string& filepath) {
		auto it = uiResourceMap_.find(filepath);
		// uiResourceMap_ �� filepath �����������ꍇ�A����ƑΉ�����O���t�B�b�N�n���h����Ԃ�
		if (it != uiResourceMap_.end()) return uiResourceMap_[filepath];

		int newGrHandle = LoadGraph(filepath.c_str());
		if (newGrHandle != -1) { // �G���[�̏ꍇ -1 ���Ԃ��Ă���̂ŁA�G���[�łȂ��ꍇ��uiResourceMap�ɒǉ�����
			uiResourceMap_.insert(std::make_pair(filepath, newGrHandle));
		}

		return newGrHandle;
	}
}