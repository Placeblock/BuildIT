//
// Created by felix on 16.07.24.
//

#include "gate.h"

using namespace Simulation;

void NotGate::update() {
    this->output = ~this->input & this->inputMask;
}