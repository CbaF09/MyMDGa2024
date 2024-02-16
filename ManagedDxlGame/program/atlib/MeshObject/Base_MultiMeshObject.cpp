#include "Base_MultiMeshObject.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	void Base_MultiMeshObject::addChildMesh(const Shared<dxe::Mesh> childMesh) {
		childMeshes_.emplace_back(childMesh);
	}

	void Base_MultiMeshObject::renderObjects(const Shared<Atl3DCamera> camera,float deltaTime){
		// �q���b�V���␳�֐�������B��ۃN���X�Œ�`
		adjustChildsMeshes(deltaTime);

		for (const auto& mesh : childMeshes_) {
			mesh->render(camera);
		}
		rootMesh_->render(camera);

	}
}