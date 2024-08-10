//
// Created by felix on 16.07.24.
//

#ifndef BUILDIT_SIM_GATE_H
#define BUILDIT_SIM_GATE_H

#include "node.h"

namespace Sim {
    class NotGate : public Node {
    public:
        std::string getType() override {return "not";};
        NotGate() : Node(1, 1) {
            this->inputNames.emplace_back("I");
            this->outputNames.emplace_back("O");
        }
        void update() override;
    };

    class AndGate : public Node {
    public:
        std::string getType() override {return "and";};
        AndGate() : Node(2, 1) {
            this->inputNames.emplace_back("I1");
            this->inputNames.emplace_back("I2");
            this->outputNames.emplace_back("O");
        }
        void update() override;
    };
}


#endif //BUILDIT_SIM_GATE_H