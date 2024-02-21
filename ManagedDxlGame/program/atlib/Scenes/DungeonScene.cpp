#include <string>
#include <algorithm>
#include "DungeonScene.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include "../Singletons/DungeonCreater.h"
#include "../Singletons/TextLogManager.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/SceneManager.h"
#include "../MagicRuneSystem/MagicRuneSystemManager.h"
#include "../MagicRuneSystem/MagicRune.h"
#include "../MeshObject/Wall.h"
#include "../MeshObject/Stairs.h"
#include "../MeshObject/PlayerPawn.h"
#include "../MeshObject/ItemPawn.h"
#include "../Object/SelectWindow.h"
#include "../Object/MenuWindow.h"
#include "../Utilities/Atl3DCamera.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/EnemyFactory.h"

namespace atl {

	DungeonScene::DungeonScene() {
		walls_.clear();
		groundTiles_.clear();
		items_.clear();

		player_ = std::make_shared<PlayerPawn>();
		menuWindow_ = std::make_shared<MenuWindow>(player_->getPlayerData()->getInventory());
	}

	DungeonScene::~DungeonScene() {
		// ������郊�\�[�X�̃t�@�C���p�X�̈ꎞ�I�z����쐬
		std::vector<std::string> tempDeleteRes = {
			"graphics/UI/HPbarBackGround.png",
			"graphics/UI/HPbarRed.bmp",
			"graphics/UI/HPbarGreen.bmp",
			"graphics/UI/Instruction.png",
			"sound/BGM/DungeonSceneBGM.ogg",
			"sound/SE/DungeonSceneCloseMenu.ogg",
			"sound/SE/DungeonSceneOpenMenu.ogg",
			"sound/SE/DungeonSceneOpenSelectMenu.ogg",
		};

		// ���\�[�X���
		for (const auto& res : tempDeleteRes) {
			ResourceManager::getResourceManager()->deleteResource(res);
		}

		// �t�H���g�f�[�^�̉��
		DeleteFontToHandle(NEXT_FLOOR_FONT);
		DeleteFontToHandle(LEVEL_STRING_FONT);
	}

	void DungeonScene::render(float deltaTime, const Shared<Atl3DCamera> camera) {
		// �X�J�C�X�t�B�A�̃A�b�v�f�[�g
		skysphere_.update(camera);

		for (const auto& ground : groundTiles_) { ground->renderObject(camera); }
		for (const auto& wall : walls_) { wall->renderObject(camera); }
		if (stairs_) { stairs_->renderObjects(camera, deltaTime); }
		if (player_) { player_->render(deltaTime); };
		for (const auto& item : items_) { item->renderObject(camera, deltaTime); }

		// �G�`��
		EnemyManager::getEnemyManager()->renderAllEnemy(camera, deltaTime);
	}

	void DungeonScene::draw2D(float deltaTime) {

		drawUI(deltaTime);

		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);

		// ���݊K�w�̕`��͐^�����ȉ�ʂ̏�ɂ�肽���̂ŁA���̈ʒu
		if (isNextFloorTransition) { drawNextFloorTransition(); }

