#include "MenuBook.h"
#include "PlayerPawn.h"

namespace atl {

	MenuBook::MenuBook(std::weak_ptr<PlayerPawn> player) {
		weakPlayerPawn = player;

		tnl::Vector3 size{ 40,50,15 };

		auto texture = dxe::Texture::CreateFromFile("graphics/texture/MenuBookTexture.png");
		auto textureFront = dxe::Texture::CreateFromFile("graphics/texture/MenuBookFrontTexture.png");

		auto mesh = dxe::Mesh::CreateBoxMV(size,
			texture,
			texture,
			texture,
			texture,
			texture,
			textureFront
		);

		mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(-50));
		mesh->rot_ *= tnl::Quaternion::RotationAxis({ 1,0,0 }, tnl::ToRadian(15));
		initRot_ = mesh->rot_;
		setMesh(mesh);
	}

	void MenuBook::renderObject(const Shared<Atl3DCamera> camera,float deltaTime) {
		auto player = weakPlayerPawn.lock();
		if (player) {
			auto cameraPos = player->getPlayerCamera()->pos_;
			auto& cameraRot = player->getPlayerCamera()->getCameraRot();
			auto& mesh = getMesh();

			// メニューを開いていない状態の姿勢
			if (!isOpenMenu_) {
				mesh->pos_ = cameraPos + tnl::Vector3::TransformCoord({ -55,-25,80 }, cameraRot);
				mesh->rot_ = initRot_ * cameraRot;
			}
			// メニューを開いている時の姿勢
			else {
				mesh->pos_ = cameraPos + tnl::Vector3::TransformCoord({ -35,0,65 }, cameraRot);
				tnl::Quaternion identityRot;
				mesh->rot_ = identityRot * tnl::Quaternion::RotationAxis({ 0,1,0 },tnl::ToRadian(-30)) * cameraRot;
			}
		}

		getMesh()->render(camera);
	}

}