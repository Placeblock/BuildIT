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
    for (const auto &childPin: joint->getNetwork()->childPins) {
        Network::connect(this->simulation, parentPin, childPin.second);
    }
    joint->getNetwork()->parentPin = {joint, parentPin};
    joint->pin = parentPin;
    joint->getNetwork()->update();
}

void SimulationFeature::disconnectParent(Joint *joint) {
    for (const auto &childPin: joint->getNetwork()->childPins) {
        Network::disconnect(this->simulation, joint->getNetwork()->parentPin.second, childPin.second);
    }
    joint->pin = {};
    joint->getNetwork()->parentPin = {};
    joint->getNetwork()->update();
}

void SimulationFeature::connectChild(Joint *joint, Pin childPin) {
    if (joint->getNetwork()->parentPin.first != nullptr) {
        Network::connect(this->simulation, joint->getNetwork()->parentPin.second, childPin);
    }
    joint->pin = childPin;
    joint->getNetwork()->childPins[joint] = childPin;
}

void SimulationFeature::disconnectChild(Joint *joint) {
    joint->getNetwork()->childPins.erase(joint);
    if (joint->getNetwork()->parentPin.first != nullptr) {
        Network::disconnect(this->simulation, joint->getNetwork()->parentPin.second, joint->pin);
    }
    joint->pin = {};
}

void SimulationFeature::update(Subject<JointAddEvent> *subject, const JointAddEvent &data) {
    Joint *joint = data.joint;
    this->checkJoint(joint, joint->getPos());
    joint->Movable<Joint>::subscribe(this);
}

void SimulationFeature::update(Subject<JointRemoveEvent> *subject, const JointRemoveEvent &data) {
    Joint *joint = data.joint;
    this->checkJoint(joint, joint->getPos(), true);
    joint->Movable<Joint>::unsubscribe(this);
}

void SimulationFeature::update(Subject<NodeAddEvent> *subject, const NodeAddEvent &data) {
    Node *node = data.node;
    node->addToSimulation(this->simulation);
    this->checkNode(node, node->getPos());
    node->Movable::subscribe(this);
    node->Rotatable::subscribe(this);
}

void SimulationFeature::update(Subject<NodeRemoveEvent> *subject, const NodeRemoveEvent &data) {
    Node *node = data.node;
    node->removeFromSimulation(this->simulation);
    this->checkNode(node, node->getPos(), true);
    node->Movable::unsubscribe(this);
    node->Rotatable::unsubscribe(this);
}

void SimulationFeature::update(Subject<MoveEvent<Node>> *subject, const MoveEvent<Node> &data) {
    Node *node = static_cast<Node*>(subject);
    this->checkNode(node, node->getPos(), data.before);
}

void SimulationFeature::update(Subject<MoveEvent<Joint>> *subject, const MoveEvent<Joint> &data) {
    Joint *joint = static_cast<Joint*>(subject);
    this->checkJoint(joint, joint->getPos(), data.before);
}

void SimulationFeature::update(Subject<RotateEvent<Node>> *subject, const RotateEvent<Node> &data) {
    Node *node = static_cast<Node*>(subject);
    this->checkNode(node, node->getPos(), data.before);
}
