#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"

namespace atl {

	// メッシュオブジェクト用クラス
	// 3D空間に実際に描画される物体で、複数のメッシュで構成されているものはこれを継承する
	class Base_MultiMeshObject {
	public:
		virtual ~Base_MultiMeshObject() {};

		// ゲッター
		inline const Shared<dxe::Mesh> getRootMesh() const { return rootMesh_; }
		inline const std::vector<Shared<dxe::Mesh>>& getChildMeshes() const { return childMeshes_; }

		// セッター
		// 親メッシュを設定する
		inline void setRootMesh(const Shared<dxe::Mesh> rootMesh) { rootMesh_ = rootMesh; }
		// 引数を子メッシュ配列の末尾に追加する
		void addChildMesh(const Shared<dxe::Mesh> childMesh);
		// 子メッシュ配列を丸ごとコピー
		inline void setChildMesh(const std::vector<Shared<dxe::Mesh>>& childMeshes) { childMeshes_ = childMeshes; }

		// 子メッシュ群の座標と回転を設定。renderObjects内で、render前に実行される
		virtual void adjustmentChildMeshes() = 0;
		// メッシュ群を描画する
		void renderObjects(const Shared<Atl3DCamera> camera);

	private:
		Shared<dxe::Mesh> rootMesh_;
		std::vector<Shared<dxe::Mesh>> childMeshes_;
	};

}