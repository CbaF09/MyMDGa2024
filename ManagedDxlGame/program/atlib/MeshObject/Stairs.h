#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	
	/// <summary>
	/// 
	/// �K�i�̃��b�V���I�u�W�F�N�g
	/// 
	/// </summary>
	
	// �萔�n
	namespace {
		// �K�i�̑傫��
		const tnl::Vector3 stairsSize_ = { 300,300,300 };

	}

	class Stairs final : public Base_MultiMeshObject {
	public:
		explicit Stairs(const tnl::Vector2i& spawnPos);

		// �K�i�͉�]�����肵�Ȃ��̂ŁA�q���b�V���̕␳�͋����
		void adjustChildsMeshes(float deltaTime) override {};
	};

}