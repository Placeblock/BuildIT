//
// Created by felix on 29.03.25.
//

#include "Node.h"

using namespace Sim;

Node::Node(bool pure, unsigned int outputs) : pure(pure), output(std::make_unique<bool[]>(outputs)) {
}

bool Node::getInput(unsigned int index) {
    return this->input[index];
}

bool Node::getOutput(unsigned int index) {
    return this->output[index];
}

void Node::setInput(unsigned int index, bool value) {
    this->input[index] = value;
}
