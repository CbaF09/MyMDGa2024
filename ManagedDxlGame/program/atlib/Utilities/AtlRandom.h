#pragma once
#include <random>
#include <cassert>

namespace atl {
 
    extern std::mt19937 mtRandom;

    // mtRandom������������֐�, �Q�[���J�n���Ɉ��Ă�
    void mtRandomInit();

    // �蓮�ŌŒ�V�[�h����͂���֐�, �蓮�ŃV�[�h���͂�������΁AmtRandomInit �̑���ɃQ�[���J�n���Ɉ��Ă�
    void mtRandomSetSeed(int64_t seed);
    
    // �͈͎w��ŗ����𓾂� ( int �^ )
    // ret ... min ���� max �͈̔͂Ń����_���Ȓl�imin �� max ���܂߂�j��Ԃ�
    //         max <= min �̏ꍇ�Amin ��Ԃ�
    int mtRandomRangeInt(int32_t min, int32_t max);

    // �͈͎w��ŗ����𓾂� ( float �^ )
    // ret ... min ���� max �͈̔͂Ń����_���Ȓl�imin �� max ���܂߂�j��Ԃ�
    //         max <= min �̏ꍇ�Amin ��Ԃ�
    float mtRandomRangeFloat(float min, float max);
}