#include "PlayerStatsManager.h"
#include "TextLogManager.h"
#include "../Utilities/AtlRandom.h"
#include "../Utilities/AtlString.h"

namespace atl {

	PlayerStatsManager* PlayerStatsManager::p_instance_ = nullptr;

	PlayerStatsManager* PlayerStatsManager::getPlayerStatsManager() {
		if (!p_instance_) p_instance_ = new PlayerStatsManager();
		return p_instance_;
	};

	void PlayerStatsManager::deletePlayerStatsManager() {
		if (p_instance_) {
			delete p_instance_;
			p_instance_ = nullptr;
		};
	}

	void PlayerStatsManager::changePlayerCurrentHP(int32_t value) {
		playerCurrentHP_ += value;
		// HPがゼロ以下になったら、ゼロにする
		if (playerCurrentHP_ < 0) { playerCurrentHP_ = 0; }

		// 現在HPの割合を計算
		calcAndSetCurrentPlayerHPpersent();
	}

	// ① 計算式を適用してからプレイヤーのHPを減らす。
	// ② 受けたダメージをテキストログ先頭に追加する
	// 計算式 ... 最大ダメージ = attackPower, 最小ダメージ = attackPower * 7/8 , の間でランダムな値
	void PlayerStatsManager::damagedPlayer(int32_t attackPower) {
		int32_t maxDamage = attackPower;
		int32_t minDamage = static_cast<int32_t>(attackPower * 7 / 8);

		auto damage = mtRandomRangeInt(minDamage, maxDamage);

		changePlayerCurrentHP(-damage);
		std::string damageLog = "プレイヤーは　" + atl::convertFullWidthNumber(damage) + "　のダメージを受けた";
		TextLogManager::getTextLogManager()->addTextLog(damageLog);
	}

	bool PlayerStatsManager::isZeroPlayerHP() {
		return playerCurrentHP_ == 0;
	}

	// プレイヤーの現在HPと最大HPの割合を計算
	float PlayerStatsManager::calcAndSetCurrentPlayerHPpersent() {
		if (playerMaxHP_ > 0) { // ゼロ除算回避
			playerCurrentHPpersent =
				static_cast<float>(playerCurrentHP_) / static_cast<float>(playerMaxHP_);
		}

		if (playerCurrentHPpersent < 0) {
			playerCurrentHPpersent = 0.0f;
		}

		return playerCurrentHPpersent;
	}
}