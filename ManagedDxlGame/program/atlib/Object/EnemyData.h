#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_ActorData.h"

namespace atl {

	class EnemyData final : public Base_ActorData {
	public:
		EnemyData();

		// �Q�b�^�[
		inline Shared<dxe::Texture> getEnemyTexture() { return enemyTexture_; }
		inline const std::string& getEnemyName() { return enemyName_; }
		inline const int32_t getEnemyExp() { return exp_; }

	private:
		// �G�̎�� ( ID )
		int32_t enemyID_ = 0;
		// �G�̖��O
		std::string enemyName_ = "";
		// �G��|�������Ɏ�ɓ���o���l
		int32_t exp_ = 0;
		// �G�ɓK�p����e�N�X�`���f�[�^
		Shared<dxe::Texture> enemyTexture_ = nullptr;
	};

}