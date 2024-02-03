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
		// HP���[���ȉ��ɂȂ�����A�[���ɂ���
		if (playerCurrentHP_ < 0) { playerCurrentHP_ = 0; }

		// ����HP�̊������v�Z
		calcAndSetCurrentPlayerHPpersent();
	}

	// �@ �v�Z����K�p���Ă���v���C���[��HP�����炷�B
	// �A �󂯂��_���[�W���e�L�X�g���O�擪�ɒǉ�����
	// �v�Z�� ... �ő�_���[�W = attackPower, �ŏ��_���[�W = attackPower * 7/8 , �̊ԂŃ����_���Ȓl
	void PlayerStatsManager::damagedPlayer(int32_t attackPower) {
		int32_t maxDamage = attackPower;
		int32_t minDamage = static_cast<int32_t>(attackPower * 7 / 8);

		auto damage = mtRandomRangeInt(minDamage, maxDamage);

		changePlayerCurrentHP(-damage);
		std::string damageLog = "�v���C���[�́@" + atl::convertFullWidthNumber(damage) + "�@�̃_���[�W���󂯂�";
		TextLogManager::getTextLogManager()->addTextLog(damageLog);
	}

	bool PlayerStatsManager::isZeroPlayerHP() {
		return playerCurrentHP_ == 0;
	}

	// �v���C���[�̌���HP�ƍő�HP�̊������v�Z
	float PlayerStatsManager::calcAndSetCurrentPlayerHPpersent() {
		if (playerMaxHP_ > 0) { // �[�����Z���
			playerCurrentHPpersent =
				static_cast<float>(playerCurrentHP_) / static_cast<float>(playerMaxHP_);
		}

		if (playerCurrentHPpersent < 0) {
			playerCurrentHPpersent = 0.0f;
		}

		return playerCurrentHPpersent;
	}
}