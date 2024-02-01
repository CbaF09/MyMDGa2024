#include "MagicWand.h"
#include "PlayerPawn.h"

namespace atl {

	// プレイヤーに保持された状態で生成される時
	MagicWand::MagicWand(std::weak_ptr<const PlayerPawn> player) : weakPlayerPawn(player) {
		auto playerLock = weakPlayerPawn.lock();
		if (playerLock) {
			isHeldByPlayer = true;

			auto& playerPos = playerLock->getPlayerPos();

			float initSize = 25;

			auto stick = dxe::Mesh::CreateConeCylinderMV(initSize / 4, initSize / 10, initSize * 2.5f);
			stick->rot_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(180));
			stick->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(90));
			initRot_ = stick->rot_;
			stick->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/Mable_White.jpg"));
			setRootMesh(stick);

			auto sphere = dxe::Mesh::CreateSphereMV(initSize / 3);
			sphere->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/Mable_White.jpg"));
			addChildMesh(sphere);

			auto outerRing = dxe::Mesh::CreateTorusMV(initSize / 1.2f, initSize / 4);
			outerRing->rot_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(90));
			outerRing->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/Mable_White.jpg"));
			addChildMesh(outerRing);
		}
	}

	// プレイヤーが保持していない（フィールドに落ちている）時
	MagicWand::MagicWand(const tnl::Vector2i spawnPos, float initSize) : wandSize_(initSize) {
		auto stick = dxe::Mesh::CreateConeCylinderMV(initSize / 4,initSize/10, initSize * 2.5f);
		stick->rot_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(180+75));
		stick->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(90));
		stick->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/Mable_White.jpg"));
		setRootMesh(stick);

		set2DposAnd3Dpos(spawnPos);
		stick->pos_ = {0, initSize, 0};

		auto sphere = dxe::Mesh::CreateSphereMV(initSize / 3);
		sphere->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/Mable_White.jpg"));
		addChildMesh(sphere);

		auto outerRing = dxe::Mesh::CreateTorusMV(initSize/1.2f, initSize/4);
		outerRing->rot_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(90));
		outerRing->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/Mable_White.jpg"));
		addChildMesh(outerRing);

		auto innerRing = dxe::Mesh::CreateTorusMV(initSize/2, initSize / 4);
		innerRing->rot_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(90));
		innerRing->setTexture(dxe::Texture::CreateFromFile("graphics/Texture/Mable_White.jpg"));
		addChildMesh(innerRing);
	}

	void MagicWand::adjustmentChildMeshes() {
		// プレイヤーが保持していない時の動き
		if (isHeldByPlayer == false) {
			auto stick = getRootMesh();
			stick->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1));

			auto& childs = getChildMeshes();
			// sphere
			childs[0]->pos_ = stick->pos_ + tnl::Vector3::TransformCoord({ 0,-wandSize_/1.2f,0 }, stick->rot_);
			childs[0]->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,1 }, tnl::ToRadian(3));

			// outerRing
			childs[1]->pos_ = stick->pos_ + tnl::Vector3::TransformCoord({ 0,-wandSize_/1.2f,0 }, stick->rot_);
			childs[1]->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1));

			// innerRing
			childs[2]->pos_ = stick->pos_ + tnl::Vector3::TransformCoord({ 0,-wandSize_/1.2f,0 }, stick->rot_);
			childs[2]->rot_ *= tnl::Quaternion::RotationAxis({ 1,1,0 }, tnl::ToRadian(3));
			return;
		}

		// プレイヤーが保持している時の動き
		if (isHeldByPlayer == true) {
			auto player = weakPlayerPawn.lock();
			if (player) {
				auto cameraPos = player->getPlayerCamera()->pos_;
				auto& cameraRot = player->getPlayerCamera()->getCameraRot();

				auto& stick = getRootMesh();
				stick->pos_ = cameraPos + tnl::Vector3::TransformCoord({ 60,-25,80 }, cameraRot);
				stick->rot_ = initRot_ * cameraRot;

				auto& childs = getChildMeshes();

				// sphere
				childs[0]->pos_ = stick->pos_ + tnl::Vector3::TransformCoord({ 0,25,0 }, cameraRot);
				childs[0]->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1));

				// outerRing
				childs[1]->pos_ = stick->pos_ + tnl::Vector3::TransformCoord({ 0,25,0 }, cameraRot);
				childs[1]->rot_ = tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(90)) * cameraRot;

			}
		}
	}

}
