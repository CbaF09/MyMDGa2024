#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	// �K�i�p���b�V���I�u�W�F�N�g
	class Stairs final : public Base_MultiMeshObject {
	public:
		Stairs(const tnl::Vector2i& spawnPos, const tnl::Vector3& initSize);
		void adjustChildsMeshes() override;

	private:
		tnl::Vector3 stairsSize_{ 0,0,0 };
	};

}