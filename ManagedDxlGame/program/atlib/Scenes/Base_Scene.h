#pragma once
#include "IE_SceneUpdater.h"


namespace atl {
	
	/// <summary>
	/// 
	/// �e�V�[���N���X�p �e�N���X
	/// IE_SceneUpdater ���p�����Ă���̂ŁASceneManager ���� sceneUpdate ���Ăяo����
	/// ���󂻂ꂾ�������A����S�V�[���ɒǉ��������C���^�[�t�F�[�X�Ȃǂ��o�����ꍇ���l���āA�ꉞ���N���X������Ă���
	/// 
	/// </summary>
	
	class Base_Scene : public IE_SceneUpdater {
	public:
		void sceneUpdate(float deltaTime) override = 0;
	};

};