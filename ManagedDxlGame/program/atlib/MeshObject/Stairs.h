#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {

	
	/// <summary>
	/// 
	/// 階段のメッシュオブジェクト
	/// 
	/// </summary>
	
	// 定数系
	namespace {
		// 階段の大きさ
		const tnl::Vector3 stairsSize_ = { 300,300,300 };

	}

	class Stairs final : public Base_MultiMeshObject {
	public:
		explicit Stairs(const tnl::Vector2i& spawnPos);

		// 階段は回転したりしないので、子メッシュの補正は空実装
		void adjustChildsMeshes(float deltaTime) override {};
	};

}