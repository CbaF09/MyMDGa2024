#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	// ���b�V���I�u�W�F�N�g�p�N���X
	// 3D��ԂɎ��ۂɕ`�悳��镨�̂́A������p������
	class Base_MeshObject {
	public:
		Base_MeshObject() {};
		// �������N���[������mesh_�ɃZ�b�g����R���X�g���N�^�B�h���N���X�ňϏ���R���X�g���N�^�ɂ���
		explicit Base_MeshObject(const Shared<dxe::Mesh> originMesh);
		virtual ~Base_MeshObject() {};

		// �Q�b�^�[
		inline const Shared<dxe::Mesh> getMesh() const { return mesh_; }
		inline const tnl::Vector3& getMeshSize() const { return meshSize_; }
		inline const tnl::Vector3& getMeshPos() const { return mesh_->pos_; }
		inline const tnl::Vector2i& get2Dpos() const { return mesh2Dpos_; }

		// �Z�b�^�[
		inline void setMesh(const Shared<dxe::Mesh> mesh) { mesh_ = mesh; }
		inline void setMeshPos(const tnl::Vector3& initPos) { mesh_->pos_ = initPos; }
		inline void setMeshSizeVector3(const tnl::Vector3& initSize) { meshSize_ = initSize; }
		inline void set2Dpos(const tnl::Vector2i& mesh2Dpos) { mesh2Dpos_ = mesh2Dpos; }
		void setTexture(const Shared<dxe::Texture> texture);
		void setMaterial(const std::string& filePath);

		// 2Dpos���상�\�b�h
		inline void add2Dpos(tnl::Vector2i addVec) { mesh2Dpos_ += addVec; }

		// ���b�V����`�悷��
		virtual void renderObject(const Shared<Atl3DCamera> camera, float deltaTime = 0);

	private:
		Shared<dxe::Mesh> mesh_;
		tnl::Vector3 meshSize_{0,0,0};
		tnl::Vector2i mesh2Dpos_{ 0,0 };
	};

}