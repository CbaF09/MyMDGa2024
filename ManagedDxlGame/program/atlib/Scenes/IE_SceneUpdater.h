#pragma once

namespace atl {

	/// <summary>
	/// 
	/// Base_Scene �p ( �C���^�[�t�F�C�X ) 
	/// sceneUpdate ���\�b�h�̒񋟁BSceneManager �� sceneUpdate ���Ăяo���܂�
	/// 
	/// </summary>

	class SceneManager;

	class IE_SceneUpdater {
		friend SceneManager;

	public:
		virtual ~IE_SceneUpdater() = default;

	private:
		// arg ... �O�t���[������̌o�ߎ��ԁBSceneManager ���n���Ă���̂ŋC�ɂ��Ȃ���OK
		virtual void sceneUpdate(float deltaTime) = 0;

	};

};