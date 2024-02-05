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

	// �t���C�E�F�C�g�p�^�[�� �O���t�B�b�N�n���h�����擾�A�����ꍇ�̓��[�h����
	int ResourceManager::getGraphRes(const std::string& filepath) {
		// graphResourceMap_ �� filepath �����������ꍇ�A����ƑΉ�����O���t�B�b�N�n���h����Ԃ�
		if (graphResourceMap_.find(filepath) != graphResourceMap_.end()) return graphResourceMap_[filepath];

		int newGrHandle = LoadGraph(filepath.c_str());
		if (newGrHandle != -1) { // �G���[�̏ꍇ -1 ���Ԃ��Ă���̂ŁA�G���[�łȂ��ꍇ��uiResourceMap�ɒǉ�����
			graphResourceMap_.insert(std::make_pair(filepath, newGrHandle));
		}
		return newGrHandle;
	}

	// ����ɐ��������� => true , ������Ȃ������� => false
	bool ResourceManager::deleteResource(const std::string& filepath) {
		// graphResourceMap_ �𑖍�
		if (auto it = graphResourceMap_.find(filepath); it != graphResourceMap_.end()) {
			// ���������ꍇ�A���
			DeleteGraph(it->second);
			graphResourceMap_.erase(filepath);
			return true;
		}
		
		// �����ł��Ȃ������ꍇ
		return false;
	}
}