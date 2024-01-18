#pragma once
#include <algorithm>
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

    // 衝突した面を表す enum
    enum class HitSurface3D {NONE, RIGHT, LEFT, UP, DOWN, FORWARD, BACK, MAX };

    // AABBと球の衝突判定
    // ret ... [衝突している => 衝突している面のHitSurface3D] [衝突していない => NONE]
    // arg1 ... 球の中心点
    // arg2 ... 球の半径
    // arg3 ... AABBの中心点
    // arg4 ... AABBのサイズ ( X,Y,Z )
    const HitSurface3D& IsCollisionSphereRect(const tnl::Vector3& sphereCenter, float sphereRadius, const tnl::Vector3& rectCenter, const tnl::Vector3& rectSize);

}