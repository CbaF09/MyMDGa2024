#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Base_MeshObject.h"

namespace atl {

	/// <summary>
	/// 
	/// 左手に持っている本 ( 3D )
	/// 見た目と、メニューを開いているかどうかの判定
	/// 
	/// </summary>

	class PlayerPawn;

	class MenuBook final : public Base_MeshObject {
	public:
		explicit MenuBook(std::weak_ptr<PlayerPawn> player);
		
		// 状態に応じて姿勢が変わるので、補正してからレンダーするため、オーバーライド
		void renderObject(const Shared<Atl3DCamera> camera, float deltaTime) override;

		// ゲッター ( メニュー開いている状態か )
		inline const bool isOpenMenu() const { return isOpenMenu_; }
		
		// メニュー開いているフラグをオン
		inline void openMenu() { isOpenMenu_ = true; }
		// メニュー開いているフラグをオフ
		inline void closeMenu() { isOpenMenu_ = false; }

	private:
		// 最初の回転状態を保存
		tnl::Quaternion initRot_;

		// プレイヤーポーンへの弱参照 ( プレイヤーに追従させるため )
		std::weak_ptr<PlayerPawn> weakPlayerPawn;

		//　メニューを開いているか
		bool isOpenMenu_ = false;

	};

}