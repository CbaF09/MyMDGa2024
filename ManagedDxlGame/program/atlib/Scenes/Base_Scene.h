#pragma once
#include "IE_SceneUpdater.h"


namespace atl {

	// �e�V�[���N���X�p �e�N���X
	// ���� ... �S�V�[���ɓ���ɒ񋟂���鎖���\�z�����@�\���A�܂Ƃ߂Čp������
	//			���̃N���X���V�[���ł��邩�̖��m���i���̃N���X���p�����Ă���΁A����̓V�[���ł���j
	// NOTE : sceneUpdate �� onEvent ���A�p����ŃI�[�o�[���C�h���������̂ŏ������z�֐��ɂ��Ă���
	//		  ���ʁA���󂱂̃N���X�̓C���^�[�t�F�C�X�ƂȂ��Ă��邪�A����C���^�[�t�F�C�X�łȂ��Ȃ�\��������̂�
	//		  IE_Base_Scene �Ƃ������O�͌l�I�ɂ�����ƌ� ( �C���^�[�t�F�C�X�Ȃ�Base�ƂȂ�͓̂�����O ) �Ȃ̂�
	//		  �ړ����̖����K������O��Ă��邪�A��U�C���^�[�t�F�C�X�̂܂ܕ��u����B
	//		  ... �C���^�[�t�F�C�X�łȂ��Ȃ�����A����NOTE�͏����Ă�������
	class Base_Scene :
		public IE_SceneUpdater {

	public:
		void sceneUpdate(float deltaTime) override = 0;

	private:
		// NONE
	};

};