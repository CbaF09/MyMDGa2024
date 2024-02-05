#include <string>
#include "DungeonScene.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/SceneManager.h"
#include "../MeshObject/Wall.h"
#include "../MeshObject/GroundTile.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/EnemyPawn.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/ItemPawn.h"
#include "../Utilities/Atl3DCamera.h"


namespace atl {

	DungeonScene::~DungeonScene(){
		{// �_���W�����V�[���Ŏg���Ă��郊�\�[�X�����

			// ������郊�\�[�X�̃t�@�C���p�X�̈ꎞ�I�z����쐬
			std::vector<std::string> tempDeleteRes = {
				"graphics/UI/HPbarBackGround.png",
				"graphics/UI/HPbarRed.bmp",
				"graphics/UI/HPbarGreen.bmp"
			};

			// �t�@�C���p�X���������B����̐������s���f�o�b�O���O�ɏo��
			for (const auto& res : tempDeleteRes) {
				bool isDelete = ResourceManager::getResourceManager()->deleteResource(res);
				if (!isDelete) { tnl::DebugTrace("\n------------------------------\nDungeonScene::�f�X�g���N�^ ��������� => ����ɉ������Ă��܂���"); }
				else { tnl::DebugTrace("\n------------------------------\nDungeonScene::�f�X�g���N�^ ��������� => ����"); }
			}
			tnl::DebugTrace("\n------------------------------\n"); // ���O�����Â炢�̂ōŌ�ɉ��s�Ɛ؂����������
		}

		{// �_���W�����N���G�C�^�[�̃V���O���g�������
			DungeonCreater::getDungeonCreater()->deleteDungeonCreater();
		}

		{// �e�L�X�g���O�̃V���O���g�������
			TextLogManager::getTextLogManager()->deleteTextLogManager();
		}
	}

	void DungeonScene::render(float deltaTime, const Shared<Atl3DCamera> camera) {

		{//MeshObject �Q�������_�����O
			for (const auto& wall : walls_) { wall->renderObject(camera); }
			for (const auto& groundTile : groundTiles_) { groundTile->renderObject(camera); }

			if (originStairs_) originStairs_->renderObjects(camera);
			for (const auto& enemy : enemies_) { enemy->renderObjects(camera); }
			for (const auto& item : items_) { item->renderObjects(camera); }
			if (player_)player_->render(deltaTime);
		}
	}

	void DungeonScene::draw2D(float deltaTime) {
		// �f�o�b�O�p
		debug_displayDungeonParam(deltaTime);

		drawUI(deltaTime);

		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);
		
