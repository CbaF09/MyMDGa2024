#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	// 階段用メッシュオブジェクト
	class Stairs final : public Base_MultiMeshObject {
	public:
		explicit Stairs(const tnl::Vector2i& spawnPos, const tnl::Vector3& initSize);
		void adjustChildsMeshes(float deltaTime) override;

	private:
		tnl::Vector3 stairsSize_{ 0,0,0 };
	};

}