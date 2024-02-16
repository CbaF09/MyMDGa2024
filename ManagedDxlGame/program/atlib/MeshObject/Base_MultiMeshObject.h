#pragma once
#include <memory>
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Utilities/Atl3DCamera.h"

namespace atl {

	/// <summary>
	/// 
	/// �����̃��b�V������\�������I�u�W�F�N�g�p�N���X
	/// 3D��Ԃɕ`�悳��镨�̂́A������p������
	/// ( ���b�V������̏ꍇ�ABase_MeshObject ���p�����Ă������� )
	/// 
	/// dxe::Mesh�N���X������̃Q�[���p�Ɋg�������������̂ŁA���b�p�[���č쐬
	/// 
	/// �e���b�V���Ǝq���b�V��������A�e���b�V���ɑ΂��Ďq���b�V���̈ʒu�� render ���ɕ␳���鎖��
	/// �����̃��b�V���ō��ꂽ���̂���̕��̂̂悤�Ɍ�����
	/// 
	/// </summary>

	class Base_MultiMeshObject  {
	public:
		virtual ~Base_MultiMeshObject() {};

		// �Q�b�^�[
		inline const Shared<dxe::Mesh> getRootMesh() const { return rootMesh_; }
		inline const std::vector<Shared<dxe::Mesh>>& getChildMeshes() const { return childMeshes_; }
		inline const tnl::Vector2i& get2Dpos() const { return mesh2Dpos_; }

		// �Z�b�^�[
		inline void setRootMesh(const Shared<dxe::Mesh> rootMesh) { rootMesh_ = rootMesh; }
		inline void set2Dpos(const tnl::Vector2i& mesh2Dpos) { mesh2Dpos_ = mesh2Dpos; }

		// �������q���b�V���z��̖����ɒǉ�����
		void addChildMesh(const Shared<dxe::Mesh> childMesh);

		// renderObjects���ŁArender�O�Ɏ��s�����B�q���b�V���Q�̈ʒu��␳�B�p����Œ�`
		virtual void adjustChildsMeshes(float deltaTime) = 0;

		// ���b�V���Q��`�悷��
		virtual void renderObjects(const Shared<Atl3DCamera> camera,float deltaTime);

		// �����ȕ��̂�����ꍇ�Arender ���鏇�Ԃ��Ăяo�����Œ�������ׂɎg��
		// adjustChildMeshes �̃^�C�~���O�ɂ���ẮA
		virtual void renderTransparentObject(const Shared<Atl3DCamera> camera, float deltaTime) {};

		// 2Dpos���상�\�b�h
		inline void add2Dpos(tnl::Vector2i addVec) { mesh2Dpos_ += addVec; };
	
	private:
		// �e���b�V��
		Shared<dxe::Mesh> rootMesh_;
		// �q���b�V��
		std::vector<Shared<dxe::Mesh>> childMeshes_;
		// ���b�V���Q��2Dpos
		tnl::Vector2i mesh2Dpos_{ 0,0 };
	};

}