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
		// �V�����t�@�N�g���[�T�u�N���X��ݒ� ( ��������G�l�~�[�̃t�@�N�g���[ )
		inline void setCurrentFactory(Shared<Base_EnemyFactory> newFactory) { currentFactory_ = newFactory; }

		// ���݂̃t�@�N�g���[�T�u�N���X�� createEnemy �����s���Alist�ɉ�����
		void generateEnemy();

		// �S�G�l�~�[�̖��t���[�����������s
		void processAllEnemy(float deltaTime);

		// �S�G�l�~�[�̕`������s
		void renderAllEnemy(Shared<Atl3DCamera> camera, float deltaTime);

	private:
		Shared<Base_EnemyFactory> currentFactory_ = nullptr;
		std::list<Shared<Base_Enemy>> enemyList_{};

	};

}