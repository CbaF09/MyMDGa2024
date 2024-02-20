#include "Enemy.h"

namespace atl {

	/// <summary>
	/// 
	/// スライム
	/// 
	void SlimeEnemy::initialize() {
		// メッシュを設定
		auto mesh = dxe::Mesh::CreateFromFileMV("mesh/mv1/SlimeMesh.mv1", 5);
		mesh->rot_ *= tnl::Quaternion::RotationAxis({ 1,0,0 }, tnl::ToRadian(90));
		setMesh(mesh);

		// テクスチャを設定
		auto texture = dxe::Texture::CreateFromFile("graphics/Texture/EnemyTexture/Albedo.png");
		mesh->setTexture(texture);
	}

	void SlimeEnemy::process(float deltaTime) {
		// デバッグ用 ゆっくり回転
		getMesh()->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1));

	}
}