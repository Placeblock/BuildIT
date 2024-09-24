//
// Created by felix on 8/4/24.
//

#include "gate.h"

#include <cmath>
#include <utility>

std::vector<uintVec2> Gate::calculateInputPins() {
    std::vector<uintVec2> cells;
    for (size_t i = 1; i <= this->simNode->parents.size(); i++) {
        cells.emplace_back(0, i);
    }
    return cells;
}

std::vector<uintVec2> Gate::calculateOutputPins() {
    std::vector<uintVec2> cells;
    for (size_t i = 1; i <= this->simNode->children.size(); i++) {
        cells.emplace_back(this->cellSize.x, i);
    }
    return cells;
}

Gate::Gate(glm::vec2 pos, std::string text, const std::shared_ptr<Sim::Node>& simNode)
    : Node(pos, Gate::calcSize(simNode)), text(std::move(text)), simNode(simNode) {
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

Gate::Gate(Gate &other) : Node(other), text(other.text), simNode(std::make_shared<Sim::Node>(*other.simNode)) {

}


intVec2 Gate::calcSize(const std::shared_ptr<Sim::Node>& simNode) {
    int inputSize = int(simNode->parents.size())+1;
    int outputSize = int(simNode->children.size())+1;
    return {3, std::max(inputSize, outputSize)};
}

SimNodeData Gate::getInputSimNode(uint8_t inputIndex) {
    return {this->simNode.get(), inputIndex};
}

SimNodeData Gate::getOutputSimNode(uint8_t outputIndex) {
    return {this->simNode.get(), outputIndex};
}

bool Gate::resetUpdated() {
    if (this->simNode->updated) {
        this->simNode->updated = false;
        return true;
    }
    return false;
}

uint32_t Gate::getOutput() {
    return this->simNode->getOutput();
}

uint32_t Gate::getInput() {
    return this->simNode->getInput();
}

bool Gate::getInput(uint8_t index) {
    return this->simNode->getInput(index);
}

bool Gate::getOutput(uint8_t index) {
    return this->simNode->getOutput(index);
}

void Gate::addToSimulation(Sim::Simulation *sim) {
    sim->addNode(this->simNode);
}

void Gate::removeFromSimulation(Sim::Simulation *sim) {
    sim->removeNode(this->simNode);
}