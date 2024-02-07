#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	// �Q�[���I�[�o�[�������ɂ���Ă���V�[��
	// �^�����w�i�ɕ����񂪏o�Ă��邾��
	// �G���^�[�L�[���X�y�[�X�L�[�������ƃ^�C�g���ɖ߂�

	class GameOverScene final : public Base_Scene {

	public:

	private:

		const int GAMEOVER_FONT = CreateFontToHandle(NULL, 25, -1);

		const int FONT_SIZE = 25;		// �t�H���g�T�C�Y
		const tnl::Vector2i STRING_POSITION{ 200,200 };	// ������̈ʒu

		void sceneUpdate(float deltaTime);

		SEQUENCE(GameOverScene, &GameOverScene::seqInit);
		bool seqInit(float deltaTime);
		bool seqDraw(float deltaTime);

	};

}