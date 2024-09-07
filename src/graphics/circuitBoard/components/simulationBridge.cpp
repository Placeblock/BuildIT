//
// Created by felix on 8/27/24.
//

#include "simulationBridge.h"

void SimulationBridge::checkNode(Node *node, glm::vec2 nodePos, bool disconnect) {
    for (const auto &iPin: node->inputPins) {
        Joint* joint = this->cabling->getJoint(nodePos + glm::vec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectChild(joint);
            } else {
                this->connectChild(joint, {node, node->getInputPinIndex(joint->getPos())});
            }
        }
    }
    for (const auto &iPin: node->outputPins) {
        Joint* joint = this->cabling->getJoint(nodePos + glm::vec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectParent(joint);
            } else {
                this->connectParent(joint, {node, node->getOutputPinIndex(joint->getPos())});
            }
        }
    }
}

void SimulationBridge::checkJoint(Joint *joint, glm::vec2 jointPos, bool disconnect) {
    if (this->nodes->inputPins.contains(jointPos)) {
        Node* node = this->nodes->inputPins[jointPos];
        if (disconnect) {
            this->disconnectChild(joint);
        } else {
            this->connectChild(joint, {node, node->getInputPinIndex(jointPos)});
        }
    }
    if (this->nodes->outputPins.contains(jointPos)) {
        Node* node = this->nodes->outputPins[jointPos];
        if (disconnect) {
            this->disconnectParent(joint);
        } else {
            this->connectParent(joint, {node, node->getOutputPinIndex(jointPos)});
        }
    }
}

SimulationBridge::SimulationBridge(Sim::Simulation *sim, NodeInteractionManager *nodes, Cabling *wires, WiresRenderer* wiresRenderer)
    : simulation(sim), nodes(nodes), cabling(wires), wiresRenderer(wiresRenderer) {

}

void SimulationBridge::connectParent(Joint *joint, Pin parentPin) {
    for (const auto &childPin: joint->network->childPins) {
        Network::connect(this->simulation, parentPin, childPin.second);
    }
    joint->network->parentPin = {joint, parentPin};
    joint->pin = parentPin;
    this->wiresRenderer->updateNetwork(this->cabling, this, joint->network);
}

void SimulationBridge::disconnectParent(Joint *joint) {
    for (const auto &childPin: joint->network->childPins) {
        Network::disconnect(this->simulation, joint->network->parentPin.second, childPin.second);
    }
    joint->pin = {};
    joint->network->parentPin = {};
    this->wiresRenderer->updateNetwork(this->cabling, this, joint->network);
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

void SimulationBridge::update(const JointAddEvent &data) {
    Joint *joint = data.joint;
    this->checkJoint(joint, joint->getPos());
    joint->subscribe(this->MultiObserver<MoveEvent<Joint>, Joint*>::addSubject(joint));
}

void SimulationBridge::update(const JointRemoveEvent &data) {
    Joint *joint = data.joint;
    this->checkJoint(joint, joint->getPos(), true);
    joint->unsubscribe(this->MultiObserver<MoveEvent<Joint>, Joint*>::removeSubject(joint));
}

void SimulationBridge::update(const NodeAddEvent &data) {
    Node *node = data.node;
    node->addToSimulation(this->simulation);
    this->checkNode(node, node->getPos());
    node->Movable::subscribe(this->MultiObserver<MoveEvent<Node>, Node*>::addSubject(node));
    node->Rotatable::subscribe(this->MultiObserver<RotateEvent<Node>, Node*>::addSubject(node));
}

void SimulationBridge::update(const NodeRemoveEvent &data) {
    Node *node = data.node;
    node->removeFromSimulation(this->simulation);
    this->checkNode(node, node->getPos(), true);
    node->Movable::unsubscribe(this->MultiObserver<MoveEvent<Node>, Node*>::removeSubject(node));
    node->Rotatable::unsubscribe(this->MultiObserver<RotateEvent<Node>, Node*>::removeSubject(node));
}

void SimulationBridge::update(const MoveEvent<Node> &data, Node *node) {
    this->checkNode(node, node->getPos(), data.before);
}

void SimulationBridge::update(const MoveEvent<Joint> &data, Joint *joint) {
    this->checkJoint(joint, joint->getPos(), data.before);
}

void SimulationBridge::update(const RotateEvent<Node> &data, Node *node) {
    this->checkNode(node, node->getPos(), data.before);
}
