#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	class Atl3DCamera;

	class Skybox final {
	public:
		Skybox();

		void update(const Shared<Atl3DCamera> camera);

	private:
		Shared<dxe::Mesh> skybox_;
	};

}