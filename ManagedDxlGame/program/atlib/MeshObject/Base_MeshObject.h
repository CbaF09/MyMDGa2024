#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	// メッシュオブジェクト用クラス
	// 3D空間に実際に描画される物体は、これを継承する
	class Base_MeshObject {
	public:
		Base_MeshObject() {};
		// 引数をクローンしてmesh_にセットするコンストラクタ。派生クラスで委譲先コンストラクタにする
		explicit Base_MeshObject(const Shared<dxe::Mesh> originMesh);
		virtual ~Base_MeshObject() {};

		// ゲッター
		inline const Shared<dxe::Mesh> getMesh() const { return mesh_; }
		inline const tnl::Vector3& getMeshSize() const { return meshSize_; }
		inline const tnl::Vector3& getMeshPos() const { return mesh_->pos_; }
		inline const tnl::Vector2i& get2Dpos() const { return mesh2Dpos_; }

		// セッター
		inline void setMesh(const Shared<dxe::Mesh> mesh) { mesh_ = mesh; }
		inline void setMeshPos(const tnl::Vector3& initPos) { mesh_->pos_ = initPos; }
		inline void setMeshSizeVector3(const tnl::Vector3& initSize) { meshSize_ = initSize; }
		inline void set2Dpos(const tnl::Vector2i& mesh2Dpos) { mesh2Dpos_ = mesh2Dpos; }
		void setTexture(const Shared<dxe::Texture> texture);
		void setMaterial(const std::string& filePath);

		// 2Dpos操作メソッド
		inline void add2Dpos(tnl::Vector2i addVec) { mesh2Dpos_ += addVec; }

		// メッシュを描画する
		virtual void renderObject(const Shared<Atl3DCamera> camera, float deltaTime = 0);

	private:
		Shared<dxe::Mesh> mesh_;
		tnl::Vector3 meshSize_{0,0,0};
		tnl::Vector2i mesh2Dpos_{ 0,0 };
	};

}