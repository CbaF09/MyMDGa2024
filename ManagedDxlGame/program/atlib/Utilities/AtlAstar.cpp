//#include "AtlAstar.h"
//
//namespace atl {
//
//	bool AtlAstar::isEnableMapPosition(const tnl::Vector2i& pos, const std::vector<std::vector<Shared<AstarNode>>>& nodes) {
//		if (pos.x < 0 ||
//			pos.y < 0 ||
//			pos.x >= mapWidth_ ||
//			pos.y >= mapHeight_) {
//			return false;
//		}
//
//		if (nodes[pos.x][pos.y]->state == e_AstarNodeState::START) return false;
//		if (nodes[pos.x][pos.y]->state == e_AstarNodeState::NONE) return true;
//		if (nodes[pos.x][pos.y]->state == e_AstarNodeState::GOAL) return true;
//
//		return false;
//	}
//
//	Shared<AtlAstar::AstarNode> AtlAstar::getSmallestScoreNodeFromOpenNodes(const std::vector<std::vector<Shared<AstarNode>>>& nodes) {
//		Shared<AstarNode> returnNode = nullptr;
//
//		for (int x = 0; x < mapWidth_; ++x) {
//			for (int y = 0; y < mapHeight_; ++y) {
//				if (nodes[x][y]->state != e_AstarNodeState::OPEN) continue;
//				if (returnNode == nullptr) returnNode = nodes[x][y];
//				
//				if (returnNode->score_ > nodes[x][y]->score_) returnNode = nodes[x][y];
//				else if (returnNode->score_ == nodes[x][y]->score_) {
//					if (returnNode->realCost_ > nodes[x][y]->realCost_) returnNode = nodes[x][y];
//				}
//			}
//		}
//
//		return returnNode;
//	}
//
//	void AtlAstar::settingAstarMapdata(int32_t mapW, int32_t mapH) {
//		mapWidth_ = mapW;
//		mapHeight_ = mapH;
//		nodeData.resize(mapW);
//		for (int x = 0; x < nodeData.size(); ++x) {
//			nodeData[x].resize(mapH);
//			for (int y = 0; y < nodeData[x].size(); ++y) {
//				nodeData[x][y] = std::make_shared<AstarNode>();
//			}
//		}
//	}
//
//	void AtlAstar::settingAstarStartAndGoal(const tnl::Vector2i& startPos, const tnl::Vector2i& goalPos) {
//		nodeData[startPos.x][startPos.y]->state = e_AstarNodeState::START;
//		nodeData[goalPos.x][goalPos.y]->state = e_AstarNodeState::GOAL;
//	}
//
//	std::vector<Shared<AtlAstar::AstarNode>> AtlAstar::doAster(std::vector<std::vector<Shared<AstarNode>>>& nodes, Shared<AstarNode> nowNode) {
//
//		// スタート地点のスコア計算
//		if (nowNode->state == e_AstarNodeState::START) {
//			nowNode->score_ = nowNode->realCost_ + nowNode->guessCost_;
//		}
//		
//		// 周囲8方向
//		tnl::Vector2i dir_[8] =
//		{	{1,0},
//			{-1,0},
//			{0,1},
//			{0,-1},
//			{1,1},
//			{-1,1},
//			{1,-1},
//			{-1,-1}
//		};
//
//		// 八方向を調べる
//		for (int i = 0; i < 8; ++i) {
//			tnl::Vector2i next = nowNode->pos_ + dir_[i];
//			
//			// 調べ先がオープン不可能ならスキップ
//			if (!isEnableMapPosition(next, nodes)) continue;
//
//
//
//		}
//
//
//	}
//
//}