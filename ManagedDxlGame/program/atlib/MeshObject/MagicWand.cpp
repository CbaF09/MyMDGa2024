#include "MagicWand.h"
#include "PlayerPawn.h"

namespace atl {

	// ƒvƒŒƒCƒ„[‚É•Û‚³‚ê‚½ó‘Ô‚Å¶¬‚³‚ê‚é
	MagicWand::MagicWand(std::weak_ptr<PlayerPawn> player) : weakPlayerPawn(player) {
		auto playerLock = weakPlayerPawn.lock();
		if (playerLock) {

			auto& playerPos = playerLock->getPlayerPos();

			float initSize = 25;

			auto stick = dxe::Mesh::CreateConeCylinderMV(initSize / 4, initSize / 10, initSize * 2.5f);
			stick->rot_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(180));
			stick->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(90));
			initRot_ = stick->rot_;
			stick->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/WandStick.png"));
			setRootMesh(stick);

			auto sphere = dxe::Mesh::CreateSphereMV(initSize / 3);
			sphere->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/WandCore.png"));
			addChildMesh(sphere);

			auto outerRing = dxe::Mesh::CreateTorusMV(initSize / 1.2f, initSize / 4);
			outerRing->rot_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(90));
			outerRing->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/WandStick.png"));
			addChildMesh(outerRing);
		}
	}

	void MagicWand::adjustChildsMeshes(float deltaTime) {
		auto player = weakPlayerPawn.lock();
		if (player) {
			auto cameraPos = player->getPlayerCamera()->pos_;
			auto& cameraRot = player->getPlayerCamera()->getCameraRot();

			auto& stick = getRootMesh();
			stick->pos_ = cameraPos + tnl::Vector3::TransformCoord({ 60,-25,80 }, cameraRot);
			stick->rot_ = initRot_ * cameraRot;

			auto& childs = getChildMeshes();

			// sphere
			childs[0]->pos_ = stick->pos_ + tnl::Vector3::TransformCoord({ 0,25,0 }, cameraRot);
			childs[0]->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1));

			// outerRing
			childs[1]->pos_ = stick->pos_ + tnl::Vector3::TransformCoord({ 0,25,0 }, cameraRot);
			childs[1]->rot_ = tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(90)) * cameraRot;

		}
	}
}

