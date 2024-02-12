#include "TextLogManager.h"

namespace atl {


    TextLogManager* atl::TextLogManager::getTextLogManager() {
        static TextLogManager* p_instance_ = nullptr;

        if (!p_instance_) p_instance_ = new TextLogManager();
        return p_instance_;
    }

    void TextLogManager::addTextLog(const std::string& addText) {
        // �`�悷�镶����0��
        drawCharCount_ = 0;
        // �O�̂��̂���`�悳��Ăق����̂ŁA�O��empalce���� ( ������list�ɂ��� )
        textLogList_.emplace_front(addText);

        // ���O�̍ő�\���s�����I�[�o�[���Ă���ꍇ�A�������폜����
        if (textLogList_.size() > MAX_LOG_COUNT) {
            textLogList_.pop_back();
        }
    }

    void TextLogManager::displayTextLog(int x, int y, float deltaTime) {

        // ���Ԍv��
        totalDeltaTime_ += deltaTime;
        // �`�悷�镶�����𑝂₷
        if (totalDeltaTime_ >= CHAR_INTERVAL) {
            drawCharCount_ += 2;
            totalDeltaTime_ = 0.0f;
        }

        // �`�悷��s�����[����
        drawLogLine_ = 0;
        for (const auto& log : textLogList_) {
            // �ŏ��̈�s�̏ꍇ ( ��ԍŌ�� addTextLog ���ꂽ�s�̏ꍇ )
            if (drawLogLine_ == 0) {
                // �ꕶ�����`�悳���A�j���[�V���� + �����̐F��Ԃ�
                std::string drawLog = log.substr(0, drawCharCount_);
                DrawStringEx(x, y, GetColor(255,0,0), "%s", drawLog.c_str());
            }
            else {
                // �ŏ��̈�s�łȂ��ꍇ�A��s���̂܂ܕ`��B�F�͏��������Ԃ�
                DrawStringEx(x, y + drawLogLine_ * 20, GetColor(200,50,50), "%s", log.c_str());
            }
            ++drawLogLine_;
        }
    }
}