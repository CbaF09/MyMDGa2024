#pragma once
#include "Base_MeshObject.h"

namespace atl {

	class Wall final : public Base_MeshObject{
	public:
		explicit Wall(const tnl::Vector3& initSize);
		explicit Wall(const Shared<dxe::Mesh> originMesh);

	private:
	};

}