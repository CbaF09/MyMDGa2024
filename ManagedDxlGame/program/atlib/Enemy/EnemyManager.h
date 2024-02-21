#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "EnemyFactory.h"

namespace atl {

	class Atl3DCamera;

	class EnemyManager final {
	//------------------------------------------
	// �V���O���g���݌v
	public:
		static EnemyManager* getEnemyManager();
		inline static void deleteEnemyManager() { delete getEnemyManager(); }
	private:
		EnemyManager() {};
		~EnemyManager() {};
	//------------------------------------------

	public:
		// �Q�b�^�[ ( �G�l�~�[���X�g )
		const std::list<Shared<Base_Enemy>>& getEnemyList() { return enemyList_; }

		// �V�����t�@�N�g���[�T�u�N���X��ݒ� ( ��������G�l�~�[�̃t�@�N�g���[ )
		inline void setCurrentFactory(Shared<Base_EnemyFactory> newFactory) { currentFactory_ = newFactory; }

		// ���݂̃t�@�N�g���[�T�u�N���X�� createEnemy �����s���Alist�ɉ�����
		void generateEnemy(const tnl::Vector2i& generate2Dpos);

		// �S�G�l�~�[�̕`������s
		void renderAllEnemy(Shared<Atl3DCamera> camera, float deltaTime);

		// �S�G�l�~�[�̈ړ������s
		// ret ... �S�G�l�~�[�̈ړ����������Ă��� => true, �������Ă��Ȃ� => false
		bool moveAllEnemy(float deltaTime);

		// �S�G�l�~�[�̍s�������s
		// ret ... �S�G�l�~�[�̍s�����������Ă��� => true, �������Ă��Ȃ� => false
		bool actionAllEnemy(float deltaTime);

		// ���S�t���O���I���̃G�l�~�[�����X�g�����������
		void deadEnemyErase();

		// �G�l�~�[���X�g���N���A����
		inline void clearEnemyList() { enemyList_.clear(); }

		// �G�l�~�[�̃^�[�������t���O���I�t�ɂ���
		void allEnemyOffFlagIsAlreadyTurn();

		// �ړ����������Ă��Ȃ��G�l�~�[��process����

	private:
		// ���ݐݒ肳��Ă���T�u�t�@�N�g���[
		Shared<Base_EnemyFactory> currentFactory_ = nullptr;

		// �G�l�~�[���X�g�BgenerateEnemy �ɂ���ď���ɒǉ�����Ă���
		std::list<Shared<Base_Enemy>> enemyList_{};

	};

}