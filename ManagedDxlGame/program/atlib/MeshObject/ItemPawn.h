#pragma once
#include "Base_MeshObject.h"

namespace atl {

	/// <summary>
	/// 
	/// ダンジョンに落ちているアイテム ( 3D )
	/// アイテムの情報を持っており、拾う事でインベントリに追加できる
	/// 
	/// </summary>

	class PlayerPawn;
	class Item;
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
		// arg ... ダンジョンシーンへの弱参照を設定する為に引数で渡す
		void initialize(std::weak_ptr<DungeonScene> dungeonScene);

	private:
		// 立方体の大きさ
		const float ITEM_PAWN_SIZE = 200;
		// アイテムのY高さ ( 地面 は -500 )
		const float ITEM_POSITION_Y = 250;
		// アイテムの回転速度
		const float ITEM_ROT_SPEED = 0.5f; 

		// ダンジョンシーンへの弱参照
		std::weak_ptr<DungeonScene> weakDungeonScene_;
		// アイテムポーンは、アイテムデータを持つ
		Shared<Item> item_ = nullptr;
		// アイテムの周りに浮かぶパーティクル用
		Shared<dxe::Particle> itemParticle_ = nullptr;
	};

}