		// �f�o�b�O���[�h���ɕ`�悷��}�b�v�₻�̑����
		if (isDebug) {
			debug_displayMap(deltaTime);
			debug_displayDungeonParam(deltaTime);
		}
	}

	// ���̊K�w��`�悷��
	void DungeonScene::drawNextFloorTransition() {
		// ���݊K�w���A�ŏ�K ( �N���A�K ) �łȂ��ꍇ�̕`��
		if (currentFloor_ != MAX_FLOOR) {
			DrawStringToHandleEx(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, -1, NEXT_FLOOR_FONT, "���C�Y�}���̏C���� [%d / %d]", currentFloor_, MAX_FLOOR);
		}
		else { // �N���A�K�̏ꍇ�̕`��
			DrawStringToHandleEx(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, -1, NEXT_FLOOR_FONT, "���C�Y�}���̏C���� [ �ŏ�K ]", currentFloor_);
		}
	}

	void DungeonScene::drawUI(float deltaTime) {
		// HP �o�[ �\��
		drawHPbar();

		// ��������̕`��
		drawInstruction();

		// ���j���[���J���Ă��鎞�̓��O�\������,���x���\������,�����x�\������
		if (!player_->getIsMenuOpen()) {
			TextLogManager::getTextLogManager()->displayTextLog(TEXT_LOG_POSITION.x, TEXT_LOG_POSITION.y, deltaTime);
			drawLevel();
			drawInvatation();
		}

		// ���j���[���J���Ă���Ԃ̕`��
		if (player_->getIsMenuOpen()) {
			menuWindow_->draw(deltaTime);
			drawMinimap(deltaTime);
		}

		// ���[�������E�B���h�E�`��
		magicRuneWindow_.draw();

		// �Z���N�g�E�B���h�E�����鎞�́A�`�悷��
		if (isSelectWindow_) selectWindow_.draw(deltaTime);
	}

	void DungeonScene::drawHPbar() {
		// �w�i ( �g ) �̕`��
		auto HPbarBackGround = ResourceManager::getResourceManager()->getGraphRes("graphics/UI/HPbarBackGround.png");
		DrawExtendGraph(HP_BAR_LEFT_UP_POINT.x, HP_BAR_LEFT_UP_POINT.y, HP_BAR_RIGHT_DOWN_POINT.x, HP_BAR_RIGHT_DOWN_POINT.y, HPbarBackGround, true);

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

		// HP �̐��l�\�� ( �t�H���g�̓��x����\�����Ă���t�H���g�𗬗p )
		auto maxHP = player_->getPlayerData()->getMaxHP();
		auto currentHP = player_->getPlayerData()->getCurrentHP();
		DrawStringToHandleEx(static_cast<float>(HP_STRING_POSITION.x), static_cast<float>(HP_STRING_POSITION.y), -1, LEVEL_STRING_FONT, "HP :  %d / %d ", currentHP, maxHP);
	}

	void DungeonScene::drawLevel() {
		DrawStringToHandleEx(static_cast<float>(LEVEL_STRING_POSITION.x), static_cast<float>(LEVEL_STRING_POSITION.y), -1, LEVEL_STRING_FONT, "���x�� ... [ %d ]", player_->getPlayerData()->getCurrentLevel());
	}

	void DungeonScene::drawInstruction() {
		// ��������̔w�i�`��
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 125);
		DrawBoxEx({ static_cast<float>(INSTRUCTION_POSITION.x),static_cast<float>(INSTRUCTION_POSITION.y),0 }, static_cast<float>(INSTRUCTION_BACK_BOX_SIZE.x), static_cast<float>(INSTRUCTION_BACK_BOX_SIZE.y), true, GetColor(0, 0, 255));
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

		// ��������̉摜�`��
		DrawRotaGraph(INSTRUCTION_POSITION.x, INSTRUCTION_POSITION.y, INSTRUCTION_SIZE, 0, ResourceManager::getResourceManager()->getGraphRes("graphics/UI/Instruction.png"), true);
	}


	void DungeonScene::drawInvatation() {
		// ���ҏ� UI ��`��
		// ���݋󕠓x��10����1���P�x��
		SetDrawBright(currentSatiety_ / 10, currentSatiety_ / 10, currentSatiety_ / 10);
		DrawRotaGraph(INVATATION_POSITION.x, INVATATION_POSITION.y, INVATATION_SIZE, tnl::ToRadian(INVATATION_ANGLE), ResourceManager::getResourceManager()->getGraphRes("graphics/UI/Invatation.png"), true);
		// �`��P�x�����ɖ߂�
		SetDrawBright(255, 255, 255);

		// ������u���ҏ�v��`��B���x����\�����Ă���t�H���g�𗬗p�B
		DrawStringToHandleEx(static_cast<float>(INVATATION_STRING_POSITION.x), static_cast<float>(INVATATION_STRING_POSITION.y), -1, LEVEL_STRING_FONT, "���ҏ�");
	}

	void DungeonScene::drawMinimap(float deltaTime) {
		auto& field = DungeonCreater::getDungeonCreater()->getFieldCells();

		// �t�B�[���h���̕`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MINIMAP_ALPHA);
		for (int x = 0; x < field.size(); ++x) {
			for (int y = 0; y < field[x].size(); ++y) {
				// �����Ȃ��ꍇ�A����continue
				if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_NONE) { continue; }
				// �������̏ꍇ�A����continue
				if (field[x][y].isDiscoverByPlayer == false) { continue; }

				// cellType �ɉ����ĐF��ς���
				int drawColor = 0;
				if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_ROOM) {
					// ��
					drawColor = GetColor(255, 255, 255);
				}
				else if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_PATH) {
					// ��
					drawColor = GetColor(0, 0, 255);
				}
				// �`��ʒu�̌v�Z
				tnl::Vector2i drawPos = calcDrawMinimapPos(x, y);
				// �`��
				DrawBoxEx({ static_cast<float>(drawPos.x),static_cast<float>(drawPos.y) ,0 }, static_cast<float>(MINIMAP_CELL_SIZE), static_cast<float>(MINIMAP_CELL_SIZE), true, drawColor);
			}
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �ǂ͓����x�����̍��ŕ`��
		for (int x = 0; x < field.size(); ++x) {
			for (int y = 0; y < field[x].size();++y) {
				// �ǈȊO�͑������^�[��
				if (field[x][y].cellType_ != DungeonCreater::e_FieldCellType::CELL_TYPE_WALL) { continue; }
				// �������̏ꍇ�A����continue
				if (field[x][y].isDiscoverByPlayer == false) { continue; }

				// �`��ʒu�̌v�Z
				tnl::Vector2i drawPos = calcDrawMinimapPos(x, y);
				// �`�� ( �� )
				DrawBoxEx({ (float)drawPos.x,(float)drawPos.y ,0 }, static_cast<float>(MINIMAP_CELL_SIZE), static_cast<float>(MINIMAP_CELL_SIZE), true, GetColor(0, 0, 0));
			}
		}

		{// �v���C���[�̈ʒu�`��
			auto& player2Dpos = player_->getPlayer2Dpos();
			tnl::Vector2i playerDrawPos = calcDrawMinimapPos(player2Dpos.x, player2Dpos.y);
			DrawCircle(playerDrawPos.x, playerDrawPos.y, MINIMAP_PLAYER_SIZE, GetColor(255, 255, 0));
		}
	}

	const tnl::Vector2i DungeonScene::calcDrawMinimapPos(int32_t x, int32_t y) {
		// Y �� X ���t�ɂȂ��Ă���̂́A�O������ԂƂ̐����������ׂȂ̂ŁA���Ȃ��ł��B
		int32_t drawPosX = (y * MINIMAP_CELL_SIZE) + MINIMAP_LEFTUP_POSITION.x;
		int32_t drawPosY = (x * MINIMAP_CELL_SIZE) + MINIMAP_LEFTUP_POSITION.y;
		return { drawPosX,drawPosY };
	};

	void DungeonScene::sceneUpdate(float deltaTime) {
		// �V�[�P���X�A�b�v�f�[�g
		seq_.update(deltaTime);

		// �J�����̃A�b�v�f�[�g
		player_->getPlayerCamera()->update();

		// �����_�[ ( �J�����A�b�v�f�[�g�̌� )
		render(deltaTime, player_->getPlayerCamera());

		// 2D�n�̕`��
		draw2D(deltaTime);

		{// �f�o�b�O���[�h�֘A
			// O + P �L�[�ŁA�f�o�b�O���[�h�ɐ؂�ւ� ( OverPower )
			if (tnl::Input::IsKeyDown(eKeys::KB_O) && tnl::Input::IsKeyDownTrigger(eKeys::KB_P)) { isDebug = !isDebug; }
			// �f�o�b�O���[�h���ɃX�y�[�X�������ƁA�_���W�����Đ�������
			if (isDebug) {
				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) 	generateDungeon();;
			}
		}
	}

	void DungeonScene::openMenu() {
		player_->openMenuBook();
		// �A�C�e���ꗗ�̕\�����X�V����
		menuWindow_->itemWindowsUpdate();
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneOpenMenu.ogg", DX_PLAYTYPE_BACK);
	}

	void DungeonScene::closeMenu() {
		player_->closeMenuBook();
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneCloseMenu.ogg", DX_PLAYTYPE_BACK);
	}

	void DungeonScene::openSelectWindow(const std::string& question) {
		selectWindow_.setSelectWindowQuestionText(question);
		ResourceManager::getResourceManager()->playSoundRes("sound/SE/DungeonSceneOpenSelectMenu.ogg", DX_PLAYTYPE_BACK);
		isSelectWindow_ = true;
	}

	void DungeonScene::closeSelectWindow() {
		isSelectWindow_ = false;
	}

	void DungeonScene::minimapUpdate(const tnl::Vector2i& openCellPos) {
		auto dCreater = DungeonCreater::getDungeonCreater();

		// �J�����Ƃ��Ă���Z���̃Z���^�C�v���擾
		auto openCellType = dCreater->getFieldCellType(openCellPos);

		// �Z���^�C�v�ɉ����ĕ���
		switch (openCellType) {
			// ���������Ȃ�u���[�N
		case DungeonCreater::e_FieldCellType::CELL_TYPE_NONE:
			break;
			// �ǂȂ�A�������I�[�v�����ău���[�N
		case DungeonCreater::e_FieldCellType::CELL_TYPE_WALL:
			dCreater->discoverFieldCell(openCellPos);
			break;
			// �ʘH�Ȃ�A�������I�[�v�����ău���[�N
		case DungeonCreater::e_FieldCellType::CELL_TYPE_PATH:
			dCreater->discoverFieldCell(openCellPos);
			dCreater->discoverFieldCell(openCellPos + tnl::Vector2i{ 0,1 });
			dCreater->discoverFieldCell(openCellPos + tnl::Vector2i{ 0,-1 });
			dCreater->discoverFieldCell(openCellPos + tnl::Vector2i{ 1,0 });
			dCreater->discoverFieldCell(openCellPos + tnl::Vector2i{ -1,0 });
			break;
			// �����Ȃ�A�������I�[�v�����Ă���A���ɔ����ς݂̏ꏊ�łȂ������m�F���Ă���ċA����
		case DungeonCreater::e_FieldCellType::CELL_TYPE_ROOM:
			dCreater->discoverFieldCell(openCellPos);
			if (!dCreater->isDiscoverFieldCell(openCellPos + tnl::Vector2i{ 0,1 })) {
				minimapUpdate(openCellPos + tnl::Vector2i{ 0,1 });
			}
			if (!dCreater->isDiscoverFieldCell(openCellPos + tnl::Vector2i{ 0,-1 })) {
				minimapUpdate(openCellPos + tnl::Vector2i{ 0,-1 });
			}
			if (!dCreater->isDiscoverFieldCell(openCellPos + tnl::Vector2i{ 1,0 })) {
				minimapUpdate(openCellPos + tnl::Vector2i{ 1,0 });
			}
			if (!dCreater->isDiscoverFieldCell(openCellPos + tnl::Vector2i{ -1,0 })) {
				minimapUpdate(openCellPos + tnl::Vector2i{ -1,0 });
			}
			break;
		}
	}

	void DungeonScene::soundVolumeFix() {
		auto rManager = ResourceManager::getResourceManager();
		rManager->changeVolumeSoundRes("sound/BGM/DungeonSceneBGM.ogg", 80);
		rManager->changeVolumeSoundRes("sound/SE/DungeonSceneCloseMenu.ogg", 200);
		rManager->changeVolumeSoundRes("sound/SE/DungeonSceneOpenMenu.ogg", 180);
	}

	// �^�C�}�[�����X�|�[���J�E���g�𒴂�����A�G����̃X�|�[������
	// �X�|�[����Ɋ��ɃG�l�~�[�������ꍇ�A���̃^�[���ɃX�|�[������
	void DungeonScene::enemyResporn() {
		++respornTurnTimer_;

		// �X�|�[�������𖞂����Ă��Ȃ��Ȃ瑁�����^�[��
		if (!(respornTurnTimer_ > RESPORN_TURN_COUNT)) { return; }

		// �X�|�[���ʒu�����߂� ( �v���C���[�Ƃ͈Ⴄ�G���A�̈ʒu���Ԃ��Ă��� ) 
		auto spawnPos = DungeonCreater::getDungeonCreater()->randomChoiceEnemyRespawnPos(player_->getPlayer2Dpos());

		// �X�|�[���ʒu�Ɋ��ɓG�������瑁�����^�[������
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (const auto& enemy : enemyList) {
			auto& enemyPos = enemy->get2Dpos();
			// �X�|�[����Ɋ��ɓG�����邩�`�F�b�N�B�����烊�^�[��
			if (enemyPos.x == spawnPos.x && enemyPos.y == spawnPos.y) { return; }
		}

		respornTurnTimer_ = 0;
		EnemyManager::getEnemyManager()->setCurrentFactory(std::make_shared<SlimeFactory>());
		EnemyManager::getEnemyManager()->generateEnemy(spawnPos);
	}

	// ���ړ��̃G�l�~�[�̃A�b�v�f�[�g����
	void DungeonScene::enemyMove(float deltaTime) {
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) {
			// ���Ɉړ��I����Ă���G�l�~�[�͑������^�[��
			if (enemy->getIsAlreadyMove()) { continue; }
			enemy->process(deltaTime);
		}
	}

	// ���s���̃G�l�~�[�̃A�b�v�f�[�g����
	void DungeonScene::enemyAction(float deltaTime) {
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) {
			// ���Ɉړ��I����Ă���G�l�~�[�͑������^�[��
			if (enemy->getIsAlreadyAction()) { continue; }
			enemy->process(deltaTime);
		}
	}

	void DungeonScene::pickUpItem() {
		auto& player2Dpos = player_->getPlayer2Dpos();
		for (auto it = items_.begin(); it != items_.end();) {
			auto& item2Dpos = (*it)->get2Dpos();
			// �A�C�e���Əd�Ȃ��Ă��Ȃ��ꍇ�A�����@continue
			if (player2Dpos.x != item2Dpos.x || player2Dpos.y != item2Dpos.y) {
				++it;
				continue;
			}

			auto item = (*it)->getItem();
			// �C���x���g���ɃA�C�e���f�[�^���i�[������
			if (player_->getPlayerData()->getInventory()->pushbackItemToInventory(item)) {
				// �e�L�X�g���O�ɏE�����A�C�e�������o��
				auto& itemName = item->getItemName();
				std::string textLog = "�@" + itemName + "���E����";
				TextLogManager::getTextLogManager()->addTextLog(textLog);

				// �A�C�e�����폜
				it = items_.erase(it);
				// �P�}�X�ɃA�C�e��������鎖�͂Ȃ��̂ŁA��E������break
				break;
			}
			else
			{
				TextLogManager::getTextLogManager()->addTextLog("�ו��������ς��ŏE���Ȃ�");
				break;
			}
		}
	}

	void DungeonScene::turnHealHP() {
		// HP ������
		player_->getPlayerData()->changeCurrentHP(EVERY_TURN_HEAL);
	}

	//---------------------
	// �V�[�P���X
	//---------------------

	// �����ݒ� ( DungeonScene�̃C���X�^���X������A��x�����ʂ� ) 
	bool DungeonScene::seqInit(float deltaTime) {
		// �t�F�[�h�͐^��������X�^�[�g
		FadeInOutManager::getFadeInOutManager()->setFadeAlphaValue(255);

		// �v���C���[�̐����Ə�����
		player_->initialize(shared_from_this());

		// ���݂̊K�w�� ������
		currentFloor_ = 0;
		// �T�E���h����
		soundVolumeFix();
		// BGM�Đ�
		ResourceManager::getResourceManager()->playSoundRes("sound/BGM/DungeonSceneBGM.ogg", DX_PLAYTYPE_LOOP);

		/*���\�p*/
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<HealRune>(), *shared_from_this());
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<StoneRune>(), *shared_from_this());
		MagicRuneSystemManager::getMagicRuneSystemManager()->equipRune(std::make_shared<FireRune>(), *shared_from_this());
		/*���\�p*/

		// �{�V�[�P���X�ɑJ��
		seq_.change(&DungeonScene::seqToNextFloor);
		return true;
	}

	// ���^�[������������
	bool DungeonScene::seqTurnInit(float deltaTime) {
		// �v���C���[�̍s���t���O���I�t
		player_->offFlagIsAlreadyTurn();
		
		// �G�l�~�[�̍s���t���O���I�t
		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (auto& enemy : enemyList) { enemy->offFlagIsAlreadyTurn(); }

		// �^�[���J�n��������
		seq_.change(&DungeonScene::seqTurnStart);
		return true;
	}

	// �^�[���J�n������
	bool DungeonScene::seqTurnStart(float deltaTime) {
		// HP ������
		turnHealHP();

		// �G�̃��X�|�[������
		enemyResporn();

		// �~�j�}�b�v�X�V�p
		minimapUpdate(player_->getPlayer2Dpos());

		// �L�[���͑҂���
		seq_.change(&DungeonScene::seqKeyInput);

		return true;
	}

	// �L�[���͑҂�
	bool DungeonScene::seqKeyInput(float deltaTime) {
		// �t�F�[�h�C�� �E �A�E�g���ł���Α���s�\
		if (FadeInOutManager::getFadeInOutManager()->isFading()) return true;

		// WASD�ňړ�
		if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_A, eKeys::KB_S, eKeys::KB_D)) {
			// �ړ��\�����肵�Ă���A�V�[�P���X�J��
			if (player_->startMove()) {
				// �ړ������ꍇ�A�K�i�ɏ���Ă���t���O���I�t�ɂ���
				isPlayerOnStairs_ = false;
				seq_.change(&DungeonScene::seqPlayerMoveTurn);
			}
			return true;
		}
		// ���N���b�N�ōU��
		else if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
			player_->startAttack();
			seq_.change(&DungeonScene::seqPlayerActionTurn);
			return true;
		}
		// �E�N���b�N�Ń��j���[�E�B���h�E
		else if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			openMenu();
			seq_.change(&DungeonScene::seqMenuWindow);
		}

		return true;
	}

	bool DungeonScene::seqPlayerMoveTurn(float deltaTime) {
		SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {

			player_->playerUpdate(deltaTime);

			enemyMove(deltaTime);

			// �S�G�l�~�[�ړ������t���O
			bool allEnemyMoved = true;
			auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
			for (const auto& enemy : enemyList) {
				// �����ړ����������Ă��Ȃ���΁A�S�G�l�~�[�ړ������t���O���I�t��
				if (!enemy->getIsAlreadyMove()) { allEnemyMoved = false; }
			}

			// �v���C���[�ƑS�ẴG�l�~�[�̈ړ����������Ă�����A�J��
			if (player_->getIsAlreadyTurn() && allEnemyMoved) { SEQ_CO_BREAK; }
			});

		SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {
			// ���s���G�l�~�[�̍s��
			enemyAction(deltaTime);

			// �S�ẴG�l�~�[�̍s�����������Ă��邩
			bool allEnemyAction = true;
			auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
			for (const auto& enemy : enemyList) {
				// �����ړ����������Ă��Ȃ���΁A�S�G�l�~�[�ړ������t���O���I�t��
				if (!enemy->getIsAlreadyAction()) { allEnemyAction = false; }
			}

			// �S�ẴG�l�~�[�̍s�����������Ă�����A�J��
			if (allEnemyAction) {
				// �^�[���G���h������
				seq_.change(&DungeonScene::seqTurnEnd);
			}
			});

		SEQ_CO_END;
	}

	// �v���C���[���U����I�������^�[��
	bool DungeonScene::seqPlayerActionTurn(float deltaTime) {
		SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {
			// �v���C���[�̃^�[������ ( �U�� )
			if (!player_->getIsAlreadyTurn()) {
				player_->playerUpdate(deltaTime);
			}

			// ���s���G�l�~�[�̍s��
			enemyAction(deltaTime);

			// �S�ẴG�l�~�[�̍s�����������Ă��邩
			bool allEnemyAction = true;
			auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
			for (const auto& enemy : enemyList) {
				// �����ړ����������Ă��Ȃ���΁A�S�G�l�~�[�ړ������t���O���I�t��
				if (!enemy->getIsAlreadyAction()) { allEnemyAction = false; }
			}

			// �S�ẴG�l�~�[�̍s�����������Ă�����R���[�`���I��
			if (allEnemyAction) {
				SEQ_CO_BREAK;
			}
			});

		// �G�l�~�[�̈ړ�
		SEQ_CO_YIELD_RETURN_FRAME(-1, deltaTime, [&] {
			// ���ړ��̃G�l�~�[�̈ړ�����
			enemyMove(deltaTime);

			// �S�ẴG�l�~�[�̈ړ����������Ă��邩
			bool allEnemyMoved = true;
			auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
			for (const auto& enemy : enemyList) {
				// �����ړ����������Ă��Ȃ���΁A�S�G�l�~�[�ړ������t���O���I�t��
				if (!enemy->getIsAlreadyMove()) { allEnemyMoved = false; }
			}

			// �v���C���[�ƑS�ẴG�l�~�[�̈ړ����������Ă�����A�R���[�`���I��
			if (player_->getIsAlreadyTurn() && allEnemyMoved) {
				SEQ_CO_BREAK;
			}
			});

		// �^�[���G���h������
		seq_.change(&DungeonScene::seqTurnEnd);

		SEQ_CO_END;
	}

	// �^�[���G���h����
	bool DungeonScene::seqTurnEnd(float deltaTime) {

		// �v���C���[��HP���[���ɂȂ��Ă���ꍇ�A���ڃQ�[���I�[�o�[�V�[�P���X�ɑJ��
		if (player_->getPlayerData()->isZeroHP()) {
			seq_.change(&DungeonScene::seqGameOver);
			return true;;
		}

		// �󕠓x�����炵�A0�ɂȂ�����A�Q�[���I�[�o�[�ɑJ��
		currentSatiety_ -= SATIETY_SUB_VALUE;
		if (currentSatiety_ <= 0) {
			currentSatiety_ = 0;
			seq_.change(&DungeonScene::seqGameOver);
			return true;;
		}

		// �K�i�ɏ�������A�K�i�ɏ���Ă���t���O�������Ă��Ȃ��ꍇ�A�K�i�V�[�P���X�ɑJ��
		auto& player2Dpos = player_->getPlayer2Dpos();
		auto& stairs2Dpos = stairs_->get2Dpos();
		if (stairs2Dpos.x == player2Dpos.x && stairs2Dpos.y == player2Dpos.y && !isPlayerOnStairs_) {
			seq_.change(&DungeonScene::seqOnStairs);
			return true;
		}

		// �����ɃA�C�e������������E��
		pickUpItem();

		// �^�[���̏�����������
		seq_.change(&DungeonScene::seqTurnInit);
		return true;
	}

	bool DungeonScene::seqGameOver(float deltaTime) {
		// �ŏ��̃t���[���Ńt�F�[�h�A�E�g���s���A�t�F�[�h�A�E�g������������Q�[���I�[�o�[�V�[���ɑJ��
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			ResourceManager::getResourceManager()->stopSoundRes("sound/BGM/DungeonSceneBGM.ogg");
			SceneManager::getSceneManager()->changeScene(std::make_shared<GameOverScene>());
		}
		return true;
	}

	// ���񂾓G��z�񂩂�폜
	//void DungeonScene::deadEnemyErase() {
	//	for (auto it = enemies_.begin(); it != enemies_.end();) {
	//		// HP ���[���̓G�𔭌�
	//		if ((*it)->getEnemyData()->isZeroHP()) {
	//			// ���S���Ă��邩�ǂ���
	//			if ((*it)->getIsDead()) {
	//				// �G�l�~�[�����S���鎞�A�v���C���[�̓G�l�~�[�̎����Ă����o���l�𓾂�
	//				auto getEXP = (*it)->getEnemyData()->getEnemyExp();
	//				player_->getPlayerData()->changeCurrentEXP(getEXP);
	//				// �G�ւ̃|�C���^�[��z�񂩂�폜
	//				it = enemies_.erase(it);
	//				continue;
	//			}
	//		}
	//		++it;
	//	}
	//}

	bool DungeonScene::seqMenuWindow(float deltaTime) {
		// ���j���[�E�B���h�E���L���łȂ��ꍇ�A�������^�[��
		if (!player_->getIsMenuOpen()) { return false; }

		// �E�N���b�N�� menuWindow�����
		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			closeMenu();
			seq_.change(&DungeonScene::seqKeyInput);
		}

		selectedMenu_ = menuWindow_->process(deltaTime);

		// MenuWindow�N���X����A����I�𒆂��̃C���f�b�N�X��enum�^�ɕϊ�����ĕԂ��Ă���̂ŁA����ɉ����ď���
		switch (selectedMenu_) {
		case MenuWindow::e_SelectedMenuWindow::NONE: break; // �����I������Ȃ������t���[���͉������Ȃ�
		case MenuWindow::e_SelectedMenuWindow::Item1: // �u���C�N�X���[
		case MenuWindow::e_SelectedMenuWindow::Item2: // �u���C�N�X���[
		case MenuWindow::e_SelectedMenuWindow::Item3: // �u���C�N�X���[
		case MenuWindow::e_SelectedMenuWindow::Item4: // �u���C�N�X���[
		case MenuWindow::e_SelectedMenuWindow::Item5: // �u���C�N�X���[
		case MenuWindow::e_SelectedMenuWindow::Item6:
			// nullptr�`�F�b�N�����Ă���A�{���Ɏg�����̊m�F�V�[�P���X�ɑJ��
			if (player_->getPlayerData()->getInventory()->getItem(static_cast<int>(selectedMenu_)) != nullptr) { seq_.change(&DungeonScene::seqReallyUseItem); } break;
		case MenuWindow::e_SelectedMenuWindow::EraseMagicRune: { // �������̃��[�����m�F����
			// �������̃��[���������
			if (!MagicRuneSystemManager::getMagicRuneSystemManager()->getEquipmentMagicRunes().empty()) {
				// �V�[�P���X�J��
				seq_.change(&DungeonScene::seqEraseMagicRuneWindow);
			}
			// �Ȃ����
			else {
				// TODO : �����L�q
			}
			break;
		}
		case MenuWindow::e_SelectedMenuWindow::CloseMenu: {// ���j���[�����
			closeMenu();
			seq_.change(&DungeonScene::seqKeyInput);
			break;
		}
		case MenuWindow::e_SelectedMenuWindow::ReturnToTitle: {// �^�C�g���ɖ߂�
			seq_.change(&DungeonScene::seqReallyReturnToTitle);
			break;
		}
		}

		return true;
	}

	bool DungeonScene::seqEraseMagicRuneWindow(float deltaTime) {
		if (seq_.isStart()) {
			// �E�B���h�E���I�[�v����Ԃ�
			magicRuneWindow_.switchOpenMagicRuneWindow();
		}

		// A,D�őI�����؂�ւ�
		magicRuneWindow_.process();

		// ���N���b�N �ŁA�I�񂾃��[�����폜����
		if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
			auto selectRune = magicRuneWindow_.getCurrentSelectRune_();
			MagicRuneSystemManager::getMagicRuneSystemManager()->removeRune(selectRune, *shared_from_this());
			magicRuneWindow_.resetIndex();
			magicRuneWindow_.switchOpenMagicRuneWindow();
			seq_.change(&DungeonScene::seqMenuWindow);
		}
		// �E�N���b�N �ŁA�L�����Z�����ă��j���[�ɖ߂�
		else if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_RIGHT)) {
			magicRuneWindow_.switchOpenMagicRuneWindow();
			seq_.change(&DungeonScene::seqMenuWindow);
		}

		return false;
	}

	// �{���ɃA�C�e�����g�����m�F
	bool DungeonScene::seqReallyUseItem(float deltaTime) {
		auto& item = player_->getPlayerData()->getInventory()->getItem(static_cast<int>(selectedMenu_));
		if (seq_.isStart()) {
			openSelectWindow(item->getItemName() + "\n���g���܂����H");
		}
		if (isSelectWindow_) {
			if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::YES) { // �͂��A�̎��̏���
				// �A�C�e���g�p���ă��j���[�E�B���h�E�����B�G�l�~�[���s������
				player_->getPlayerData()->getInventory()->useItem(static_cast<int32_t>(selectedMenu_));
				closeMenu();
				closeSelectWindow();
				player_->onFlagIsAlreadyTurn();
				seq_.change(&DungeonScene::seqPlayerActionTurn);
			}
			else if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::NO) {	// �������A�̎��̏���
				closeSelectWindow();
				seq_.change(&DungeonScene::seqMenuWindow);
			}
		}
		return true;
	}

	// �{���Ƀ^�C�g���ɖ߂邩�ǂ����m�F��
	bool DungeonScene::seqReallyReturnToTitle(float deltaTime) {
		if (seq_.isStart()) {
			openSelectWindow("�^�C�g����ʂɖ߂�܂����H\n�i���f�[�^�͎c��܂���");
		}

		if (isSelectWindow_) {
			if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::YES) { // �͂��A�̎��̏���
				seq_.change(&DungeonScene::seqReturnToTitle);
			}
			else if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::NO) {	// �������A�̎��̏���
				closeSelectWindow();
				seq_.change(&DungeonScene::seqMenuWindow);
			}
		}

		return true;
	}

	// �^�C�g���ɖ߂鏈��
	bool DungeonScene::seqReturnToTitle(float deltaTime) {
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			// 1�b �ҋ@
			SEQ_CO_YIELD_RETURN_TIME(1, deltaTime, [&] {});

			ResourceManager::getResourceManager()->stopSoundRes("sound/BGM/DungeonSceneBGM.ogg");
			SceneManager::getSceneManager()->changeScene(std::make_shared<TitleScene>());
		}

		SEQ_CO_END
	}

	// �K�i�ɏ�������̏���
	bool DungeonScene::seqOnStairs(float deltaTime) {
		// �E�B���h�E���܂������ꍇ�͐�������
		if (seq_.isStart()) {
			isPlayerOnStairs_ = true;
			openSelectWindow("���̊K�w�ɐi�݂܂����H");
		}

		if (isSelectWindow_) {
			// �͂��A�̎��̏���
			if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::YES) {
				closeSelectWindow();
				seq_.change(&DungeonScene::seqToNextFloor);

			}
			// �������A�̎�
			else if (selectWindow_.windowChoice() == SelectWindow::e_SelectChoice::NO) {
				closeSelectWindow();
				seq_.change(&DungeonScene::seqTurnInit);
			}
		}

		return true;
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

		// �t�F�[�h���͏������Ȃ�
		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			// ���K�w�ɑJ�ڒ��t���O�𗧂Ă�
			isNextFloorTransition = true;

			// �ҋ@ 
			SEQ_CO_YIELD_RETURN_TIME(nextFloorTransitionTime, deltaTime, [&] {});

			// �ő�K�w�ɓ��B������A�N���A�V�[���ɑJ��
			if (currentFloor_ >= MAX_FLOOR) {
				ResourceManager::getResourceManager()->stopSoundRes("sound/BGM/DungeonSceneBGM.ogg");
				SceneManager::getSceneManager()->changeScene(std::make_shared<GameClearScene>());
			}
			else {
				// �_���W�����𐶐����A�t�F�[�h�C������ʂ�\��������
				generateDungeon();
				FadeInOutManager::getFadeInOutManager()->startFadeIn();
				seq_.change(&DungeonScene::seqTurnInit);
			}

			// ���K�w�֑J�ڒ��t���O���I�t��
			isNextFloorTransition = false;
		}

		SEQ_CO_END
	}

	//---------------------
	// �_���W��������
	//---------------------

	void DungeonScene::initDungeon() {
		// �n�`������
		walls_.clear();
		groundTiles_.clear();

		// �A�C�e��������
		items_.clear();
	}

	void DungeonScene::generateDungeon() {
		initDungeon();

		DungeonCreater::getDungeonCreater()->createDungeon();
		auto& fieldData = DungeonCreater::getDungeonCreater()->getFieldCells();

		for (int x = 0; x < fieldData.size(); ++x) {
			for (int y = 0; y < fieldData[x].size(); ++y) {
				// �����Ȃ��ꏊ�Ȃ瑁�����^�[��
				if (fieldData[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_NONE) { continue; }

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
			auto& playerSpawnPos = DungeonCreater::getDungeonCreater()->getPlayerSpawnPos();
			player_->playerSpawn2Dpos(playerSpawnPos);

			// �K�i
			auto& stairsSpawnPos = DungeonCreater::getDungeonCreater()->getStairsSpawnPos();
			stairs_ = std::make_shared<Stairs>(stairsSpawnPos);

			// �G�l�~�[
			auto& enemySpawnPos = DungeonCreater::getDungeonCreater()->getEnemySpawnPos();
			EnemyManager::getEnemyManager()->setCurrentFactory(std::make_shared<SlimeFactory>());
			for (int i = 0; i < DungeonCreater::getDungeonCreater()->getEnemySpawnNum(); ++i) {
				EnemyManager::getEnemyManager()->generateEnemy(enemySpawnPos[i]);
			}

			// �A�C�e��
			auto& itemSpawnPos = DungeonCreater::getDungeonCreater()->getItemSpawnPos();
			for (int i = 0; i < DungeonCreater::getDungeonCreater()->getItemSpawnNum(); ++i) {
				auto item = std::make_shared<ItemPawn>(itemSpawnPos[i]);
				item->initialize();
				items_.emplace_back(item);
			}
		}

		// �e�N�X�`���ɂ�郁�������[�N�΍�
		dxe::Texture::DestroyUnReferenceTextures();
	}

	void DungeonScene::generateWall(int generatePosX, int generatePosZ) {
		Shared<Wall> newWall = std::make_shared<Wall>(originWall_->getMesh());
		newWall->getMesh()->pos_.x = static_cast<float>(generatePosX * CELL_FULL_LENGTH);
		newWall->getMesh()->pos_.y = static_cast<float>(CELL_FULL_LENGTH);
		newWall->getMesh()->pos_.z = static_cast<float>(generatePosZ * CELL_FULL_LENGTH);
		walls_.emplace_back(newWall);
	}

	void DungeonScene::generateGround(int generatePosX, int generatePosZ) {
		Shared<GroundTile> newGround = std::make_shared<GroundTile>(originGroundTile_->getMesh());
		Shared<GroundTile> newGroundTile = std::make_shared<GroundTile>(originGroundTile_->getMesh());
		newGroundTile->getMesh()->pos_.x = static_cast<float>(generatePosX * CELL_FULL_LENGTH);
		newGroundTile->getMesh()->pos_.z = static_cast<float>(generatePosZ * CELL_FULL_LENGTH);
		groundTiles_.emplace_back(newGroundTile);
	}

	//------------------------
	// �f�o�b�O�p
	//------------------------

	void DungeonScene::debug_displayDungeonParam(float deltaTime) {
		DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, deltaTime);

		// �K�i�̈ʒu
		if (stairs_) {
			auto& stairsPos = stairs_->get2Dpos();
			DrawStringEx(600, 25, -1, "stairsPos ... [ %d, %d ]", stairsPos.x, stairsPos.y);
		}

		player_->debug_displayPlayerParam(600, 100);
		player_->getPlayerData()->debug_playerDataParam(600, 300);


	}

	void DungeonScene::debug_displayMap(float deltaTime) {
		auto dCreater = DungeonCreater::getDungeonCreater();
		auto& field = dCreater->getFieldCells();

		DrawBox(0, 0, 450, 450, GetColor(0, 0, 0), true);

		// �t�B�[���h���̕`��
		for (int x = 0; x < field.size(); ++x) {
			for (int y = 0; y < field[x].size(); ++y) {
				if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_ROOM) {
					DrawStringEx((y * 15), (x * 15), -1, " ");
				}
				else if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_PATH) {
					DrawStringEx((y * 15), (x * 15), -1, "-");
				}
				else if (field[x][y].cellType_ == DungeonCreater::e_FieldCellType::CELL_TYPE_WALL) {
					DrawStringEx((y * 15), (x * 15), -1, "*");
				}
			}
		}

		auto& enemyList = EnemyManager::getEnemyManager()->getEnemyList();
		for (const auto& enemy : enemyList) {
			auto& enemyPos = enemy->get2Dpos();
			DrawStringEx(enemyPos.y * 15, enemyPos.x * 15, GetColor(255, 0, 0), "e");
		}

		for (const auto& item : items_) {
			auto& itemPos = item->get2Dpos();
			DrawStringEx(itemPos.y * 15, itemPos.x * 15, GetColor(0, 255, 0), "I");
		}

		auto& player2Dpos = player_->getPlayer2Dpos();
		DrawStringEx(player2Dpos.y * 15, player2Dpos.x * 15, GetColor(0, 0, 255), "P");

		if (stairs_) {
			auto& stairs2Dpos = stairs_->get2Dpos();
			DrawStringEx(stairs2Dpos.y * 15, stairs2Dpos.x * 15, GetColor(200, 200, 200), "S");
		}
	}

}