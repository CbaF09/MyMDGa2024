#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MeshObject.h"

namespace atl {

	class PlayerPawn;

	class MenuBook final : public Base_MeshObject {
	public:
		explicit MenuBook(std::weak_ptr<const PlayerPawn> player);
		
		void renderObject(const Shared<Atl3DCamera> camera, float deltaTime) override;

		inline const bool isOpenMenu() const { return isOpenMenu_; }
		inline void openMenu() { isOpenMenu_ = true; }
		inline void closeMenu() { isOpenMenu_ = false; }

	private:
		std::weak_ptr<const PlayerPawn> weakPlayerPawn;
		tnl::Quaternion initRot_;
		bool isOpenMenu_ = false;

	};

}