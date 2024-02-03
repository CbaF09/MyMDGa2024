#pragma once
#include <string>
#include <sstream>

namespace atl {

    std::string convertFullWidthNumber(int number) {
        // �S�p����
        const std::string fullWidthNumbers[] = { "�O", "�P", "�Q", "�R", "�S", "�T", "�U", "�V", "�W", "�X" };
        std::stringstream ss;

        // ���l�𕶎���ɕϊ�
        std::string numberStr = std::to_string(number);
        for (char digit : numberStr) {
            // ASCII��'0'����������ƂŁA�Ή�����C���f�b�N�X���擾���A�S�p�����ɕϊ�
            ss << fullWidthNumbers[digit - '0'];
        }

        return ss.str();
    }

}