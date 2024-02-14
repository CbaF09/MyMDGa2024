#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	class MagicRuneWindow {

	public:
		void draw();

	private:
		const tnl::Vector2i RUNE_BACKGRAPH_POSITION{ 950,60 };
		const int32_t RUNE_BACKGRAPH_OFFSET = 70;

	};

}