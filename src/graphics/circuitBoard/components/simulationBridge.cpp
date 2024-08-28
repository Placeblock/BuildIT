//
// Created by felix on 8/27/24.
//

#include "simulationBridge.h"


void SimulationBridge::addNode(const std::shared_ptr<Node> &node) {
    this->simulation->addNode(node->simNode);
    this->checkNode(node.get());
    this->nodes->addNode(node);
}

void SimulationBridge::moveNode(Node *node, glm::vec2 newPos, bool updateBuffer) {
    this->checkNode(node, true);
    this->nodes->moveNode(node, newPos, updateBuffer);
    this->checkNode(node);
}

void SimulationBridge::removeNode(Node *node) {
    this->simulation->removeNode(node->simNode);
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
        Joint* joint = this->wires->getJoint(node->cell + glm::vec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectChild(joint);
            } else {
                this->connectChild(joint, {node, node->getInputPinIndex(joint->cell)});
            }
        }
    }
    for (const auto &iPin: node->outputPins) {
        Joint* joint = this->wires->getJoint(node->cell + glm::vec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectParent(joint);
            } else {
                this->connectParent(joint, {node, node->getOutputPinIndex(joint->cell)});
            }
        }
    }
}

void SimulationBridge::checkJoint(Joint *joint, bool disconnect) {
    if (this->nodes->inputPins.contains(joint->cell)) {
        Node* node = this->nodes->inputPins[joint->cell];
        if (disconnect) {
            this->disconnectChild(joint);
        } else {
            this->connectChild(joint, {node, node->getInputPinIndex(joint->cell)});
        }
    }
    if (this->nodes->outputPins.contains(joint->cell)) {
        Node* node = this->nodes->outputPins[joint->cell];
        if (disconnect) {
            this->disconnectParent(joint);
        } else {
            this->connectParent(joint, {node, node->getOutputPinIndex(joint->cell)});
        }
    }
}

SimulationBridge::SimulationBridge(Sim::Simulation *sim, Nodes *nodes, Wires *wires)
    : simulation(sim), nodes(nodes), wires(wires) {

}

void SimulationBridge::connectParent(Joint *joint, Pin parentPin) {
    for (const auto &childRef: joint->network->childReferences) {
        Network::connect(this->simulation, parentPin, childRef);
    }
    joint->network->parentReference = parentPin;
    joint->pin = parentPin;
}

void SimulationBridge::disconnectParent(Joint *joint) {
    for (const auto &childRef: joint->network->childReferences) {
        Network::disconnect(this->simulation, joint->network->parentReference, childRef);
    }
    joint->pin = {};
    joint->network->parentReference = {};
}

void SimulationBridge::connectChild(Joint *joint, Pin childPin) {
    if (joint->network->parentReference.node != nullptr) {
        Network::connect(this->simulation, joint->network->parentReference, childPin);
    }
    joint->network->childReferences.insert(childPin);
}

void SimulationBridge::disconnectChild(Joint *joint) {
    const auto childRefs = &joint->network->childReferences;
    const auto iter = std::find_if(childRefs->begin(), childRefs->end(), [&joint](Pin ref){
        return ref == joint->pin;
    });
    if (iter != childRefs->end()) {
        if (joint->network->parentReference.node != nullptr) {
            Network::disconnect(this->simulation, joint->network->parentReference, joint->pin);
        }
        childRefs->erase(iter);
    } else {
        assert("Did not find pin in network which was stored in joint as pointer.");
    }
}
