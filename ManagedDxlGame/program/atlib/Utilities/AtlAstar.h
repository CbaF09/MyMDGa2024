#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class AtlAstar {
		// インナークラス
		enum class e_AstarNodeState {
			NONE,
			WALL, 
			CLOSED,
			OPEN,
			START,
			GOAL
		};

		class AstarNode {
		public:
			tnl::Vector2i pos_{ 0,0 };
			e_AstarNodeState state = e_AstarNodeState::NONE;

			int32_t realCost_ = 0;
			int32_t guessCost_ = 0;
			int32_t score_ = 0;

			Shared<AstarNode> parentNode = nullptr;

			bool operator < (const AstarNode& node) const {	return score_ > node.score_; }

		};
		

	public:
		static void settingAstarMapdata(int32_t mapW, int32_t mapH);
		static void settingAstarStartAndGoal(const tnl::Vector2i& startPos, const tnl::Vector2i& goalPos);
		// A*経路探索実行
		static std::vector<Shared<AstarNode>> doAster(std::vector<std::vector<Shared<AstarNode>>>& nodes, Shared<AstarNode> nowNode);


	private:
		static int32_t mapWidth_;
		static int32_t mapHeight_;
		static std::vector<std::vector<Shared<AstarNode>>> nodeData;

		// 指定座標が有効か ( OPEN 可能か ) 判定
		static bool isEnableMapPosition(const tnl::Vector2i& pos, const std::vector<std::vector<Shared<AstarNode>>>& nodes);
		Shared<AstarNode> getSmallestScoreNodeFromOpenNodes(const std::vector<std::vector<Shared<AstarNode>>>& nodes);

	};

}