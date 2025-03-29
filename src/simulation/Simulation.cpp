//
// Created by felix on 29.03.25.
//

#include "Simulation.h"

using namespace Sim;

void Simulation::pollAndUpdate() {
    const std::shared_ptr<Node> node = this->updateQueue.front();
    this->updateQueue.pop();
    node->update();
}
