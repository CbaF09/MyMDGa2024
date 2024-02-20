#pragma once
#include "../MeshObject/Base_MeshObject.h"

namespace atl {

	/// <summary>
	/// 
	/// 全てのエネミーの基底クラス
	/// 継承クラスには、1 対 1 の関係にあるファクトリークラスが存在する
	/// 
	class Base_Enemy : public Base_MeshObject {
	public:
		virtual ~Base_Enemy() {};

		// 初期化
		virtual void initialize() = 0;

		// 毎フレームの処理
		virtual void process(float deltaTime) = 0;
		
	};

	/// <summary>
	/// 
	/// スライム
	/// 
	class SlimeEnemy final : public Base_Enemy {
	public:
		void initialize() override;
		void process(float deltaTime) override;
	};
}