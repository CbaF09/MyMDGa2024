#include "Stairs.h"
#include "../Scenes/DungeonScene.h"

namespace atl {

	Stairs::Stairs(const tnl::Vector2i& spawnPos) {
		set2Dpos(spawnPos);
		auto cellLength = DungeonScene::getCellLength();
		tnl::Vector3 spawn3DPos = { static_cast<float>(spawnPos.x * cellLength), 0 ,static_cast<float>(spawnPos.y * cellLength) };
		auto texture = dxe::Texture::CreateFromFile("graphics/Texture/Stairs.png");

		auto bottom = dxe::Mesh::CreateBoxMV(
			{ stairsSize_.x,stairsSize_.y / 3,stairsSize_.z },
			texture,
			texture,
			texture,
			texture,
			texture,
			texture
		);
		bottom->pos_ = spawn3DPos + tnl::Vector3{ 0, stairsSize_.y / 6, 0 };
		setRootMesh(bottom);

		auto middle = dxe::Mesh::CreateBoxMV(
			{ stairsSize_.x,stairsSize_.y / 3,(stairsSize_.z/3)*2 },
			texture,
			texture,
			texture,
			texture,
			texture,
			texture
		);
		middle->pos_ = getRootMesh()->pos_ + tnl::Vector3::TransformCoord(tnl::Vector3{ 0, stairsSize_.y / 3, stairsSize_.z / 6 }, bottom->rot_);
		addChildMesh(middle);

		auto top = dxe::Mesh::CreateBoxMV(
			{ stairsSize_.x,stairsSize_.y / 3,stairsSize_.z/3 },
			texture,
			texture,
			texture,
			texture,
			texture,
			texture
		);
		top->pos_ = getRootMesh()->pos_ + tnl::Vector3::TransformCoord(tnl::Vector3{ 0,(stairsSize_.y / 3) * 2,stairsSize_.z / 3 }, bottom->rot_);
		addChildMesh(top);
	}
}