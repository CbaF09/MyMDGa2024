#pragma once
#include <random>
#include <cassert>

namespace atl {
 
    extern std::mt19937 mtRandom;

    // mtRandomを初期化する関数, ゲーム開始時に一回呼ぶ
    void mtRandomInit();

    // 手動で固定シードを入力する関数, 手動でシード入力したければ、mtRandomInit の代わりにゲーム開始時に一回呼ぶ
    void mtRandomSetSeed(int64_t seed);
    
    // 範囲指定で乱数を得る ( int 型 )
    // ret ... min から max の範囲でランダムな値（min と max を含める）を返す
    //         max <= min の場合、min を返す
    int mtRandomRangeInt(int32_t min, int32_t max);

    // 範囲指定で乱数を得る ( float 型 )
    // ret ... min から max の範囲でランダムな値（min と max を含める）を返す
    //         max <= min の場合、min を返す
    float mtRandomRangeFloat(float min, float max);
}