		// ���݊K�w�̕`��͐^�����ȉ�ʂ̏�ɂ�肽���̂ŁA���̈ʒu
		if (isNextFloorTransition) { drawNextFloorTransition(); }

	}

	// ���̊K�w��`�悷��
	void DungeonScene::drawNextFloorTransition() {
		// �t�H���g�T�C�Y�ύX
		int beforeFontSize = GetFontSize();
		SetFontSize(NEXT_FLOOR_TEXT_FONTSIZE);

		// ���݊K�w���A�ŏ�K ( �N���A�K ) �łȂ��ꍇ�̕`��
		if (currentFloor_ != MAX_FLOOR) {
			DrawStringEx(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, -1, "���C�Y�}���̏C���� [%d / %d]", currentFloor_,MAX_FLOOR);
		}
		else { // �N���A�K�̏ꍇ�̕`��
			DrawStringEx(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, -1, "���C�Y�}���̏C���� [ �ŏ�K ]", currentFloor_);
		}
		
		SetFontSize(beforeFontSize);
	}

	void DungeonScene::drawOnStairsChoice() {
	}

	void DungeonScene::drawUI(float deltaTime) {
		// HP �o�[ �\��
		drawHPbar();

		// ���j���[���J���Ă��鎞�̓��O�\������
		if (!isOpenMenu_) { TextLogManager::getTextLogManager()->displayTextLog(TEXT_LOG_POSITION.x, TEXT_LOG_POSITION.y, deltaTime); }

		// �K�i�̏�ɏ�������̑I�����`��
		if (isPlayerOnStairs_) { drawOnStairsChoice(); }
	}

	void DungeonScene::drawHPbar() {
		// �w�i ( �g ) �̕`��
		auto HPbarBackGround = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/HPbarBackGround.png");
		DrawExtendGraph(HP_BAR_LEFT_UP_POINT.x, HP_BAR_LEFT_UP_POINT.y, HP_BAR_RIGHT_DOWN_POINT.x,HP_BAR_RIGHT_DOWN_POINT.y,HPbarBackGround,true);

		// �ԃQ�[�W�̕`��
		auto HPbarRed = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/HPbarRed.bmp");
		DrawExtendGraph(
			HP_BAR_LEFT_UP_POINT.x + HP_BAR_ADJUST_VALUE.x, 
			HP_BAR_LEFT_UP_POINT.y + HP_BAR_ADJUST_VALUE.y, 
			HP_BAR_RIGHT_DOWN_POINT.x - HP_BAR_ADJUST_VALUE.x, 
			HP_BAR_RIGHT_DOWN_POINT.y - HP_BAR_ADJUST_VALUE.y, HPbarRed, true);

		// �΃Q�[�W�̕`�� ( HP ���[���łȂ��ꍇ�̂ݕ`�� )
		if (!player_->getPlayerData()->isZeroHP()) {
			auto hpPersent = player_->getPlayerData()->getCurrentHPpersent();
			auto HPbarGreen = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/HPbarGreen.bmp");
			DrawExtendGraph(
				HP_BAR_LEFT_UP_POINT.x + HP_BAR_ADJUST_VALUE.x,
				HP_BAR_LEFT_UP_POINT.y + HP_BAR_ADJUST_VALUE.y,
				static_cast<int>((HP_BAR_RIGHT_DOWN_POINT.x * hpPersent)) - HP_BAR_ADJUST_VALUE.x,
				HP_BAR_RIGHT_DOWN_POINT.y - HP_BAR_ADJUST_VALUE.y, HPbarGreen, true);
		}
	}
	
	void DungeonScene::sceneUpdate(float deltaTime) {
		seq_.update(deltaTime);

		// �J�����̃A�b�v�f�[�g
		player_->getPlayerCamera()->update();

		// �����_�[ ( �J�����A�b�v�f�[�g�̌� )
		render(deltaTime, player_->getPlayerCamera());

		// 2D�n�̕`��
		draw2D(deltaTime);

		{// �f�o�b�O�p����
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) 	generateDungeon();;
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);
		}
	}

	//---------------------
	// �V�[�P���X
	//---------------------

	// �����ݒ� ( DungeonScene�̃C���X�^���X������A��x�����ʂ� ) 
	bool DungeonScene::seqInit(float deltaTime) {
		// �t�F�[�h�͐^��������X�^�[�g
		FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(255);

		// �v���C���[�̐����Ə�����
		player_ = std::make_shared<PlayerPawn>();
		player_->initialize(shared_from_this());

		// ���݂̊K�w�� ������
		currentFloor_ = 0;

		// �{�V�[�P���X�ɑJ��
		seq_.change(&DungeonScene::seqToNextFloor);
		return true;
	}

	// �s���t���O�I�t�p�V�[�P���X
	bool DungeonScene::seqAllTurnFlagOff(float deltaTime) {
		player_->offFlagIsAlreadyTurn();
		for (auto& enemy : enemies_) { enemy->offFlagIsAlreadyTurn(); }

		currentTurn_ = e_turnState::KEY_INPUT;
		seq_.change(&DungeonScene::seqTurnStateProcess);
		return true;
	}

	// ���݃^�[���ɉ����ď������s
	bool DungeonScene::seqTurnStateProcess(float deltaTime) {
		// �^�[���̓��ʑJ�ڂ�����ꍇ�A�L�q
		if (seq_.isStart()) {

			auto player2Dpos = player_->getPlayer2Dpos();
			{// �K�i�ɏ�����ꍇ
				// �K�i�ɏ��t���O�������Ă��Ȃ��ꍇ�A�K�i�ɏ�����t���O�𗧂Ă�
				if (!isPlayerOnStairs_) {
					auto stairs2Dpos = originStairs_->get2Dpos();
					if (stairs2Dpos.x == player2Dpos.x && stairs2Dpos.y == player2Dpos.y) {
						isPlayerOnStairs_ = true;
						currentTurn_ = e_turnState::PLAYER_ON_STAIRS;
					}
				}
			}

			{// �v���C���[��HP���[���ɂȂ��Ă���ꍇ�A���ڃQ�[���I�[�o�[�V�[�P���X�ɑJ��
				if (player_->getPlayerData()->isZeroHP()) {
					seq_.change(&DungeonScene::seqGameOver);
				}
			}
		}

		switch (currentTurn_) {
		case e_turnState::KEY_INPUT:
			processKeyInput(deltaTime);
			break;
		case e_turnState::PLAYER_MOVE:
			processPlayerMoveTurn(deltaTime);
			break;
		case e_turnState::PLAYER_ON_STAIRS:
			processPlayerOnStairs(deltaTime);
			break;
		}
		return true;
	}

	bool DungeonScene::seqGameOver(float deltaTime) {
		// �ŏ��̃t���[���Ńt�F�[�h�A�E�g���s���A�t�F�[�h�A�E�g������������Q�[���I�[�o�[�V�[���ɑJ��
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			SceneManager::getSceneManager()->changeScene(std::make_shared<GameOverScene>());
		}
		return true;
	}

	void DungeonScene::processKeyInput(float deltaTime) {
		// �t�F�[�h�C�� �E �A�E�g���ł���Α���s�\
		if (FadeInOutManager::getFadeInOutManager()->isFading()) return;

		if (!isOpenMenu_) {
			if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_A, eKeys::KB_S, eKeys::KB_D)) {
				// �v���C���[���ł��L�[���͑҂������Ă��āA�v���C���[����� 1 �t���[�����x���̂ŁA��� �Ă�
				player_->playerUpdate(deltaTime);
				// �ړ������ꍇ�A�K�i�ɏ���Ă���t���O���I�t�ɂ���
				isPlayerOnStairs_ = false;
				currentTurn_ = e_turnState::PLAYER_MOVE;
			}

			if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
				player_->playerUpdate(deltaTime);
				currentTurn_ = e_turnState::PLAYER_MOVE;
			}
		}

		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			isOpenMenu_ = !isOpenMenu_;
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
		// �S�G�l�~�[�̍s�����I��������t���O���
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
		
		// �v���C���[�E�G�l�~�[���s������������A�V�[�P���X�J��
		if (player_->getIsAlreadyTurn() && allEnemyTurned) seq_.change(&DungeonScene::seqDeadEnemyProcess);
	}

	// �K�i�ɏ�������̏���
	void DungeonScene::processPlayerOnStairs(float deltaTime) {
		// ( �f�o�b�O�� ) �ړ����܂����H�ɃC�G�X�̎�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
			seq_.change(&DungeonScene::seqToNextFloor);
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_2)) { // �m�[�̎�
			isPlayerOnStairs_ = false;
			currentTurn_ = e_turnState::KEY_INPUT;
		}
	}

	// ���̊K�w�ɐi�ޏ���
	bool DungeonScene::seqToNextFloor(float deltaTime) {
		// �ŏ��̃t���[���Ńt�F�[�h�A�E�g�J�n
		if (seq_.isStart()) {
			// �K�i�ɏ���Ă���t���O���I�t�ɂ���
			isPlayerOnStairs_ = false;
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
			// �t���A�����C���N�������g
			++currentFloor_;

		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			// ���K�w�ɑJ�ڒ��t���O�𗧂Ă�
			isNextFloorTransition = true;

			// �O�b�ҋ@
			SEQ_CO_YIELD_RETURN_TIME(3.0f, deltaTime, [&] {})


			// �ő�K�w�ɓ��B������A�N���A�V�[���ɑJ��
			if (currentFloor_ >= MAX_FLOOR) {
				SceneManager::getSceneManager()->changeScene(std::make_shared<GameClearScene>());
			}
			else {
				// �_���W�����𐶐����A�t�F�[�h�C������ʂ�\��������
				generateDungeon();
				FadeInOutManager::getFadeInOutManager()->startFadeIn();
				seq_.change(&DungeonScene::seqAllTurnFlagOff);
			}
			
			// ���K�w�֑J�ڒ��t���O���I�t��
			isNextFloorTransition = false;
		}

		SEQ_CO_END
	}

	bool DungeonScene::seqDeadEnemyProcess(float deltaTime) {
		bool allEnemyAlive = true;

		for (auto it = enemies_.begin(); it != enemies_.end();) {
			if ((*it)->getEnemyData()->isZeroHP()) {
				// HP ���[���ɂȂ��Ă���G�𔭌�
				allEnemyAlive = false;
				// HP ���[���ɂȂ��Ă��� enemy �� update ���� ( enemy �� ���S���o �́AenemyUpdate �ŊǗ� )
				(*it)->enemyUpdate(deltaTime);
				// Dead �ɂȂ����G�l�~�[���폜����
				if ((*it)->getCurrentState() == EnemyPawn::e_EnemyState::Dead) {
					it = enemies_.erase(it);
					continue;
				}
			}
			++it;
		}

		if (allEnemyAlive) {
			seq_.change(&DungeonScene::seqAllTurnFlagOff);
		}
		
		return true;
	}

	//---------------------
	// �_���W��������
	//---------------------

	void DungeonScene::initDungeon() {
		walls_.clear();
		groundTiles_.clear();
		enemies_.clear();
		items_.clear();
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
			// �v���C���[
			auto& playerSpawnPos = dungeonCreater->getPlayerSpawnPos();
			player_->playerSpawn2Dpos(playerSpawnPos);

			// �K�i
			auto& stairsSpawnPos = dungeonCreater->getStairsSpawnPos();
			// �Z���S���̔����̑傫���Ő���
			originStairs_ = std::make_shared<Stairs>(stairsSpawnPos, tnl::Vector3{ CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2,CELL_FULL_LENGTH / 2 });

			// �G�l�~�[
			auto& enemySpawnPos = dungeonCreater->getEnemySpawnPos();
			for (int i = 0; i < dungeonCreater->getEnemySpawnNum(); ++i) {
				// TODO: �}�W�b�N�i���o�[�B�{�ԗp�G�l�~�[����������ɓ����ɂȂ�Ƃ�����ׂ�
				auto enemy = std::make_shared<EnemyPawn>(enemySpawnPos[i], tnl::Vector3{ 200, 200, 200 });
				enemy->assignWeakPlayer(player_);
				enemies_.emplace_back(enemy);
			}

			// �A�C�e��
			auto& itemSpawnPos = dungeonCreater->getItemSpawnPos();
			for (int i = 0; i < dungeonCreater->getItemSpawnNum(); ++i) {
				auto item = std::make_shared<ItemPawn>(itemSpawnPos[i]);
				item->assignWeakPlayer(player_);
				items_.emplace_back(item);
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

		DrawStringEx(0, 75, -1, "curentTurn ... [ %d ]", currentTurn_);

		// �K�i�̈ʒu
		if (originStairs_) {
			auto& stairsPos = originStairs_->get2Dpos();
			DrawStringEx(0, 100, -1, "stairsPos ... [ %d, %d ]", stairsPos.x, stairsPos.y);
		}

		DrawStringEx(0, 125, -1, "currentFloor ... [ %d ]", currentFloor_);
	}

}