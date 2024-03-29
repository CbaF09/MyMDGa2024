#include "MagicWand.h"
#include "PlayerPawn.h"

namespace atl {

	MagicWand::MagicWand(std::weak_ptr<PlayerPawn> player) : weakPlayerPawn(player) {
		
		auto stick = dxe::Mesh::CreateConeCylinderMV(WAND_SIZE / 4, WAND_SIZE / 10, WAND_SIZE * 2.5f);
		stick->rot_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(180));
		stick->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(90));
		initRot_ = stick->rot_;
		stick->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/WandStick.png"));
		setRootMesh(stick);
		
		auto sphere = dxe::Mesh::CreateSphereMV(WAND_SIZE / 3);
		sphere->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/WandCore.png"));
		addChildMesh(sphere);

		auto outerRing = dxe::Mesh::CreateTorusMV(WAND_SIZE, WAND_SIZE / 4);
		outerRing->rot_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(90));
		outerRing->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/WandStick.png"));
		addChildMesh(outerRing);
	}

	void MagicWand::adjustChildsMeshes(float deltaTime) {
		auto player = weakPlayerPawn.lock();
		if (!player) {
			return;
		}

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

