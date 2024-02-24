#pragma once
#include "../Item/Item.h"

namespace atl {

	/// <summary>
	/// 
	/// ダンジョンに落ちているアイテム ( 3D )
	/// アイテムの情報を持っており、拾う事でインベントリに追加できる
	/// 
	/// </summary>

	class PlayerPawn;
	class DungeonScene;

	class ItemPawn final : public Base_MeshObject {
	public:
		~ItemPawn() { itemParticle_.reset(); }

		explicit ItemPawn(const tnl::Vector2i& spawnPos);
		
		// ゲッター
		inline const Shared<Item> getItem() { return item_; }

		//　オーバーライド
		void renderObject(const Shared<Atl3DCamera> camera,float deltaTime) override;

		// 遅延コンストラクタ
		void initialize();

	private:
		// 立方体の大きさ
		const float ITEM_PAWN_SIZE = 200;
		// アイテムのY高さ ( 地面 は -500 )
		const float ITEM_POSITION_Y = 250;
		// アイテムの回転速度
		const float ITEM_ROT_SPEED = 0.5f; 

		// アイテムポーンは、アイテムデータを持つ
		Shared<Item> item_ = nullptr;
		// アイテムの周りに浮かぶパーティクル用
		Shared<dxe::Particle> itemParticle_ = nullptr;
	};

}