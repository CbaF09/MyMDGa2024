#include "Base_MultiMeshObject.h"

namespace atl {

	void Base_MultiMeshObject::addChildMesh(const Shared<dxe::Mesh> childMesh) {
		childMeshes_.emplace_back(childMesh);
	}

	void Base_MultiMeshObject::renderObjects(const Shared<Atl3DCamera> camera){
		adjustmentChildMeshes();

		rootMesh_->render(camera);
		for (const auto& mesh : childMeshes_) {
			mesh->render(camera);
		}
	}

}