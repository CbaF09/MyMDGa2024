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

	// 定数系
	namespace {
		// アイテムポーン ( 立方体 ) の大きさ
		const float ITEM_PAWN_SIZE = 200;
		// アイテムポーンのY高さ ( 地面 は -500 )
		const float ITEM_POSITION_Y = 250;
		// アイテムポーンの回転速度
		const float ITEM_ROT_SPEED = 0.5f;
	}

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
		// アイテムポーンは、アイテムデータを持つ
		Shared<Item> item_ = nullptr;
		// アイテムの周りに浮かぶパーティクル用
		Shared<dxe::Particle> itemParticle_ = nullptr;

	};

}