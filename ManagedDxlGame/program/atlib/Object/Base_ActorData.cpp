#include "Base_ActorData.h"
#include "../Utilities/AtlRandom.h"

namespace atl {

	void Base_ActorData::changeCurrentHP(int32_t addsubValue) {
		currentHP_ += addsubValue;
		// HP‚ªƒ[ƒ‚ğ‰º‰ñ‚Á‚½‚çAƒ[ƒ‚ÉƒNƒ‰ƒ“ƒv
		if (currentHP_ < 0) { currentHP_ = 0; }
		// HP ‚ª maxHP ‚ğ’´‚¦‚½‚çAmaxHP ‚ÉƒNƒ‰ƒ“ƒv
		if (currentHP_ > maxHP_) { currentHP_ = maxHP_; }

		// Œ»İHP‚ÌŠ„‡‚ğŒvZ
		calcAndSetCurrentHPpersent();
	}

	int32_t Base_ActorData::damaged(int32_t attackedPower) {
		int32_t maxDamage = attackedPower;
		int32_t minDamage = static_cast<int32_t>(attackedPower * 7 / 8);

		auto damage = mtRandomRangeInt(minDamage, maxDamage);

		changeCurrentHP(-damage);
		return damage;
	}

	bool Base_ActorData::isZeroHP() {
		return currentHP_ == 0;
	}

	void Base_ActorData::calcAndSetCurrentHPpersent() {
		if (maxHP_ > 0) { // ƒ[ƒœZ‰ñ”ğ
			currentHPpersent_ =
				static_cast<float>(currentHP_) / static_cast<float>(maxHP_);
		}

		if (currentHPpersent_ < 0) {
			currentHPpersent_ = 0.0f;
		}
	}

}