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
//		// �m�[�h�N���X
//		class AstarNode {
//		public:
//			// XY���W
//			tnl::Vector2i pos_{ 0,0 };
//			// �X�e�[�g
//			e_AstarNodeState state = e_AstarNodeState::NONE;
//			// ���R�X�g
//			int32_t realCost_ = 0;
//			// ����R�X�g
//			int32_t guessCost_ = 0;
//			// �ʉ߃R�X�g
//			int32_t score_ = 0;
//
//			// �e�m�[�h�ւ̃|�C���^
//			Shared<AstarNode> parentNode = nullptr;
//
//			// �X�R�A��r�p ( �e�m�[�h�Ɣ�r
//			bool operator < (const AstarNode& node) const {	return score_ > node.score_; }
//		};
//		
//
//	public:
//		// A*�o�H�T�����s
//		static std::vector<Shared<AstarNode>> doAster(std::vector<);
//
//
//	private:
//
//	};
//
//}