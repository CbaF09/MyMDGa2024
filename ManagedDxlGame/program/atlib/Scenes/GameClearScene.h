#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	// �Q�[���N���A���̃V�[��
	// �G�s���[�O�ƃ^�C�g���ɖ߂鏈���̂�
	
	// �萔�n
	namespace {
		// �w�i�̖��邳 ( 0 �Ō��Ɠ����A255 �Ő^���� )
		const int32_t EPILOGUE_BACKGROUND_BRIGHT = 126;	
		// �u�X�y�[�X�L�[�Ń^�C�g���ɖ߂�܂��v��\������ʒu
		const tnl::Vector2i CAN_BACK_TEXT_POSITION{ 10,10 }; 
		// ��ԏ�̍s�̕\���ʒu
		const tnl::Vector2i EPILOGUE_TEXT_POSITION{ 25,150 };
		// ��s���ƂɃI�t�Z�b�g������
		const tnl::Vector2i EPILOGUE_TEXT_OFFSET{ 0,45 };
	}

	class GameClearScene final : public Base_Scene{
	public:
		GameClearScene();
		~GameClearScene();

	private:
		// -------------------------------------
		// �ϐ�

		// �t�H���g
		int EPILOGUE_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

		// �G�s���[�O�̕�����
		std::vector<std::string> epilogueString_{};
		int32_t currentTopLogIndex_ = 0;

		// -------------------------------------
		// ���\�b�h

		// ���t���[���Ă�
		void sceneUpdate(float deltaTime) override;
		
		// �w�i�`��
		void drawBackground(float deltaTime);

		// �V�[�P���X ( �����̕`����s�� )
		SEQUENCE(GameClearScene,&GameClearScene::seqInit);

		// ������ ( �ŏ��Ɉ�x�����Ă΂�� )
		bool seqInit(float deltaTime);

		// �{�V�[�P���X
		bool seqEpilogue(float deltaTime);

		//void invokeDrawLogLineIncrement();

		// �^�C�g���V�[���ɑJ�ڂ���t�F�[�h�C���A�E�g
		bool seqToTitleScene(float deltaTime);

	};

}