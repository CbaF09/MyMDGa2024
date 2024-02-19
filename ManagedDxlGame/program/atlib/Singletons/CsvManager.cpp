#include "CsvManager.h"

namespace atl {

    CsvManager* CsvManager::getCsvManager() {
        static CsvManager* p_instance_ = nullptr;

        if (!p_instance_) p_instance_ = new CsvManager();
        return p_instance_;
    }

}