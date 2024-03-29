#pragma once
#include <string>
#include <sstream>

namespace atl {

    // 半角数字を全角数字に変換する
    // arg ... 変換したい半角数字
    std::string convertFullWidthNumber(int number);

}