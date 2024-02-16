#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	/// <summary>
	/// 
	/// メッシュオブジェクト用クラス
	/// 3D空間に描画される物体は、これを継承する
	/// ( 複数のメッシュから構成される場合、Base_MultiMeshObject を継承してください )
	/// 
	/// dxe::Meshクラスを今回のゲーム用に拡張したかったので、ラッパーして作成
	/// 
	/// </summary>

	class Base_MeshObject {
	public:
		Base_MeshObject() {};

		// 引数をcreateCloneしてmesh_にセットするコンストラクタ。継承したクラスが委譲先コンストラクタにする
		explicit Base_MeshObject(const Shared<dxe::Mesh> originMesh);
	
		virtual ~Base_MeshObject() {};

		// ゲッター
		inline const Shared<dxe::Mesh> getMesh() const { return mesh_; }
		inline const tnl::Vector2i& get2Dpos() const { return mesh2Dpos_; }

		// セッター
		inline void setMesh(const Shared<dxe::Mesh> mesh) { mesh_ = mesh; }
		inline void set2Dpos(const tnl::Vector2i& mesh2Dpos) { mesh2Dpos_ = mesh2Dpos; }
		void setTexture(const Shared<dxe::Texture> texture);

		// 2Dpos操作メソッド
		inline void add2Dpos(tnl::Vector2i addVec) { mesh2Dpos_ += addVec; }

		// メッシュを描画する
		virtual void renderObject(const Shared<Atl3DCamera> camera, float deltaTime = 0);

	private:
		// 保持するメッシュ
		Shared<dxe::Mesh> mesh_ = nullptr;

		// 2D次元上の座標
		tnl::Vector2i mesh2Dpos_{ 0,0 };
	};

}