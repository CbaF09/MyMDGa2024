#pragma once
#include <random>
#include <cassert>

namespace atl {
 
    extern std::mt19937 mtRandom;

    // mtRandom������������֐�
    void mtRandomInit();

    // �蓮�ŌŒ�V�[�h����͂���֐�
    void mtRandomSetSeed(int64_t seed);
    
    // �͈͎w�藐�������Bmax <= min �̏ꍇ�A�A�T�[�g�G���[
    // ret ... min ���� max �͈̔͂Ń����_���Ȓl�imin �� max ���܂߂�j��Ԃ�
    //         max <= min �̏ꍇ�Amin ��Ԃ�
    int mtRandomRangeInt(int32_t min, int32_t max);
}