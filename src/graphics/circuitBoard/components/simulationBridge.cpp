//
// Created by felix on 8/27/24.
//

#include "simulationBridge.h"


void SimulationBridge::addNode(const std::shared_ptr<Node> &node) {
    this->checkNode(node.get());
    this->nodes->addNode(node);
}

void SimulationBridge::moveNode(Node *node, glm::vec2 newPos, bool updateBuffer) {
    this->checkNode(node, true);
    this->nodes->moveNode(node, newPos, updateBuffer);
    this->checkNode(node);
}

void SimulationBridge::removeNode(Node *node) {
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
                std::unique_ptr<Pin> pin = std::make_unique<Pin>(node, node->getInputPinIndex(joint->cell));
                this->connectChild(joint, pin);
            }
        }
    }
    for (const auto &iPin: node->outputPins) {
        Joint* joint = this->wires->getJoint(node->cell + glm::vec2(iPin));
        if (joint != nullptr) {
            if (disconnect) {
                this->disconnectParent(joint);
            } else {
                std::unique_ptr<Pin> pin = std::make_unique<Pin>(node, node->getOutputPinIndex(joint->cell));
                this->connectParent(joint, pin);
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
            std::unique_ptr<Pin> pin = std::make_unique<Pin>(node, node->getInputPinIndex(joint->cell));
            this->connectChild(joint, pin);
        }
    }
    if (this->nodes->outputPins.contains(joint->cell)) {
        Node* node = this->nodes->outputPins[joint->cell];
        if (disconnect) {
            this->disconnectParent(joint);
        } else {
            std::unique_ptr<Pin> pin = std::make_unique<Pin>(node, node->getOutputPinIndex(joint->cell));
            this->connectParent(joint, pin);
        }
    }
}

SimulationBridge::SimulationBridge(Sim::Simulation *sim, Nodes *nodes, Wires *wires)
    : simulation(sim), nodes(nodes), wires(wires) {

}

void SimulationBridge::connectParent(Joint *joint, std::unique_ptr<Pin>& parentPin) {
    for (const auto &childRef: joint->network->childReferences) {
        Sim::Node* childSimNode = childRef->node->simNode.get();
        const auto parent = Sim::Reference(parentPin->node->simNode.get(), childSimNode, parentPin->index);
        const auto child = Sim::Reference(childSimNode, parentPin->node->simNode.get(), childRef->index);
        simulation->connect(parent, child, true);
    }
    joint->pin = parentPin.get();
    joint->network->parentReference = std::move(parentPin);
}

void SimulationBridge::disconnectParent(Joint *joint) {
    for (const auto &childRef: joint->network->childReferences) {
        Sim::Node* childSimNode = childRef->node->simNode.get();
        const auto parent = Sim::Reference(joint->pin->node->simNode.get(), childSimNode, joint->pin->index);
        const auto child = Sim::Reference(childSimNode, joint->pin->node->simNode.get(), childRef->index);
        simulation->disconnect(parent, child);
    }
    joint->pin = nullptr;
    joint->network->parentReference.reset();
}

void SimulationBridge::connectChild(Joint *joint, std::unique_ptr<Pin>& childPin) {
    if (joint->network->parentReference != nullptr) {
        Sim::Node* parentSimNode = joint->network->parentReference->node->simNode.get();
        const auto parent = Sim::Reference(parentSimNode, childPin->node->simNode.get(), joint->network->parentReference->index);
        const auto child = Sim::Reference(childPin->node->simNode.get(), parentSimNode, childPin->index);
        simulation->connect(parent, child, true);
    }
    joint->network->childReferences.insert(std::move(childPin));
}

void SimulationBridge::disconnectChild(Joint *joint) {
    const auto childRefs = &joint->network->childReferences;
    const auto iter = std::find_if(childRefs->begin(), childRefs->end(), [&joint](const std::unique_ptr<Pin>& ref){
        return ref.get() == joint->pin;
    });
    if (joint->network->parentReference != nullptr) {
        Sim::Node* parentSimNode = joint->network->parentReference->node->simNode.get();
        const auto parent = Sim::Reference(parentSimNode, joint->pin->node->simNode.get(), joint->network->parentReference->index);
        const auto child = Sim::Reference(joint->pin->node->simNode.get(), parentSimNode, joint->pin->index);
        simulation->disconnect(parent, child);
    }
    if (iter != childRefs->end()) {
        childRefs->erase(iter);
    } else {
        assert("Did not find pin in network which was stored in joint as pointer.");
    }
}
