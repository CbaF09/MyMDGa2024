#include "Wall.h"

namespace atl {

	Wall::Wall(const tnl::Vector3& initSize) {

		auto texture = dxe::Texture::CreateFromFile("graphics/Texture/WallBrickBlue.png");
		
		auto mesh = dxe::Mesh::CreateBoxMV(initSize,
			texture,
			texture,
			texture,
			texture,
			texture,
			texture
		);
		setMesh(mesh);
	}

	Wall::Wall(const Shared<dxe::Mesh> originMesh) : Base_MeshObject(originMesh) {

	}

}