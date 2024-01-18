#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"

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

		// �Z�b�^�[
		inline void setMesh(const Shared<dxe::Mesh> mesh) { mesh_ = mesh; }
		inline void setMeshPos(const tnl::Vector3& initPos) { mesh_->pos_ = initPos; }
		inline void setMeshSizeVector3(const tnl::Vector3& initSize) { meshSize_ = initSize; }
		void setTexture(const Shared<dxe::Texture> texture);
		void setMaterial(const std::string& filePath);

		// ���b�V����`�悷��
		void renderObject(const Shared<Atl3DCamera> camera) const;

	private:
		Shared<dxe::Mesh> mesh_;
		tnl::Vector3 meshSize_{0,0,0};
	};

}