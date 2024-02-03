#include "Atl3DCamera.h"

namespace atl {

	// ���t���[���Ăяo��
	void Atl3DCamera::update() {
		cameraControl();
		dxe::Camera::update();
	};

	void Atl3DCamera::cameraControl() {
        target_ = pos_ + forward();
        up();

		tnl::Quaternion beforeRot = cameraRot_;

		{// �}�E�X�̈ړ����������ꍇ�A�J��������]������
			auto mouseMoveVelocity = tnl::Input::GetMouseVelocity();
			// �}�E�X�ړ��ʂ��������ꍇ�̂ݏ���
			if (mouseMoveVelocity.x != 0 || mouseMoveVelocity.y != 0 || mouseMoveVelocity.z != 0) {
				// �c�U��
				cameraRotateAxis(right(), mouseMoveVelocity.y * MOUSE_SENSITIVITY);
				float newAngle = tnl::ToDegree(forward().angle({ 0,1,0 }));
				// newAngle �� MIN_PITCH ~ MAX_PITCH �̊ԂɃN�����v
				if (newAngle < MIN_PITCH || MAX_PITCH < newAngle) {
					cameraRot_ = beforeRot;
				}

				// ���U��
				cameraRotateAxis({ 0, 1, 0 }, mouseMoveVelocity.x * MOUSE_SENSITIVITY);

				// ���݌����Ă�����p���v�Z
				calcCurrentFowardDir();
			}
		}
    }

	void Atl3DCamera::calcCurrentFowardDir() {
		tnl::Vector3 cameraForward = forward().xz();
		cameraForward.normalize();

		float angleToZplus = cameraForward.angle({ 0, 0, 1 });
		float angleToZminus = cameraForward.angle({ 0,0,-1 });
		float angleToXplus = cameraForward.angle({ 1, 0, 0 });
		float angleToXminus = cameraForward.angle({ -1,0,0 });

		float minAngle = (std::min)({ angleToZplus, angleToZminus, angleToXplus, angleToXminus });

		if (minAngle == angleToZplus) { currentForwardDir_ = e_XZdir::Zplus; }
		else if (minAngle == angleToZminus) { currentForwardDir_ = e_XZdir::Zminus; }
		else if (minAngle == angleToXplus) { currentForwardDir_ = e_XZdir::Xplus; }
		else if (minAngle == angleToXminus) { currentForwardDir_ = e_XZdir::Xminus; }
	}

	void Atl3DCamera::cameraRotateAxis(tnl::Vector3 rotateAxis, float rotateDegree) {
		cameraRot_ *= tnl::Quaternion::RotationAxis({ rotateAxis }, tnl::ToRadian(rotateDegree));
	}

	void Atl3DCamera::debug_displayCameraParam(int drawPosX,int drawPosY) {
		DrawStringEx(drawPosX, drawPosY, -1,
			"cameraPos ... [ %.2f , %.2f , %.2f ]", pos_.x, pos_.y, pos_.z);
	}

};