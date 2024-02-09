#include "Stairs.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	Stairs::Stairs(const tnl::Vector2i& spawnPos, const tnl::Vector3& initSize) : stairsSize_(initSize) {
		set2Dpos(spawnPos);
		auto cellLength = DungeonScene::getCellLength();
		tnl::Vector3 spawn3DPos = { static_cast<float>(spawnPos.x * cellLength), 0 ,static_cast<float>(spawnPos.y * cellLength) };

		auto bottom = dxe::Mesh::CreateBoxMV(
			{ stairsSize_.x,stairsSize_.y / 3,stairsSize_.z },
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png")
		);
		bottom->pos_ = spawn3DPos + tnl::Vector3{ 0, stairsSize_.y / 6, 0 };
		setRootMesh(bottom);

		auto middle = dxe::Mesh::CreateBoxMV(
			{ stairsSize_.x,stairsSize_.y / 3,(stairsSize_.z/3)*2 },
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png")
		);
		addChildMesh(middle);

		auto top = dxe::Mesh::CreateBoxMV(
			{ stairsSize_.x,stairsSize_.y / 3,stairsSize_.z/3 },
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png"),
			dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png")
		);
		addChildMesh(top);

	}

	void Stairs::adjustChildsMeshes(float deltaTime) {
		auto rootMesh = getRootMesh();
		rootMesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(0.2f));

		auto& childs = getChildMeshes();
		childs[0]->rot_ = rootMesh->rot_;
		childs[1]->rot_ = rootMesh->rot_;

		childs[0]->pos_ = rootMesh->pos_ + tnl::Vector3::TransformCoord(tnl::Vector3{ 0, stairsSize_.y / 3, stairsSize_.z / 6 }, childs[0]->rot_);
		childs[1]->pos_ = rootMesh->pos_ + tnl::Vector3::TransformCoord(tnl::Vector3{ 0,(stairsSize_.y / 3) * 2,stairsSize_.z / 3 }, childs[1]->rot_);
	}

}