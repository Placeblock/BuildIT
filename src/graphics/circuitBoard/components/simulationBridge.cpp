//
// Created by felix on 8/27/24.
//

#include "simulationBridge.h"

void SimulationBridge::moveNode(Node *node, glm::vec2 newPos, bool updateBuffer) {
    this->nodes.moveNode(node, newPos, updateBuffer);
}

void SimulationBridge::addNode(const std::shared_ptr<Node> &node) {
    this->nodes.addNode(node);
}

void SimulationBridge::removeNode(Node *node) {
    this->nodes.removeNode(node);
}

void SimulationBridge::addJoint(const std::shared_ptr<Joint> &joint) {
    this->wires.addJoint(joint);
}

void SimulationBridge::removeJoint(Joint *joint) {
    this->wires.removeJoint(joint);
}

void SimulationBridge::moveJoint(Joint *joint, glm::vec2 newPos) {
    this->wires.moveJoint(joint, newPos);
}

void SimulationBridge::checkJoint(Joint *joint, bool disconnect) {
    if (this->nodes.inputPins.contains(joint->cell)) {
        Node* node = this->nodes.inputPins[joint->cell];
        joint->network->onChildConnect(this->simulation, joint, node, node->getInputPinIndex(joint->cell));
    }
    if (this->nodes.outputPins.contains(joint->cell)) {
        Node* node = this->nodes,outputPins[joint->cell];
        joint->network->onParentConnect(this->simulation, joint, node, node->getOutputPinIndex(joint->cell));
    }
}
