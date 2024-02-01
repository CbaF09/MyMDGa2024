#pragma once
#include "Base_MeshObject.h"

namespace atl {

	class PlayerPawn;

	class MagicBook : public Base_MeshObject {
	public:
		explicit MagicBook(const tnl::Vector3& size) {};
		explicit MagicBook(Shared<dxe::Mesh> originMesh) {};
		explicit MagicBook(std::weak_ptr<const PlayerPawn> player);
		
		void renderObject(const Shared<Atl3DCamera> camera) const override;

	private:
		std::weak_ptr<const PlayerPawn> weakPlayerPawn;
		tnl::Quaternion initRot_;
		bool isHeldByPlayer = false;
	};

}