#pragma once
#include "IE_SceneUpdater.h"


namespace atl {
	
	/// <summary>
	/// 
	/// 各シーンクラス用 親クラス
	/// IE_SceneUpdater を継承しているので、SceneManager から sceneUpdate を呼び出せる
	/// 現状それだけだが、今後全シーンに追加したいインターフェースなどが出来た場合を考えて、一応基底クラスを作っておく
	/// 
	/// </summary>
	
	class Base_Scene : public IE_SceneUpdater {
	public:
		void sceneUpdate(float deltaTime) override = 0;
	};

};