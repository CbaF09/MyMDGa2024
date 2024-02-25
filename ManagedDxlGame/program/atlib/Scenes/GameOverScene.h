#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	// �Q�[���I�[�o�[�������ɂ���Ă���V�[��
	// �^�����w�i�ɕ����񂪏o�Ă��邾��
	// �G���^�[�L�[���X�y�[�X�L�[�������ƃ^�C�g���ɖ߂�

	class GameOverScene final : public Base_Scene {
	public:
		~GameOverScene();

	private:
		// �t�H���g
		int GAMEOVER_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

		const int FONT_SIZE = 25;		// �t�H���g�T�C�Y
		const tnl::Vector2i STRING_POSITION{ 200,200 };	// ������̈ʒu

		void sceneUpdate(float deltaTime);

		SEQUENCE(GameOverScene, &GameOverScene::seqInit);
		bool seqInit(float deltaTime);
		bool seqUpdata(float deltaTime);

	};

}