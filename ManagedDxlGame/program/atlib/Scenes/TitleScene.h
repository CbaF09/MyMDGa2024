#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	class TitleScene final : public atl::Base_Scene {
	public:
		~TitleScene();

	private:
		void sceneUpdate(float deltaTime) override;

		// �`����܂Ƃ߂�
		void draw(float deltaTime);

		
		// �V�[�P���X�p
		SEQUENCE(TitleScene, &TitleScene::seqInit);
		// �����ݒ�
		bool seqInit(float deltaTime);
		bool seqKeyInputWait(float deltaTime);
		bool seqToDungeonScene(float deltaTime);

		// �f�o�b�O�p
		void debug_keyInput();
	};

}