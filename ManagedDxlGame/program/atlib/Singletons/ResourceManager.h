#pragma once
#include <unordered_map>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"

namespace atl {

	// �����Ńt���C�E�F�C�g�p�^�[����K�p���Ă��܂��B
	//  ... ���݂��Ȃ����\�[�X�Ȃ烍�[�h�A���[�h�ς݂̃��\�[�X�Ȃ炻�̃n���h����Ԃ�
	class ResourceManager final {
	//------------------------------------------
	// �V���O���g���݌v
	public:
		static ResourceManager* getResourceManager();
		static void deleteResourceManager();
	private:
		static ResourceManager* p_instance_;
	//------------------------------------------
	public:

		// 2D �C���X�g�p �t���C�E�F�C�g�p�^�[�� ( filepath, graphHandle)
		int getGraphRes(const std::string& filepath);

		// �����f�[�^�p �t���C�E�F�C�g�p�^�[�� ( filepath, playtype,topPositionFlag )
		// filepath ... �Đ��������T�E���h��filepath
		// playtype ... DX_PLAYTYPE_NORMAL	... �Đ����I���܂ŏ������~�߂�
		//			... DX_PLAYTYPE_BACK	... �炵�n�߂�Ƃ������̏���
		//			... DX_PLAYTYPE_LOOP	... �n�܂�Ƃ������̏����B�Đ����I���Ƃ����ŏ�����Đ��B
		// topPositionFlag ... �Đ�����O�ɍĐ��J�n�ʒu��擪�ɖ߂����B�ȗ��\ ... �ȗ������ꍇ�� TRUE
		void playSoundRes(const std::string& filepath,int playType, int topPositionFlag = TRUE);

		// filepath ... ���ʕύX�������T�E���h��filepath
		// volume ... 0 ~ 255 , 255�ōő剹�� ( �������f�[�^�Ɠ������� )
		void changeVolumeSoundRes(const std::string& filepath, int volume);

		// �Đ����̃T�E���h���~����
		void stopSoundRes(const std::string& filepath);

		// �w��p�X�̃��\�[�X���������B���� => false , ���s => false
		bool deleteResource(const std::string& filepath);

	private:
		// 2D �C���X�g�p Map ( filepath, graphHandle)
		std::unordered_map<std::string, int> graphResourceMap_;
		// �����f�[�^�p map ( filepath, soundHandle )
		std::unordered_map<std::string, int> soundResouceMap_;

		// �����f�[�^�p �t���C�E�F�C�g�p�^�[���B���[�h�Ǝ擾���s�������B
		// �T�E���h�͒��ڍĐ��E���ʒ����E��~���o�������������Ǝv���̂ŁA���[�h�Ǝ擾���s�������̊֐��͓����ɉB��
		int getSoundRes(const std::string& filepath);
		
	};

}