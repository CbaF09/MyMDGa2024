#include "ForwardArrow.h"
#include "PlayerPawn.h"

namespace atl {
	
	ForwardArrow::ForwardArrow(std::weak_ptr<const PlayerPawn> player): weakPlayerPawn(player) {
		auto mesh = dxe::Mesh::CreateHollowOutDiskPlaneMV(250);
		mesh->rot_ *= tnl::Quaternion::RotationAxis({ 1,0,0 }, tnl::ToRadian(90));
		setMesh(mesh);
		setTexture(dxe::Texture::CreateFromFile("graphics/Texture/Red.bmp"));

	}

	void ForwardArrow::renderObject(const Shared<Atl3DCamera> camera,float deltaTime) {
		auto player = weakPlayerPawn.lock();
		if (player) {
			// カメラの向いている向きに、セット
			auto& forward = player->getPlayerCamera()->getCurrentForwardDir();
			auto& cameraPos = player->getPlayerCamera()->pos_;
			getMesh()->pos_ = cameraPos + tnl::Vector3{static_cast<float>(forward.x * 1000), FORWARD_ARROW_Y, static_cast<float>(forward.z * 1000)};

		}
		getMesh()->render(camera);
	}
}