#pragma once
#include <memory>
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	/// <summary>
	/// 
	/// 複数のメッシュから構成されるオブジェクト用クラス
	/// 3D空間に描画される物体は、これを継承する
	/// ( メッシュが一つの場合、Base_MeshObject を継承してください )
	/// 
	/// dxe::Meshクラスを今回のゲーム用に拡張したかったので、ラッパーして作成
	/// 
	/// 親メッシュと子メッシュがあり、親メッシュに対して子メッシュの位置を render 時に補正する事で
	/// 複数のメッシュで作られたものを一つの物体のように見せる
	/// 
	/// </summary>

	class Base_MultiMeshObject  {
	public:
		virtual ~Base_MultiMeshObject() {};

		// ゲッター
		inline const Shared<dxe::Mesh> getRootMesh() const { return rootMesh_; }
		inline const std::vector<Shared<dxe::Mesh>>& getChildMeshes() const { return childMeshes_; }
		inline const tnl::Vector2i& get2Dpos() const { return mesh2Dpos_; }

		// セッター
		inline void setRootMesh(const Shared<dxe::Mesh> rootMesh) { rootMesh_ = rootMesh; }
		inline void set2Dpos(const tnl::Vector2i& mesh2Dpos) { mesh2Dpos_ = mesh2Dpos; }

		// 引数を子メッシュ配列の末尾に追加する
		void addChildMesh(const Shared<dxe::Mesh> childMesh);

		// renderObjects内で、render前に実行される。子メッシュ群の位置を補正。継承先で定義
		virtual void adjustChildsMeshes(float deltaTime) = 0;

		// メッシュ群を描画する
		virtual void renderObjects(const Shared<Atl3DCamera> camera,float deltaTime);

		// 透明な物体がある場合、render する順番を呼び出し側で調整する為に使う
		// adjustChildMeshes のタイミングによっては、
		virtual void renderTransparentObject(const Shared<Atl3DCamera> camera, float deltaTime) {};

		// 2Dpos操作メソッド
		inline void add2Dpos(tnl::Vector2i addVec) { mesh2Dpos_ += addVec; };
	
	private:
		// 親メッシュ
		Shared<dxe::Mesh> rootMesh_;
		// 子メッシュ
		std::vector<Shared<dxe::Mesh>> childMeshes_;
		// メッシュ群の2Dpos
		tnl::Vector2i mesh2Dpos_{ 0,0 };
	};

}