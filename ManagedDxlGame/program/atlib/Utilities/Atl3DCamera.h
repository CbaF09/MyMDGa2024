#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	class Atl3DCamera final: public dxe::Camera{

	public:
		Atl3DCamera(int screen_w, int screen_h) : dxe::Camera(screen_w, screen_h) {};

		void update() override;

		inline tnl::Vector3 up() { return tnl::Vector3::TransformCoord({ 0, 1, 0 }, cameraRot_); }
		inline tnl::Vector3 down() { return -up(); }
		inline tnl::Vector3 forward() override { return tnl::Vector3::TransformCoord({ 0, 0, 1 }, cameraRot_); }
		inline tnl::Vector3 back() override { return -forward(); }
		inline tnl::Vector3 left() override { return tnl::Vector3::Cross(forward(), up()); }
		inline tnl::Vector3 right() override { return tnl::Vector3::Cross(up(), forward()); }

		// マウスでカメラを回転・制御
		void cameraControl();

		// カメラをX,Y,Z軸で回転させる
		// arg1 ... 回転軸 ( 例 : X+軸 ... {1,0,0} )
		// arg2 ... 回転角度 ( 度数法 )
		void cameraRotateAxis(tnl::Vector3 rotateAxis, float rotateValue);

		// ゲッター
		inline const tnl::Quaternion& getCameraRot() const { return cameraRot_; }
		inline const tnl::Vector3& getCurrentForwardDir() const { return currentForwardNormalDir_; }
		inline const tnl::Vector2i& getCurrentForwardDirToV2i() const { return { static_cast<int>(currentForwardNormalDir_.x),static_cast<int>(currentForwardNormalDir_.z) }; }

		// デバッグ用
		void debug_displayCameraParam(int drawPosX = 0, int drawPosY = 0);

		// 現在の向いている方向を計算
		void calcCurrentFowardNormalDir();

	private:
		const float MOUSE_SENSITIVITY = 0.3f;
		const float MIN_PITCH = 40.0f;
		const float MAX_PITCH = 150.0f;

		tnl::Quaternion cameraRot_;
		tnl::Vector3 currentForwardNormalDir_{ 0,0,1 }; // 現在の正面方向

	};

}
