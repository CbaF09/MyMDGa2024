#include "Skybox.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	Skybox::Skybox() {
		skybox_ = dxe::Mesh::CreateCubeMV(30000, 30, 30);
		skybox_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/Skybox.jpg"));
	}

	void Skybox::update(const Shared<Atl3DCamera> camera) {
		SetFogEnable(false);
		skybox_->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.1f));
		skybox_->pos_ = camera->pos_;
		skybox_->render(camera);
		SetFogEnable(true);
	}
}