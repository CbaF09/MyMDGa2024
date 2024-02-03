#include <string>
#include "DungeonScene.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/PlayerStatsManager.h"
#include "../MeshObject/Wall.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/EnemyPawn.h"
#include "../MeshObject/ItemPawn.h"
#include "../MeshObject/PlayerPawn.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	void DungeonScene::render(float deltaTime, const Shared<Atl3DCamera> camera) {

		{//MeshObject �Q�������_�����O
			for (const auto& wall : walls_) { wall->renderObject(camera); }
			for (const auto& groundTile : groundTiles_) { groundTile->renderObject(camera); }

			originStairs_->renderObjects(camera);
			for (const auto& enemy : enemies_) { enemy->renderObjects(camera); }
			for (const auto& item : items_) { item->renderObjects(camera); }
			player_->render(deltaTime);
		}

	}

	void DungeonScene::draw2D(float deltaTime) {
		debug_displayDungeonParam(deltaTime);
		if (!isOpenMenu) { TextLogManager::getTextLogManager()->displayTextLog(60, 400, deltaTime); }
		drawUI(deltaTime);
	}

	void DungeonScene::drawUI(float deltaTime) {
		drawHPbar();
	}

	void DungeonScene::drawHPbar() {
		// �w�i ( �g ) �̕`��
		auto HPbarBackGround = ResourceManager::getResourceManager()->getUIres("graphics/UI/HPbarBackGround.png");
		DrawExtendGraph(HP_BAR_LEFT_UP_POINT.x, HP_BAR_LEFT_UP_POINT.y, HP_BAR_RIGHT_DOWN_POINT.x,HP_BAR_RIGHT_DOWN_POINT.y,HPbarBackGround,true);

		// �ԃQ�[�W�̕`��
		auto HPbarRed = ResourceManager::getResourceManager()->getUIres("graphics/UI/HPbarRed.bmp");
		DrawExtendGraph(
			HP_BAR_LEFT_UP_POINT.x + HP_BAR_ADJUST_VALUE.x, 
			HP_BAR_LEFT_UP_POINT.y + HP_BAR_ADJUST_VALUE.y, 
			HP_BAR_RIGHT_DOWN_POINT.x - HP_BAR_ADJUST_VALUE.x, 
			HP_BAR_RIGHT_DOWN_POINT.y - HP_BAR_ADJUST_VALUE.y, HPbarRed, true);

		// �΃Q�[�W�̕`�� ( HP ���[���łȂ��ꍇ�̂ݕ`�� )
		if (!PlayerStatsManager::getPlayerStatsManager()->isZeroPlayerHP()) {
			auto hpPersent = PlayerStatsManager::getPlayerStatsManager()->getPlayerCurrentHPpersent();
			auto HPbarGreen = ResourceManager::getResourceManager()->getUIres("graphics/UI/HPbarGreen.bmp");
			DrawExtendGraph(
				HP_BAR_LEFT_UP_POINT.x + HP_BAR_ADJUST_VALUE.x,
				HP_BAR_LEFT_UP_POINT.y + HP_BAR_ADJUST_VALUE.y,
				(HP_BAR_RIGHT_DOWN_POINT.x * hpPersent) - HP_BAR_ADJUST_VALUE.x,
				HP_BAR_RIGHT_DOWN_POINT.y - HP_BAR_ADJUST_VALUE.y, HPbarGreen, true);
		}
	}

	void DungeonScene::sceneUpdate(float deltaTime) {
		seq_.update(deltaTime);

		{// �J�����R���g���[�� ( �ړ��̌�ɂ��Ȃ��ƁA�Ȃ񂩕ςɂȂ� )
			player_->getPlayerCamera()->cameraControl(CAMERA_ROT_SPEED);
		}

		{// �����_�[ ( �J�����R���g���[���̌� )
			render(deltaTime, player_->getPlayerCamera());
		}

		{// 2D�n�̕`��e�L�X�g���O�̕`��
			draw2D(deltaTime);
		}

		{// �f�o�b�O�p����
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) seq_.change(&DungeonScene::seqInit);
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) PlayerStatsManager::getPlayerStatsManager()->damagedPlayer(10);
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);
		}
	}


	//---------------------
	// �V�[�P���X
	//---------------------


	// ������
	bool DungeonScene::seqInit(float deltaTime) {
		generateDungeon();
		player_->initialize();
		seq_.change(&DungeonScene::seqAllTurnFlagOff);
		return true;
	}

	// �s���t���O�I�t�p�V�[�P���X
	bool DungeonScene::seqAllTurnFlagOff(float deltaTime) {
		player_->setIsAlreadyTurn();
		for (auto& enemy : enemies_) { enemy->setIsAlreadyTurn(); }

		currentTurn_ = e_turnState::KEY_INPUT;
		seq_.change(&DungeonScene::seqTurnStateProcess);
		return true;
	}

	// ���݃^�[���ɉ����ď������s
	bool DungeonScene::seqTurnStateProcess(float deltaTime) {
		switch (currentTurn_) {
		case e_turnState::KEY_INPUT:
			processKeyInput(deltaTime);
			break;
		case e_turnState::PLAYER_MOVE:
			processPlayerMoveTurn(deltaTime);
			break;
		}
		return true;
	}

	void DungeonScene::processKeyInput(float deltaTime) {
		if (!isOpenMenu) {
			if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_A, eKeys::KB_S, eKeys::KB_D)) {
				// �v���C���[���ł��L�[���͑҂������Ă��āA�v���C���[����� 1 �t���[�����x���̂ŁA��� �Ă�
				player_->playerUpdate(deltaTime);
				currentTurn_ = e_turnState::PLAYER_MOVE;
			}

			if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
				player_->playerUpdate(deltaTime);
				currentTurn_ = e_turnState::PLAYER_MOVE;
			}
		}

		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			isOpenMenu = !isOpenMenu;
			player_->playerUpdate(deltaTime);
		}

	}

	// �v���C���[�̈ړ�����
	void DungeonScene::processPlayerMoveTurn(float deltaTime) {
		// �v���C���[�̃^�[��
		if (!player_->getIsAlreadyTurn()) {
			player_->playerUpdate(deltaTime);
		}

		// �G�l�~�[�̃^�[��
		bool allEnemyTurned = true;
		
		// �ړ�����
		for (auto& enemy : enemies_) {
			if (enemy->getIsAlreadyMove()) { continue; }
			enemy->enemyUpdate(deltaTime);
			allEnemyTurned = false;
		}

		// �s������
		for (auto& enemy : enemies_) {
			if (enemy->getIsAlreadyAction()) { continue; }
			enemy->enemyUpdate(deltaTime);
			allEnemyTurned = false;
		}

		// �v���C���[�ړ������E�G�l�~�[�s������������A�V�[�P���X�J��
		if (player_->getIsAlreadyTurn() && allEnemyTurned) seq_.immediatelyChange(&DungeonScene::seqAllTurnFlagOff);
	}

	//---------------------
	// �_���W��������
	//---------------------

	void DungeonScene::initDungeon() {
		walls_.clear();
		groundTiles_.clear();
		enemies_.resize(3);
		items_.resize(5);
	}

	void DungeonScene::generateDungeon() {
		initDungeon();

		auto dungeonCreater = DungeonCreater::getDungeonCreater();

		dungeonCreater->createDungeon();
		auto& fieldData = dungeonCreater->getFieldCells();

		for (int x = 0; x < fieldData.size(); ++x) {
			for (int y = 0; y < fieldData[x].size(); ++y) {
				// ���̈ʒu���ǂȂ�ǂ𐶐��A�ǂłȂ��Ȃ�n�ʂ𐶐�
				if (fieldData[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_WALL) {
					generateWall(x, y);
				}
				else {
					generateGround(x, y);
				}
			}
		}

		{// �e��X�|�[�� ( DungeonCreater��������X�|�[���ʒu���擾���A���� )
			auto& playerSpawnPos = dungeonCreater->getPlayerSpawnPos();
			player_ = std::make_shared<PlayerPawn>();
			player_->playerSpawn2Dpos(playerSpawnPos);

			auto& stairsSpawnPos = dungeonCreater->getStairsSpawnPos();
			// �Z���S���̔����̑傫���Ő���
			originStairs_ = std::make_shared<Stairs>(stairsSpawnPos, tnl::Vector3{ CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2 });

			auto& enemySpawnPos = dungeonCreater->getEnemySpawnPos();
			for (int i = 0; i < dungeonCreater->getEnemySpawnNum(); ++i) {
				// TODO : �}�W�b�N�i���o�[�B��ŃG�l�~�[�̌����ڂ�ς������ɕύX���鎖�B
				enemies_[i] = std::make_shared<EnemyPawn>(enemySpawnPos[i], tnl::Vector3{ 200, 200, 200 });
				enemies_[i]->assignWeakPlayer(player_);
			}

			auto& itemSpawnPos = dungeonCreater->getItemSpawnPos();
			for (int i = 0; i < dungeonCreater->getItemSpawnNum(); ++i) {
				items_[i] = std::make_shared<ItemPawn>(itemSpawnPos[i]);
				items_[i]->assignWeakPlayer(player_);
			}
		}

		// �e�N�X�`���ɂ�郁�������[�N�΍�
		dxe::Texture::DestroyUnReferenceTextures();
	}

	void DungeonScene::generateWall(int generatePosX, int generatePosZ) {
		if (!originWall_) originWall_ = std::make_shared<Wall>(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH ,CELL_FULL_LENGTH });
		Shared<Wall> newWall = std::make_shared<Wall>(originWall_->getMesh());
		newWall->getMesh()->pos_.x = static_cast<float>(generatePosX * CELL_FULL_LENGTH);
		newWall->getMesh()->pos_.y = static_cast<float>(CELL_FULL_LENGTH) / 2;
		newWall->getMesh()->pos_.z = static_cast<float>(generatePosZ * CELL_FULL_LENGTH);
		newWall->setMeshSizeVector3(tnl::Vector3{ CELL_FULL_LENGTH, CELL_FULL_LENGTH, CELL_FULL_LENGTH });
		walls_.emplace_back(newWall);
	}

	void DungeonScene::generateGround(int generatePosX, int generatePosZ) {
		if (!originGroundTile_) originGroundTile_ = std::make_shared<GroundTile>(tnl::Vector3{ CELL_FULL_LENGTH,CELL_FULL_LENGTH,0 }); // Plane ���f���̐����ɂ�����֌W�ŁAY�̏���Z�̒l�����Ă܂�
		Shared<GroundTile> newGroundTile = std::make_shared<GroundTile>(originGroundTile_->getMesh());
		newGroundTile->getMesh()->pos_.x = static_cast<float>(generatePosX * CELL_FULL_LENGTH);
		newGroundTile->getMesh()->pos_.z = static_cast<float>(generatePosZ * CELL_FULL_LENGTH);
		groundTiles_.emplace_back(newGroundTile);
	}

	//------------------------
	// �f�o�b�O�p
	//------------------------
	
	void DungeonScene::debug_displayDungeonParam(float deltaTime) {
		DrawGridGround(player_->getPlayerCamera(), 50, 20);
		DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);

		player_->debug_displayPlayerParam(600, 0);

		DrawStringEx(0, 100, -1, "curentTurn ... [ %d ]", currentTurn_);

		// �K�i�̈ʒu
		//auto& stairsPos = originStairs_->get2Dpos();
		//DrawStringEx(0, 100, -1, "stairsPos ... [ %d, %d ]", stairsPos.x, stairsPos.y);

		// �G�̈ʒu
		for (int i = 0; i < 3; ++i) {
			DrawStringEx(600, 115 + (15 * i), -1, "Enemy[%d] pos ... [ %d , %d ]", i, enemies_[i]->get2Dpos().x, enemies_[i]->get2Dpos().y);
		}
	}

}