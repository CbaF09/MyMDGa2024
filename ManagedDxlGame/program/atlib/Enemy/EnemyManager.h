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
		EnemyManager() {
			// ���Ń����_���X�|�[�������삷��悤�ɁA�X�|�[����⃊�X�g�������
			// �����I�ɂ́A�K�w���Ƃɕω�����悤�ɂ���
			addSpawnableEnemyFactory(std::make_shared<BlueSlimeFactory>());
			addSpawnableEnemyFactory(std::make_shared<GreenSlimeFactory>());
			addSpawnableEnemyFactory(std::make_shared<RedSlimeFactory>());
		};
		~EnemyManager() {};
	//------------------------------------------

	public:
		// �Q�b�^�[ ( �G�l�~�[���X�g )
		const std::list<Shared<Base_Enemy>>& getEnemyList() { return enemyList_; }

		// �V�����G�l�~�[�T�u�t�@�N�g���[�N���X�𒼐ڐݒ�
		inline void setEnemyFactory(Shared<Base_EnemyFactory> newFactory) { currentFactory_ = newFactory; }

		// �����_���ɐV�����G�l�~�[�T�u�t�@�N�g���[�N���X��ݒ�
		void setRandomEnemyFactory();

		// �����_���ɃG�l�~�[�T�u�t�@�N�g���[���ݒ肳���ۂ̌���ǉ�
		inline void addSpawnableEnemyFactory(Shared<Base_EnemyFactory> spawnableFactory) { spawnableEnemyFactoryList_.emplace_back(spawnableFactory); }

		// �X�|�[�����̃G�l�~�[�T�u�t�@�N�g���[���X�g���N���A����
		inline void clearSpawnableEnemyFactoryList() { spawnableEnemyFactoryList_.clear(); }

		// ���݂̃t�@�N�g���[�T�u�N���X�� createEnemy �����s���Alist�ɉ�����
		// arg ... �X�|�[������G�l�~�[��2D���W��̈ʒu
		void spawnEnemy(const tnl::Vector2i& spawn2Dpos);

		// �X�|�[���G�l�~�[�������_���ɐݒ肵�Ă���A�X�|�[�����������s
		void spawnRandomEnemy(const tnl::Vector2i spawn2Dpos);

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

	private:
		// ���ݐݒ肳��Ă���T�u�t�@�N�g���[
		Shared<Base_EnemyFactory> currentFactory_ = nullptr;

		// �ݒ���̃T�u�t�@�N�g���[�̃��X�g�BaddEnemyFactory�ɂ���Ēǉ�����Ă���
		std::vector<Shared<Base_EnemyFactory>> spawnableEnemyFactoryList_{};

		// ���ݑ��݂��Ă���G�l�~�[���X�g
		std::list<Shared<Base_Enemy>> enemyList_{};

	};

}