#include "MagicBook.h"
#include "../Object/PlayerPawn.h"

namespace atl {

	MagicBook::MagicBook(std::weak_ptr<const PlayerPawn> player) {
		isHeldByPlayer = true;
		weakPlayerPawn = player;

		tnl::Vector3 size{ 40,50,15 };

		auto mesh = dxe::Mesh::CreateBoxMV(size,
			dxe::Texture::CreateFromFile("graphics/box/box_left.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_right.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_up.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_down.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_back.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_forword.bmp")
		);
		mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(-50));
		mesh->rot_ *= tnl::Quaternion::RotationAxis({ 1,0,0 }, tnl::ToRadian(15));
		initRot_ = mesh->rot_;

		setMesh(mesh);
		setMeshSizeVector3(size);
	}

	void MagicBook::renderObject(const Shared<Atl3DCamera> camera) const {
		// プレイヤーが保持している
		if (isHeldByPlayer) {
			auto player = weakPlayerPawn.lock();
			if (player) {
				auto cameraPos = player->getPlayerCamera()->pos_;
				auto& cameraRot = player->getPlayerCamera()->getCameraRot();
				auto& mesh = getMesh();
				
				mesh->pos_ = cameraPos + tnl::Vector3::TransformCoord({ -55,-25,80 }, cameraRot);
				mesh->rot_ = initRot_ * cameraRot;
			}
		}
		// プレイヤーが保持していない ( フィールドに落ちている )
		else {
			
		}

		getMesh()->render(camera);
	}

}