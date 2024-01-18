#pragma once
#include <memory>
#include <functional>

namespace atl {

	// �x���������p �����֐�
	// argu ... ���s�������������֐� ( ������w��ł��܂��� )
	// tips ... ������ɏ������֐������s�����̂ŁA�R���X�g���N�^�ŏo���Ȃ��������o���܂� ( shared_from_this ���g���֐��Ȃ� )
	template <typename T>
	std::shared_ptr<T> make_shared_withInitFunc(const std::function <void()>& initFunction) {
		std::shared_ptr<T> constructClass = std::make_shared<T>;
		initFunction;
		return constructClass;
	};

}