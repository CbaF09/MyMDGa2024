#pragma once

namespace atl {

	/// <summary>
	/// 
	/// Base_Scene 用 ( インターフェイス ) 
	/// sceneUpdate メソッドの提供。SceneManager が sceneUpdate を呼び出します
	/// 
	/// </summary>

	class SceneManager;

	class IE_SceneUpdater {
		friend SceneManager;

	public:
		virtual ~IE_SceneUpdater() = default;

	private:
		// arg ... 前フレームからの経過時間。SceneManager が渡してくるので気にしないでOK
		virtual void sceneUpdate(float deltaTime) = 0;

	};

};