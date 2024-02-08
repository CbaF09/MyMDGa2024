#include "GroundTile.h"

namespace atl {

	GroundTile::GroundTile(const tnl::Vector3& initSize) {
		auto mesh = dxe::Mesh::CreatePlaneMV(initSize);
		setMesh(mesh);
		auto texture = dxe::Texture::CreateFromFile("graphics/Texture/Mable_Granite.jpg");
		setTexture(texture);
		getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	}

	GroundTile::GroundTile(const Shared<dxe::Mesh> originMesh) : Base_MeshObject(originMesh) {
		getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	}
}