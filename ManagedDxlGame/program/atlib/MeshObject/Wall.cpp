#include "Wall.h"

namespace atl {

	Wall::Wall(const tnl::Vector3& initSize) {
		auto mesh = dxe::Mesh::CreateBoxMV(initSize,
			dxe::Texture::CreateFromFile("graphics/Texture/WallBrickBlue.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/WallBrickBlue.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/WallBrickBlue.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Blue.bmp"),
			dxe::Texture::CreateFromFile("graphics/Texture/WallBrickBlue.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/WallBrickBlue.png")
		);
		setMesh(mesh);
	}

	Wall::Wall(const Shared<dxe::Mesh> originMesh) : Base_MeshObject(originMesh) {

	}

}