#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "Base_ActorData.h"

namespace atl {

	class EnemyData final : public Base_ActorData {
	public:
		EnemyData();

		// ÉQÉbÉ^Å[
		inline Shared<dxe::Texture> getEnemyTexture() { return enemyTexture_; }

	private:
		int32_t enemyID_ = 0;
		Shared<dxe::Texture> enemyTexture_ = nullptr;
	};

}