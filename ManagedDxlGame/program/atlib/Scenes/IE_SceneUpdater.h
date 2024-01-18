#pragma once

namespace atl {

	class SceneManager;

	// Base_Scene �p ( �C���^�[�t�F�C�X ) 
	// ����		... sceneUpdate ���\�b�h�̒񋟁BSceneManager �� sceneUpdate ���Ăяo���܂�
	// �g����	... sceneUpdate ���\�b�h�� override ���āA���t���[�����s����������������
	class IE_SceneUpdater {
		friend SceneManager;

	public:
		virtual ~IE_SceneUpdater() = default;

	private:
		// argu ... �o�ߎ��ԁBSceneManager ���n���Ă���̂ŋC�ɂ��Ȃ���OK
		virtual void sceneUpdate(float deltaTime) = 0;

	};

};