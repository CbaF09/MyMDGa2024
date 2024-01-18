#pragma once
#include <random>
#include <cassert>

namespace atl {
 
    extern std::mt19937 mtRandom;

    // mtRandomを初期化する関数
    void mtRandomInit();

    // 手動で固定シードを入力する関数
    void mtRandomSetSeed(int64_t seed);
    
    // 範囲指定乱数生成。max <= min の場合、アサートエラー
    // ret ... min から max の範囲でランダムな値（min と max を含める）を返す
    //         max <= min の場合、min を返す
    int mtRandomRangeInt(int32_t min, int32_t max);
}