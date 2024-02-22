#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "Enemy.h"

namespace atl {

	/// <summary>
	/// 
	/// �S�ẴG�l�~�[�t�@�N�g���[�̊��ƂȂ�N���X
	/// �p����ŁA�e�G�l�~�[��p�T�u�t�@�N�g���[�ɋ�ۉ������
	/// �T�u�t�@�N�g���[�́A�e�G�l�~�[�� 1 �� 1 �̊֌W
	/// ���̃����X�^�[��p�̃X�|�[�������� ( ���Ƃ��H ) �Ȃǂ́A�T�u�t�@�N�g���[�N���X�ɋL�q
	/// 
	class Base_EnemyFactory {
	public:
		// ���N���X�Ȃ̂Ńf�X�g���N�^�� virtual
		virtual ~Base_EnemyFactory() {};

		// �G�l�~�[�𐶐����鏃�����z�֐��B��ۃN���X ( �e�t�@�N�g���[�T�u�N���X ) �ɂĎ��������
		virtual Shared<Base_Enemy> createEnemy() = 0;

	};

	
	/// <summary>
	/// 
	/// �X���C���t�@�N�g���[
	/// 
	/// </summary>
	class BlueSlimeFactory : public Base_EnemyFactory {
		Shared<Base_Enemy> createEnemy() override;
	};

	/// <summary>
	/// 
	/// �΃X���C���t�@�N�g���[
	/// 
	/// </summary>
	class GreenSlimeFactory : public Base_EnemyFactory {
		Shared<Base_Enemy> createEnemy() override;
	};

	/// <summary>
	/// 
	/// �ԃX���C���t�@�N�g���[
	/// 
	/// </summary>
	class RedSlimeFactory : public Base_EnemyFactory {
		Shared<Base_Enemy> createEnemy() override;
	};

}