#pragma once
#include "Base_MultiMeshObject.h"

namespace atl {
	
	/// <summary>
	/// 
	/// 右手に持っている杖 ( 3D )
	/// 見た目だけ
	/// 
	/// </summary>

	class PlayerPawn;

	class MagicWand final : public Base_MultiMeshObject {
	public:
		// コンストラクタ
		explicit MagicWand(std::weak_ptr<PlayerPawn> player);

		// 子メッシュの補正 ( 詳細は Base_MultiMeshObject )
		void adjustChildsMeshes(float deltaTime) override;

	private:
		// 杖の大きさ
		const float WAND_SIZE = 25.0f;

		// RootMesh の最初の回転状態を保存
		tnl::Quaternion initRot_;

		// プレイヤーポーンへの弱参照 ( プレイヤーに追従させるため )
		std::weak_ptr<PlayerPawn> weakPlayerPawn;
	};

}