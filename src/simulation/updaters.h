//
// Created by felix on 16.07.24.
//

#ifndef BUILDIT_SIM_GATE_H
#define BUILDIT_SIM_GATE_H

#include "node.h"
#include <unordered_map>
#include <cmath>
#include <cstdint>
#include <cassert>

namespace Sim {
    class NotUpdater : public Updater {
        uint32_t update(uint32_t input, uint32_t inputMask, uint32_t outputMask) override;
    };

    class AndUpdater : public Updater {
        uint32_t update(uint32_t input, uint32_t inputMask, uint32_t outputMask) override;
    };

    class TruthTableUpdater : public Updater {
    private:
        std::unordered_map<uint32_t, uint32_t> truthTable;
    public:
        explicit TruthTableUpdater(std::unordered_map<uint32_t, uint32_t>& truthTable);
        uint32_t update(uint32_t input, uint32_t inputMask, uint32_t outputMask) override;
    };
}


#endif //BUILDIT_SIM_GATE_H