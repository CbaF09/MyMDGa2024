#include "AtlRandom.h"

namespace atl {

    std::mt19937 mtRandom;

    void mtRandomInit() {
        std::random_device rd;
        mtRandom.seed(rd());
    }

    void mtRandomSetSeed(uint32_t seed) {
        mtRandom.seed(seed);
    }

    int mtRandomRangeInt(int32_t min, int32_t max) {
        // max が min 以下の場合、minを返す
        if (max <= min) return min;

        int ret = std::uniform_int_distribution<int>(min, max)(mtRandom);
        return ret;
    }


    float mtRandomRangeFloat(float min, float max) {
        // max が min 以下の場合、minを返す
        if (max <= min) return min;

        float ret = std::uniform_real_distribution<float>(min, max)(mtRandom);
        return ret;
    }

}