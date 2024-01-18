#include "Wall.h"

namespace atl {

	Wall::Wall(const tnl::Vector3& initSize) {
		auto mesh = dxe::Mesh::CreateBoxMV(initSize,
			dxe::Texture::CreateFromFile("graphics/Texture/Mable_Black.jpg"),
			dxe::Texture::CreateFromFile("graphics/Texture/Mable_Black.jpg"),
			dxe::Texture::CreateFromFile("graphics/Texture/Mable_Black.jpg"),
			dxe::Texture::CreateFromFile("graphics/Texture/Mable_Black.jpg"),
			dxe::Texture::CreateFromFile("graphics/Texture/Mable_Black.jpg"),
			dxe::Texture::CreateFromFile("graphics/Texture/Mable_Black.jpg")
		);
		setMesh(mesh);
		setMeshSizeVector3(initSize);
	}

	Wall::Wall(const Shared<dxe::Mesh> originMesh) : Base_MeshObject(originMesh) {

	}

}