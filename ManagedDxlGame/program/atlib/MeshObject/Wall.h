#pragma once
#include "Base_MeshObject.h"

namespace atl {

	/// <summary>
	/// 
	/// �_���W�����̕�
	/// 
	/// </summary>

	class Wall final : public Base_MeshObject {
	public:
		explicit Wall(const tnl::Vector3& initSize);
		explicit Wall(const Shared<dxe::Mesh> originMesh);

	private:
	};

}