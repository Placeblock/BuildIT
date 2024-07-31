//
// Created by felix on 16.07.24.
//

#include "gate.h"

using namespace Sim;

void NotGate::update() {
    this->output = ~this->input & this->inputMask;
}

void AndGate::update() {
    this->output = (this->input & this->inputMask) == this->inputMask ? this->outputMask : 0;
}
