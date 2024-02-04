#include "AtlString.h"

namespace atl {

    std::string convertFullWidthNumber(int number) {
        // 全角数字
        const std::string fullWidthNumbers[] = { "０", "１", "２", "３", "４", "５", "６", "７", "８", "９" };
        std::stringstream ss;

        // 数値を文字列に変換
        std::string numberStr = std::to_string(number);
        for (char digit : numberStr) {
            // ASCIIの'0'から引くことで、対応するインデックスを取得し、全角数字に変換
            ss << fullWidthNumbers[digit - '0'];
        }

        return ss.str();
    }

}