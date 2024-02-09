#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_ActorData.h"

namespace atl {

	class EnemyData final : public Base_ActorData {
	public:
		EnemyData();

		// ゲッター
		inline Shared<dxe::Texture> getEnemyTexture() { return enemyTexture_; }
		inline const std::string& getEnemyName() { return enemyName_; }
		inline const int32_t getEnemyExp() { return exp_; }

	private:
		// 敵の種類 ( ID )
		int32_t enemyID_ = 0;
		// 敵の名前
		std::string enemyName_ = "";
		// 敵を倒した時に手に入る経験値
		int32_t exp_ = 0;
		// 敵に適用するテクスチャデータ
		Shared<dxe::Texture> enemyTexture_ = nullptr;
	};

}