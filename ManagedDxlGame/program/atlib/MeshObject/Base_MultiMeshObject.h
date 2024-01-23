#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"

namespace atl {

	// メッシュオブジェクト用クラス
	// 3D空間に実際に描画される物体で、複数のメッシュで構成されているものはこれを継承する
	class Base_MultiMeshObject  {
	public:
		virtual ~Base_MultiMeshObject() {};

		// ゲッター
		inline const Shared<dxe::Mesh> getRootMesh() const { return rootMesh_; }
		inline const std::vector<Shared<dxe::Mesh>>& getChildMeshes() const { return childMeshes_; }
		inline const tnl::Vector2i& get2Dpos() const { return mesh2Dpos_; }

		// セッター
		// 親メッシュを設定する
		inline void setRootMesh(const Shared<dxe::Mesh> rootMesh) { rootMesh_ = rootMesh; }
		// 引数を子メッシュ配列の末尾に追加する
		void addChildMesh(const Shared<dxe::Mesh> childMesh);
		inline void set2Dpos(const tnl::Vector2i& mesh2Dpos) { mesh2Dpos_ = mesh2Dpos; }
		// 2D座標{x,y}を指定すると、その座標にあたる3D空間上の位置に設定される。Y はそのまま
		void set2DposAnd3Dpos(const tnl::Vector2i& mesh2Dpos);

		// 子メッシュ群の座標と回転を設定。renderObjects内で、render前に実行される
		virtual void adjustmentChildMeshes() = 0;
		// メッシュ群を描画する
		void renderObjects(const Shared<Atl3DCamera> camera);

		// 2Dpos操作メソッド
		inline void add2Dpos(tnl::Vector2i addVec) { mesh2Dpos_ += addVec; };
	
	private:
		Shared<dxe::Mesh> rootMesh_;
		std::vector<Shared<dxe::Mesh>> childMeshes_;
		tnl::Vector2i mesh2Dpos_{ 0,0 };
	};

}