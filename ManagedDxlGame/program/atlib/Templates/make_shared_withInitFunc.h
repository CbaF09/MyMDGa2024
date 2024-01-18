#pragma once
#include <memory>
#include <functional>

namespace atl {

	// 遅延初期化用 生成関数
	// argu ... 実行したい初期化関数 ( 一つしか指定できません )
	// tips ... 生成後に初期化関数が実行されるので、コンストラクタで出来ない処理が出来ます ( shared_from_this を使う関数など )
	template <typename T>
	std::shared_ptr<T> make_shared_withInitFunc(const std::function <void()>& initFunction) {
		std::shared_ptr<T> constructClass = std::make_shared<T>;
		initFunction;
		return constructClass;
	};

}