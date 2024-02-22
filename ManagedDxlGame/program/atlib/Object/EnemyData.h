#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_ActorData.h"

namespace atl {

	/// <summary>
	/// 
	/// エネミーのステータスとデータを保持
	/// 
	/// </summary>

	class EnemyData final : public Base_ActorData {
	public:
		// 生成するエネミーID ( CSV のA列参照 )
		explicit EnemyData(int32_t enemyID);

		// ゲッター
		inline const std::string& getEnemyName() { return enemyName_; }
		inline const std::string& getEnemyMeshPath() { return enemyMeshFilepath_; }
		inline const int32_t getEnemySizeScale() { return enemySizeScale_; }
		inline const std::string& getEnemyTexturePath() { return enemyTextureFilepath_; }
		inline const std::string& getEnemyMaterialPath() { return enemyMaterialFilepath_; }
		inline const int32_t getEnemyExp() { return exp_; }

	private:
		// 敵の名前
		std::string enemyName_ = "";
		// 敵を倒した時に手に入る経験値
		int32_t exp_ = 0;
		// メッシュのファイルパス	
		std::string enemyMeshFilepath_ = "";
		// メッシュのサイズスケール
		int32_t enemySizeScale_ = 0;
		// テクスチャのファイルパス
		std::string enemyTextureFilepath_ = "";
		// マテリアルのファイルパス
		std::string enemyMaterialFilepath_ = "";
	};

}