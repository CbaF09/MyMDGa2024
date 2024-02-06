//#pragma once
//#include "../dxlib_ext/dxlib_ext.h"
//
//namespace atl {
//
//	class AtlAstar {
//		enum class e_AstarNodeState {
//			NONE,
//			CLOSED,
//			OPEN,
//		};
//
//		// ノードクラス
//		class AstarNode {
//		public:
//			// XY座標
//			tnl::Vector2i pos_{ 0,0 };
//			// ステート
//			e_AstarNodeState state = e_AstarNodeState::NONE;
//			// 実コスト
//			int32_t realCost_ = 0;
//			// 推定コスト
//			int32_t guessCost_ = 0;
//			// 通過コスト
//			int32_t score_ = 0;
//
//			// 親ノードへのポインタ
//			Shared<AstarNode> parentNode = nullptr;
//
//			// スコア比較用 ( 親ノードと比較
//			bool operator < (const AstarNode& node) const {	return score_ > node.score_; }
//		};
//		
//
//	public:
//		// A*経路探索実行
//		static std::vector<Shared<AstarNode>> doAster(std::vector<);
//
//
//	private:
//
//	};
//
//}