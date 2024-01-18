#include "AtlRandom.h"

namespace atl {

    std::mt19937 mtRandom;

    void mtRandomInit() {
        std::random_device rd;
        mtRandom.seed(rd());
    }

    void mtRandomSetSeed(int64_t seed) {
        mtRandom.seed(seed);
    }

    int mtRandomRangeInt(int32_t min, int32_t max) {

        if (max <= min) return min;

        int ret = std::uniform_int_distribution<int>(min, max)(mtRandom);
        return ret;
    }

}