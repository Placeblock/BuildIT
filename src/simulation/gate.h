//
// Created by felix on 16.07.24.
//

#ifndef BUILDIT_SIM_GATE_H
#define BUILDIT_SIM_GATE_H

#include "node.h"

namespace Sim {
    class NotGate : public Node {
    public:
        NotGate() : Node(1, 1) {
            this->inputNames.emplace_back("I");
            this->outputNames.emplace_back("O");
        }
        void update() override;
    };

    class AndGate : public Node {
    public:
        AndGate() : Node(2, 1) {
            this->inputNames.emplace_back("I1");
            this->inputNames.emplace_back("I2");
            this->outputNames.emplace_back("O");
        }
        void update() override;
    };

    class EndGate : public Node {
    public:
        EndGate(uint8_t inputs, const std::vector<std::string>& inputNames) : Node(inputs, 0) {
            this->inputNames = inputNames;
        }
        void update() override {};
    };
}


#endif //BUILDIT_SIM_GATE_H