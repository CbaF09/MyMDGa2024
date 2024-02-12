#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_Scene.h"

namespace atl {

	// �Q�[���N���A���̃V�[��
	// �G�s���[�O�ƃ^�C�g���ɖ߂鏈���̂�
	
	class GameClearScene final : public Base_Scene{
	public:
		GameClearScene();
		~GameClearScene();

	private:
		// -------------------------------------
		// �ϐ�

		// �v�����[�O�֘A
		const int EPILOGUE_FONT = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

		const int32_t EPILOGUE_BACKGROUND_BRIGHT = 126;	// �w�i�̖��邳 ( 0 �Ō��Ɠ����A255 �Ő^���� )
		const tnl::Vector2i CAN_BACK_TEXT_POSITION{ 10,10 }; // �u�X�y�[�X�L�[�Ń^�C�g���ɖ߂�܂��v��\������ʒu
		const tnl::Vector2i TEXT_POSITION{ 25,150 };	// ��ԏ�̍s�̕\���ʒu
		const tnl::Vector2i TEXT_OFFSET{ 0,45 };		// ��s���ƂɃI�t�Z�b�g������

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