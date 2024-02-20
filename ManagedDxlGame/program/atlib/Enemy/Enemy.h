#pragma once
#include "../MeshObject/Base_MeshObject.h"

namespace atl {

	/// <summary>
	/// 
	/// �S�ẴG�l�~�[�̊��N���X
	/// �p���N���X�ɂ́A1 �� 1 �̊֌W�ɂ���t�@�N�g���[�N���X�����݂���
	/// 
	class Base_Enemy : public Base_MeshObject {
	public:
		virtual ~Base_Enemy() {};

		// ������
		virtual void initialize() = 0;

		// ���t���[���̏���
		virtual void process(float deltaTime) = 0;
		
	};

	/// <summary>
	/// 
	/// �X���C��
	/// 
	class SlimeEnemy final : public Base_Enemy {
	public:
		void initialize() override;
		void process(float deltaTime) override;
	};
}