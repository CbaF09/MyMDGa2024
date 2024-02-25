#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_ActorData.h"

namespace atl {

	/// <summary>
	/// 
	/// �G�l�~�[�̃X�e�[�^�X�ƃf�[�^��ێ�
	/// 
	/// </summary>

	class EnemyData final : public Base_ActorData {
	public:
		// ��������G�l�~�[ID ( CSV ��A��Q�� )
		explicit EnemyData(int32_t enemyID);

		// �Q�b�^�[
		inline const std::string& getEnemyName() const { return enemyName_; }
		inline const std::string& getEnemyMeshPath() const { return enemyMeshFilepath_; }
		inline float getEnemySizeScale() const { return enemySizeScale_; }
		inline const std::string& getEnemyTexturePath() const { return enemyTextureFilepath_; }
		inline const std::string& getEnemyMaterialPath() const { return enemyMaterialFilepath_; }
		inline int32_t getEnemyExp() const { return exp_; }

	private:
		// �G�̖��O
		std::string enemyName_ = "";
		// �G��|�������Ɏ�ɓ���o���l
		int32_t exp_ = 0;
		// ���b�V���̃t�@�C���p�X	
		std::string enemyMeshFilepath_ = "";
		// ���b�V���̃T�C�Y�X�P�[��
		float enemySizeScale_ = 0;
		// �e�N�X�`���̃t�@�C���p�X
		std::string enemyTextureFilepath_ = "";
		// �}�e���A���̃t�@�C���p�X
		std::string enemyMaterialFilepath_ = "";
	};

}