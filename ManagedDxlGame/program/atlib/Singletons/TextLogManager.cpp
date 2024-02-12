#include "TextLogManager.h"

namespace atl {


    TextLogManager* atl::TextLogManager::getTextLogManager() {
        static TextLogManager* p_instance_ = nullptr;

        if (!p_instance_) p_instance_ = new TextLogManager();
        return p_instance_;
    }

    void TextLogManager::addTextLog(const std::string& addText) {
        // 描画する文字を0に
        drawCharCount_ = 0;
        // 前のものから描画されてほしいので、前にempalceする ( だからlistにした )
        textLogList_.emplace_front(addText);

        // ログの最大表示行数をオーバーしている場合、末尾を削除する
        if (textLogList_.size() > MAX_LOG_COUNT) {
            textLogList_.pop_back();
        }
    }

    void TextLogManager::displayTextLog(int x, int y, float deltaTime) {

        // 時間計測
        totalDeltaTime_ += deltaTime;
        // 描画する文字数を増やす
        if (totalDeltaTime_ >= CHAR_INTERVAL) {
            drawCharCount_ += 2;
            totalDeltaTime_ = 0.0f;
        }

        // 描画する行数をゼロに
        drawLogLine_ = 0;
        for (const auto& log : textLogList_) {
            // 最初の一行の場合 ( 一番最後に addTextLog された行の場合 )
            if (drawLogLine_ == 0) {
                // 一文字ずつ描画されるアニメーション + 文字の色を赤に
                std::string drawLog = log.substr(0, drawCharCount_);
                DrawStringEx(x, y, GetColor(255,0,0), "%s", drawLog.c_str());
            }
            else {
                // 最初の一行でない場合、一行そのまま描画。色は少し薄い赤に
                DrawStringEx(x, y + drawLogLine_ * 20, GetColor(200,50,50), "%s", log.c_str());
            }
            ++drawLogLine_;
        }
    }
}