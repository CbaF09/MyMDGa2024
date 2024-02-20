#include "ResourceManager.h"

namespace atl {

	ResourceManager* ResourceManager::getResourceManager() {
		static ResourceManager* p_instance_ = nullptr;

		if (!p_instance_) p_instance_ = new ResourceManager();
		return p_instance_;
	};

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

	// �T�E���h���\�[�X�A�t���C�E�F�C�g�p�^�[��������
	int ResourceManager::getSoundRes(const std::string& filepath) {
		// ���[�h�ς݂̏ꍇ�A���̃T�E���h�n���h����Ԃ�
		if (soundResouceMap_.find(filepath) != soundResouceMap_.end()) {
			return soundResouceMap_[filepath];
		}

		// ���[�h�ς݂łȂ��ꍇ�A���[�h����
		int newSoundHandle = LoadSoundMem(filepath.c_str());

		// ���[�h�ŃG���[���N�������烊�^�[��
		if (newSoundHandle == -1) return -1;

		// ���[�h�������̂� map �ɒǉ�����
		soundResouceMap_.insert(std::make_pair(filepath, newSoundHandle));
		return newSoundHandle;
	}

	// �Đ�
	void ResourceManager::playSoundRes(const std::string& filepath, int playType, int topPositionFlag) {
		auto handle = getSoundRes(filepath);
		PlaySoundMem(handle, playType, topPositionFlag);
	}

	// ���ʕύX
	void ResourceManager::changeVolumeSoundRes(const std::string& filepath, int volume) {
		auto handle = getSoundRes(filepath);
		// ���H�����[���̒l�����������`�F�b�N
		if (0 <= volume || volume <= 255) {
			// PlaySoundMem�ƃn���h������������̏����Ⴂ�܂����ADXlib�̎d�l�Ȃ̂Ŗ��Ȃ�
			ChangeVolumeSoundMem(volume, handle);
		}
	}

	// ��~
	void ResourceManager::stopSoundRes(const std::string& filepath) {
		auto handle = getSoundRes(filepath);
		StopSoundMem(handle);
	}

	// ����ɐ��������� => true , ������Ȃ������� => false
	bool ResourceManager::deleteResource(const std::string& filepath) {
		// graphResourceMap_ �𑖍�
		if (auto it = graphResourceMap_.find(filepath); it != graphResourceMap_.end()) {
			// ���������ꍇ�A���
			// ����Ɏ��s����� -1 ���Ԃ��Ă���
			if (DeleteGraph(it->second) != -1) {
				graphResourceMap_.erase(filepath);
				return true;
			}
			// ����Ɏ��s�����ꍇ
			else {
				tnl::DebugTrace("\n ResourceManager���\�[�X��� => %s �̃���������Ɏ��s���܂��� \n", filepath.c_str());
				return false;
			}
		}

		// soundResourceMap_ �𑖍�
		if (auto it = soundResouceMap_.find(filepath); it != soundResouceMap_.end()) {
			if (DeleteSoundMem(it->second) != -1) {
				soundResouceMap_.erase(filepath);
				return true;
			}
			else {
				tnl::DebugTrace("\n ResourceManager���\�[�X��� => %s �̃���������Ɏ��s���܂��� \n", filepath.c_str());
				return false;
			}
		}
		
		// �����ł��Ȃ������ꍇ
		tnl::DebugTrace("\n ResourceManager���\�[�X��� => %s ��������܂���ł��� \n",filepath.c_str());
		return false;
	}

	Shared<dxe::Particle> ResourceManager::getParticleRes(const std::string& filepath) {
		// particleResourceMap_ �� filepath �����������ꍇ�A����ƑΉ�����O���t�B�b�N�n���h����Ԃ�
		if (particleResourceMap_.find(filepath) != particleResourceMap_.end()) return particleResourceMap_[filepath];

		Shared<dxe::Particle> newParticle = std::make_shared<dxe::Particle>(filepath);
		if (newParticle != nullptr) { // �G���[�̏ꍇ nullptr ���Ԃ��Ă���̂ŁA�G���[�łȂ��ꍇ��uiResourceMap�ɒǉ�����
			particleResourceMap_.insert(std::make_pair(filepath, newParticle));
		}
		return newParticle;
	}


}