#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	class Atl3DCamera final: public dxe::Camera{

	public:
		Atl3DCamera(int screen_w, int screen_h) : dxe::Camera(screen_w, screen_h) {};

		void update() override;

		inline tnl::Vector3 up() {
			up_ = tnl::Vector3::TransformCoord({ 0, 1, 0 }, cameraRot_);
			return up_;
		}
		inline tnl::Vector3 down() { return -up(); }

		inline tnl::Vector3 forward() override {
			return tnl::Vector3::TransformCoord({ 0, 0, 1 }, cameraRot_);
		}
		inline tnl::Vector3 back() override { return -forward(); }
		inline tnl::Vector3 left() override { return tnl::Vector3::Cross(forward(), up()); }
		inline tnl::Vector3 right() override { return tnl::Vector3::Cross(up(), forward()); }

		// カメラをマウスで操作する
		void cameraControl(float controlSpeed);

		// カメラをX,Y,Z軸で回転させる
		// arg1 ... 回転軸 ( 例 : X軸 ... {1,0,0} )
		// arg2 ... 回転角度 ( 度数法 )
		void cameraRotateAxis(tnl::Vector3 rotateAxis, float rotateValue);

		inline void setCameraRot(const tnl::Quaternion& newCameraRot) { cameraRot_ = newCameraRot; }
		inline const tnl::Quaternion& getCameraRot() const { return cameraRot_; }
		inline const float getMIN_PITCH() const { return MIN_PITCH; }
		inline const float getMAX_PITCH() const { return MAX_PITCH; }

		void debug_displayCameraParam(int drawPosX = 0, int drawPosY = 0);

	private:
		const float MIN_PITCH = 40.0f;
		const float MAX_PITCH = 150.0f;

		tnl::Quaternion cameraRot_ = tnl::Quaternion();
	};

}
