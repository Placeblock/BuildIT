//
// Created by felix on 8/27/24.
//

#include "simulationFeature.h"

SimulationFeature::SimulationFeature(Sim::Simulation *sim, NodePinHandler *pinHandler, Cabling *cabling)
        : simulation(sim), pinHandler(pinHandler), cabling(cabling) {
}

void SimulationFeature::checkNode(Node *node, const glm::vec2 nodePos, const bool disconnect) {
    for (const auto &iPin: node->inputPins) {
        Joint* joint = this->cabling->getJoint(intVec2(nodePos / 32.0f) + intVec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectChild(joint);
            } else {
                this->connectChild(joint, {node, node->getInputPinIndex(joint->getPos())});
            }
        }
    }
    for (const auto &iPin: node->outputPins) {
        Joint* joint = this->cabling->getJoint(intVec2(nodePos / 32.0f) + intVec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectParent(joint);
            } else {
                this->connectParent(joint, {node, node->getOutputPinIndex(joint->getPos())});
            }
        }
    }
}

void SimulationFeature::checkJoint(Joint *joint, const glm::vec2 jointPos, const bool disconnect) {
    Node* node = this->pinHandler->getNode(jointPos);
    if (node == nullptr) return;
    if (this->pinHandler->isInputPin(jointPos)) {
        if (disconnect) {
            this->disconnectChild(joint);
        } else {
            this->connectChild(joint, {node, node->getInputPinIndex(jointPos)});
        }
    }
    if (this->pinHandler->isOutputPin(jointPos)) {
        if (disconnect) {
            this->disconnectParent(joint);
        } else {
            this->connectParent(joint, {node, node->getOutputPinIndex(jointPos)});
        }
    }
}

void SimulationFeature::connectParent(Joint *joint, Pin parentPin) const {
    for (const auto &[_, pin]: joint->getNetwork()->childPins) {
        Network::connect(this->simulation, parentPin, pin);
    }
    joint->getNetwork()->parentPin = {joint, parentPin};
    joint->pin = parentPin;
    joint->getNetwork()->update();
    parentPin.node->outputNetworks[parentPin.index] = joint->getNetwork();
}

void SimulationFeature::disconnectParent(Joint *joint) const {
    Pin parentPin = joint->getNetwork()->parentPin.second;
    for (const auto &[_, pin]: joint->getNetwork()->childPins) {
        Network::disconnect(this->simulation, parentPin, pin);
    }
    parentPin.node->outputNetworks[parentPin.index] = nullptr;
    joint->pin = {};
    joint->getNetwork()->parentPin = {};
    joint->getNetwork()->update();
}

void SimulationFeature::connectChild(Joint *joint, const Pin childPin) const {
    if (joint->getNetwork()->parentPin.first != nullptr) {
        Network::connect(this->simulation, joint->getNetwork()->parentPin.second, childPin);
    }
    joint->pin = childPin;
    joint->getNetwork()->childPins[joint] = childPin;
}

void SimulationFeature::disconnectChild(Joint *joint) const {
    joint->getNetwork()->childPins.erase(joint);
    if (joint->getNetwork()->parentPin.first != nullptr) {
        Network::disconnect(this->simulation, joint->getNetwork()->parentPin.second, joint->pin);
    }
    joint->pin = {};
}

void SimulationFeature::notify(const ComponentAddEvent &data) {
    if (auto *joint = dynamic_cast<Joint*>(data.component)) {
        this->checkJoint(joint, joint->getPos());
        joint->Movable::subscribe(this);
    } else if (const auto node = dynamic_cast<Node*>(data.component))  {
        node->addToSimulation(this->simulation);
        this->checkNode(node, node->getPos());
        node->Movable::subscribe(this);
        node->Rotatable::subscribe(this);
    }
}

void SimulationFeature::notify(const ComponentRemoveEvent &data) {
    if (auto *joint = dynamic_cast<Joint*>(data.component)) {
        this->checkJoint(joint, joint->getPos(), true);
        joint->Movable::unsubscribe(this);
    } else if (const auto node = dynamic_cast<Node*>(data.component))  {
        node->removeFromSimulation(this->simulation);
        this->checkNode(node, node->getPos(), true);
        node->Movable::unsubscribe(this);
        node->Rotatable::unsubscribe(this);
    }
}

void SimulationFeature::notify(const MoveEvent &data) {
    if (const auto node = dynamic_cast<Node*>(data.movable)) {
        this->checkNode(node, node->getPos(), data.before);
    } else if (auto *joint = dynamic_cast<Joint*>(data.movable)) {
        this->checkJoint(joint, joint->getPos(), data.before);
    }
}

void SimulationFeature::notify(const RotateEvent &data) {
    if (const auto node = dynamic_cast<Node*>(data.rotatable)) {
        this->checkNode(node, node->getPos(), data.before);
    }
}

void SimulationFeature::notify(const NetworksMergeEvent &data) {
    // If the deleted network has a parent reference we have to connect the child references
    if (data.deleted->parentPin.first != nullptr &&
        data.persisting->parentPin.first == nullptr) {
        for (const auto &[_, childPin]: data.persisting->childPins) {
            Network::connect(this->simulation, data.deleted->parentPin.second, childPin);
        }
    }
    for (auto &[oldChildJoint, oldChildPin] : data.deleted->childPins) {
        // If the new merged network has an input reference we have to connect the output references!
        if (data.persisting->parentPin.first != nullptr &&
            data.deleted->parentPin.first == nullptr) {
            Network::connect(this->simulation, data.persisting->parentPin.second, oldChildPin);
        }
    }
}

void SimulationFeature::notify(const NetworksSplitEvent &data) {
    for (const auto &network: data.splitted) {
        if (data.old->parentPin.first != nullptr &&
            network->parentPin.first == nullptr) {
            for (const auto &[_, childPin]: network->childPins) {
                Network::disconnect(this->simulation, data.old->parentPin.second, childPin);
            }
        }
    }
}
