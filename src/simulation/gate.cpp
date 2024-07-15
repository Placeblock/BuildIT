//
// Created by felix on 16.07.24.
//

#include "gate.h"

void Simulation::NotGate::update() {
    this->output = ~this->input & this->inputMask;
}