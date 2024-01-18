#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "../atlib/Utilities/Atl3DCamera.h"

namespace atl {

	// ���b�V���I�u�W�F�N�g�p�N���X
	// 3D��ԂɎ��ۂɕ`�悳��镨�̂ŁA�����̃��b�V���ō\������Ă�����̂͂�����p������
	class Base_MultiMeshObject {
	public:
		virtual ~Base_MultiMeshObject() {};

		// �Q�b�^�[
		inline const Shared<dxe::Mesh> getRootMesh() const { return rootMesh_; }
		inline const std::vector<Shared<dxe::Mesh>>& getChildMeshes() const { return childMeshes_; }

		// �Z�b�^�[
		// �e���b�V����ݒ肷��
		inline void setRootMesh(const Shared<dxe::Mesh> rootMesh) { rootMesh_ = rootMesh; }
		// �������q���b�V���z��̖����ɒǉ�����
		void addChildMesh(const Shared<dxe::Mesh> childMesh);
		// �q���b�V���z����ۂ��ƃR�s�[
		inline void setChildMesh(const std::vector<Shared<dxe::Mesh>>& childMeshes) { childMeshes_ = childMeshes; }

		// �q���b�V���Q�̍��W�Ɖ�]��ݒ�BrenderObjects���ŁArender�O�Ɏ��s�����
		virtual void adjustmentChildMeshes() = 0;
		// ���b�V���Q��`�悷��
		void renderObjects(const Shared<Atl3DCamera> camera);

	private:
		Shared<dxe::Mesh> rootMesh_;
		std::vector<Shared<dxe::Mesh>> childMeshes_;
	};

}