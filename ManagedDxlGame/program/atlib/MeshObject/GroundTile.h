#pragma once
#include "Base_MeshObject.h"

namespace atl {

	/// <summary>
	/// 
	/// �_���W�����̒n�� ( 3D )
	/// 
	/// </summary>

	class GroundTile final : public Base_MeshObject {
	public:
		explicit GroundTile(const tnl::Vector3& initSize);
		explicit GroundTile(const Shared<dxe::Mesh> originMesh);

	private:
	};

}