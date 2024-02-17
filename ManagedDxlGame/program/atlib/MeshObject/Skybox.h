#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// ダンジョンシーンでのSkysphere
	/// フォグのオンオフを行っています ( スカイボックスとフォグを併用する関係で )
	/// テクスチャはcpp内で設定
	/// 
	/// </summary>

	class Atl3DCamera;

	class Skysphere final {
	public:
		Skysphere();

		// 位置の補正と render をまとめて行っている
		// フォグのオンオフをしている
		void update(const Shared<Atl3DCamera> camera);

	private:
		Shared<dxe::Mesh> skysphere_;
	};

}