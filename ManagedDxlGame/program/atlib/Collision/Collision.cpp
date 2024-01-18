#include "Collision.h"

namespace atl {

    const HitSurface3D& IsCollisionSphereRect(const tnl::Vector3& sphereCenter, float sphereRadius, const tnl::Vector3& rectCenter, const tnl::Vector3& rectSize) {
        // �����`�̊e�ʂ̒��S�_
        tnl::Vector3 rectMin = rectCenter - (rectSize / 2);
        tnl::Vector3 rectMax = rectCenter + (rectSize / 2);

        // ���̒��S����ł��߂��_�����߂�
        tnl::Vector3 closestPoint = {
            (std::max)(rectMin.x, (std::min)(sphereCenter.x, rectMax.x)),
            (std::max)(rectMin.y, (std::min)(sphereCenter.y, rectMax.y)),
            (std::max)(rectMin.z, (std::min)(sphereCenter.z, rectMax.z))
        };

        // ���̒��S�ƍł��߂��_�Ƃ̋������v�Z
        tnl::Vector3 distanceVec = sphereCenter - closestPoint;
        float distance = distanceVec.length();

        // �Փ˂��Ă��Ȃ�
        if (distance > sphereRadius) return HitSurface3D::NONE;
        // �Փ˂��Ă��� ( �Փ˖ʂ�Ԃ� )
        else if (closestPoint.x == rectMin.x) return HitSurface3D::LEFT;
        else if (closestPoint.x == rectMax.x) return HitSurface3D::RIGHT;
        else if (closestPoint.y == rectMin.y) return HitSurface3D::DOWN;
        else if (closestPoint.y == rectMax.y) return HitSurface3D::UP;
        else if (closestPoint.z == rectMin.z) return HitSurface3D::FORWARD;
        else if (closestPoint.z == rectMax.z) return HitSurface3D::BACK;

        return HitSurface3D::NONE;
    }

}