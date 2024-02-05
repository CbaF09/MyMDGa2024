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
		int getGraphRes(const std::string& filepath);

		// �w��p�X�̃��\�[�X���������B���� => false , ���s => false
		bool deleteResource(const std::string& filepath);

	private:
		// 2D �C���X�g�p resourceMap
		std::unordered_map<std::string, int> graphResourceMap_;
	};

}