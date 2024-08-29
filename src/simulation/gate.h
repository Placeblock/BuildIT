//
// Created by felix on 16.07.24.
//

#ifndef BUILDIT_SIM_GATE_H
#define BUILDIT_SIM_GATE_H

#include "node.h"
#include <unordered_map>
#include <complex>
#include <cassert>

namespace Sim {
    class NotGate : public Node {
    public:
        NotGate() : Node(1, 1) {}
        void update() override;
    };

    class AndGate : public Node {
    public:
        AndGate() : Node(2, 1) {}
        void update() override;
    };

    class EndGate : public Node {
    public:
        explicit EndGate(uint8_t inputs) : Node(inputs, 0) {}
        void update() override {};
    };

    class TruthTableGate : public Node {
    private:
        std::unordered_map<uint32_t, uint32_t> truthTable;
    public:
        TruthTableGate(uint8_t inputs, uint8_t outputs, const std::unordered_map<uint32_t, uint32_t>& truthTable)
               : truthTable(truthTable), Node(inputs, outputs) {
            for (int i = 0; i < std::pow(2, inputs); ++i) {
                if (!truthTable.contains(i)) {
                    assert("Incomplete truth table for gate");
                }
            }
        }
        void update() override;
    };
}


#endif //BUILDIT_SIM_GATE_H