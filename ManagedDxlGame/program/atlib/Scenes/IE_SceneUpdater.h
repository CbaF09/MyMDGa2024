#pragma once

namespace atl {

	class SceneManager;

	// Base_Scene 用 ( インターフェイス ) 
	// 役割		... sceneUpdate メソッドの提供。SceneManager が sceneUpdate を呼び出します
	// 使い方	... sceneUpdate メソッドを override して、毎フレーム実行したい処理を実装
	class IE_SceneUpdater {
		friend SceneManager;

	public:
		virtual ~IE_SceneUpdater() = default;

	private:
		// argu ... 経過時間。SceneManager が渡してくるので気にしないでOK
		virtual void sceneUpdate(float deltaTime) = 0;

	};

};