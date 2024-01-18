#pragma once
#include <algorithm>
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

    // �Փ˂����ʂ�\�� enum
    enum class HitSurface3D {NONE, RIGHT, LEFT, UP, DOWN, FORWARD, BACK, MAX };

    // AABB�Ƌ��̏Փ˔���
    // ret ... [�Փ˂��Ă��� => �Փ˂��Ă���ʂ�HitSurface3D] [�Փ˂��Ă��Ȃ� => NONE]
    // arg1 ... ���̒��S�_
    // arg2 ... ���̔��a
    // arg3 ... AABB�̒��S�_
    // arg4 ... AABB�̃T�C�Y ( X,Y,Z )
    const HitSurface3D& IsCollisionSphereRect(const tnl::Vector3& sphereCenter, float sphereRadius, const tnl::Vector3& rectCenter, const tnl::Vector3& rectSize);

}