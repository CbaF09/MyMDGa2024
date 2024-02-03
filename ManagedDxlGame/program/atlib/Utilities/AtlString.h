#pragma once
#include <string>
#include <sstream>

namespace atl {

    std::string convertFullWidthNumber(int number) {
        // ‘SŠp”š
        const std::string fullWidthNumbers[] = { "‚O", "‚P", "‚Q", "‚R", "‚S", "‚T", "‚U", "‚V", "‚W", "‚X" };
        std::stringstream ss;

        // ”’l‚ğ•¶š—ñ‚É•ÏŠ·
        std::string numberStr = std::to_string(number);
        for (char digit : numberStr) {
            // ASCII‚Ì'0'‚©‚çˆø‚­‚±‚Æ‚ÅA‘Î‰‚·‚éƒCƒ“ƒfƒbƒNƒX‚ğæ“¾‚µA‘SŠp”š‚É•ÏŠ·
            ss << fullWidthNumbers[digit - '0'];
        }

        return ss.str();
    }

}