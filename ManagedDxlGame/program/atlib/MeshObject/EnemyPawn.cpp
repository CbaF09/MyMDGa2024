#include "EnemyPawn.h"

namespace atl {

	EnemyPawn::EnemyPawn(const tnl::Vector3& enemyPos, const tnl::Vector3& enemySize, float cellLength) : enemySize_(enemySize), oneCellLength_(cellLength) {
		auto rootMesh = dxe::Mesh::CreateBoxMV(
			enemySize,
			dxe::Texture::CreateFromFile("graphics/box/box_left.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_right.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_up.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_down.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_forward.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_back.bmp")
		);
		rootMesh->pos_ = enemyPos;
		setRootMesh(rootMesh);

		auto dirArrow = dxe::Mesh::CreateBoxMV(
			enemySize / 2,
			dxe::Texture::CreateFromFile("graphics/box/box_left.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_right.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_up.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_down.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_forward.bmp"),
			dxe::Texture::CreateFromFile("graphics/box/box_back.bmp")
		);
		dirArrow->pos_ = rootMesh->pos_ + tnl::Vector3{ 0, 0, enemySize_.z / 2 };
		addChildMesh(dirArrow);
	}

	void EnemyPawn::adjustmentChildMeshes() {
		auto rootMesh = getRootMesh();
		auto& childs = getChildMeshes();

		childs[0]->rot_ = rootMesh->rot_;
		childs[0]->pos_ = rootMesh->pos_ + tnl::Vector3::TransformCoord(tnl::Vector3{ 0,0,enemySize_.z / 2 }, childs[0]->rot_);
	}

	void EnemyPawn::enemyUpdate(float deltaTime) {
		seq_.update(deltaTime);
	}

	void EnemyPawn::registerTarget(const tnl::Vector3& target) {
		tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0,0,1 }, getRootMesh()->rot_);
		targetPos_ = tnl::Vector3::TransformCoord(tnl::Vector3::Normalize(target), getRootMesh()->rot_);
		float mark = (target.x < 0) ? -1.0f : 1.0f;
		targetRot_ = getRootMesh()->rot_ * tnl::Quaternion::RotationAxis({ 0,1,0 }, targetPos_.angle(forward) * mark);
	}

	/// --------------------------
	/// �V�[�P���X
	/// --------------------------

	bool EnemyPawn::seqCheckCurrentState(float deltaTime) {
		switch (currentState_) {
		case e_EnemyState::Wandering:	seq_.change(&EnemyPawn::seqWandering); break;
		case e_EnemyState::Searching:; break;
		case e_EnemyState::Chasing:; break;
			return true;
		}
	}

	bool EnemyPawn::seqWandering(float deltaTime) {
		{
			// �ڕW�n�_�ɓ��B���Ă���i�ڕW�n�_���Ȃ��j���̏���
			if (targetPos_.length() <= FLT_EPSILON) {// ������
			}
		}

		{// �f�o�b�O�p�C���v�b�g
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD6)) {
				enemyRotateSpeed_ = 1.0f;
				registerTarget({ 50,0,0 });
				seq_.change(&EnemyPawn::seqRotate);
			}
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD4)) {
				enemyRotateSpeed_ = -1.0f;
				registerTarget({ -50,0,0 });
				seq_.change(&EnemyPawn::seqRotate);
			}
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_NUMPAD8)) {
				registerTarget({ 0,0,50 });
				seq_.change(&EnemyPawn::seqRotate);
			}
		}

		// �ڕW�n�_�ɓ��B������A�ړI�n�� {0,0,0} ��
		if (fabs(targetPos_.x - getRootMesh()->pos_.x) <= FLT_EPSILON &&
			fabs(targetPos_.z - getRootMesh()->pos_.z) <= FLT_EPSILON
			)
		{
			targetPos_ = { 0,0,0 };
		}
		
		return true;
	}

	bool EnemyPawn::seqRotate(float deltaTime) {
		tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0,0,1 }, getRootMesh()->rot_);

		if (forward.angle(targetPos_) > 0) {
			getRootMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(enemyRotateSpeed_));
		}

		if (fabs(forward.angle(targetPos_)) < tnl::ToRadian(1.5f)) {
			getRootMesh()->rot_ = targetRot_;
			seq_.change(&EnemyPawn::seqMove);
		}

		return true;
	}

	bool EnemyPawn::seqMove(float deltaTime) {
		if (fabs(needMoveAmount_) <= FLT_EPSILON) {
			seq_.change(&EnemyPawn::seqCheckCurrentState);
			return true;
		}
		
		float actualMoveAmount = (std::min)(fabs(enemyMoveSpeed_), fabs(needMoveAmount_));
		getRootMesh()->pos_ += tnl::Vector3::TransformCoord({ 0,0,actualMoveAmount }, getRootMesh()->rot_);
		needMoveAmount_ -= actualMoveAmount;

		return true;
	}

}