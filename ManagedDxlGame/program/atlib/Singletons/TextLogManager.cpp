#include "TextLogManager.h"

namespace atl {


    TextLogManager* atl::TextLogManager::getTextLogManager() {
        static TextLogManager* p_instance_ = nullptr;

        if (!p_instance_) p_instance_ = new TextLogManager();
        return p_instance_;
    }

    void TextLogManager::addTextLog(const std::string& addText) {
        drawCharCount_ = 0;
        textLogList_.emplace_front(addText);

        if (textLogList_.size() > MAX_LOG_COUNT) {
            textLogList_.pop_back();
        }
    }

    void TextLogManager::displayTextLog(int x, int y, float deltaTime) {

        totalDeltaTime_ += deltaTime;
        if (totalDeltaTime_ >= CHAR_INTERVAL) {
            drawCharCount_ += 2;
            totalDeltaTime_ = 0.0f;
        }

        // 
        drawLogLine_ = 0;
        for (const auto& log : textLogList_) {
            if (drawLogLine_ == 0) {
                std::string drawLog = log.substr(0, drawCharCount_);
                DrawStringEx(x, y, GetColor(255,0,0), "%s", drawLog.c_str());
            }
            else {
                DrawStringEx(x, y + drawLogLine_ * 20, GetColor(200,50,50), "%s", log.c_str());
            }
            ++drawLogLine_;
        }
    }
}