#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"

namespace atl {

	// ���b�V���I�u�W�F�N�g�p�N���X
	// 3D��ԂɎ��ۂɕ`�悳��镨�̂ŁA�����̃��b�V���ō\������Ă�����̂͂�����p������
	class Base_MultiMeshObject  {
	public:
		virtual ~Base_MultiMeshObject() {};

		// �Q�b�^�[
		inline const Shared<dxe::Mesh> getRootMesh() const { return rootMesh_; }
		inline const std::vector<Shared<dxe::Mesh>>& getChildMeshes() const { return childMeshes_; }
		inline const tnl::Vector2i& get2Dpos() const { return mesh2Dpos_; }

		// �Z�b�^�[
		// �e���b�V����ݒ肷��
		inline void setRootMesh(const Shared<dxe::Mesh> rootMesh) { rootMesh_ = rootMesh; }
		// �������q���b�V���z��̖����ɒǉ�����
		void addChildMesh(const Shared<dxe::Mesh> childMesh);
		inline void set2Dpos(const tnl::Vector2i& mesh2Dpos) { mesh2Dpos_ = mesh2Dpos; }
		// 2D���W{x,y}���w�肷��ƁA���̍��W�ɂ�����3D��ԏ�̈ʒu�ɐݒ肳���BY �͂��̂܂�
		void set2DposAnd3Dpos(const tnl::Vector2i& mesh2Dpos);

		// �q���b�V���Q�̍��W�Ɖ�]��ݒ�BrenderObjects���ŁArender�O�Ɏ��s�����
		virtual void adjustmentChildMeshes() = 0;
		// ���b�V���Q��`�悷��
		void renderObjects(const Shared<Atl3DCamera> camera);

		// 2Dpos���상�\�b�h
		inline void add2Dpos(tnl::Vector2i addVec) { mesh2Dpos_ += addVec; };
	
	private:
		Shared<dxe::Mesh> rootMesh_;
		std::vector<Shared<dxe::Mesh>> childMeshes_;
		tnl::Vector2i mesh2Dpos_{ 0,0 };
	};

}