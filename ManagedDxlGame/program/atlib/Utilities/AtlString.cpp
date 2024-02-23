#include "AtlString.h"

namespace atl {

    std::string convertFullWidthNumber(int number) {
        // ‘SŠp”š‚Ì”z—ñ‚ğì‚é
        const std::string fullWidthNumbers[] = { "‚O", "‚P", "‚Q", "‚R", "‚S", "‚T", "‚U", "‚V", "‚W", "‚X" };

        std::stringstream strstream;

        // ”’l‚ğ•¶š—ñ‚É•ÏŠ·
        std::string numberStr = std::to_string(number);

        for (char digit : numberStr) {
            // ‘Î‰‚·‚éƒCƒ“ƒfƒbƒNƒX‚ğæ“¾‚µ‚Ä‘SŠp”š‚É•ÏŠ·
            strstream << fullWidthNumbers[digit - '0'];
        }

        return strstream.str();
    }

}