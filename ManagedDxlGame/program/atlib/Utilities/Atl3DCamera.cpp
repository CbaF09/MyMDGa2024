#include "Atl3DCamera.h"

namespace atl {

	void Atl3DCamera::update() {
		dxe::Camera::update();
	};

	void Atl3DCamera::cameraControl(float controlSpeed) {
        target_ = pos_ + forward();
        up();

		tnl::Quaternion beforeRot = cameraRot_;

        tnl::Vector3 mvel = tnl::Input::GetMouseVelocity();
        
		// ècêUÇË
		cameraRotateAxis(right(), mvel.y * controlSpeed);
		float newAngle =  tnl::ToDegree(forward().angle({ 0,1,0 }));
		if (newAngle < MIN_PITCH || MAX_PITCH < newAngle) {
			cameraRot_ = beforeRot;
		}
		
		// â°êUÇË
		cameraRotateAxis({ 0, 1, 0 }, mvel.x * controlSpeed);

    }

	void Atl3DCamera::cameraRotateAxis(tnl::Vector3 rotateAxis, float rotateDegree) {
		cameraRot_ *= tnl::Quaternion::RotationAxis({ rotateAxis }, tnl::ToRadian(rotateDegree));
	}

	void Atl3DCamera::debug_displayCameraParam(int drawPosX,int drawPosY) {
		DrawStringEx(drawPosX, drawPosY, -1,
			"cameraPos ... [ %.2f , %.2f , %.2f ]", pos_.x, pos_.y, pos_.z);
	}

};