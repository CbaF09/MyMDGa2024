#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	/// <summary>
	/// 
	/// ���b�V���I�u�W�F�N�g�p�N���X
	/// 3D��Ԃɕ`�悳��镨�̂́A������p������
	/// ( �����̃��b�V������\�������ꍇ�ABase_MultiMeshObject ���p�����Ă������� )
	/// 
	/// dxe::Mesh�N���X������̃Q�[���p�Ɋg�������������̂ŁA���b�p�[���č쐬
	/// 
	/// </summary>

	class Base_MeshObject {
	public:
		Base_MeshObject() {};

		// ������createClone����mesh_�ɃZ�b�g����R���X�g���N�^�B�p�������N���X���Ϗ���R���X�g���N�^�ɂ���
		explicit Base_MeshObject(const Shared<dxe::Mesh> originMesh);
	
		virtual ~Base_MeshObject() {};

		// �Q�b�^�[
		inline const Shared<dxe::Mesh> getMesh() const { return mesh_; }
		inline const tnl::Vector2i& get2Dpos() const { return mesh2Dpos_; }

		// �Z�b�^�[
		inline void setMesh(const Shared<dxe::Mesh> mesh) { mesh_ = mesh; }
		inline void set2Dpos(const tnl::Vector2i& mesh2Dpos) { mesh2Dpos_ = mesh2Dpos; }
		void setTexture(const Shared<dxe::Texture> texture);

		// 2Dpos���상�\�b�h
		inline void add2Dpos(tnl::Vector2i addVec) { mesh2Dpos_ += addVec; }

		// ���b�V����`�悷��
		virtual void renderObject(const Shared<Atl3DCamera> camera, float deltaTime = 0);

	private:
		// �ێ����郁�b�V��
		Shared<dxe::Mesh> mesh_ = nullptr;

		// 2D������̍��W
		tnl::Vector2i mesh2Dpos_{ 0,0 };
	};

}