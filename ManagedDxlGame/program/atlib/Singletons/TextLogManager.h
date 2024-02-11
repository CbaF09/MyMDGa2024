#pragma once
#include <string>
#include "../../dxlib_ext/dxlib_ext.h"

namespace atl {

	class TextLogManager {
	//------------------------------------------
	// �V���O���g���݌v
	public:
		static TextLogManager* getTextLogManager();
		inline static void deleteTextLogManager() { delete getTextLogManager(); }
	private:
		TextLogManager() {};
		~TextLogManager() {};
	//------------------------------------------

	public:
		// �Q�b�^�[
		inline const std::list<std::string>& getTextLog() const { return textLogList_; }

		// �e�L�X�g���O�̐擪�ւ̒ǉ�, MAX_LOG_COUNT�𒴉߂��Ă���ꍇ�A�������폜����
		// arg ... �ǉ�����e�L�X�g�B���p�͓��͂��Ȃ��ł�������
		void addTextLog(const std::string& addText);


		// �S�e�L�X�g���O�̍폜
		inline void clearTextLog() { textLogList_.clear(); }

		// �e�L�X�g���O��`�悷��B
		// arg ... �`��ʒu�̍��㒸�_���W
		void displayTextLog(int x, int y,float deltaTime);

	private:
		std::list<std::string> textLogList_;

		// ---------------------------- -
		// ������`��A�j���[�V�����p 
		int32_t drawCharCount_ = 0;
		int32_t drawLogLine_ = 0;
		float totalDeltaTime_ = 0.0f;	// �ݐώ���
		const float CHAR_INTERVAL = 0.05f; // �\�������𑝂₷�Ԋu ( �b )

		const int32_t MAX_LOG_COUNT = 7;
	};

}