//
// Created by felix on 8/27/24.
//

#include "simulationFeature.h"

SimulationFeature::SimulationFeature(Sim::Simulation *sim, NodePinHandler *pinHandler, Cabling *cabling,
                                     JointContainer *joints, NodeContainer *nodes)
        : simulation(sim), pinHandler(pinHandler), cabling(cabling),
          nodes(nodes), joints(joints) {
    this->nodes->Subject<NodeAddEvent>::subscribe(this);
    this->nodes->Subject<NodeRemoveEvent>::subscribe(this);
    this->joints->Subject<JointAddEvent>::subscribe(this);
    this->joints->Subject<JointRemoveEvent>::subscribe(this);
}

void SimulationFeature::checkNode(Node *node, glm::vec2 nodePos, bool disconnect) {
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

void SimulationFeature::checkJoint(Joint *joint, glm::vec2 jointPos, bool disconnect) {
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

void SimulationFeature::connectParent(Joint *joint, Pin parentPin) {
    for (const auto &childPin: joint->network->childPins) {
        Network::connect(this->simulation, parentPin, childPin.second);
    }
    joint->network->parentPin = {joint, parentPin};
    joint->pin = parentPin;
    joint->network->update();
}

void SimulationFeature::disconnectParent(Joint *joint) {
    for (const auto &childPin: joint->network->childPins) {
        Network::disconnect(this->simulation, joint->network->parentPin.second, childPin.second);
    }
    joint->pin = {};
    joint->network->parentPin = {};
    joint->network->update();
}

void SimulationFeature::connectChild(Joint *joint, Pin childPin) {
    if (joint->network->parentPin.first != nullptr) {
        Network::connect(this->simulation, joint->network->parentPin.second, childPin);
    }
    joint->pin = childPin;
    joint->network->childPins[joint] = childPin;
}

void SimulationFeature::disconnectChild(Joint *joint) {
    joint->network->childPins.erase(joint);
    if (joint->network->parentPin.first != nullptr) {
        Network::disconnect(this->simulation, joint->network->parentPin.second, joint->pin);
    }
    joint->pin = {};
}

void SimulationFeature::update(const JointAddEvent &data) {
    Joint *joint = data.joint;
    this->checkJoint(joint, joint->getPos());
    joint->subscribe(this->MultiObserver<MoveEvent<Joint>, Joint*>::addSubject(joint));
}

void SimulationFeature::update(const JointRemoveEvent &data) {
    Joint *joint = data.joint;
    this->checkJoint(joint, joint->getPos(), true);
    joint->unsubscribe(this->MultiObserver<MoveEvent<Joint>, Joint*>::removeSubject(joint));
}

void SimulationFeature::update(const NodeAddEvent &data) {
    Node *node = data.node;
    node->addToSimulation(this->simulation);
    this->checkNode(node, node->getPos());
    node->Movable::subscribe(this->MultiObserver<MoveEvent<Node>, Node*>::addSubject(node));
    node->Rotatable::subscribe(this->MultiObserver<RotateEvent<Node>, Node*>::addSubject(node));
}

void SimulationFeature::update(const NodeRemoveEvent &data) {
    Node *node = data.node;
    node->removeFromSimulation(this->simulation);
    this->checkNode(node, node->getPos(), true);
    node->Movable::unsubscribe(this->MultiObserver<MoveEvent<Node>, Node*>::removeSubject(node));
    node->Rotatable::unsubscribe(this->MultiObserver<RotateEvent<Node>, Node*>::removeSubject(node));
}

void SimulationFeature::update(const MoveEvent<Node> &data, Node *node) {
    this->checkNode(node, node->getPos(), data.before);
}

void SimulationFeature::update(const MoveEvent<Joint> &data, Joint *joint) {
    this->checkJoint(joint, joint->getPos(), data.before);
}

void SimulationFeature::update(const RotateEvent<Node> &data, Node *node) {
    this->checkNode(node, node->getPos(), data.before);
}

SimulationFeature::~SimulationFeature() {
    for (const auto &node: *this->nodes->getNodes()) {
        node->Movable::unsubscribe(this->MultiObserver<MoveEvent<Node>, Node*>::removeSubject(node.get()));
        node->Rotatable::unsubscribe(this->MultiObserver<RotateEvent<Node>, Node*>::removeSubject(node.get()));
    }
    for (const auto &joint: *this->joints->getJoints()) {
        joint->Movable::unsubscribe(this->MultiObserver<MoveEvent<Joint>, Joint*>::removeSubject(joint.get()));
    }
    this->nodes->Subject<NodeAddEvent>::unsubscribe(this);
    this->nodes->Subject<NodeRemoveEvent>::unsubscribe(this);
    this->joints->Subject<JointAddEvent>::unsubscribe(this);
    this->joints->Subject<JointRemoveEvent>::unsubscribe(this);
}
