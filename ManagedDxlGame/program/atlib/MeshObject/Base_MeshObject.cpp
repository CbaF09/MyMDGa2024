#include <memory>
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MeshObject.h"

namespace atl {

	Base_MeshObject::Base_MeshObject(const Shared<dxe::Mesh> originMesh) {
		auto mesh = originMesh->createClone();
		setMesh(mesh);
	}

	void Base_MeshObject::renderObject(const Shared<Atl3DCamera> camera, float deltaTime) {
		mesh_->render(camera);
	}
}