//
// Created by felix on 8/27/24.
//

#include "simulationBridge.h"


void SimulationBridge::addNode(const std::shared_ptr<Node> &node) {
    node->addToSimulation(this->simulation);
    this->checkNode(node.get());
    this->nodes->addNode(node);
}

void SimulationBridge::moveNode(Node *node, glm::vec2 newPos, bool updateBuffer) {
    this->checkNode(node, true);
    this->nodes->moveNode(node, newPos, updateBuffer);
    this->checkNode(node);
}

void SimulationBridge::removeNode(Node *node) {
    node->removeFromSimulation(this->simulation);
    this->checkNode(node, true);
    this->nodes->removeNode(node);
}

void SimulationBridge::addJoint(const std::shared_ptr<Joint> &joint) {
    this->wires->addJoint(joint);
    this->checkJoint(joint.get());
}

void SimulationBridge::removeJoint(Joint *joint) {
    this->checkJoint(joint, true);
    this->wires->removeJoint(joint);
}

void SimulationBridge::moveJoint(Joint *joint, glm::vec2 newPos) {
    this->checkJoint(joint, true);
    this->wires->moveJoint(joint, newPos);
    this->checkJoint(joint);
}

void SimulationBridge::checkNode(Node *node, bool disconnect) {
    for (const auto &iPin: node->inputPins) {
        Joint* joint = this->wires->getJoint(node->pos + glm::vec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectChild(joint);
            } else {
                this->connectChild(joint, {node, node->getInputPinIndex(joint->pos)});
            }
        }
    }
    for (const auto &iPin: node->outputPins) {
        Joint* joint = this->wires->getJoint(node->pos + glm::vec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectParent(joint);
            } else {
                this->connectParent(joint, {node, node->getOutputPinIndex(joint->pos)});
            }
        }
    }
}

void SimulationBridge::checkJoint(Joint *joint, bool disconnect) {
    if (this->nodes->inputPins.contains(joint->pos)) {
        Node* node = this->nodes->inputPins[joint->pos];
        if (disconnect) {
            this->disconnectChild(joint);
        } else {
            this->connectChild(joint, {node, node->getInputPinIndex(joint->pos)});
        }
    }
    if (this->nodes->outputPins.contains(joint->pos)) {
        Node* node = this->nodes->outputPins[joint->pos];
        if (disconnect) {
            this->disconnectParent(joint);
        } else {
            this->connectParent(joint, {node, node->getOutputPinIndex(joint->pos)});
        }
    }
}

SimulationBridge::SimulationBridge(Sim::Simulation *sim, NodeInteractionManager *nodes, Wires *wires, WiresRenderer* wiresRenderer)
    : simulation(sim), nodes(nodes), wires(wires), wiresRenderer(wiresRenderer) {

}

void SimulationBridge::connectParent(Joint *joint, Pin parentPin) {
    for (const auto &childPin: joint->network->childPins) {
        Network::connect(this->simulation, parentPin, childPin.second);
    }
    joint->network->parentPin = {joint, parentPin};
    joint->pin = parentPin;
    this->wiresRenderer->updateNetwork(this->wires, this, joint->network);
}

void SimulationBridge::disconnectParent(Joint *joint) {
    for (const auto &childPin: joint->network->childPins) {
        Network::disconnect(this->simulation, joint->network->parentPin.second, childPin.second);
    }
    joint->pin = {};
    joint->network->parentPin = {};
    this->wiresRenderer->updateNetwork(this->wires, this, joint->network);
}

void SimulationBridge::connectChild(Joint *joint, Pin childPin) {
    if (joint->network->parentPin.first != nullptr) {
        Network::connect(this->simulation, joint->network->parentPin.second, childPin);
    }
    joint->pin = childPin;
    joint->network->childPins[joint] = childPin;
}

void SimulationBridge::disconnectChild(Joint *joint) {
    joint->network->childPins.erase(joint);
    if (joint->network->parentPin.first != nullptr) {
        Network::disconnect(this->simulation, joint->network->parentPin.second, joint->pin);
    }
    joint->pin = {};
}
