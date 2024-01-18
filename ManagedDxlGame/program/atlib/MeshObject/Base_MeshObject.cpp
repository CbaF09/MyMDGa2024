#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "Base_MeshObject.h"

namespace atl {

	Base_MeshObject::Base_MeshObject(const Shared<dxe::Mesh> originMesh) {
		auto mesh = originMesh->createClone();
		setMesh(mesh);
	}

	void Base_MeshObject::setTexture(const Shared<dxe::Texture> texture) {
		mesh_->setTexture(texture);
	}

	void Base_MeshObject::setMaterial(const std::string& filePath) {
		mesh_->loadMaterial(filePath);
	}

	void Base_MeshObject::renderObject(const Shared<Atl3DCamera> camera) const {
		mesh_->render(camera);
	}
}