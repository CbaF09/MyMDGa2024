#include "AtlString.h"

namespace atl {

    std::string convertFullWidthNumber(int number) {
        // �S�p�����̔z������
        const std::string fullWidthNumbers[] = { "�O", "�P", "�Q", "�R", "�S", "�T", "�U", "�V", "�W", "�X" };

        std::stringstream strstream;

        // ���l�𕶎���ɕϊ�
        std::string numberStr = std::to_string(number);

        for (char digit : numberStr) {
            // �Ή�����C���f�b�N�X���擾���đS�p�����ɕϊ�
            strstream << fullWidthNumbers[digit - '0'];
        }

        return strstream.str();
    }

}