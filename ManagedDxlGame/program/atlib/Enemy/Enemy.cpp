#include "Enemy.h"

namespace atl {

	/// <summary>
	/// 
	/// �X���C��
	/// 
	void SlimeEnemy::initialize() {
		// ���b�V����ݒ�
		auto mesh = dxe::Mesh::CreateFromFileMV("mesh/mv1/SlimeMesh.mv1", 5);
		mesh->rot_ *= tnl::Quaternion::RotationAxis({ 1,0,0 }, tnl::ToRadian(90));
		setMesh(mesh);

		// �e�N�X�`����ݒ�
		auto texture = dxe::Texture::CreateFromFile("graphics/Texture/EnemyTexture/Albedo.png");
		mesh->setTexture(texture);
	}

	void SlimeEnemy::process(float deltaTime) {
		// �f�o�b�O�p ��������]
		getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1));

	}
}