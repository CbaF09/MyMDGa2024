#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	/// <summary>
	/// 
	/// �_���W�����V�[���ł�Skysphere
	/// �t�H�O�̃I���I�t���s���Ă��܂� ( �X�J�C�{�b�N�X�ƃt�H�O�𕹗p����֌W�� )
	/// �e�N�X�`����cpp���Őݒ�
	/// 
	/// </summary>

	class Atl3DCamera;

	class Skysphere final {
	public:
		Skysphere();

		// �ʒu�̕␳�� render ���܂Ƃ߂čs���Ă���
		// �t�H�O�̃I���I�t�����Ă���
		void update(const Shared<Atl3DCamera> camera);

	private:
		Shared<dxe::Mesh> skysphere_;
	};

}