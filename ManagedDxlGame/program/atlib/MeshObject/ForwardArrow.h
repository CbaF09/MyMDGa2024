#pragma once
#include "Base_MeshObject.h"

namespace atl {

	/// <summary>
	/// 
	/// プレイヤーが向いている向き ( カメラの向きではなく、移動先・攻撃先となる方角 ) を分かりやすくする為の 3DUI
	/// プレイヤーが向いている地面に、枠のような感じで表示する
	/// 
	/// </summary>

	class PlayerPawn;

	// 定数系
	namespace {
		// 枠の大きさ ( 半径 )
		const float FORWARD_ARROW_SIZE = 300;
		// 枠の高さ ( -500 で地面と同じ位置。地面より僅かに上に設定 )
		const float FORWARD_ARROW_Y = -480;
	}

	class ForwardArrow final : public Base_MeshObject {
	public:
		explicit ForwardArrow(std::weak_ptr<const PlayerPawn> player);

		void renderObject(const Shared<Atl3DCamera> camera,float deltaTime = 0) override;

	private:
		// プレイヤーへの弱参照
		std::weak_ptr<const PlayerPawn> weakPlayerPawn_;
	};

}