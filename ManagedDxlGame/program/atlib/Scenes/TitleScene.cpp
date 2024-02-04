#include "TitleScene.h"
#include "../Singletons/ResourceManager.h"
#include "../Singletons/FadeInOutManager.h"
#include "../Singletons/SceneManager.h"
#include "DungeonScene.h"

namespace atl {

	TitleScene::~TitleScene() {
		{// �^�C�g���V�[���Ŏg���Ă��郊�\�[�X�����
			
			// ������郊�\�[�X�̃t�@�C���p�X�̈ꎞ�I�z����쐬
			std::vector<std::string> tempDeleteRes = {
				"graphics/BackGroundIllust/TitleSceneBack.png"
			};
			
			// �t�@�C���p�X���������B����̐������s���f�o�b�O���O�ɏo��
			for (const auto& res : tempDeleteRes) {
				bool isDelete = ResourceManager::getResourceManager()->deleteResource(res);
				if(!isDelete) { tnl::DebugTrace("\n------------------------------\nTitleScene::�f�X�g���N�^ ��������� => ����ɉ������Ă��܂���"); }
				else { tnl::DebugTrace("\n------------------------------\nTitleScene::�f�X�g���N�^ ��������� => ����"); }
			}
			tnl::DebugTrace("\n------------------------------\n"); // ���O�����Â炢�̂ōŌ�ɉ��s�Ɛ؂����������
		}
	}
	void TitleScene::sceneUpdate(float deltaTime) {
		seq_.update(deltaTime);
		draw(deltaTime);
	}

	void TitleScene::draw(float deltaTime) {
		DrawExtendGraph(0,0,DXE_WINDOW_WIDTH,DXE_WINDOW_HEIGHT,ResourceManager::getResourceManager()->getIllustRes("graphics/BackGroundIllust/TitleSceneBack.png"),true);
		FadeInOutManager::getFadeInOutManager()->drawFadeBlackRect(deltaTime);
	}

	bool TitleScene::seqInit(float deltaTime) {

		auto fadeManager = FadeInOutManager::getFadeInOutManager();
		fadeManager->setFadeAlphaValue(255);
		fadeManager->startFadeIn();
		
		seq_.change(&TitleScene::seqKeyInputWait);

		return false;
	}

	bool TitleScene::seqKeyInputWait(float deltaTime) {
		debug_keyInput();

		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			seq_.change(&TitleScene::seqToDungeonScene);
		}

		return true;
	}

	bool TitleScene::seqToDungeonScene(float deltaTime) {
		// �ŏ��� 1 �t���[���ŁA�t�F�[�h�A�E�g���J�n
		if (seq_.isStart()) {
			FadeInOutManager::getFadeInOutManager()->startFadeOut();
		}

		if (!FadeInOutManager::getFadeInOutManager()->isFading()) {
			SceneManager::getSceneManager()->changeScene(std::make_shared<DungeonScene>());
		}

		return true;
	}

	void TitleScene::debug_keyInput() {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);
	}

}