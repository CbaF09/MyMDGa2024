#include "Skybox.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	Skysphere::Skysphere() {
		skysphere_ = dxe::Mesh::CreateSphereMV(50000, 30, 30);
		skysphere_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/Skybox.jpg"));
		skysphere_->setCullingMode(DX_CULLING_LEFT);
	}

	void Skysphere::update(const Shared<Atl3DCamera> camera) {
		// ��������]
		skysphere_->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.1f));
		skysphere_->pos_ = camera->pos_;

		// �t�H�O��؂��ă����_�[���Ă���A�ēx�t�H�O��L���ɂ���
		SetFogEnable(false);
		skysphere_->render(camera);
		SetFogEnable(true);
	}
}