#include "Base_MultiMeshObject.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	void Base_MultiMeshObject::addChildMesh(const Shared<dxe::Mesh> childMesh) {
		childMeshes_.emplace_back(childMesh);
	}

	void Base_MultiMeshObject::set2DposAnd3Dpos(const tnl::Vector2i& mesh2Dpos) {
		auto cellLength = DungeonScene::getCellLength();
		mesh2Dpos_ = mesh2Dpos;
		rootMesh_->pos_ = { static_cast<float>(mesh2Dpos_.x * cellLength),static_cast<float>(rootMesh_->pos_.y),static_cast<float>(mesh2Dpos_.y * cellLength) };
	}

	void Base_MultiMeshObject::renderObjects(const Shared<Atl3DCamera> camera){
		adjustChildsMeshes();

		for (const auto& mesh : childMeshes_) {
			mesh->render(camera);
		}
		rootMesh_->render(camera);

	}

}