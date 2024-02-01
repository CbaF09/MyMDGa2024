#include "ForwardArrow.h"
#include "PlayerPawn.h"

namespace atl {
	
	ForwardArrow::ForwardArrow(std::weak_ptr<const PlayerPawn> player) {
		weakPlayerPawn = player;
		
		auto mesh = dxe::Mesh::CreatePlaneMV({ 200,500,0 });
		setMesh(mesh);

		auto texture = dxe::Texture::CreateFromFile("graphics/Texture/ForwardArrow.png");
		setTexture(texture);
	}

	void ForwardArrow::renderObject(const Shared<Atl3DCamera> camera) const {
		auto player = weakPlayerPawn.lock();
		if (player) {
			auto& mesh = getMesh();
			auto cameraPos = player->getPlayerCamera()->pos_;

			tnl::Vector3 cameraForward = player->getPlayerCamera()->forward().xz();
			cameraForward.normalize();

			float angleToZplus = cameraForward.angle({ 0, 0, 1 });
			float angleToZminus = cameraForward.angle({ 0,0,-1 });
			float angleToXplus = cameraForward.angle({ 1, 0, 0 });
			float angleToXminus = cameraForward.angle({ -1,0,0 });

			float minAngle = (std::min)({ angleToZplus, angleToZminus, angleToXplus, angleToXminus });

			mesh->rot_ = tnl::Quaternion::RotationAxis({ 1,0,0 }, tnl::ToRadian(75));
			if (minAngle == angleToZplus) { 
				mesh->pos_ = cameraPos + tnl::Vector3{ 0,-400,900 };
			}
			else if (minAngle == angleToZminus) { 
				mesh->pos_ = cameraPos + tnl::Vector3{ 0,-400,-900 };
				mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(180));
			}
			else if (minAngle == angleToXplus) { 
				mesh->pos_ = cameraPos + tnl::Vector3{ 900,-400,0 };
				mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(90));
			}
			else if (minAngle == angleToXminus) { 
				mesh->pos_ = cameraPos + tnl::Vector3{ -900,-400,0 };
				mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(-90));
			}
		}
		getMesh()->render(camera);
	}

}