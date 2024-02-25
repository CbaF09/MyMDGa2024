#include "ForwardArrow.h"
#include "PlayerPawn.h"

namespace atl {
	
	ForwardArrow::ForwardArrow(std::weak_ptr<const PlayerPawn> player): weakPlayerPawn_(player) {
		auto mesh = dxe::Mesh::CreateHollowOutDiskPlaneMV(FORWARD_ARROW_SIZE);
		mesh->rot_ *= tnl::Quaternion::RotationAxis({ 1,0,0 }, tnl::ToRadian(90));
		setMesh(mesh);

		auto texture = dxe::Texture::CreateFromFile("graphics/Texture/Green.bmp");
		mesh->setTexture(texture);
	}

	void ForwardArrow::renderObject(const Shared<Atl3DCamera> camera,float deltaTime) {
		auto player = weakPlayerPawn_.lock();
		if (!player) { 
			return; 
		}

		// カメラの向いている向きに、セット
		auto& forward = player->getPlayerCamera()->getCurrentForwardDir();
		auto& cameraPos = player->getPlayerCamera()->pos_;
		auto cellLength = DungeonScene::getCellLength();
		getMesh()->pos_ = cameraPos + tnl::Vector3{static_cast<float>(forward.x * cellLength), FORWARD_ARROW_Y, static_cast<float>(forward.z * cellLength)};

		getMesh()->render(camera);
	}
}